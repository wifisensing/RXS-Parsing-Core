//
// Created by csi on 12/17/19.
//

#include "VariablePicoScenesFrame.hxx"

std::string DeviceType2String(PicoScenesDeviceType type) {
    switch (type) {
        case PicoScenesDeviceType::QCA9300:
            return "QCA9300";
        case PicoScenesDeviceType::IWL5300:
            return "IWL5300";
        case PicoScenesDeviceType::MAC80211Compatible:
            return "MAC80211 Compatible NIC";
        case PicoScenesDeviceType::USRP:
            return "USRP(SDR)";
        case PicoScenesDeviceType::Unknown:
            return "Unknown";
        default:
            throw std::runtime_error("unrecognized PicoScenesDeviceType.");
    }
}

std::ostream &operator<<(std::ostream &os, const PicoScenesDeviceType &deviceType) {
    os << DeviceType2String(deviceType);
    return os;
}

bool ieee80211_mac_frame_header_frame_control_field::operator==(const ieee80211_mac_frame_header_frame_control_field &rhs) const {
    return version == rhs.version &&
           type == rhs.type &&
           subtype == rhs.subtype &&
           toDS == rhs.toDS &&
           fromDS == rhs.fromDS &&
           moreFrags == rhs.moreFrags &&
           retry == rhs.retry &&
           power_mgmt == rhs.power_mgmt &&
           more == rhs.more &&
           protect == rhs.protect &&
           order == rhs.order;
}

std::string ieee80211_mac_frame_header::toString() const {
    std::stringstream ss;
    ss << "MACHeader:[dest[4-6]=" << std::nouppercase << std::setfill('0') << std::setw(2) << std::right << std::hex << int(addr1[3]) << ":" << int(addr1[4]) << ":" << int(addr1[5]) << ", ";
    ss << "src[4-6]=" << std::nouppercase << std::setfill('0') << std::setw(2) << std::right << std::hex << int(addr2[3]) << ":" << int(addr2[4]) << ":" << int(addr2[5]) << ", ";
    ss << "seq=" << std::dec << seq << ", frag=" << frag << ", ";
    ss << "mfrags=" << std::to_string(fc.moreFrags) << "]";
    return ss.str();
}

bool ieee80211_mac_frame_header::operator==(const ieee80211_mac_frame_header &rhs) const {
    return fc == rhs.fc &&
           addr1 == rhs.addr1 &&
           addr2 == rhs.addr2 &&
           addr3 == rhs.addr3 &&
           frag == rhs.frag &&
           seq == rhs.seq;
}

std::optional<RxSBasic> RxSBasic::fromBuffer(const uint8_t *buffer) {
    RxSBasic basic = *((RxSBasic *) (buffer));
    if (basic.num_tones != 52 && basic.num_tones != 56 && basic.num_tones != 114) {
        printf("RxBasic: Impossible values in nrx (%d), ntx (%d), or num_tones (%d).\n", basic.nrx, basic.ntx, basic.num_tones);
        return std::nullopt;
    }
    return basic;
}

std::string RxSBasic::toString() const {
    std::stringstream ss;
    ss << "RxS:[device=" + DeviceType2String((PicoScenesDeviceType(deviceType))) + ", freq=" + std::to_string(channel) + ", bonding=" + std::to_string(channelBonding) + ", MCS=" + std::to_string(rate) + ", SGI=" + std::to_string(sgi) + ", CSI=" + std::to_string(csi_len) + "B(" + std::to_string(ntx) + ", " + std::to_string(nrx) + ", " + std::to_string(num_tones) + "), LTF=" + std::to_string(nltf) + ", NSS=" + std::to_string(nss) + ", timestamp=" + std::to_string(tstamp) + "]";
    return ss.str();
}

bool PicoScenesRxFrameStructure::isOldRXSEnhancedFrame(const uint8_t bufferHead[6]) {
    auto magicValue = *((uint32_t *) (bufferHead + 2));
    return (magicValue == 0x20150315 || magicValue == 0x20120930);
}

std::optional<PicoScenesFrameHeader> PicoScenesFrameHeader::fromBuffer(const uint8_t *buffer) {
    auto magicValue = *((uint32_t *) (buffer));
    if (magicValue == 0x20150315 || magicValue == 0x20120930) {
        auto frameHeader = *((PicoScenesFrameHeader *) (buffer));
        return frameHeader;
    }
    return std::nullopt;
}

std::string PicoScenesFrameHeader::toString() const {
    std::stringstream ss;
    ss << "PSFHeader:[ver=0x" << std::hex << version << std::dec << ", device=" << deviceType << ", seg=" << int(segments) << ", type=" << int(frameType) << ", taskId=" << int(taskId) << "]";
    return ss.str();
}

bool PicoScenesFrameHeader::operator==(const PicoScenesFrameHeader &rhs) const {
    return magicValue == rhs.magicValue &&
           version == rhs.version &&
           deviceType == rhs.deviceType &&
           segments == rhs.segments &&
           frameType == rhs.frameType &&
           taskId == rhs.taskId;
}

std::string CSIData::toString() const {
    std::string baseString;
    char headerLineChar[128];
    std::sprintf(headerLineChar, "CSI Print [NTx=%u, NRx=%u, NLTF=%u, NSS=%u, NTONES=%u]\n", ntx, nrx, nltf, nss, num_tones);
    std::string headerLine(headerLineChar);
    headerLine += "-------------------------------\n";
    std::string tabularContent;
    for (int i = 0; i < num_tones; ++i) {
        char lineStringChar[50];
        std::sprintf(lineStringChar, "Tone #%4d || ", i);
        std::string lineString(lineStringChar);
        for (int j = 0; j < nss; ++j) {
            char dataChar[256];
            auto pos = num_tones * j + i;
            std::sprintf(dataChar, "#%d(%6.2f %6.2fi %5.2f %6.2f) ", j, csi_matrix[pos].real(), csi_matrix[pos].imag(), unwrappedMag[pos], unwrappedPhase[pos]);
            std::string dataString(dataChar);
            lineString += dataString;
        }
        lineString += "\n";
        tabularContent += lineString;
    }
//
    baseString += headerLine;
    baseString += tabularContent;
    baseString += "-------------------------------\n";
    return baseString;
}


std::optional<PicoScenesRxFrameStructure> PicoScenesRxFrameStructure::fromBuffer(const uint8_t *buffer, std::optional<uint32_t> bufferLength, enum RXSParsingLevel parsingLevel) {
    if (PicoScenesRxFrameStructure::isOldRXSEnhancedFrame(buffer)) {
        return PicoScenesRxFrameStructure::fromRXSEnhancedBuffer(buffer);
    }

    uint16_t totalLength = *((uint16_t *) (buffer));
    uint16_t pos = 2;

    if (bufferLength && totalLength + 2U != *bufferLength) {
//        printf("Corrupted PicoScenes frame, extracted length:%u, supplied length:%u\n", totalLength, *bufferLength);
        return {};
    }

    PicoScenesRxFrameStructure rxFrame;
    if (auto basic = RxSBasic::fromBuffer(buffer + pos)) {
        rxFrame.rxs_basic = *basic;
        pos += sizeof(struct RxSBasic);
        rxFrame.deviceType = PicoScenesDeviceType(rxFrame.rxs_basic.deviceType);
    } else
        return std::nullopt;

    pos += ExtraInfo::fromBinary(buffer + pos, &rxFrame.rxExtraInfo);
    if (rxFrame.rxExtraInfo.hasEVM) {
        for (auto &evm : rxFrame.rxExtraInfo.evm) {
            evm += rxFrame.rxs_basic.noise;
            if (evm > 0) {
                evm = -128;
            }
        }
    }

    if (parsingLevel >= RXSParsingLevel::EXTRA_CSI) {
        if (rxFrame.deviceType == PicoScenesDeviceType::QCA9300)
            ar_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.csi.csi_matrix);
        else if (rxFrame.deviceType == PicoScenesDeviceType::IWL5300)
            iwl_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.ntx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nltf, rxFrame.rxs_basic.num_tones, rxFrame.rxExtraInfo, rxFrame.csi.csi_matrix);
        else if (rxFrame.deviceType == PicoScenesDeviceType::USRP)
            usrp_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.csi.csi_matrix);
        // commit the following two lines to acquire raw csi matrix
        auto new_tones_num = phaseUnwrapAroundDC(rxFrame.csi.csi_matrix, rxFrame.csi.unwrappedMag, rxFrame.csi.unwrappedPhase, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.rxs_basic.channelBonding);
        rxFrame.rxs_basic.num_tones = new_tones_num;
        rxFrame.csi.ntx = rxFrame.rxs_basic.ntx;
        rxFrame.csi.nrx = rxFrame.rxs_basic.nrx;
        rxFrame.csi.nss = rxFrame.rxs_basic.nss;
        rxFrame.csi.nltf = rxFrame.rxs_basic.nltf;
        rxFrame.csi.num_tones = rxFrame.rxs_basic.num_tones;
    }
    pos += rxFrame.rxs_basic.csi_len;

    rxFrame.posMSDU = pos;
    rxFrame.standardHeader = *((ieee80211_mac_frame_header *) (buffer + pos));
    pos += sizeof(ieee80211_mac_frame_header);

    if (auto PSHeader = PicoScenesFrameHeader::fromBuffer(buffer + pos)) {
        rxFrame.PicoScenesHeader = *PSHeader;
        rxFrame.posPicoScenesHeader = pos;
        pos += sizeof(PicoScenesFrameHeader);

        if (rxFrame.PicoScenesHeader->magicValue != 0x20150315) {
            printf("PicoScenesHeader->magicValue is not correct.\n");
            return std::nullopt;
        }

        rxFrame.posSegments = pos;
        char identifier[3];
        identifier[2] = '\0';
        for (auto i = 0; i < rxFrame.PicoScenesHeader->segments; i++) {
            identifier[0] = *((char *) (buffer + pos++));
            identifier[1] = *((char *) (buffer + pos++));
            std::string identifierString = identifier;

            if (identifierString == "EI") { // Tx ExtraInfo is a special case.
                rxFrame.posExtraInfo = pos - 2;
                if (auto extraInfo = ExtraInfo::fromBuffer(buffer + pos)) {
                    rxFrame.txExtraInfo = extraInfo;
                    pos += extraInfo->length + 6;
                    rxFrame.posSegments = pos;
                    continue;
                }
                return std::nullopt;
            }

            if (!rxFrame.segmentMap) {
                rxFrame.posSegments = pos - 2;
                rxFrame.segmentMap = std::map<std::string, std::pair<uint32_t, std::shared_ptr<uint8_t>>>();
            }


            auto segmentLength = *((uint16_t *) (buffer + pos));
            pos += 2;
            auto segmentBuffer = std::shared_ptr<uint8_t>(new uint8_t[segmentLength], std::default_delete<uint8_t[]>());
            memcpy(segmentBuffer.get(), buffer + pos, segmentLength);
            rxFrame.segmentMap->emplace(std::make_pair(identifierString, std::make_pair(segmentLength, segmentBuffer)));
            pos += segmentLength;
        }

        if (pos == totalLength + 2) {
            rxFrame.rawBuffer = std::shared_ptr<uint8_t>(new uint8_t[pos], std::default_delete<uint8_t[]>());
            memcpy(rxFrame.rawBuffer.get(), buffer, pos);
            rxFrame.rawBufferLength = pos;
            return rxFrame;
        }
        return std::nullopt;
    }

    if (pos > totalLength + 2)
        return std::nullopt;

    uint32_t msduLength = totalLength + 2 - pos;
    auto msduBuffer = std::shared_ptr<uint8_t>(new uint8_t[msduLength], std::default_delete<uint8_t[]>());
    memcpy(msduBuffer.get(), buffer + pos, msduLength);
    rxFrame.msduBuffer = std::make_pair(msduBuffer, msduLength);
    return rxFrame;
}

