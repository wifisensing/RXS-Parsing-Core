//
// Created by 蒋志平 on 2020/11/6.
//

#include "ModularPicoScenesFrame.hxx"
#include <boost/algorithm/string.hpp>
#include <random>
#include <iomanip>


std::string ieee80211_mac_frame_header::toString() const {
    std::stringstream ss;
    ss << "MACHeader:[dest[4-6]=" << std::nouppercase << std::setfill('0') << std::setw(2) << std::right << std::hex << int(addr1[3]) << ":" << int(addr1[4]) << ":" << int(addr1[5]) << ", ";
    ss << "src[4-6]=" << std::nouppercase << std::setfill('0') << std::setw(2) << std::right << std::hex << int(addr2[3]) << ":" << int(addr2[4]) << ":" << int(addr2[5]) << ", ";
    ss << "seq=" << std::dec << seq << ", frag=" << frag << ", ";
    ss << "mfrags=" << std::to_string(fc.moreFrags) << "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header) {
    os << header.toString();
    return os;
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
    ss << "PSFHeader:[ver=0x" << std::hex << version << std::dec << ", device=" << deviceType << ", numSegs=" << int(numSegments) << ", type=" << int(frameType) << ", taskId=" << int(taskId) << "]";
    return ss.str();
}


std::optional<ModularPicoScenesRxFrame> ModularPicoScenesRxFrame::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    uint32_t pos = 0;

    // first 4 bytes should be the buffer length
    if (*(uint32_t *) buffer + 4 == bufferLength) {
        pos += 4;
    } else {
        return {};
    }

    // next 4 bytes should be the magic number
    if (*(uint32_t *) (buffer + pos) != 0x20150315)
        return {};
    pos += 4;

    // next 2 bytes is the frame version
    uint16_t frameVersion = *(uint16_t *) (buffer + pos);
    pos += 2;
    if (frameVersion != 0x1U)
        return {};
    uint8_t numRxSegments = *(uint16_t *) (buffer + pos++);

    auto frame = ModularPicoScenesRxFrame();
    for (auto i = 0 ; i < numRxSegments; i ++) {
        auto[segmentName, segmentLength, versionId, offset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(buffer + pos, bufferLength - pos);
        if (boost::iequals(segmentName, "RxSBasic")) {
            frame.rxSBasicSegment.fromBuffer(buffer + pos, segmentLength + 4);
        } else if (boost::iequals(segmentName, "ExtraInfo")) {
            frame.rxExtraInfoSegment.fromBuffer(buffer + pos, segmentLength + 4);
        } else if (boost::iequals(segmentName, "CSI")) {
            frame.csiSegment.fromBuffer(buffer + pos, segmentLength + 4);
        } else {
            frame.rxUnknownSegmentMap.emplace(segmentName, Uint8Vector(buffer + pos, buffer + pos + segmentLength + 4));
        }
        pos += (segmentLength + 4);
    }

    frame.standardHeader = *((ieee80211_mac_frame_header *) (buffer + pos));
    pos += sizeof(ieee80211_mac_frame_header);

    if (auto PSHeader = PicoScenesFrameHeader::fromBuffer(buffer + pos)) {
        frame.PicoScenesHeader = *PSHeader;
        pos += sizeof(PicoScenesFrameHeader);

        for (auto i = 0; i < frame.PicoScenesHeader->numSegments; i++) {
            auto[segmentName, segmentLength, versionId, offset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(buffer + pos, bufferLength);
            if (boost::iequals(segmentName, "ExtraInfo")) {
                frame.txExtraInfoSegment.fromBuffer(buffer + pos, segmentLength);
            } else {
                frame.txUnknownSegmentMap.emplace(segmentName, Uint8Vector(buffer + pos, buffer + pos + segmentLength));
            }
        }
    } else {
        uint32_t msduLength = bufferLength - pos;
        auto msduBuffer = std::shared_ptr<uint8_t>(new uint8_t[msduLength], std::default_delete<uint8_t[]>());
        std::copy(buffer + pos, buffer + bufferLength, frame.nonPicoScenesMSDUContent.begin());
    }

    frame.rawBuffer.resize(bufferLength);
    std::copy(buffer, buffer + pos, frame.rawBuffer.begin());

    return frame;
}

std::string ModularPicoScenesRxFrame::toString() const {
    return std::string();
}

std::optional<ModularPicoScenesRxFrame> ModularPicoScenesRxFrame::concatenateFragmentedPicoScenesRxFrames(const std::vector<ModularPicoScenesRxFrame> &frameQueue) {
    return std::optional<ModularPicoScenesRxFrame>();
}

bool ModularPicoScenesRxFrame::operator==(const ModularPicoScenesRxFrame &rhs) const {
    return false;
}

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesRxFrame &rxframe) {
    os << rxframe.toString();
    return os;
}

