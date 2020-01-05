//
// Created by csi on 12/17/19.
//

#ifndef PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX

#include "rxs_enhanced.h"
#include <optional>
#include <map>

#define PICOSCENES_FRAME_SEGMENT_MAX_LENGTH 2048

enum class PicoScenesDeviceType : uint16_t {
    QCA9300 = 0x9300,
    IWL5300 = 0x5300,
    MAC80211Compatible = 0x802,
    USRP = 0x1234,
    Unknown = 0x404
};

inline std::string DeviceType2String(PicoScenesDeviceType type) {
    switch (type) {
        case PicoScenesDeviceType::QCA9300:
            return "QCA9300";
        case PicoScenesDeviceType::IWL5300:
            return "IWL5300";
        case PicoScenesDeviceType::MAC80211Compatible:
            return "MAC80211 Compatible NIC";
        case PicoScenesDeviceType::USRP:
            return "USRP(SDR)";
        case PicoScenesDeviceType::Unknown:
            return "Unknown";
    }
}

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

    ieee80211_mac_frame_header_frame_control_field() : version(0), type(2), subtype(0), toDS(0), fromDS(0), moreFrags(0), retry(0), power_mgmt(0), more(0), protect(0), order(1) {}

} __attribute__ ((__packed__));

struct ieee80211_mac_frame_header {
    struct ieee80211_mac_frame_header_frame_control_field fc;
    uint16_t dur = 0;
    uint8_t addr1[6] = {0x00, 0x16, 0xea, 0x12, 0x34, 0x56};
    uint8_t addr2[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t addr3[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint16_t seq = 0;

} __attribute__ ((__packed__));

struct PicoScenesFrameHeader {
    uint32_t magicValue = 0x20150315;
    uint32_t version = 0x20191218;
    PicoScenesDeviceType deviceType = PicoScenesDeviceType::QCA9300;
    uint8_t segments = 0;
    uint8_t frameType = 0;
    uint16_t taskId = 0;

    static std::optional<PicoScenesFrameHeader> fromBuffer(const uint8_t *buffer);
} __attribute__ ((__packed__));

struct CSIData {
    std::complex<double> csi_matrix[MAX_OFDM_TONES_UNWRAP];
    double unwrappedMag[MAX_OFDM_TONES_UNWRAP] = {0};
    double unwrappedPhase[MAX_OFDM_TONES_UNWRAP]= {0};
};

class PicoScenesRxFrameStructure {
public:
    PicoScenesDeviceType deviceType = PicoScenesDeviceType::QCA9300;
    rx_status_basic rxs_basic = {};
    ExtraInfo rxExtraInfo;
    CSIData csi;
    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> PicoScenesHeader;
    std::optional<std::vector<uint16_t>> frameSegmentStarts;
    std::optional<ExtraInfo> txExtraInfo;
    std::optional<std::map<std::string, std::shared_ptr<uint8_t>>> segmentMap;

    static bool isOldRXSEnhancedFrame(const uint8_t bufferHead[6]);

    static std::optional<PicoScenesRxFrameStructure> fromBuffer(const uint8_t *buffer, uint32_t start = 0, enum RXSParsingLevel parsingLevel = RXSParsingLevel::EXTRA_NOCSI);

    std::optional<uint16_t> parseRxMACFramePart(const uint8_t *buffer);
};


class PicoScenesFrameTxParameters {
public:
    uint8_t mcs = 0;
    bool channelBonding = false;
    bool sgi = false;
    bool greenField = false;
};

class PicoScenesTxFrameStructure {
public:
    ieee80211_mac_frame_header standardHeader;
    PicoScenesFrameHeader frameHeader;
    PicoScenesFrameTxParameters txParameters;
    std::optional<ExtraInfo> extraInfo;
    std::map<std::string, std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH>> segmentBuffer;
    std::map<std::string, uint16_t> segmentLength;

    void reset();

    uint16_t totalLength() const;

    int toBuffer(uint8_t *buffer, std::optional<uint16_t> bufferLength = std::nullopt) const;

    std::shared_ptr<uint8_t> toBuffer() const;

    PicoScenesTxFrameStructure &addExtraInfo(const ExtraInfo &txExtraInfo);

    PicoScenesTxFrameStructure &addSegmentBuffer(const std::string &identifier, const uint8_t *buffer, uint16_t length);

    PicoScenesTxFrameStructure &addSegmentBuffer(const std::string &identifier, const std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH> &bufferArray, uint16_t length);

    PicoScenesTxFrameStructure &setRetry();

    PicoScenesTxFrameStructure &setTaskId(uint16_t taskId);

    PicoScenesTxFrameStructure &setRandomTaskId();

    PicoScenesTxFrameStructure &setPicoScenesFrameType(uint8_t frameType);

    PicoScenesTxFrameStructure &setChannelBonding(bool useChannelBonding);

    PicoScenesTxFrameStructure &setSGI(bool useSGI);

    PicoScenesTxFrameStructure &setMCS(uint8_t mcs);

    PicoScenesTxFrameStructure &setGreenField(bool useGreenField);

    PicoScenesTxFrameStructure &setDestinationAddress(const uint8_t macAddr[6]);

    PicoScenesTxFrameStructure &setSourceAddress(const uint8_t macAddr[6]);

    PicoScenesTxFrameStructure &set3rdAddress(const uint8_t macAddr[6]);
};


#endif //PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
