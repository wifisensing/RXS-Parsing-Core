//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "CSISegment.hxx"
#include "RxSBasicSegment.hxx"
#include "ExtraInfoSegment.hxx"
#include "BasebandSignalSegment.hxx"

struct ieee80211_mac_frame_header_frame_control_field {
    uint16_t version: 2,
            type: 2,
            subtype: 4,
            toDS: 1,
            fromDS: 1,
            moreFrags: 1,
            retry: 1,
            power_mgmt: 1,
            more: 1,
            protect: 1,
            order: 1;

    // type 0 -- Management Frame, subtype 0xE -- Action Frame NO ACK.  Supported by both QCA9300 and IWL5300.
    ieee80211_mac_frame_header_frame_control_field() : version(0), type(0), subtype(0xe), toDS(0), fromDS(0), moreFrags(0), retry(0), power_mgmt(0), more(0), protect(0), order(1) {}

    bool operator==(const ieee80211_mac_frame_header_frame_control_field &rhs) const;

} __attribute__ ((__packed__));

struct ieee80211_mac_frame_header {
    ieee80211_mac_frame_header_frame_control_field fc;
    uint16_t dur = 0;
    uint8_t addr1[6] = {0x00, 0x16, 0xea, 0x12, 0x34, 0x56};
    uint8_t addr2[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t addr3[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint16_t frag: 4,
            seq: 12;

    ieee80211_mac_frame_header() : frag(0), seq(0) {};

    bool operator==(const ieee80211_mac_frame_header &rhs) const;

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header);

struct PicoScenesFrameHeader {
    uint32_t magicValue = 0x20150315;
    uint32_t version = 0x20191218;
    PicoScenesDeviceType deviceType = PicoScenesDeviceType::QCA9300;
    uint8_t numSegments = 0;
    uint8_t frameType = 0;
    uint16_t taskId = 0;

    static std::optional<PicoScenesFrameHeader> fromBuffer(const uint8_t *buffer);

    bool operator==(const PicoScenesFrameHeader &rhs) const;

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameHeader &frameHeader);


class ModularPicoScenesRxFrame {
public:
    RxSBasicSegment rxSBasicSegment;
    CSISegment csiSegment;
    ExtraInfoSegment extraInfoSegment;
    BasebandSignalSegment basebandSignalSegment;

    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> PicoScenesHeader;

    static std::optional<ModularPicoScenesRxFrame> fromBuffer(const uint8_t *buffer, uint32_t bufferLength);

    Uint8Vector rawBuffer;
    Uint8Vector nonPicoScenesMSDUContent;

    std::map<std::string, std::vector<uint8_t>> rxSegmentMap;
    std::map<std::string, std::vector<uint8_t>> txSegmentMap;
};

#endif //PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