uint32_t ModularPicoScenesTxFrame::totalLength() const {
    uint32_t length = sizeof(decltype(standardHeader)) + sizeof(decltype(frameHeader));
    for (const auto &segment : segments) {
        length += segment->totalLength() + 4;
    }
    return length;
}

Uint8Vector ModularPicoScenesTxFrame::toBuffer() const {
    auto bufferLength = totalLength();
    Uint8Vector buffer(bufferLength);
    toBuffer(&buffer[0], bufferLength);
    return buffer;
}

int ModularPicoScenesTxFrame::toBuffer(uint8_t *buffer, uint32_t bufferLength) const {
    if (bufferLength < totalLength())
        throw std::overflow_error("Buffer not long enough for TX frame dumping...");

    if (frameHeader.numSegments != segments.size())
        throw std::overflow_error("ModularPicoScenesTxFrame toBuffer method segment number in-consistent!");

    memcpy(buffer, &standardHeader, sizeof(ieee80211_mac_frame_header));
    memcpy(buffer + sizeof(ieee80211_mac_frame_header), &frameHeader, sizeof(PicoScenesFrameHeader));
    uint32_t pos = sizeof(ieee80211_mac_frame_header) + sizeof(PicoScenesFrameHeader);

    for (const auto &segment : segments) {
        segment->toBuffer(true, buffer + pos);
        pos += segment->totalLength() + 4;
    }

    return pos;
}


void ModularPicoScenesTxFrame::reset() {
    standardHeader = ieee80211_mac_frame_header();
    frameHeader = PicoScenesFrameHeader();
    txParameters = PicoScenesFrameTxParameters();
    segments.clear();
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setMoreFrags() {
    standardHeader.fc.moreFrags = 1;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setFragNumber(uint8_t fragNumber) {
    standardHeader.frag = fragNumber;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setRetry() {
    standardHeader.fc.retry = 1;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setTaskId(uint16_t taskId) {
    frameHeader.taskId = taskId;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setRandomTaskId() {
    static std::random_device r;
    static std::default_random_engine randomEngine(r());
    static std::uniform_int_distribution<uint16_t> randomGenerator(10000, UINT16_MAX);
    auto newValue = randomGenerator(randomEngine);
    setTaskId(newValue);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setPicoScenesFrameType(uint8_t frameType) {
    frameHeader.frameType = frameType;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setChannelBandwidth(const ChannelBandwidthEnum &cbw) {
    txParameters.cbw = cbw;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setGuardInterval(GuardIntervalEnum guardInterval) {
    txParameters.guardInterval = guardInterval;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setMCS(uint8_t mcs) {
    txParameters.mcs.clear();
    txParameters.mcs.emplace_back(mcs);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setNumSTS(uint8_t numSTS) {
    txParameters.numSTS.clear();
    txParameters.numSTS.emplace_back(numSTS);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setNumSTS(const std::vector<uint8_t> &numSTSs) {
    txParameters.numSTS = numSTSs;
    return *this;
}


ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setMCS(const std::vector<uint8_t> &mcs) {
    txParameters.mcs = mcs;
    return *this;
}


ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setDestinationAddress(const uint8_t macAddr[6]) {
    memcpy(standardHeader.addr1, macAddr, 6);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setSourceAddress(const uint8_t macAddr[6]) {
    memcpy(standardHeader.addr2, macAddr, 6);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::set3rdAddress(const uint8_t macAddr[6]) {
    memcpy(standardHeader.addr3, macAddr, 6);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setForceSounding(bool forceSounding) {
    txParameters.forceSounding = forceSounding;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setNumExtraSounding(uint8_t numExtraSounding) {
    txParameters.numExtraSounding = numExtraSounding;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setChannelCoding(ChannelCodingEnum codings) {
    txParameters.coding.clear();
    txParameters.coding.emplace_back(codings);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setChannelCoding(const std::vector<ChannelCodingEnum> &channelCoding) {
    txParameters.coding = channelCoding;
    return *this;
}

std::string ModularPicoScenesTxFrame::toString() const {
    std::stringstream ss;
    ss << "TxFrame:{" << standardHeader;
    ss << ", " << frameHeader.toString();
    ss << ", " << txParameters;

    if (!segments.empty()) {
        std::stringstream segss;
        segss << "Segment:(";
        for (const auto &segment: segments) {
            segss << segment->segmentName << ":" << segment->segmentLength << "B, ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }
    ss << "}";
    return ss.str();
}


std::ostream &operator<<(std::ostream &os, const ModularPicoScenesTxFrame &txframe) {
    os << txframe.toString();
    return os;
}
