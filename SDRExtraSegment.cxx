//
// Created by csi on 10/23/22.
//

#include "SDRExtraSegment.hxx"


std::vector<uint8_t> SDRExtra::toBuffer() {
    return std::vector<uint8_t>{(uint8_t *) this, (uint8_t *) this + sizeof(SDRExtra)};
}

std::string SDRExtra::toString() const {
    std::stringstream ss;
    ss << "SDRExtra:[CFO=" + std::to_string(CFO / 1e3) + " kHz, scrambler=" + std::to_string(scramblerInit) + ", packetStartInternal=" + std::to_string(packetStartInternal) + "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const SDRExtra &sdrExtra) {
    os << sdrExtra.toString();
    return os;
}

struct SDRExtraV1 {
    double CFO;
    int8_t scramblerInit;
    uint64_t packetStartInternal;
} __attribute__ ((__packed__));

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV1))
        throw std::runtime_error("SDRExtra v1Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.CFO = *(double *) (buffer + pos);
    pos += sizeof(double);
    r.scramblerInit = *(int8_t *) (buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *(uint64_t *) (buffer + pos);
    pos += sizeof(uint64_t);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v1Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> SDRExtraSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> SDRExtraSegment::initializeSolutionMap() noexcept {
    std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> map;
    map.emplace(0x1U, v1Parser);
    return map;
}

SDRExtraSegment::SDRExtraSegment() : AbstractPicoScenesFrameSegment("SDRExtra", 0x1U) {}

SDRExtraSegment::SDRExtraSegment(const uint8_t *buffer, uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "SDRExtra")
        throw std::runtime_error("SDRExtraSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.count(segmentVersionId))
        throw std::runtime_error("SDRExtraSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    sdrExtra = versionedSolutionMap.at(segmentVersionId)(segmentPayload.data(), segmentPayload.size());
    setSdrExtra(sdrExtra);
}

SDRExtraSegment::SDRExtraSegment(const SDRExtra &sdrExtra) : SDRExtraSegment() {
    setSdrExtra(sdrExtra);
}

const SDRExtra &SDRExtraSegment::getSdrExtra() const {
    return sdrExtra;
}

void SDRExtraSegment::setSdrExtra(const SDRExtra &sdrExtraV) {
    sdrExtra = sdrExtraV;
    setSegmentPayload(sdrExtra.toBuffer());
}

std::string SDRExtraSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[";
    ss << "CFO=" + std::to_string(sdrExtra.CFO / 1e3) + " kHz, scrambler=" + std::to_string(sdrExtra.scramblerInit) + ", packetStartInternal=" + std::to_string(sdrExtra.packetStartInternal) + "]";
    auto temp = ss.str();
    return temp;
}
