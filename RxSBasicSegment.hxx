//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX
#define PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

struct RxSBasic {
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
    union {
        int8_t rssAntenna[8] __attribute__ ((__packed__));
        struct {
            int8_t rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
            int8_t rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
            int8_t rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */
            int8_t rssi_ctl3;   /* rx frame RSSI [ctl, chain 3] */
            int8_t rssi_ctl4;   /* rx frame RSSI [ctl, chain 4] */
            int8_t rssi_ctl5;   /* rx frame RSSI [ctl, chain 5] */
            int8_t rssi_ctl6;   /* rx frame RSSI [ctl, chain 6] */
            int8_t rssi_ctl7;   /* rx frame RSSI [ctl, chain 7] */
        };
    } __attribute__ ((__packed__));

    [[nodiscard]] std::string toString() const;

    std::vector<uint8_t> toBuffer();

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const RxSBasic &rxSBasic);


class RxSBasicSegment : public AbstractPicoScenesFrameSegment {

public:
    RxSBasicSegment();

    RxSBasicSegment(const uint8_t *buffer, uint32_t bufferLength);

    explicit RxSBasicSegment(const RxSBasic &rvBasic);

    [[nodiscard]] const RxSBasic &getBasic() const;

    void setBasic(const RxSBasic &basic);

private:
    static std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    RxSBasic basic{};
};

#endif //PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX
