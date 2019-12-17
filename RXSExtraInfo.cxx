//
// Created by 蒋志平 on 2018/11/10.
//

#include "RXSExtraInfo.h"
#include "rxs_enhanced.h"

void featureCodeInterpretation(uint32_t featureCode, struct ExtraInfo *extraInfo) {
    extraInfo->hasLength = extraInfoHasLength(featureCode);
    extraInfo->hasVersion = extraInfoHasVersion(featureCode);
    extraInfo->hasMacAddr_cur = extraInfoHasMacAddress_Current(featureCode);
    extraInfo->hasMacAddr_rom = extraInfoHasMacAddress_Rom(featureCode);
    extraInfo->hasChansel = extraInfoHasChansel(featureCode);
    extraInfo->hasBMode = extraInfoHasBMode(featureCode);
    extraInfo->hasEVM = extraInfoHasEVM(featureCode);
    extraInfo->hasTxChainMask = extraInfoHasTxChainMask(featureCode);
    extraInfo->hasRxChainMask = extraInfoHasRxChainMask(featureCode);
    extraInfo->hasTxpower = extraInfoHasTxPower(featureCode);
    extraInfo->hasCF = extraInfoHasCF(featureCode);
    extraInfo->hasTxTSF = extraInfoHasTxTSF(featureCode);
    extraInfo->hasLastHWTxTSF = extraInfoHasLastHWTxTSF(featureCode);
    extraInfo->hasChannelFlags = extraInfoHasChannelFlags(featureCode);
    extraInfo->hasTxNess = extraInfoHasTxNess(featureCode);
    extraInfo->hasTuningPolicy = extraInfoHasTuningPolicy(featureCode);
    extraInfo->hasPLLRate = extraInfoHasPLLRate(featureCode);
    extraInfo->hasPLLRefDiv = extraInfoHasPLLRefDiv(featureCode);
    extraInfo->hasPLLClkSel = extraInfoHasPLLClkSel(featureCode);
    extraInfo->hasAGC = extraInfoHasAGC(featureCode);
    extraInfo->hasAntennaSelection = extraInfoHasAntennaSelection(featureCode);
}


