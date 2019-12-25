//
// Created by csi on 12/17/19.
//

#include "VariablePicoScenesFrame.hxx"


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

std::optional<PicoScenesRxFrameStructure> PicoScenesRxFrameStructure::fromBuffer(const uint8_t *buffer, uint32_t start, enum RXSParsingLevel parsingLevel) {
    uint16_t totalLength = 2;
    uint16_t pos = start;
    PicoScenesRxFrameStructure rxFrame;
    totalLength += *((uint16_t *) (buffer + pos));
    pos += 2;

    rxFrame.rxs_basic = *((struct rx_status_basic *) (buffer + pos));
    pos += sizeof(struct rx_status_basic);
    if (rxFrame.rxs_basic.nrx <= 0 || rxFrame.rxs_basic.nrx > 3 || rxFrame.rxs_basic.ntx <= 0 || rxFrame.rxs_basic.ntx > 3) {
        printf("RXS Parser: Impossible values in nrx (%d), ntx (%d), or num_tones (%d). Error occurs in file format or parsing.\n", rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.ntx, rxFrame.rxs_basic.num_tones);
        return std::nullopt;
    }

    ExtraInfo::fromBinary(buffer + pos, &rxFrame.rxExtraInfo);
    pos += rxFrame.rxExtraInfo.length + 2;
    if (rxFrame.rxExtraInfo.hasEVM) {
        for (auto &evm : rxFrame.rxExtraInfo.evm) {
            evm += rxFrame.rxs_basic.noise;
            if (evm > 0) {
                evm = -128;
            }
        }
    }

    if (parsingLevel >= EXTRA_CSI) {
        if (rxFrame.deviceType == PicoScenesDeviceType::QCA9300)
            ar_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.csi.csi_matrix);
        else
            iwl_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.ntx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nltf, rxFrame.rxs_basic.num_tones, rxFrame.rxExtraInfo, rxFrame.csi.csi_matrix);

        // commit the following two lines to acquire raw csi matrix
        auto new_tones_num = phaseUnwrapAroundDC(rxFrame.csi.csi_matrix, rxFrame.csi.unwrappedMag, rxFrame.csi.unwrappedPhase, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.rxs_basic.channelBonding);
        rxFrame.rxs_basic.num_tones = new_tones_num;
    }
    pos += rxFrame.rxs_basic.csi_len;

    if (auto extraLength = rxFrame.parseRxMACFramePart(buffer + pos)) {
        pos += *extraLength;
    } else
        return std::nullopt;

    if (pos == totalLength)
        return rxFrame;
    return std::nullopt;
}

std::optional<uint16_t> PicoScenesRxFrameStructure::parseRxMACFramePart(const uint8_t *buffer) {
    uint16_t pos = 0;
    this->standardHeader = *((ieee80211_mac_frame_header *) (buffer + pos));
    pos += sizeof(ieee80211_mac_frame_header);

    this->PicoScenesHeader = PicoScenesFrameHeader::fromBuffer(buffer + pos);
    pos += sizeof(PicoScenesFrameHeader);

    if (this->PicoScenesHeader) {
        if (this->PicoScenesHeader->magicValue != 0x20150315)
            return std::nullopt;
        this->frameSegmentStarts = std::vector<uint16_t>(this->PicoScenesHeader->segments);
        this->segmentMap = std::map<std::string, std::shared_ptr<uint8_t>>();
        char identifier[3];
        identifier[2] = '\0';
        for (auto i = 0; i < this->PicoScenesHeader->segments; i++) {
            identifier[0] = *((char *) (buffer + pos++));
            identifier[1] = *((char *) (buffer + pos++));
            std::string identifierString = identifier;

            if (identifierString == "EI") { // Tx ExtraInfo is a special case.
                if (auto extraInfo = ExtraInfo::fromBuffer(buffer + pos)) {
                    this->txExtraInfo = extraInfo;
                    pos += extraInfo->length;
                    continue;
                }
                return std::nullopt;
            }

            auto segmentLength = *((uint16_t *) (buffer + pos));
            auto segmentBuffer = std::shared_ptr<uint8_t>(new uint8_t[segmentLength], std::default_delete<uint8_t[]>());
            memcpy(segmentBuffer.get(), buffer + pos, segmentLength);
            pos += segmentLength;
            this->segmentMap->emplace(std::make_pair(identifierString, segmentBuffer));
        }
        return pos;
    }
    return std::nullopt;
}

PicoScenesTxFrameStructure & PicoScenesTxFrameStructure::addSegmentBuffer(const std::string &identifier, const uint8_t *buffer, uint16_t length) {
    if (length > PICOSCENES_FRAME_SEGMENT_MAX_LENGTH)
        throw std::overflow_error("PicoScenes Frame segment max length :PICOSCENES_FRAME_SEGMENT_MAX_LENGTH");
    auto bufferArray = std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH>();
    memcpy(bufferArray.data(), buffer, length);
    addSegmentBuffer(identifier, bufferArray, length);
    frameHeader.segments = segmentLength.size() + (extraInfo ? 1 : 0);
    return *this;
}

PicoScenesTxFrameStructure & PicoScenesTxFrameStructure::addSegmentBuffer(const std::string &identifier, const std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH> &bufferArray, uint16_t length) {
    if (segmentLength.find(identifier) != segmentLength.end())
        throw std::runtime_error("Duplicated segment buffer identifier: " + identifier);

    segmentBuffer.emplace(std::make_pair(identifier, bufferArray));
    segmentLength.emplace(std::make_pair(identifier, length));
    frameHeader.segments = segmentLength.size() + (extraInfo ? 1 : 0);
    return *this;
}

PicoScenesTxFrameStructure & PicoScenesTxFrameStructure::addExtraInfo(const ExtraInfo &txExtraInfo) {
    this->extraInfo = txExtraInfo;
    frameHeader.segments = segmentLength.size() + 1;
    return *this;
}

int PicoScenesTxFrameStructure::toBuffer(uint8_t *buffer, std::optional<uint16_t> bufferLength) const {
    if (bufferLength) {
        if (*bufferLength < totalLength())
            throw std::overflow_error("Buffer not long enough for TX frame dumping...");
    }
    if(segmentLength.size() + (extraInfo ? 1 : 0) != frameHeader.segments)
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
        pos += 2;
        memcpy(buffer + pos, segmentBuffer.at(segmentPair.first).data(), segmentPair.second);
        pos += segmentPair.second;
    }

    return pos;
}

uint16_t PicoScenesTxFrameStructure::totalLength() const {
    uint16_t length = sizeof(decltype(standardHeader)) + sizeof(decltype(frameHeader));
    if (extraInfo) {
        length += (2 + extraInfo->calculateBufferLength());
    }
    for (const auto &segmentPair : segmentLength) {
        length += (2 + segmentPair.second);
    }
    return length;
}

std::shared_ptr<uint8_t[]> PicoScenesTxFrameStructure::toBuffer() const {
    auto length = totalLength();
    auto builtBuffer = std::shared_ptr<uint8_t[]>(new uint8_t[length]);
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

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setRetry() {
    standardHeader.fc.retry = 1;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setTaskId(uint16_t taskId) {
    frameHeader.taskId = taskId;
    return *this;
}

PicoScenesTxFrameStructure &PicoScenesTxFrameStructure::setRandomTaskId() {
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
    txParameters.greenField = true;
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