std::string PicoScenesRxFrameStructure::toString() const {
    std::stringstream ss;
    ss << "RxFrame:{" << rxs_basic << ", " << standardHeader;
    if (PicoScenesHeader) {
        ss << ", " << PicoScenesHeader->toString();
    }
    ss << ", Rx" << rxExtraInfo;
    if (txExtraInfo)
        ss << ", Tx" << txExtraInfo->toString();

    if (segmentMap) {
        std::stringstream segss;
        segss << "Segment:(";
        for (const auto &pair: *segmentMap) {
            segss << pair.first << ":" << pair.second.first << "B, ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }
    ss << "}";
    return ss.str();
}

int PicoScenesRxFrameStructure::addOrReplaceSegment(const std::pair<std::string, std::pair<uint32_t, std::shared_ptr<uint8_t>>> &newSegment) {
    if (!PicoScenesHeader)
        return 1;

    if (!segmentMap)
        segmentMap = std::map<std::string, std::pair<uint32_t, std::shared_ptr<uint8_t>>>();

    segmentMap->erase(newSegment.first);
    segmentMap->emplace(newSegment);
    uint16_t newBufferLength = *posSegments;
    for (const auto &segmentPair : *segmentMap) {
        newBufferLength += (4 + segmentPair.second.first); // 4B for 2B segment code and 2B length
    }
    auto newBuffer = std::shared_ptr<uint8_t>(new uint8_t[newBufferLength], std::default_delete<uint8_t[]>());
    memcpy(newBuffer.get(), rawBuffer.get(), *posSegments);

    // modify MACHeader
    auto *macHeader = (ieee80211_mac_frame_header *) (newBuffer.get() + posMSDU);
    macHeader->fc.moreFrags = 0;
    // modify PSHeader
    auto *PSHeader = (PicoScenesFrameHeader *) (newBuffer.get() + *posPicoScenesHeader);
    PSHeader->segments = segmentMap->size() + (txExtraInfo ? 1 : 0);
    // modify totalLength
    newBufferLength -= 2;
    memcpy(newBuffer.get(), &newBufferLength, 2);
    newBufferLength += 2;

    // re-copy all segments
    auto curPos = *posSegments;
    for (const auto &originalSegment: *segmentMap) {
        *((char *) (newBuffer.get() + curPos++)) = originalSegment.first.c_str()[0];
        *((char *) (newBuffer.get() + curPos++)) = originalSegment.first.c_str()[1];
        *((uint16_t *) (newBuffer.get() + curPos)) = originalSegment.second.first;
        curPos += 2;
        memcpy(newBuffer.get() + curPos, originalSegment.second.second.get(), originalSegment.second.first);
        curPos += originalSegment.second.first;
    }
    if (curPos != newBufferLength)
        return 1;
    rawBuffer = newBuffer;
    rawBufferLength = newBufferLength;
    PicoScenesHeader->segments = segmentMap->size() + (txExtraInfo ? 1 : 0);

    return 0;
}

bool PicoScenesRxFrameStructure::operator==(const PicoScenesRxFrameStructure &rhs) const {
    auto headerEq = standardHeader == rhs.standardHeader;
    auto psHeaderEq = PicoScenesHeader == rhs.PicoScenesHeader;
    if (!headerEq && psHeaderEq && standardHeader.frag == rhs.standardHeader.frag && standardHeader.fc == rhs.standardHeader.fc)
        headerEq = true;
    auto lengthEq = rawBufferLength == rhs.rawBufferLength;
    return headerEq && psHeaderEq && lengthEq;
}

std::optional<PicoScenesRxFrameStructure> PicoScenesRxFrameStructure::concatenateFragmentedPicoScenesRxFrames(const std::vector<PicoScenesRxFrameStructure> &frameQueue) {
    std::set<std::string> segmentNames;
    for (const auto &frame: frameQueue) {
        std::transform(frame.segmentMap->cbegin(), frame.segmentMap->cend(), std::inserter(segmentNames, segmentNames.end()), [](const auto &pair) {
            return pair.first;
        });
    }
    std::map<std::string, std::pair<uint32_t, std::shared_ptr<uint8_t>>> mergedSegmentMap;
    for (const auto &segmentName: segmentNames) {
        uint32_t segmentLength = 0;
        for (const auto &frame : frameQueue)
            segmentLength += frame.segmentMap->at(segmentName).first;
        auto segmentBuffer = std::shared_ptr<uint8_t>(new uint8_t[segmentLength], std::default_delete<uint8_t[]>());

        auto curPos = 0;
        for (const auto &frame : frameQueue) {
            if (frame.segmentMap->find(segmentName) != frame.segmentMap->cend()) {
                memcpy(segmentBuffer.get() + curPos, frame.segmentMap->at(segmentName).second.get(), frame.segmentMap->at(segmentName).first);
                curPos += frame.segmentMap->at(segmentName).first;
            }
        }
        mergedSegmentMap.emplace(std::make_pair(segmentName, std::make_pair(segmentLength, segmentBuffer)));
    }

    auto merged = frameQueue.front();
    bool updateSegmentFailed = false;
    for (const auto &pair : mergedSegmentMap) {
        if (merged.addOrReplaceSegment(pair))
            return std::nullopt;
    }
    return merged;
}

std::string PicoScenesFrameTxParameters::toString() const {
    std::stringstream ss;
    ss << "tx_param[mcs=" << int(mcs) << ", bonding=" << channelBonding << ", sgi=" << sgi << ", gf=" << greenField << ", sounding=" << forceSounding << ", LDPC=" << useLDPC << "]";
    return ss.str();
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::addSegmentBuffer(const std::string &identifier, const uint8_t *buffer, uint16_t length) {
    if (length > PICOSCENES_FRAME_SEGMENT_MAX_LENGTH)
        throw std::overflow_error("PicoScenes Frame segment max length :PICOSCENES_FRAME_SEGMENT_MAX_LENGTH");
    auto bufferArray = std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH>();
    memcpy(bufferArray.data(), buffer, length);
    addSegmentBuffer(identifier, bufferArray, length);
    frameHeader.segments = segmentLength.size() + (extraInfo ? 1 : 0);
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::addSegmentBuffer(const std::string &identifier, const std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH> &bufferArray, uint16_t length) {
    if (segmentLength.find(identifier) != segmentLength.end())
        throw std::runtime_error("Duplicated segment buffer identifier: " + identifier);

    segmentBuffer.emplace(std::make_pair(identifier, bufferArray));
    segmentLength.emplace(std::make_pair(identifier, length));
    frameHeader.segments = segmentLength.size() + (extraInfo ? 1 : 0);
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::addExtraInfo(const ExtraInfo &txExtraInfo) {
    this->extraInfo = txExtraInfo;
    frameHeader.segments = segmentLength.size() + 1;
    return *this;
}

int PicoScenesTxFrameStructure::toBuffer(uint8_t *buffer, std::optional<uint16_t> bufferLength) const {
    if (bufferLength) {
        if (*bufferLength < totalLength())
            throw std::overflow_error("Buffer not long enough for TX frame dumping...");
    }
    if (segmentLength.size() + (extraInfo ? 1 : 0) != frameHeader.segments)
        throw std::overflow_error("PicoScenesTxFrameStructure toBuffer method segment number in-consistent!");

    memcpy(buffer, &standardHeader, sizeof(ieee80211_mac_frame_header));
    memcpy(buffer + sizeof(ieee80211_mac_frame_header), &frameHeader, sizeof(PicoScenesFrameHeader));
    uint16_t pos = sizeof(ieee80211_mac_frame_header) + sizeof(PicoScenesFrameHeader);

    if (extraInfo) {
        *((char *) (buffer + pos++)) = 'E';
        *((char *) (buffer + pos++)) = 'I';
        auto extraLength = extraInfo->toBuffer(buffer + pos);
        pos += extraLength;
    }

    for (const auto &segmentPair : segmentLength) {
        *((char *) (buffer + pos++)) = segmentPair.first.c_str()[0];
        *((char *) (buffer + pos++)) = segmentPair.first.c_str()[1];
        *((uint16_t *) (buffer + pos)) = segmentPair.second;
        pos += 2;
        memcpy(buffer + pos, segmentBuffer.at(segmentPair.first).data(), segmentPair.second);
        pos += segmentPair.second;
    }

    return pos;
}

uint16_t PicoScenesTxFrameStructure::totalLength() const {
    uint16_t length = sizeof(decltype(standardHeader)) + sizeof(decltype(frameHeader));
    if (extraInfo) {
        length += extraInfo->calculateBufferLength() + 6; // 4B for Feature Code, 2B for 'EI'
    }
    for (const auto &segmentPair : segmentLength) {
        length += (4 + segmentPair.second); // 4B for 2B segment code and 2B length
    }
    return length;
}

std::shared_ptr<uint8_t> PicoScenesTxFrameStructure::toBuffer() const {
    auto length = totalLength();
    auto builtBuffer = std::shared_ptr<uint8_t>(new uint8_t[length], std::default_delete<uint8_t[]>());
    toBuffer(builtBuffer.get(), length);
    return builtBuffer;
}

void PicoScenesTxFrameStructure::reset() {
    standardHeader = ieee80211_mac_frame_header();
    frameHeader = PicoScenesFrameHeader();
    txParameters = PicoScenesFrameTxParameters();
    extraInfo = std::nullopt;
    segmentBuffer.clear();
    segmentLength.clear();
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setMoreFrags() {
    standardHeader.fc.moreFrags = 1;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setFragNumber(uint8_t fragNumber) {
    standardHeader.frag = fragNumber;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setRetry() {
    standardHeader.fc.retry = 1;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setTaskId(uint16_t taskId) {
    frameHeader.taskId = taskId;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setRandomTaskId() {
    static std::random_device r;
    static std::default_random_engine randomEngine(r());
    static std::uniform_int_distribution<uint16_t> randomGenerator(10000, UINT16_MAX);
    auto newValue = randomGenerator(randomEngine);
    setTaskId(newValue);
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setPicoScenesFrameType(uint8_t frameType) {
    frameHeader.frameType = frameType;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setChannelBonding(bool useChannelBonding) {
    txParameters.channelBonding = useChannelBonding;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setSGI(bool useSGI) {
    txParameters.sgi = useSGI;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setMCS(uint8_t mcs) {
    txParameters.mcs = mcs;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setGreenField(bool useGreenField) {
    txParameters.greenField = useGreenField;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setDestinationAddress(const uint8_t macAddr[6]) {
    memcpy(standardHeader.addr1, macAddr, 6);
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setSourceAddress(const uint8_t macAddr[6]) {
    memcpy(standardHeader.addr2, macAddr, 6);
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::set3rdAddress(const uint8_t macAddr[6]) {
    memcpy(standardHeader.addr3, macAddr, 6);
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setForceSounding(bool forceSounding) {
    txParameters.forceSounding = forceSounding;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::useLDPC(bool useLDPC) {
    txParameters.useLDPC = useLDPC;
    return *this;
}

std::string PicoScenesTxFrameStructure::toString() const {
    std::stringstream ss;
    ss << "TxFrame:{" << standardHeader;
    ss << ", " << frameHeader.toString();
    ss << ", " << txParameters;
    if (extraInfo)
        ss << ", Tx" << extraInfo->toString();

    if ((frameHeader.segments > 1 && extraInfo) || (frameHeader.segments > 0 && !extraInfo)) {
        std::stringstream segss;
        segss << "Segment:(";
        for (const auto &pair: segmentLength) {
            segss << pair.first << ":" << pair.second << "B, ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }
    ss << "}";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header) {
    os << header.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameHeader &frameHeader) {
    os << frameHeader.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const RxSBasic &rxSBasic) {
    os << rxSBasic.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const PicoScenesRxFrameStructure &rxframe) {
    os << rxframe.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const PicoScenesTxFrameStructure &txframe) {
    os << txframe.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameTxParameters &parameters) {
    os << parameters.toString();
    return os;
}

std::ostream &operator<<(std::ostream &os, const CSIData &data) {
    os << data.toString();
    return os;
}
