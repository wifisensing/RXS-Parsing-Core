//
// Created by 蒋志平 on 2020/11/6.
//

#include "RxSBasicSegment.hxx"

struct QCA9300RxsBasicContentV1 {
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
    int8_t noiseFloor;       /* noise floor */
    uint8_t rssi;        /* rx frame RSSI */
    uint8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
    uint8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
    uint8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
} __attribute__((packed));

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> RxSBasic {
    uint32_t pos = 0;
    if (bufferLength < sizeof(RxSBasic))
        throw std::runtime_error("RxSBasicSegment v1Parser cannot parse the segment with insufficient buffer length.");

    auto r = RxSBasic();
    r.deviceType = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.tstamp = *(uint64_t *) (buffer + pos); pos +=8;
    r.channelFreq = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.packetFormat = *(uint8_t *) (buffer + pos++);
    r.cbw = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.guardInterval = *(uint16_t *) (buffer + pos);
    pos += 2;
    r.mcs = *(uint8_t *) (buffer + pos++);
    r.numSTS = *(uint8_t *) (buffer + pos++);
    r.numESS = *(uint8_t *) (buffer + pos++);
    r.numRx = *(uint8_t *) (buffer + pos++);
    r.noiseFloor = *(int8_t *) (buffer + pos++);
    r.rssi = *(uint8_t *) (buffer + pos++);
    r.rssi_ctl0 = *(uint8_t *) (buffer + pos++);
    r.rssi_ctl1 = *(uint8_t *) (buffer + pos++);
    r.rssi_ctl2 = *(uint8_t *) (buffer + pos++);

    if (pos != bufferLength)
        throw std::runtime_error("RxSBasicSegment v1Parser cannot parse the segment with mismatched buffer length.");

    return r;
};

std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> RxSBasicSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> RxSBasicSegment::initializeSolutionMap() noexcept {
    std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> map;
    map.emplace(0x1U, v1Parser);
    return map;
}

std::string RxSBasic::toString() const {
    std::stringstream ss;
//    ss << "RxS:[device=" + DeviceType2String((PicoScenesDeviceType(deviceType))) + ", freq=" + std::to_string(channel) + ", CBW=" + ChannelBandwidth2String(cbw) + ", MCS=" + std::to_string(rate) + ", SGI=" + std::to_string(sgi) + ", CSI=" + std::to_string(csi_len) + "B(" + std::to_string(ntx) + ", " + std::to_string(nrx) + ", " + std::to_string(num_tones) + "), NLTF=" + std::to_string(nltf) + ", NGroup=" + std::to_string(ncsi_group) + ", timestamp=" + std::to_string(tstamp) + ", NF=" +
//          std::to_string(noise) + ", RSS=" + std::to_string(rssi) + "]";
    return ss.str();
}

RxSBasicSegment::RxSBasicSegment() : AbstractPicoScenesFrameSegment("RxSBasic", 0x1U) {}

void RxSBasicSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto[segmentName, segmentLength, versionId, offset] = extractSegmentMetaData(buffer, bufferLength);
    if (segmentName != "RxSBasic")
        throw std::runtime_error("RxSBasicSegment cannot parse the segment named " + segmentName + ".");
    if (segmentLength + 4 > bufferLength)
        throw std::underflow_error("RxSBasicSegment cannot parse the segment with less than " + std::to_string(segmentLength + 4) + "B.");
    if (!versionedSolutionMap.contains(versionId)) {
        throw std::runtime_error("RxSBasicSegment cannot parse the segment with version v" + std::to_string(versionId) + ".");
    }

    basic = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    rawBuffer.resize(bufferLength);
    std::copy(buffer, buffer + bufferLength, rawBuffer.begin());
}
