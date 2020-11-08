//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX
#define PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"


struct RxSBasic {
    uint16_t deviceType;  ///< device type code
    uint64_t tstamp;      ///< h/w assigned RX timestamp
    uint16_t channel;     ///< receiving channel frequency, unit is MHz, e.g, 2437
    PacketFormatEnum packetFormat;
    ChannelBandwidthEnum cbw;
    uint16_t gi;          ///< 400/800/1600/3200 ns
    uint8_t rate;         ///< MCS index
    uint8_t numSTS;
    uint8_t numESS;
    uint8_t numRx;
    int8_t noise;         ///< noise floor
    uint8_t rssi;        ///< rx frame RSSI
    uint8_t rssi0;       ///< rx frame RSSI [ctl, chain 0]
    uint8_t rssi1;       ///< rx frame RSSI [ctl, chain 1]
    uint8_t rssi2;       ///< rx frame RSSI [ctl, chain 2]

    static std::optional<RxSBasic> fromBuffer(const uint8_t *buffer);

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const RxSBasic &rxSBasic);


class RxSBasicSegment : AbstractPicoScenesFrameSegment {

public:
    RxSBasicSegment();

    void setRxsBasic(const RxSBasic &basic);

    void fromBuffer(const uint8_t *string, uint32_t length) override;

private:
    RxSBasic basic;

    void updateFieldMap() override;

};

#endif //PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX
