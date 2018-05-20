#include "rxs_enhanced.h"
#include "CSIMatrixParser.h"
#ifdef BUILD_WITH_MEX
    #include "mex.h"
#endif

uint16_t pkt_duration(uint16_t length, uint8_t mcs, bool wide40BW, bool usingSGI, bool lengthWithoutFCS) {
    static const auto L_STF = 8;
    static const auto L_LTF = 8;
    static const auto L_SIG = 4;
    static const auto HT_SIG = 8;
    static const auto HT_STF = 4;
    static const auto HT_LTF = 4;
    static const auto OFDM_PLCP_BITS = 22;
    static const auto LGI = 4.0;
    static const auto SGI = 3.6;

    static const uint16_t bits_per_symbol[][2] = {
            /* 20MHz 40MHz */
            {    26,   54 },     /*  0: BPSK */
            {    52,  108 },     /*  1: QPSK 1/2 */
            {    78,  162 },     /*  2: QPSK 3/4 */
            {   104,  216 },     /*  3: 16-QAM 1/2 */
            {   156,  324 },     /*  4: 16-QAM 3/4 */
            {   208,  432 },     /*  5: 64-QAM 2/3 */
            {   234,  486 },     /*  6: 64-QAM 3/4 */
            {   260,  540 },     /*  7: 64-QAM 5/6 */
    };
//
    auto streams = mcs / 8 + 1;
    auto bits = length * 8 + OFDM_PLCP_BITS;
    if (lengthWithoutFCS)
        bits += 32;
    auto symbolbits = bits_per_symbol[mcs % 8][wide40BW] * streams;
    auto symbols = (bits + symbolbits -1) / symbolbits;
    auto pktDuration = (usingSGI ? SGI: LGI) * symbols;

    return L_STF + L_LTF + L_SIG + HT_SIG + HT_STF + HT_LTF*streams + pktDuration;
}

void featureCodeInterpretation(uint32_t featureCode, struct ExtraInfo * extraInfo) {
    extraInfo->hasVersion     = extraInfoHasVersion(featureCode);
    extraInfo->hasLength      = extraInfoHasLength(featureCode);
    extraInfo->hasMacAddr_cur = extraInfoHasMacAddress_Current(featureCode);
    extraInfo->hasMacAddr_rom = extraInfoHasMacAddress_Rom(featureCode);
    extraInfo->hasChansel     = extraInfoHasChansel(featureCode);
    extraInfo->hasBMode       = extraInfoHasBMode(featureCode);
    extraInfo->hasEVM         = extraInfoHasEVM(featureCode);
    extraInfo->hasTxChainMask = extraInfoHasTxChainMask(featureCode);
    extraInfo->hasRxChainMask = extraInfoHasRxChainMask(featureCode);
    extraInfo->hasTxpower     = extraInfoHasTxPower(featureCode);
    extraInfo->hasCF          = extraInfoHasCF(featureCode);
    extraInfo->hasTxTSF       = extraInfoHasTxTSF(featureCode);
    extraInfo->hasLastHWTxTSF = extraInfoHasLastHWTxTSF(featureCode);
    extraInfo->hasChannelFlags= extraInfoHasChannelFlags(featureCode);
    extraInfo->hasPLLSlope    = extraInfoHasPLLSlope(featureCode);
    extraInfo->hasTuningPolicy= extraInfoHasTuningPolicy(featureCode);
}

struct RXS_enhanced parseRXS(const uint8_t * inBytes, enum RXSParsingLevel parsingLevel) {
    struct RXS_enhanced rxs;
    parse_rxs_enhanced(inBytes, &rxs, parsingLevel);
    return rxs;
}

int parse_rxs_enhanced(const uint8_t * inBytes, struct RXS_enhanced *rxs, enum RXSParsingLevel parsingLevel) {
    uint32_t magicValue = 0;
    uint32_t rxFeatureCode=0;
    uint32_t pos = 0;
    
    magicValue = *((uint32_t *)(inBytes+pos)); pos += 4;
    if (magicValue == 0x20150315) {
        rxFeatureCode = *((uint32_t *)(inBytes+pos)); pos += 4;
    }

    struct ath_rx_status_basic * rx_status_basic_ptr = (struct ath_rx_status_basic *)(inBytes + pos);
    rxs->rxs_basic = *rx_status_basic_ptr;
    rxs->rxs_basic.rate -= 0x80;
    pos += sizeof (struct ath_rx_status_basic);

    if(rxs->rxs_basic.nrx <=0 || rxs->rxs_basic.nrx > 3 || rxs->rxs_basic.ntx <=0 || rxs->rxs_basic.ntx >3 || (rxs->rxs_basic.num_tones != TONE_20M && rxs->rxs_basic.num_tones != TONE_40M) ) {
        printf("RXS Parser: Impossible values in nrx (%d), ntx (%d), or num_tones (%d). Error occurs in file format or parsing.\n", rxs->rxs_basic.nrx, rxs->rxs_basic.ntx, rxs->rxs_basic.num_tones);
        return 1;
    }

    if (extraInfoHasLength(rxFeatureCode)) {
        ExtraInfo::fromBinary(inBytes+pos, &rxs->rxExtraInfo, rxFeatureCode);
        if (rxs->rxExtraInfo.hasEVM) {
            for (auto i = 0 ; i < 18; i++) {
                rxs->rxExtraInfo.evm[i] += rxs->rxs_basic.noise;
                if (rxs->rxExtraInfo.evm[i] > 0) {
                    rxs->rxExtraInfo.evm[i] = -128;
                }
            }
        }
        pos += rxs->rxExtraInfo.length + 2;
    }

    rxs->payloadLength = *((uint16_t *)(inBytes+pos));
    pos += 2;
    rxs->txDuration = pkt_duration(rxs->payloadLength, rxs->rxs_basic.rate, rxs->rxs_basic.chanBW, rxs->rxs_basic.sgi);

    rxs->csi_pos = pos;
    if (parsingLevel >= EXTRA_CSI ) {
        parse_csi_matrix(inBytes + rxs->csi_pos, rxs->rxs_basic.nrx, rxs->rxs_basic.ntx, rxs->rxs_basic.num_tones, rxs->csi_matrix);
        auto new_tones_num = phaseUnwrapAroundDC(rxs->csi_matrix, rxs->unwrappedMag, rxs->unwrappedPhase, rxs->rxs_basic.nrx, rxs->rxs_basic.ntx, rxs->rxs_basic.num_tones);
        rxs->rxs_basic.num_tones = new_tones_num;
    }
    pos += rxs->rxs_basic.csi_len;

    auto packetHeader = (struct ieee80211_packet_header *)(inBytes + pos);
    rxs->txHeader = * packetHeader;
    pos += sizeof(struct ieee80211_packet_header);

    if ( packetHeader->header_info.hasTxExtraInfo ) {
        ExtraInfo::fromBinary(inBytes+pos, &rxs->txExtraInfo);
        pos += rxs->txExtraInfo.length + 6;
    }

    if (packetHeader->header_info.hasChronosInfo) {
        struct ChronosInfo * chronosInfoPtr = (struct ChronosInfo *)(inBytes + pos);
        rxs->chronosInfo = * chronosInfoPtr;
        pos += sizeof(struct ChronosInfo);
        if (rxs->chronosInfo.TxRXSLength > 0) {
            memcpy(rxs->chronosACKBody, inBytes + pos, rxs->chronosInfo.TxRXSLength);
        }
        pos += rxs->chronosInfo.TxRXSLength;
    }

    rxs->rawBufferLength = static_cast<uint16_t>(pos);
    memcpy(rxs->rawBuffer, inBytes, rxs->rawBufferLength);
    rxs->parsingLevel = parsingLevel;

    return 0;
}

int reparseCSIMatrixInRXS(struct RXS_enhanced *rxs) {
    if (rxs->parsingLevel < EXTRA_CSI) {
        parse_csi_matrix(rxs->rawBuffer + rxs->csi_pos, rxs->rxs_basic.nrx, rxs->rxs_basic.ntx, rxs->rxs_basic.num_tones, rxs->csi_matrix);
        auto new_tones_num = phaseUnwrapAroundDC(rxs->csi_matrix, rxs->unwrappedMag, rxs->unwrappedPhase, rxs->rxs_basic.nrx, rxs->rxs_basic.ntx, rxs->rxs_basic.num_tones);
        rxs->rxs_basic.num_tones = new_tones_num;
        rxs->parsingLevel = EXTRA_CSI;
    }
    return 0;
}

