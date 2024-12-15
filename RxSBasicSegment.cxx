//
// Created by Zhiping Jiang on 2020/11/6.
//

#include "RxSBasicSegment.hxx"

struct RxsBasicContentV1 {
    uint16_t deviceType;  /* device type code */
    uint64_t tstamp;      /* h/w assigned timestamp */
    uint16_t channelFreq;     /* receiving channel frequency */
    int8_t packetFormat;/* 0 for NonHT, 1 for HT, 2 for VHT, 3 for HE-SU, 4 for HE-MU */
    uint16_t cbw;         /* channel bandwidth [20, 40, 80, 160] */
    uint16_t guardInterval; /* 400/800/1600/3200ns */
    uint8_t mcs;
    uint8_t numSTS;
    uint8_t numESS;
    uint8_t numRx;
    int8_t noiseFloor;   /* noise floor */
    uint8_t rssi;        /* rx frame RSSI */
    uint8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
    uint8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
    uint8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
} __attribute__((packed));

// compared to V1, add numUser and userIndex
struct RxsBasicContentV2 {
    uint16_t deviceType;  /* device type code */
    uint64_t tstamp;      /* h/w assigned timestamp */
    uint16_t channelFreq;     /* receiving channel frequency */
    int8_t packetFormat;/* 0 for NonHT, 1 for HT, 2 for VHT, 3 for HE-SU, 4 for HE-MU */
    uint16_t cbw;         /* channel bandwidth [20, 40, 80, 160] */
    uint16_t guardInterval; /* 400/800/1600/3200ns */
    uint8_t mcs;
    uint8_t numSTS;
    uint8_t numESS;
    uint8_t numRx;
    uint8_t numUser;
    uint8_t userIndex;
    int8_t noiseFloor;   /* noise floor */
    uint8_t rssi;        /* rx frame RSSI */
    uint8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
    uint8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
    uint8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
} __attribute__((packed));

// compared to V2, add controlFreq, add pkt_cbw, and rename channelFreq to centerFreq
struct RxsBasicContentV3 {
    uint16_t deviceType;    /* device type code */
    uint64_t tstamp;        /* h/w assigned timestamp */
    int16_t centerFreq;     /* receiving channel frequency */
    int16_t controlFreq;    /* control channel frequency */
    int16_t cbw;            /* channel bandwidth [20, 40, 80, 160] */
    uint8_t packetFormat;   /* 0 for NonHT, 1 for HT, 2 for VHT, 3 for HE-SU, 4 for HE-MU */
    uint16_t pkt_cbw;       /* packet CBW [20, 40, 80, 160] */
    uint16_t guardInterval; /* 400/800/1600/3200ns */
    uint8_t mcs;
    uint8_t numSTS;
    uint8_t numESS;
    uint8_t numRx;
    uint8_t numUser;
    uint8_t userIndex;
    int8_t noiseFloor;   /* noise floor */
    int8_t rssi;        /* rx frame RSSI */
    int8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
    int8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
    int8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
} __attribute__((packed));

// compared to V3, add systemTime
struct RxsBasicContentV4 {
    uint16_t deviceType;    /* device type code */
    uint64_t tstamp;        /* h/w assigned timestamp */
    uint64_t systemTime;    /* system nanosecond time via ktime_get_real() */
    int16_t centerFreq;     /* receiving channel frequency */
    int16_t controlFreq;    /* control channel frequency */
    int16_t cbw;            /* channel bandwidth [20, 40, 80, 160] */
    uint8_t packetFormat;   /* 0 for NonHT, 1 for HT, 2 for VHT, 3 for HE-SU, 4 for HE-MU */
    uint16_t pkt_cbw;       /* packet CBW [20, 40, 80, 160] */
    uint16_t guardInterval; /* 400/800/1600/3200ns */
    uint8_t mcs;
    uint8_t numSTS;
    uint8_t numESS;
    uint8_t numRx;
    uint8_t numUser;
    uint8_t userIndex;
    int8_t noiseFloor;   /* noise floor */
    int8_t rssi;        /* rx frame RSSI */
    int8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
    int8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
    int8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
} __attribute__((packed));

// compared to V4, add centerFreq2 and 5 additional rss values
struct RxsBasicContentV5 {
    uint16_t deviceType;    /* device type code */
    uint64_t tstamp;        /* h/w assigned timestamp */
    uint64_t systemTime;    /* system nanosecond time via ktime_get_real() */
    int16_t centerFreq;     /* primary RF center frequency */
    int16_t centerFreq2;    /* second RF center frequency, mainly used for multi-channel splitting, e.g., 80 + 80 mode */
    int16_t controlFreq;    /* control channel frequency */
    uint16_t cbw;           /* channel bandwidth [20, 40, 80, 160, 320] */
    uint8_t packetFormat;   /* 0 for NonHT, 1 for HT, 2 for VHT, 3 for HE-SU, 4 for HE-MU, 5 for EHT-MU */
    uint16_t pkt_cbw;       /* packet CBW [20, 40, 80, 160, 320] */
    uint16_t guardInterval; /* 400/800/1600/3200ns */
    uint8_t mcs;
    uint8_t numSTS;         /* number of Space-Time Stream */
    uint8_t numESS;         /* number of Extra Spatial Sounding (an 802.11n only feature) */
    uint8_t numRx;          /* number of Rx antenna */
    uint8_t numUser;
    uint8_t userIndex;
    int8_t noiseFloor;   /* noise floor */
    int8_t rssi;        /* rx frame RSSI */
    int8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
    int8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
    int8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
    int8_t rssi_ctl3;   /* rx frame RSSI [ctl, chain 3] */
    int8_t rssi_ctl4;   /* rx frame RSSI [ctl, chain 4] */
    int8_t rssi_ctl5;   /* rx frame RSSI [ctl, chain 5] */
    int8_t rssi_ctl6;   /* rx frame RSSI [ctl, chain 6] */
    int8_t rssi_ctl7;   /* rx frame RSSI [ctl, chain 7] */
} __attribute__((packed));

