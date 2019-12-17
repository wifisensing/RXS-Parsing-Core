//
// Created by csi on 12/17/19.
//

#ifndef PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX

#include "rxs_enhanced.h"
#include <optional>
#include <map>

struct ieee80211_mac_frame_header_frame_control_field {
    uint16_t version         :2,
            type            :2,
            subtype         :4,
            toDS            :1,
            fromDS          :1,
            moreFrags       :1,
            retry           :1,
            power_mgmt      :1,
            more            :1,
            protect         :1,
            order           :1;
} __attribute__ ((__packed__));

struct ieee80211_mac_frame_header {
    struct ieee80211_mac_frame_header_frame_control_field fc;
    uint16_t dur;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq;
} __attribute__ ((__packed__));

struct PicoScenesFrameHeader {
    uint32_t magicValue;
    uint8_t version :4,
            segments :4;
    uint8_t frameType;
    uint16_t taskId;

    static std::optional<PicoScenesFrameHeader> fromBuffer(const uint8_t *buffer);
} __attribute__ ((__packed__));

enum class PicoScenesDeviceType : uint16_t {
    QCA9300 = 9300,
    IWL5300 = 5300,
    USRP = 1234,
};

struct CSIData {
    std::complex<double> csi_matrix[MAX_OFDM_TONES_UNWRAP];
    double unwrappedMag[MAX_OFDM_TONES_UNWRAP];
    double unwrappedPhase[MAX_OFDM_TONES_UNWRAP];
};

class PicoScenesRxFrameStructure {
public:
    PicoScenesDeviceType deviceType;
    rx_status_basic rxs_basic;
    ExtraInfo rxExtraInfo;
    CSIData csi;
    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> PicoScenesHeader;
    std::optional<std::vector<uint16_t>> frameSegmentStarts;
    std::optional<ExtraInfo> txExtraInfo;
    std::optional<std::map<std::string, std::shared_ptr<uint8_t>>> segmentMap;

    static bool isOldRXSEnhancedFrame(const uint8_t bufferHead[6]);

    static std::optional<PicoScenesRxFrameStructure> fromBuffer(const uint8_t *buffer, uint32_t start = 0, enum RXSParsingLevel parsingLevel = RXSParsingLevel::EXTRA_NOCSI);
};

class PicoScenesTxFrameStructure {
public:
    PicoScenesDeviceType deviceType;
    ieee80211_mac_frame_header standardHeader;
    PicoScenesFrameHeader frameHeader;
    std::optional<ExtraInfo> extraInfo;
    std::map<std::string, std::vector<uint8_t>> segmentMap;

    int toBuffer(uint8_t *buffer);
};


#endif //PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