void inplaceAddRxExtraInfo(uint8_t *inBytes, uint32_t featureCode, uint8_t *value, int length) {
    uint32_t * rxFeatureCode = nullptr;
    uint32_t pos = 4;

    rxFeatureCode = (uint32_t *)(inBytes+pos); pos += 4;

    pos += sizeof(struct ath_rx_status_basic);
    pos += extraInfoHasLength(*rxFeatureCode) ? 2:0;
    pos += extraInfoHasVersion(*rxFeatureCode) ? 1:0;
    pos += extraInfoHasMacAddress_Current(*rxFeatureCode) ? 6:0;
    pos += extraInfoHasMacAddress_Rom(*rxFeatureCode) ? 6:0;
    pos += extraInfoHasChansel(*rxFeatureCode) ? 4:0;
    pos += extraInfoHasBMode(*rxFeatureCode) ? 1:0;
    pos += extraInfoHasEVM(*rxFeatureCode) ? 20:0;
    pos += extraInfoHasTxChainMask(*rxFeatureCode) ? 1: 0;
    pos += extraInfoHasRxChainMask(*rxFeatureCode) ? 1: 0;
    pos += extraInfoHasTxPower(*rxFeatureCode) ? 1: 0;
    pos += extraInfoHasCF(*rxFeatureCode) ? 8: 0;
    pos += extraInfoHasTxTSF(*rxFeatureCode) ? 4: 0;
    pos += extraInfoHasLastHWTxTSF(*rxFeatureCode) ? 4: 0;
    pos += extraInfoHasChannelFlags(*rxFeatureCode) ? 2: 0;
    pos += extraInfoHasPLLSlope(*rxFeatureCode) ? sizeof(double): 0;
    pos += extraInfoHasTuningPolicy(*rxFeatureCode) ? 1: 0;

    *rxFeatureCode |= featureCode;
    memcpy(inBytes + pos, value, length);
}

void inplaceAddTxChronosData(RXS_enhanced * rxs, uint8_t *dataBuffer, int bufferLength) {
    auto * rawBuffer = rxs->rawBuffer;
    auto * payloadLength_ptr = (uint16_t *)(rawBuffer+rxs->csi_pos - 2);
    assert(rxs->payloadLength == *payloadLength_ptr);

    auto headerPos = rxs->csi_pos + rxs->rxs_basic.csi_len;
    auto * packetHeader_ptr = (struct ieee80211_packet_header *)(rawBuffer + headerPos);
    assert(rxs->txHeader.header_info.taskId == packetHeader_ptr->header_info.taskId);

    auto chronosPos = headerPos + sizeof(struct ieee80211_packet_header);
    if (packetHeader_ptr->header_info.hasTxExtraInfo) {
        auto * txExtraInfoLength_ptr = (uint16_t *)(rawBuffer+chronosPos + 4);
        chronosPos += *txExtraInfoLength_ptr;
        chronosPos += 6;
    }
    auto * chronos_ptr = (struct ChronosInfo *)(rawBuffer + chronosPos);
    if(!packetHeader_ptr->header_info.hasChronosInfo) {
        packetHeader_ptr->header_info.hasChronosInfo |= 1;
        *chronos_ptr = {};
        chronos_ptr->length = sizeof(struct ChronosInfo);
        chronos_ptr->version = 2;

        rxs->txHeader.header_info.hasChronosInfo |= 1;
        rxs->chronosInfo = *chronos_ptr;
        rxs->payloadLength += sizeof(struct ChronosInfo);
        rxs->rawBufferLength += sizeof(struct ChronosInfo);
    }

    assert(chronos_ptr->TxRXSLength == rxs->chronosInfo.TxRXSLength);
    auto * chronos_rxs_ptr = rawBuffer + chronosPos + sizeof(struct ChronosInfo) + chronos_ptr->TxRXSLength;
    memcpy(chronos_rxs_ptr, dataBuffer, bufferLength);
    memcpy(rxs->chronosACKBody + rxs->chronosInfo.TxRXSLength, dataBuffer, bufferLength);
    chronos_ptr->TxRXSLength += bufferLength;
    *payloadLength_ptr += bufferLength;
    rxs->rawBufferLength += bufferLength;
    rxs->payloadLength += bufferLength;
    rxs->chronosInfo.TxRXSLength += bufferLength;
}

int TxExtraInfoMinSet::getTxTSFPos() {
    if (extraInfoHasTxTSF(txExtraInfoFeatureCode)) {
        auto pos = sizeof(struct TxExtraInfoMinSet);
        pos += extraInfoHasChansel(txExtraInfoFeatureCode) ? 4:0;
        pos += extraInfoHasBMode(txExtraInfoFeatureCode) ? 1:0;
        pos += extraInfoHasEVM(txExtraInfoFeatureCode) ? 20:0;
        pos += extraInfoHasTxChainMask(txExtraInfoFeatureCode) ? 1: 0;
        pos += extraInfoHasRxChainMask(txExtraInfoFeatureCode) ? 1: 0;
        pos += extraInfoHasTxPower(txExtraInfoFeatureCode) ? 1: 0;
        pos += extraInfoHasCF(txExtraInfoFeatureCode) ? 8: 0;

        return pos;
    }

    return -1;
}

int ExtraInfo::fromBinary(const uint8_t *extraInfoPtr, struct ExtraInfo * extraInfo, uint32_t suppliedFeatureCode) {
    int pos = 0;
    if (suppliedFeatureCode == 0) {
        auto featureCode = *((uint32_t *)extraInfoPtr);
        pos += 4;
        featureCodeInterpretation(featureCode, extraInfo);
    } else {
        featureCodeInterpretation(suppliedFeatureCode, extraInfo);
    }

    if (extraInfo->hasLength) {
        uint16_t exLength = *((uint16_t *)(extraInfoPtr+pos));
        pos += 2;
        extraInfo->length = exLength;
    }

    if (extraInfo->hasVersion) {
        extraInfo->version = extraInfoPtr[pos++];
    }

    if (extraInfo->hasMacAddr_cur) {
        memcpy(extraInfo->macaddr_cur, extraInfoPtr + pos, 6);
        pos += 6;
    }

    if (extraInfo->hasMacAddr_rom) {
        memcpy(extraInfo->macaddr_rom, extraInfoPtr + pos, 6);
        pos += 6;
    }

    if (extraInfo->hasChansel) {
        extraInfo->chansel = *((uint32_t *)(extraInfoPtr+pos));
        pos += 4;
    }

    if (extraInfo->hasBMode) {
        extraInfo->bmode = extraInfoPtr[pos++];
    }

    if (extraInfo->hasEVM) {
        memcpy(extraInfo->evm, (extraInfoPtr + pos), 18);
        pos += 20; // +20, but using first 18 only. because only first 2 of evm4 is used.
    }

    if (extraInfo->hasTxChainMask) {
        extraInfo->txChainMask = extraInfoPtr[pos++];
    }

    if (extraInfo->hasRxChainMask) {
        extraInfo->rxChainMask = extraInfoPtr[pos++];
    }

    if (extraInfo->hasTxpower) {
        extraInfo->txpower = extraInfoPtr[pos++];
    }

    if (extraInfo->hasCF) {
        extraInfo->cf =  *((uint64_t *)(extraInfoPtr+pos));
        pos += 8;
    }

    if (extraInfo->hasTxTSF) {
        extraInfo->txTSF =  *((uint32_t *)(extraInfoPtr+pos));
        pos += 4;
    }

    if (extraInfo->hasLastHWTxTSF) {
        extraInfo->lastHwTxTSF =  *((uint32_t *)(extraInfoPtr+pos));
        pos += 4;
    }

    if (extraInfo->hasChannelFlags) {
        extraInfo->channelFlags =  *((uint16_t *)(extraInfoPtr+pos));
        pos += 2;
    }

    if (extraInfo->hasPLLSlope) {
        extraInfo->pllSlope = *((double *)(extraInfoPtr + pos));
        pos += sizeof(double);
    }

    if (extraInfo->hasTuningPolicy) {
        extraInfo->tuningPolicy = extraInfoPtr[pos++];
    }
    
    return 0;
}
