//
// Created by Zhiping Jiang on 5/22/21.
//

#include "DPASRequestSegment.hxx"

struct DPASRequestV1 {
    uint16_t batchId;
    uint16_t batchLength;
    uint16_t sequenceId;
    uint16_t intervalTime;

} __attribute__ ((__packed__));

struct DPASRequestV2 {
    uint16_t batchId;
    uint16_t batchLength;
    uint16_t sequenceId;
    uint16_t intervalTime;
    uint16_t intervalStep;

} __attribute__ ((__packed__));

struct DPASRequestV3 {
    uint16_t batchId;
    uint16_t batchLength;
    uint16_t sequenceId;
    uint16_t intervalTime;
    uint16_t intervalStep;
    PicoScenesDeviceType deviceType;
    uint64_t carrierFrequency;
    uint32_t samplingFrequency;

} __attribute__ ((__packed__));

struct DPASRequestV4 {
    uint8_t requestMode;
    uint16_t batchId;
    uint16_t batchLength;
    uint16_t sequenceId;
    uint16_t intervalTime;
    uint16_t intervalStep;
    PicoScenesDeviceType deviceType;
    uint16_t deviceSubtype;
    uint64_t carrierFrequency;
    uint32_t samplingFrequency;

} __attribute__ ((__packed__));

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> DPASRequest {
    uint32_t pos = 0;
    if (bufferLength < sizeof(DPASRequestV1))
        throw std::runtime_error("DPASRequest v1Parser cannot parse the segment with insufficient buffer length.");

    auto r = DPASRequest();
    std::memset(&r, 0, sizeof(r));
    r.batchId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.batchLength = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.sequenceId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalTime = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalStep = 0;
    if (pos != bufferLength)
        throw std::runtime_error("DPASRequest v1Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v2Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> DPASRequest {
    uint32_t pos = 0;
    if (bufferLength < sizeof(DPASRequestV2))
        throw std::runtime_error("DPASRequest v2Parser cannot parse the segment with insufficient buffer length.");

    auto r = DPASRequest();
    std::memset(&r, 0, sizeof(r));
    r.batchId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.batchLength = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.sequenceId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalTime = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalStep = *(uint16_t *) (buffer + pos);
    pos += 2;
    if (pos != bufferLength)
        throw std::runtime_error("DPASRequest v2Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v3Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> DPASRequest {
    uint32_t pos = 0;
    if (bufferLength < sizeof(DPASRequestV3))
        throw std::runtime_error("DPASRequest v3Parser cannot parse the segment with insufficient buffer length.");

    auto r = DPASRequest();
    std::memset(&r, 0, sizeof(r));
    r.batchId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.batchLength = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.sequenceId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalTime = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalStep = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.deviceType = *(PicoScenesDeviceType *) (buffer + pos);
    pos += sizeof(PicoScenesDeviceType);
    r.carrierFrequency = *(uint64_t *) (buffer + pos);
    pos += 8;
    r.samplingFrequency = *(uint32_t *) (buffer + pos);
    pos += 4;
    if (pos != bufferLength)
        throw std::runtime_error("DPASRequest v3Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v4Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> DPASRequest {
    uint32_t pos = 0;
    if (bufferLength < sizeof(DPASRequestV4))
        throw std::runtime_error("DPASRequest v4Parser cannot parse the segment with insufficient buffer length.");

    auto r = DPASRequest();
    std::memset(&r, 0, sizeof(r));
    r.requestMode = *(uint8_t *) (buffer + pos++);
    r.batchId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.batchLength = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.sequenceId = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalTime = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.intervalStep = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.deviceType = *(PicoScenesDeviceType *) (buffer + pos);
    pos += sizeof(PicoScenesDeviceType);
    r.deviceSubtype = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.carrierFrequency = *(uint64_t *) (buffer + pos);
    pos += 8;
    r.samplingFrequency = *(uint32_t *) (buffer + pos);
    pos += 4;
    if (pos != bufferLength)
        throw std::runtime_error("DPASRequest v4Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

std::vector<uint8_t> DPASRequest::toBuffer() {
    return std::vector<uint8_t>((uint8_t *) this, (uint8_t *) this + sizeof(DPASRequest));
}

std::map<uint16_t, std::function<DPASRequest(const uint8_t *, uint32_t)>> DPASRequestSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<DPASRequest(const uint8_t *, uint32_t)>> DPASRequestSegment::initializeSolutionMap() noexcept {
    std::map<uint16_t, std::function<DPASRequest(const uint8_t *, uint32_t)>> map;
    map.emplace(0x1U, v1Parser);
    map.emplace(0x2U, v2Parser);
    map.emplace(0x3U, v3Parser);
    map.emplace(0x4U, v4Parser);
    return map;
}

DPASRequestSegment::DPASRequestSegment() : AbstractPicoScenesFrameSegment("DPASRequest", 0x4U) {
}

DPASRequestSegment::DPASRequestSegment(const DPASRequest &request) : DPASRequestSegment() {
    setRequest(request);
}

DPASRequest DPASRequestSegment::getRequest() const {
    return dpasRequest;
}

void DPASRequestSegment::setRequest(const DPASRequest &requestv) {
    dpasRequest = requestv;
    clearFieldCache();
    addField("req", dpasRequest.toBuffer());
}

std::vector<uint8_t> DPASRequestSegment::toBuffer() const {
    return AbstractPicoScenesFrameSegment::toBuffer(true);
}

void DPASRequestSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto[segmentName, segmentLength, versionId, offset] = extractSegmentMetaData(buffer, bufferLength);
    if (segmentName != "DPASRequest")
        throw std::runtime_error("DPASRequestSegment cannot parse the segment named " + segmentName + ".");
    if (segmentLength + 4 > bufferLength)
        throw std::underflow_error("DPASRequestSegment cannot parse the segment with less than " + std::to_string(segmentLength + 4) + "B.");
    if (!versionedSolutionMap.contains(versionId)) {
        throw std::runtime_error("DPASRequestSegment cannot parse the segment with version v" + std::to_string(versionId) + ".");
    }

    dpasRequest = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    rawBuffer.resize(bufferLength);
    std::copy(buffer, buffer + bufferLength, rawBuffer.begin());
    this->segmentLength = rawBuffer.size() - 4;
    successfullyDecoded = true;
}

DPASRequestSegment DPASRequestSegment::createByBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto seg = DPASRequestSegment();
    seg.fromBuffer(buffer, bufferLength);
    return seg;
}

std::string DPASRequestSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[";
    ss << "batch_id=" << dpasRequest.batchId << ", batch_len=" << dpasRequest.batchLength << ", seq=" << dpasRequest.sequenceId << ", interval=" << dpasRequest.intervalTime << ", step=" << dpasRequest.intervalStep << ", device=" << DeviceType2String(dpasRequest.deviceType) << ", f_c=" << double(dpasRequest.carrierFrequency) / 1e6 << " MHz, f_s=" << double(dpasRequest.samplingFrequency) / 1e6 << " MHz]";
    auto temp = ss.str();
    return temp;
}

std::ostream &operator<<(std::ostream &os, const DPASRequestSegment &dpasRequestSegment) {
    os << dpasRequestSegment.toString();
    return os;
}