static auto v1Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> RxSBasic {
    uint32_t pos = 0;
    if (bufferLength < sizeof(RxsBasicContentV1))
        throw std::runtime_error("RxSBasicSegment v1Parser cannot parse the segment with insufficient buffer length.");

    auto r = RxSBasic();
    r.deviceType = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.tstamp = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.systemTime = 0;
    r.centerFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.centerFreq2 = r.centerFreq;
    r.controlFreq = r.centerFreq;
    r.packetFormat = *(buffer + pos++);
    r.cbw = *reinterpret_cast<const uint16_t *>(buffer + pos);
    r.pkt_cbw = r.cbw;
    pos += 2;
    r.guardInterval = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.mcs = *(buffer + pos++);
    r.numSTS = *(buffer + pos++);
    r.numESS = *(buffer + pos++);
    r.numRx = *(buffer + pos++);
    r.numUser = 1;
    r.userIndex = 0;
    r.noiseFloor = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl0 = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl1 = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl2 = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);

    if (pos != bufferLength)
        throw std::runtime_error("RxSBasicSegment v1Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v2Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> RxSBasic {
    uint32_t pos = 0;
    if (bufferLength < sizeof(RxsBasicContentV2))
        throw std::runtime_error("RxSBasicSegment v2Parser cannot parse the segment with insufficient buffer length.");

    auto r = RxSBasic();
    r.deviceType = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.tstamp = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.systemTime = 0;
    r.centerFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.centerFreq2 = r.centerFreq;
    r.controlFreq = r.centerFreq;
    r.packetFormat = *(buffer + pos++);
    r.cbw = *reinterpret_cast<const uint16_t *>(buffer + pos);
    r.pkt_cbw = r.cbw;
    pos += 2;
    r.guardInterval = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.mcs = *(buffer + pos++);
    r.numSTS = *(buffer + pos++);
    r.numESS = *(buffer + pos++);
    r.numRx = *(buffer + pos++);
    r.numUser = *(buffer + pos++);
    r.userIndex = *(buffer + pos++);
    r.noiseFloor = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl0 = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl1 = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl2 = r.noiseFloor + *reinterpret_cast<const int8_t *>(buffer + pos++);

    if (pos != bufferLength)
        throw std::runtime_error("RxSBasicSegment v2Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v3Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> RxSBasic {
    uint32_t pos = 0;
    if (bufferLength < sizeof(RxsBasicContentV3))
        throw std::runtime_error("RxSBasicSegment v3Parser cannot parse the segment with insufficient buffer length.");

    auto r = RxSBasic();
    r.deviceType = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.tstamp = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.systemTime = 0;
    r.centerFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.controlFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.cbw = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.packetFormat = *(buffer + pos++);
    r.pkt_cbw = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.guardInterval = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.mcs = *(buffer + pos++);
    r.numSTS = *(buffer + pos++);
    r.numESS = *(buffer + pos++);
    r.numRx = *(buffer + pos++);
    r.numUser = *(buffer + pos++);
    r.userIndex = *(buffer + pos++);
    r.noiseFloor = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl0 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl1 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl2 = *reinterpret_cast<const int8_t *>(buffer + pos++);

    if (pos != bufferLength)
        throw std::runtime_error("RxSBasicSegment v3Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v4Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> RxSBasic {
    uint32_t pos = 0;
    if (bufferLength < sizeof(RxsBasicContentV4))
        throw std::runtime_error("RxSBasicSegment v4Parser cannot parse the segment with insufficient buffer length.");

    auto r = RxSBasic();
    r.deviceType = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.tstamp = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.systemTime = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.centerFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.centerFreq2 = r.centerFreq;
    r.controlFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.cbw = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.packetFormat = *(buffer + pos++);
    r.pkt_cbw = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.guardInterval = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.mcs = *(buffer + pos++);
    r.numSTS = *(buffer + pos++);
    r.numESS = *(buffer + pos++);
    r.numRx = *(buffer + pos++);
    r.numUser = *(buffer + pos++);
    r.userIndex = *(buffer + pos++);
    r.noiseFloor = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi = *reinterpret_cast<const int8_t *>(buffer + pos++);
    std::memset(r.rssAntenna, 0, sizeof(r.rssAntenna));
    r.rssi_ctl0 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl1 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl2 = *reinterpret_cast<const int8_t *>(buffer + pos++);

    if (pos != bufferLength)
        throw std::runtime_error("RxSBasicSegment v4Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

static auto v5Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> RxSBasic {
    uint32_t pos = 0;
    if (bufferLength < sizeof(RxsBasicContentV5))
        throw std::runtime_error("RxSBasicSegment v5Parser cannot parse the segment with insufficient buffer length.");

    auto r = RxSBasic();
    r.deviceType = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.tstamp = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.systemTime = *reinterpret_cast<const uint64_t *>(buffer + pos);
    pos += 8;
    r.centerFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.centerFreq2 = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.controlFreq = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.cbw = *reinterpret_cast<const int16_t *>(buffer + pos);
    pos += 2;
    r.packetFormat = *(const uint8_t *) (buffer + pos++);
    r.pkt_cbw = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.guardInterval = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    r.mcs = *(buffer + pos++);
    r.numSTS = *(buffer + pos++);
    r.numESS = *(buffer + pos++);
    r.numRx = *(buffer + pos++);
    r.numUser = *(buffer + pos++);
    r.userIndex = *(buffer + pos++);
    r.noiseFloor = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl0 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl1 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl2 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl3 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl4 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl5 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl6 = *reinterpret_cast<const int8_t *>(buffer + pos++);
    r.rssi_ctl7 = *reinterpret_cast<const int8_t *>(buffer + pos++);

    if (pos != bufferLength)
        throw std::runtime_error("RxSBasicSegment v5Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> RxSBasicSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> RxSBasicSegment::initializeSolutionMap() noexcept {
    std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> map;
    map.emplace(0x1U, v1Parser);
    map.emplace(0x2U, v2Parser);
    map.emplace(0x3U, v3Parser);
    map.emplace(0x4U, v4Parser);
    map.emplace(0x5U, v5Parser);
    return map;
}

std::string RxSBasic::toString() const {
    std::stringstream ss;
    ss << "RxSBasic:[device=" << DeviceType2String(static_cast<PicoScenesDeviceType>(deviceType))
            << ", center=" << centerFreq << (centerFreq != centerFreq2 ? (", center2=" + std::to_string(centerFreq2)) : "")
            << ", control=" << controlFreq << ", CBW=" << cbw << ", format=" << PacketFormat2String(static_cast<PacketFormatEnum>(packetFormat))
            << ", Pkt_CBW=" << pkt_cbw << ", MCS=" << static_cast<uint16_t>(mcs) << ", numSTS=" << static_cast<uint16_t>(numSTS) << ", GI=" << GuardInterval2String(static_cast<GuardIntervalEnum>(guardInterval))
            << ", UsrIdx/NUsr=(" << static_cast<uint16_t>(userIndex) << "/" << static_cast<uint16_t>(numUser) << "), timestamp=" << tstamp << ", system_ns=" << systemTime
            << ", NF=" << static_cast<int16_t>(noiseFloor) << ", RSS=" << static_cast<int16_t>(rssi) << "]";
    return ss.str();
}

std::vector<uint8_t> RxSBasic::toBuffer() const {
    return std::vector<uint8_t>{reinterpret_cast<const uint8_t *>(this), reinterpret_cast<const uint8_t *>(this) + sizeof(RxSBasic)};
}

RxSBasicSegment::RxSBasicSegment() : AbstractPicoScenesFrameSegment("RxSBasic", 0x5U) {}

RxSBasicSegment::RxSBasicSegment(const RxSBasic &rvBasic) : AbstractPicoScenesFrameSegment("RxSBasic", 0x5U), basic(rvBasic) {
    setSegmentPayload(basic.toBuffer());
}

RxSBasicSegment::RxSBasicSegment(const uint8_t *buffer, uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "RxSBasic")
        throw std::runtime_error("RxSBasicSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.contains(segmentVersionId)) {
        throw std::runtime_error("RxSBasicSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");
    }

    basic = versionedSolutionMap.at(segmentVersionId)(segmentPayload.data(), segmentPayload.size());
}

const RxSBasic &RxSBasicSegment::getBasic() const {
    return basic;
}

void RxSBasicSegment::setBasic(const RxSBasic &basicV) {
    basic = basicV;
    setSegmentPayload(basic.toBuffer());
}

std::ostream &operator<<(std::ostream &os, const RxSBasic &rxSBasic) {
    os << rxSBasic.toString();
    return os;
}

std::string RxSBasicSegment::toString() const {
    return basic.toString();
}