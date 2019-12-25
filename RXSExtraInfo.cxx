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

ExtraInfo::ExtraInfo() {
    memset(this, 0, sizeof(ExtraInfo));
    setLength(2);
}

int ExtraInfo::fromBinary(const uint8_t *extraInfoPtr, struct ExtraInfo *extraInfo, uint32_t suppliedFeatureCode) {
    int pos = 0;
    if (suppliedFeatureCode == 0) {
        extraInfo->featureCode = *((uint32_t *) extraInfoPtr);
        pos += 4;
    } else
        extraInfo->featureCode = suppliedFeatureCode;
    featureCodeInterpretation(extraInfo->featureCode, extraInfo);

#define GETVALUE(hasV, V) \
    if (extraInfo->hasV) { \
        memcpy(&extraInfo->V, extraInfoPtr + pos, sizeof(extraInfo->V)); \
        pos += sizeof(extraInfo->V); \
    } \

    GETVALUE(hasLength, length)
    GETVALUE(hasVersion, version)
    GETVALUE(hasMacAddr_cur, macaddr_cur)
    GETVALUE(hasMacAddr_rom, macaddr_rom)
    GETVALUE(hasChansel, chansel)
    GETVALUE(hasBMode, bmode)
    GETVALUE(hasEVM, evm)
    GETVALUE(hasTxChainMask, txChainMask)
    GETVALUE(hasRxChainMask, rxChainMask)
    GETVALUE(hasTxpower, txpower)
    GETVALUE(hasCF, cf)
    GETVALUE(hasTxTSF, txTSF)
    GETVALUE(hasTxpower, txpower)
    GETVALUE(hasLastHWTxTSF, lastHwTxTSF)
    GETVALUE(hasChannelFlags, channelFlags)
    GETVALUE(hasTxNess, tx_ness)
    GETVALUE(hasTuningPolicy, tuningPolicy)
    GETVALUE(hasPLLRate, pll_rate)
    GETVALUE(hasPLLRefDiv, pll_refdiv)
    GETVALUE(hasPLLClkSel, pll_clock_select)
    GETVALUE(hasAGC, agc)
    if (extraInfo->hasAntennaSelection) {
        auto ant_sel_raw = extraInfoPtr[pos++];
        extraInfo->ant_sel[0] = ((ant_sel_raw) & 0x1U) + 1;
        extraInfo->ant_sel[1] = (((unsigned) ant_sel_raw >> 0x2U) & 0x3U) + 1;
        extraInfo->ant_sel[2] = (((unsigned) ant_sel_raw >> 0x4U) & 0x3U) + 1;
    }

    return pos;
#undef GETVALUE
}

std::optional<ExtraInfo> ExtraInfo::fromBuffer(const uint8_t *extraInfoPtr, uint32_t suppliedFeatureCode) {
    ExtraInfo extraInfo;
    auto parsedLength = fromBinary(extraInfoPtr, &extraInfo, suppliedFeatureCode);
    if (suppliedFeatureCode != 0 && parsedLength == extraInfo.calculateBufferLength())
        return extraInfo;
    if (suppliedFeatureCode == 0 && parsedLength == extraInfo.calculateBufferLength() + 4)
        return extraInfo;
    return std::nullopt;
}

uint16_t ExtraInfo::calculateBufferLength() const {
    uint16_t pos = 0;

#define ADDLENGTH(hasV, V) \
        pos += (this->hasV ? sizeof(V) : 0); \

    ADDLENGTH(hasLength, length)
    ADDLENGTH(hasVersion, version)
    ADDLENGTH(hasMacAddr_cur, macaddr_cur)
    ADDLENGTH(hasMacAddr_rom, macaddr_rom)
    ADDLENGTH(hasChansel, chansel)
    ADDLENGTH(hasBMode, bmode)
    ADDLENGTH(hasEVM, evm)
    ADDLENGTH(hasTxChainMask, txChainMask)
    ADDLENGTH(hasRxChainMask, rxChainMask)
    ADDLENGTH(hasTxpower, txpower)
    ADDLENGTH(hasCF, cf)
    ADDLENGTH(hasTxTSF, txTSF)
    ADDLENGTH(hasTxpower, txpower)
    ADDLENGTH(hasLastHWTxTSF, lastHwTxTSF)
    ADDLENGTH(hasChannelFlags, channelFlags)
    ADDLENGTH(hasTxNess, tx_ness)
    ADDLENGTH(hasTuningPolicy, tuningPolicy)
    ADDLENGTH(hasPLLRate, pll_rate)
    ADDLENGTH(hasPLLRefDiv, pll_refdiv)
    ADDLENGTH(hasPLLClkSel, pll_clock_select)
    ADDLENGTH(hasAGC, agc)
    pos += this->hasAntennaSelection ? 1 : 0;

    return pos;
#undef ADDLENGTH
}