void inplaceAddRxExtraInfo(uint8_t *inBytes, uint32_t featureCode, uint8_t *value, int length) {
    static uint8_t buffer[200];
    uint32_t *rxFeatureCode = nullptr;
    uint32_t pos = 6;
    uint32_t insertPos = 0;
    uint16_t *lengthField_ptr = nullptr;
    uint32_t bufferUsedLength = 0;

    rxFeatureCode = (uint32_t *) (inBytes + pos);
    pos += 4;
    pos += sizeof(struct rx_status_basic);
    lengthField_ptr = extraInfoHasLength(*rxFeatureCode) ? (uint16_t *) inBytes + pos : nullptr;
    pos += extraInfoHasLength(*rxFeatureCode) ? 2 : 0;
    pos += extraInfoHasVersion(*rxFeatureCode) ? 8 : 0;
    pos += extraInfoHasMacAddress_Current(*rxFeatureCode) ? 6 : 0;
    pos += extraInfoHasMacAddress_Rom(*rxFeatureCode) ? 6 : 0;
    pos += extraInfoHasChansel(*rxFeatureCode) ? 4 : 0;
    pos += extraInfoHasBMode(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasEVM(*rxFeatureCode) ? 20 : 0;
    pos += extraInfoHasTxChainMask(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasRxChainMask(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasTxPower(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasCF(*rxFeatureCode) ? 8 : 0;
    pos += extraInfoHasTxTSF(*rxFeatureCode) ? 4 : 0;
    pos += extraInfoHasLastHWTxTSF(*rxFeatureCode) ? 4 : 0;
    pos += extraInfoHasChannelFlags(*rxFeatureCode) ? 2 : 0;
    pos += extraInfoHasTxNess(*rxFeatureCode) ? 1 : 0;
    insertPos = extraInfoHasTuningPolicy(featureCode) ? pos : (insertPos > 0 ? insertPos : 0);
    pos += extraInfoHasTuningPolicy(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasPLLRate(*rxFeatureCode) ? 2 : 0;
    pos += extraInfoHasPLLRefDiv(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasPLLClkSel(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasAGC(*rxFeatureCode) ? 1 : 0;
    pos += extraInfoHasAntennaSelection(*rxFeatureCode) ? 1 : 0;

    *rxFeatureCode |= featureCode;
    if (insertPos == 0) {
        memcpy(inBytes + insertPos, value, length);
        *lengthField_ptr += length;
    } else {
        memset(buffer, 0, 200);
        bufferUsedLength = pos - insertPos;
        memcpy(buffer, inBytes + insertPos, bufferUsedLength);
        memcpy(inBytes + insertPos, value, length);
        memcpy(inBytes + insertPos + length, buffer, bufferUsedLength);
        *lengthField_ptr += length;
    }
}


int TxExtraInfoMinSet::getTxTSFPos() {
    if (extraInfoHasTxTSF(txExtraInfoFeatureCode)) {
        auto pos = sizeof(struct TxExtraInfoMinSet);
        pos += extraInfoHasChansel(txExtraInfoFeatureCode) ? 4 : 0;
        pos += extraInfoHasBMode(txExtraInfoFeatureCode) ? 1 : 0;
        pos += extraInfoHasEVM(txExtraInfoFeatureCode) ? 20 : 0;
        pos += extraInfoHasTxChainMask(txExtraInfoFeatureCode) ? 1 : 0;
        pos += extraInfoHasRxChainMask(txExtraInfoFeatureCode) ? 1 : 0;
        pos += extraInfoHasTxPower(txExtraInfoFeatureCode) ? 1 : 0;
        pos += extraInfoHasCF(txExtraInfoFeatureCode) ? 8 : 0;

        return pos;
    }

    return -1;
}

int ExtraInfo::fromBinary(const uint8_t *extraInfoPtr, struct ExtraInfo *extraInfo, uint32_t suppliedFeatureCode) {
    int pos = 0;
    if (suppliedFeatureCode == 0) {
        auto featureCode = *((uint32_t *) extraInfoPtr);
        pos += 4;
        featureCodeInterpretation(featureCode, extraInfo);
    } else {
        featureCodeInterpretation(suppliedFeatureCode, extraInfo);
    }

    if (extraInfo->hasLength) {
        uint16_t exLength = *((uint16_t *) (extraInfoPtr + pos));
        pos += 2;
        extraInfo->length = exLength;
    }

    if (extraInfo->hasVersion) {
        extraInfo->version = *((uint64_t *) (extraInfoPtr + pos));
        pos += 8;
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
        extraInfo->chansel = *((uint32_t *) (extraInfoPtr + pos));
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
        extraInfo->cf = *((uint64_t *) (extraInfoPtr + pos));
        pos += 8;
    }

    if (extraInfo->hasTxTSF) {
        extraInfo->txTSF = *((uint32_t *) (extraInfoPtr + pos));
        pos += 4;
    }

    if (extraInfo->hasLastHWTxTSF) {
        extraInfo->lastHwTxTSF = *((uint32_t *) (extraInfoPtr + pos));
        pos += 4;
    }

    if (extraInfo->hasChannelFlags) {
        extraInfo->channelFlags = *((uint16_t *) (extraInfoPtr + pos));
        pos += 2;
    }

    if (extraInfo->hasTxNess) {
        extraInfo->tx_ness = extraInfoPtr[pos++];
    }

    if (extraInfo->hasTuningPolicy) {
        extraInfo->tuningPolicy = extraInfoPtr[pos++];
    }

    if (extraInfo->hasPLLRate) {
        extraInfo->pll_rate = *((uint16_t *) (extraInfoPtr + pos));
        pos += 2;
    }

    if (extraInfo->hasPLLRefDiv) {
        extraInfo->pll_refdiv = extraInfoPtr[pos++];
    }

    if (extraInfo->hasPLLClkSel) {
        extraInfo->pll_clock_select = extraInfoPtr[pos++];
    }

    if (extraInfo->hasAGC) {
        extraInfo->agc = extraInfoPtr[pos++];
    }

    if (extraInfo->hasAntennaSelection) {
        auto ant_sel_raw = extraInfoPtr[pos++];
        extraInfo->ant_sel[0] = ((ant_sel_raw) & 0x3) + 1;
        extraInfo->ant_sel[1] = ((ant_sel_raw >> 2) & 0x3) + 1;
        extraInfo->ant_sel[2] = ((ant_sel_raw >> 4) & 0x3) + 1;
    }

    return 0;
}

uint16_t ExtraInfo::getLength() {
    int pos = 4;
    pos += this->hasLength ? 2 : 0;
    pos += this->hasVersion ? 8 : 0;
    pos += this->hasMacAddr_cur ? 6 : 0;
    pos += this->hasMacAddr_rom ? 6 : 0;
    pos += this->hasChansel ? 4 : 0;
    pos += this->hasBMode ? 1 : 0;
    pos += this->hasEVM ? 20 : 0;
    pos += this->hasTxChainMask ? 1 : 0;
    pos += this->hasRxChainMask ? 1 : 0;
    pos += this->hasTxpower ? 1 : 0;
    pos += this->hasCF ? 8 : 0;
    pos += this->hasTxTSF ? 4 : 0;
    pos += this->hasLastHWTxTSF ? 4 : 0;
    pos += this->hasChannelFlags ? 2 : 0;
    pos += this->hasTxNess ? 1 : 0;
    pos += this->hasTuningPolicy ? 1 : 0;
    pos += this->hasPLLRate ? 2 : 0;
    pos += this->hasPLLRefDiv ? 1 : 0;
    pos += this->hasPLLClkSel ? 1 : 0;
    pos += this->hasAGC ? 1 : 0;
    pos += this->hasAntennaSelection ? 1 : 0;

    return pos;
}

int ExtraInfo::toBinary(void *extraInfoPtr) {
    return 0;
}
