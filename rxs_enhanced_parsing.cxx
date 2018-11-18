#include "rxs_enhanced.h"

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

    auto streams = mcs / 8 + 1;
    auto bits = length * 8 + OFDM_PLCP_BITS;
    if (lengthWithoutFCS)
        bits += 32;
    auto symbolbits = bits_per_symbol[mcs % 8][wide40BW] * streams;
    auto symbols = (bits + symbolbits -1) / symbolbits;
    auto pktDuration = (usingSGI ? SGI: LGI) * symbols;

    return L_STF + L_LTF + L_SIG + HT_SIG + HT_STF + HT_LTF*streams + pktDuration;
}

void hexDump(const uint8_t * inBytes, uint64_t length, std::optional<std::string> title) {
    printf("\n"
           "   %s, length=%lu \n"
           "------------------------------------------------\n", title.value_or("").c_str(), length);
    printf("  Offset: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f");
    for (auto i = 0 ; i < length; i ++) {
        if (i % 16 == 0) {
            printf("\n%08x:", i/16);
        }
        printf(" %02x", *(inBytes + i));
    }
    printf("\n"
           "------------------------------------------------\n");
}

struct RXS_enhanced parseRXS(const uint8_t * inBytes, enum RXSParsingLevel parsingLevel) {
    struct RXS_enhanced rxs;
    parse_rxs_enhanced(inBytes, &rxs, parsingLevel);
    return rxs;
}

int parse_rxs_enhanced(const uint8_t * inBytes, struct RXS_enhanced *rxs, enum RXSParsingLevel parsingLevel) {
    uint16_t totalLength = 2;
    uint32_t magicValue = 0;
    uint32_t rxFeatureCode=0;
    uint32_t pos = 0;

    totalLength += *((uint16_t *)(inBytes+pos)); pos += 2;
    magicValue  = *((uint32_t *)(inBytes+pos)); pos += 4;
    if (magicValue != 0x20150315 && magicValue != 0x20120930) {
        hexDump(inBytes, 200);
        assert(magicValue == 0x20150315 || magicValue == 0x20120930);
    }

    rxs->isAR9300 = magicValue == 0x20150315 ? true : false;

    rxFeatureCode = *((uint32_t *)(inBytes+pos)); pos += 4;
    rxs->rxs_basic = *((struct rx_status_basic *)(inBytes + pos)); pos += sizeof (struct rx_status_basic);
    if(rxs->rxs_basic.nrx <=0 || rxs->rxs_basic.nrx > 3 || rxs->rxs_basic.ntx <=0 || rxs->rxs_basic.ntx >3 || (rxs->isAR9300 && rxs->rxs_basic.num_tones != TONE_20M && rxs->rxs_basic.num_tones != TONE_40M) ) {
        printf("RXS Parser: Impossible values in nrx (%d), ntx (%d), or num_tones (%d). Error occurs in file format or parsing.\n", rxs->rxs_basic.nrx, rxs->rxs_basic.ntx, rxs->rxs_basic.num_tones);
        return 1;
    }

    ExtraInfo::fromBinary(inBytes+pos, &rxs->rxExtraInfo, rxFeatureCode); pos += rxs->rxExtraInfo.length + 2;
    if (rxs->rxExtraInfo.hasEVM) {
        for (auto i = 0 ; i < 18; i++) {
            rxs->rxExtraInfo.evm[i] += rxs->rxs_basic.noise;
            if (rxs->rxExtraInfo.evm[i] > 0) {
                rxs->rxExtraInfo.evm[i] = -128;
            }
        }
    }

    rxs->csi_pos = pos;
    if (parsingLevel >= EXTRA_CSI ) {
        if (rxs->isAR9300)
            ar_parse_csi_matrix(inBytes + rxs->csi_pos, rxs->rxs_basic.ntx, rxs->rxs_basic.nrx, rxs->rxs_basic.num_tones,
                            rxs->csi_matrix);
        else
            iwl_parse_csi_matrix(inBytes + rxs->csi_pos, rxs->rxs_basic.ntx, rxs->rxs_basic.nrx, rxs->rxs_basic.num_tones,
                                rxs->csi_matrix);

        // commit the following two lines to aquire raw csi matrix
        auto new_tones_num = phaseUnwrapAroundDC(rxs->csi_matrix, rxs->unwrappedMag, rxs->unwrappedPhase, rxs->rxs_basic.ntx, rxs->rxs_basic.nrx, rxs->rxs_basic.num_tones);
        rxs->rxs_basic.num_tones = new_tones_num;
    }
    pos += rxs->rxs_basic.csi_len;
    rxs->payload_pos = pos;

    rxs->payloadLength = totalLength - pos;
    rxs->txDuration = pkt_duration(rxs->payloadLength, rxs->rxs_basic.rate, rxs->rxs_basic.chanBW, rxs->rxs_basic.sgi);

    uint8_t frameType = *((uint8_t *)(inBytes+pos));
    if (frameType == 0x8) { // Frame injected by PicoScenes
        auto packetHeader = (struct ieee80211_packet_header *)(inBytes + pos);
        rxs->txHeader = * packetHeader;
        pos += sizeof(struct ieee80211_packet_header);

        if ( packetHeader->header_info.hasTxExtraInfo ) {
            ExtraInfo::fromBinary(inBytes+pos, &rxs->txExtraInfo); pos += rxs->txExtraInfo.length + 6;
        }

        if (packetHeader->header_info.hasEchoProbeInfo) {
            rxs->echoProbeInfo = *((struct EchoProbeInfo *)(inBytes + pos)); pos += sizeof(struct EchoProbeInfo);
            if (rxs->echoProbeInfo.TxRXSLength > 0) {
                memcpy(rxs->chronosACKBody, inBytes + pos, rxs->echoProbeInfo.TxRXSLength);
            }
            pos += rxs->echoProbeInfo.TxRXSLength;
        }
    }

    if (pos != totalLength) {
        hexDump(inBytes, totalLength);
        assert(pos == totalLength); // this is for validation
    }

    rxs->rawBufferLength = static_cast<uint16_t>(pos);
    memcpy(rxs->rawBuffer, inBytes, rxs->rawBufferLength);
    rxs->parsingLevel = parsingLevel;

    return 0;
}

