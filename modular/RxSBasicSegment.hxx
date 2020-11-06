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
    uint16_t csi_len;     ///< csi length
    uint16_t channel;     ///< receiving channel frequency, unit is MHz, e.g, 2437
    uint8_t sgi;          ///< short GI, 1 for Short

    int8_t noise;         ///< noise floor
    uint8_t rate;         ///< MCS index
    uint8_t numSTS;
    uint8_t numESS;
    PacketFormatEnum packetFormat;
    ChannelBandwidthEnum cbw;
    uint16_t num_tones;   ///< number of tones (subcarriers), should be 56 or 114
    uint8_t nrx;         ///< number of RX antennas, 1~3
    uint8_t ntx;         ///< number of TX spatial time stream (N_{STS})
    uint8_t nltf;        ///< number of HT-LTF fields appeared in the received packet, i.e., 1-5 for 11n packet (N_{HT-LTF} = N_{STS} + N_{ESS})
    uint8_t ncsi_group;  ///< number of CSI measurement groups (N_{CSI Group} = N_{HT-LTF} x N_{RX-Ant})

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
