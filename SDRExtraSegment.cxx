//
// Created by Zhiping Jiang on 10/23/22.
//

#include "SDRExtraSegment.hxx"


std::vector<uint8_t> SDRExtra::toBuffer() {
    return std::vector<uint8_t>{reinterpret_cast<uint8_t *>(this), reinterpret_cast<uint8_t *>(this) + sizeof(SDRExtra)};
}

std::string SDRExtra::toString() const {
    std::stringstream ss;
    ss << "SDRExtra:[scrambler=" << static_cast<int16_t>(scramblerInit) << ", packetStartInternal="
       << packetStartInternal << ", rxIndex=" << hardwareRxSamplingIndex
       << std::setprecision(9) << ", rxTime=" << preciseRxTime << ", decodingDelay=" << decodingDelay()
       << ", lastTxTime=" << lastTxTime << ", sigEVM=" << std::fixed << std::setprecision(1) << sigEVM
       << ", initialCFO=" << initialCFO << ", residualCFO=" << residualCFO << "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const SDRExtra &sdrExtra) {
    os << sdrExtra.toString();
    return os;
}

struct SDRExtraV1 {
    int8_t scramblerInit;
    int64_t packetStartInternal;
    double lastTxTime;
} __attribute__ ((__packed__));

struct SDRExtraV2 {
    int8_t scramblerInit;
    int64_t packetStartInternal;
    double preciseRxTime;
    double lastTxTime;
} __attribute__ ((__packed__));

struct SDRExtraV3 {
    int8_t scramblerInit;
    int64_t packetStartInternal;
    int64_t hardwareRxSamplingIndex;
    double preciseRxTime;
    double lastTxTime;
} __attribute__ ((__packed__));

struct SDRExtraV4 {
    int8_t scramblerInit{0};
    int64_t packetStartInternal{0};
    int64_t hardwareRxSamplingIndex{0};
    double preciseRxTime{0};
    double lastTxTime{0};
    double signalInputSystemTime{0};
    double signalDecodeSystemTime{0};
} __attribute__ ((__packed__));

struct SDRExtraV5 {
    int8_t scramblerInit{0};
    int64_t packetStartInternal{0};
    int64_t hardwareRxSamplingIndex{0};
    double preciseRxTime{0};
    double lastTxTime{0};
    double signalInputSystemTime{0};
    double signalDecodeSystemTime{0};
    double sigEVM{0};
} __attribute__ ((__packed__));

struct SDRExtraV6 {
    int8_t scramblerInit{0};
    int64_t packetStartInternal{0};
    int64_t hardwareRxSamplingIndex{0};
    double preciseRxTime{0};
    double lastTxTime{0};
    double signalInputSystemTime{0};
    double signalDecodeSystemTime{0};
    double sigEVM{0};
    double initialCFO{0};
    double residualCFO{0};
} __attribute__ ((__packed__));

static auto v1Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV1))
        throw std::runtime_error("SDRExtra v1Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.scramblerInit = *reinterpret_cast<const int8_t *>(buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.preciseRxTime = 0;
    r.lastTxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v1Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v2Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV2))
        throw std::runtime_error("SDRExtra v2Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.scramblerInit = *reinterpret_cast<const int8_t *>(buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.preciseRxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.lastTxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v2Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v3Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV3))
        throw std::runtime_error("SDRExtra v3Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.scramblerInit = *reinterpret_cast<const int8_t *>(buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.hardwareRxSamplingIndex = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.preciseRxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.lastTxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v3Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v4Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV4))
        throw std::runtime_error("SDRExtra v4Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.scramblerInit = *reinterpret_cast<const int8_t *>(buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.hardwareRxSamplingIndex = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.preciseRxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.lastTxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.signalInputSystemTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.signalDecodeSystemTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v4Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v5Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV5))
        throw std::runtime_error("SDRExtra v5Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.scramblerInit = *reinterpret_cast<const int8_t *>(buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.hardwareRxSamplingIndex = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.preciseRxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.lastTxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.signalInputSystemTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.signalDecodeSystemTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.sigEVM = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v5Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v6Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> SDRExtra {
    uint32_t pos = 0;
    if (bufferLength < sizeof(SDRExtraV6))
        throw std::runtime_error("SDRExtra v6Parser cannot parse the segment with insufficient buffer length.");

    auto r = SDRExtra();
    std::memset(&r, 0, sizeof(r));
    r.scramblerInit = *reinterpret_cast<const int8_t *>(buffer + pos);
    pos += sizeof(int8_t);
    r.packetStartInternal = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.hardwareRxSamplingIndex = *reinterpret_cast<const int64_t *>(buffer + pos);
    pos += sizeof(int64_t);
    r.preciseRxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.lastTxTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.signalInputSystemTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.signalDecodeSystemTime = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.sigEVM = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.initialCFO = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);
    r.residualCFO = *reinterpret_cast<const double *>(buffer + pos);
    pos += sizeof(double);

    if (pos != bufferLength)
        throw std::runtime_error("SDRExtra v6Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> SDRExtraSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> SDRExtraSegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>>{{0x1U, v1Parser},
                                                                                  {0x2U, v2Parser},
                                                                                  {0x3U, v3Parser},
                                                                                  {0x4U, v4Parser},
                                                                                  {0x5U, v5Parser},
                                                                                  {0x6U, v6Parser}};
}

SDRExtraSegment::SDRExtraSegment() : AbstractPicoScenesFrameSegment("SDRExtra", 0x6U) {}

SDRExtraSegment::SDRExtraSegment(const uint8_t *buffer, const uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "SDRExtra")
        throw std::runtime_error("SDRExtraSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.contains(segmentVersionId))
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

void SDRExtraSegment::setSdrExtra(const SDRExtra &sdrExtra) {
    SDRExtraSegment::sdrExtra = sdrExtra;
    setSegmentPayload(SDRExtraSegment::sdrExtra.toBuffer());
}

std::string SDRExtraSegment::toString() const {
    return sdrExtra.toString();
}