void inplaceAddTxChronosData(RXS_enhanced * rxs, uint8_t *dataBuffer, int bufferLength) {
    auto * rawBuffer = rxs->rawBuffer;

    auto * packetHeader_ptr = (struct ieee80211_packet_header *)(rawBuffer + rxs->payload_pos);
    assert(rxs->txHeader.header_info.taskId == packetHeader_ptr->header_info.taskId);

    auto chronosPos = rxs->payload_pos + sizeof(struct ieee80211_packet_header);
    if (packetHeader_ptr->header_info.hasTxExtraInfo) {
        auto * txExtraInfoLength_ptr = (uint16_t *)(rawBuffer+chronosPos + 4);
        chronosPos += *txExtraInfoLength_ptr;
        chronosPos += 6;
    }
    auto * chronos_ptr = (struct EchoProbeInfo *)(rawBuffer + chronosPos);
    if(!packetHeader_ptr->header_info.hasEchoProbeInfo) {
        packetHeader_ptr->header_info.hasEchoProbeInfo |= 1;
        *chronos_ptr = {};
        chronos_ptr->length = sizeof(struct EchoProbeInfo);
        chronos_ptr->version = 2;

        rxs->txHeader.header_info.hasEchoProbeInfo |= 1;
        rxs->echoProbeInfo = *chronos_ptr;
        rxs->payloadLength += sizeof(struct EchoProbeInfo);
        rxs->rawBufferLength += sizeof(struct EchoProbeInfo);
    }

    assert(chronos_ptr->TxRXSLength == rxs->echoProbeInfo.TxRXSLength);
    auto * chronos_rxs_ptr = rawBuffer + chronosPos + sizeof(struct EchoProbeInfo) + chronos_ptr->TxRXSLength;
    memcpy(chronos_rxs_ptr, dataBuffer, bufferLength);
    auto * totalLength_ptr = (uint16_t *)(rawBuffer);
    * totalLength_ptr += bufferLength;
    memcpy(rxs->chronosACKBody + rxs->echoProbeInfo.TxRXSLength, dataBuffer, bufferLength);
    chronos_ptr->TxRXSLength += bufferLength;
    rxs->rawBufferLength += bufferLength;
    rxs->payloadLength += bufferLength;
    rxs->echoProbeInfo.TxRXSLength += bufferLength;
}