int ExtraInfo::toBuffer(uint8_t *buffer) const {
#define SETBUFF(hasV, v) \
    if (hasV) { \
        memcpy(buffer + pos, &v, sizeof(v)); \
        pos += sizeof(v); \
    } \

    *(uint32_t *) buffer = this->featureCode;
    uint16_t pos = 4;
    SETBUFF(hasLength, length)
    SETBUFF(hasVersion, version)
    SETBUFF(hasMacAddr_cur, macaddr_cur)
    SETBUFF(hasMacAddr_rom, macaddr_rom)
    SETBUFF(hasChansel, chansel)
    SETBUFF(hasBMode, bmode)
    SETBUFF(hasEVM, evm)
    SETBUFF(hasTxChainMask, txChainMask)
    SETBUFF(hasRxChainMask, rxChainMask)
    SETBUFF(hasTxpower, txpower)
    SETBUFF(hasCF, cf)
    SETBUFF(hasTxTSF, txTSF)
    SETBUFF(hasTxpower, txpower)
    SETBUFF(hasLastHWTxTSF, lastHwTxTSF)
    SETBUFF(hasChannelFlags, channelFlags)
    SETBUFF(hasTxNess, tx_ness)
    SETBUFF(hasTuningPolicy, tuningPolicy)
    SETBUFF(hasPLLRate, pll_rate)
    SETBUFF(hasPLLRefDiv, pll_refdiv)
    SETBUFF(hasPLLClkSel, pll_clock_select)
    SETBUFF(hasAGC, agc)
    if (hasAntennaSelection) {
        auto *antV = (uint8_t *) (buffer + pos++);
        *antV = (ant_sel[0] - 1) + ((unsigned) (ant_sel[1] - 1) << 2U) + ((unsigned) (ant_sel[2] - 1) << 4U);
    }

    return pos;
#undef SETBUFF
}

void ExtraInfo::setLength(uint16_t lengthV) {
    hasLength = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASLENGTH;
    ExtraInfo::length = lengthV;
}

void ExtraInfo::setVersion(uint64_t versionV) {
    hasVersion = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASVERSION;
    ExtraInfo::version = versionV;
    updateLength();
}

void ExtraInfo::setMacaddr_rom(const uint8_t addr_rom[6]) {
    hasMacAddr_rom = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASMACROM;
    memcpy(macaddr_rom, addr_rom, 6);
    updateLength();
}

void ExtraInfo::setMacaddr_cur(const uint8_t addr_cur[6]) {
    hasMacAddr_cur = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASMACCUR;
    memcpy(macaddr_cur, addr_cur, 6);
    updateLength();
}

void ExtraInfo::setChansel(uint32_t chanselV) {
    hasChansel = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASCHANSEL;
    ExtraInfo::chansel = chanselV;
    updateLength();
}

void ExtraInfo::setBmode(uint8_t bmodeV) {
    hasBMode = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASBMODE;
    ExtraInfo::bmode = bmodeV;
    updateLength();
}

void ExtraInfo::setTxChainMask(uint8_t txChainMaskV) {
    hasTxChainMask = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASTXCHAINMASK;
    ExtraInfo::txChainMask = txChainMaskV;
    updateLength();
}

void ExtraInfo::setRxChainMask(uint8_t rxChainMaskV) {
    hasRxChainMask = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASRXCHAINMASK;
    ExtraInfo::rxChainMask = rxChainMaskV;
    updateLength();
}

void ExtraInfo::setTxpower(uint8_t txpowerV) {
    hasTxpower = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASTXPOWER;
    ExtraInfo::txpower = txpowerV;
    updateLength();
}

void ExtraInfo::setCf(uint64_t cfV) {
    hasCF = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASCF;
    ExtraInfo::cf = cfV;
    updateLength();
}

void ExtraInfo::setTxTsf(uint32_t txTsfV) {
    hasTxTSF = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASTXTSF;
    txTSF = txTsfV;
    updateLength();
}

void ExtraInfo::setLastHwTxTsf(uint32_t lastHwTxTsfV) {
    hasLastHWTxTSF = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASLASTHWTXTSF;
    lastHwTxTSF = lastHwTxTsfV;
    updateLength();
}

void ExtraInfo::setChannelFlags(uint16_t channelFlagsV) {
    hasChannelFlags = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASCHANNELFLAGS;
    ExtraInfo::channelFlags = channelFlagsV;
    updateLength();
}

void ExtraInfo::setTxNess(uint8_t txNess) {
    hasTxNess = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASTXNESS;
    tx_ness = txNess;
    updateLength();
}

void ExtraInfo::setTuningPolicy(uint8_t tuningPolicyV) {
    hasTuningPolicy = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASTUNINGPOLICY;
    ExtraInfo::tuningPolicy = tuningPolicyV;
    updateLength();
}

void ExtraInfo::setPllRate(uint16_t pllRateV) {
    hasPLLRate = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASPLLRATE;
    pll_rate = pllRateV;
    updateLength();
}

void ExtraInfo::setPllRefdiv(uint8_t pllRefdivV) {
    hasPLLRefDiv = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASPLLREFDIV;
    pll_refdiv = pllRefdivV;
    updateLength();
}

void ExtraInfo::setPllClockSelect(uint8_t pllClockSelectV) {
    hasPLLClkSel = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASPLLCLKSEL;
    pll_clock_select = pllClockSelectV;
    updateLength();
}

void ExtraInfo::setAgc(uint8_t agcV) {
    hasAGC = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASAGC;
    ExtraInfo::agc = agcV;
    updateLength();
}

void ExtraInfo::setAntennaSelection(const uint8_t sel[3]) {
    hasAntennaSelection = true;
    featureCode |= PICOSCENES_EXTRAINFO_HASANTENNASELECTION;
    memcpy(ant_sel, sel, 3);
    updateLength();
}

void ExtraInfo::updateLength() {
    setLength(calculateBufferLength());
}

