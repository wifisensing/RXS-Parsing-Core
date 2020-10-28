//
// Created by csi on 12/17/19.
//

#ifndef PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX

#include "rxs_enhanced.h"
#include <optional>
#include <memory>
#include <iomanip>
#include <map>
#include <set>
#include <random>

#define PICOSCENES_FRAME_SEGMENT_MAX_LENGTH 2048

enum class PicoScenesDeviceType : uint16_t {
    QCA9300 = 0x9300,
    IWL5300 = 0x5300,
    MAC80211Compatible = 0x802,
    USRP = 0x1234,
    Unknown = 0x404
};

std::string DeviceType2String(PicoScenesDeviceType type);

std::ostream &operator<<(std::ostream &os, const PicoScenesDeviceType &deviceType);

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
    uint8_t segments = 0;
    uint8_t frameType = 0;
    uint16_t taskId = 0;

    static std::optional<PicoScenesFrameHeader> fromBuffer(const uint8_t *buffer);

    bool operator==(const PicoScenesFrameHeader &rhs) const;

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameHeader &frameHeader);

struct RxSBasic {
    uint16_t deviceType;  ///< device type code
    uint64_t tstamp;      ///< h/w assigned RX timestamp
    uint16_t csi_len;     ///< csi length
    uint16_t channel;     ///< receiving channel frequency, unit is MHz, e.g, 2437
    uint8_t sgi;          ///< short GI, 1 for Short

    int8_t noise;         ///< noise floor
    uint8_t rate;         ///< MCS index
    uint8_t channelBonding; ///< receiving channel bandwidth, 0 for 20MHz, 1 for 40MHz
    uint8_t num_tones;   ///< number of tones (subcarriers), should be 56 or 114
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

struct CSIData {
    uint8_t nrx;         ///< number of RX antennas, 1~3
    uint8_t ntx;         ///< number of TX spatial time stream (N_{STS})
    uint8_t nltf;        ///< number of HT-LTF fields appeared in the received packet, i.e., 1-5 for 11n packet (N_{HT-LTF} = N_{STS} + N_{ESS})
    uint8_t ncsi_group;  ///< number of CSI measurement groups (N_{CSI Group} = N_{HT-LTF} x N_{RX-Ant})
    uint8_t num_tones;
    std::complex<double> csi_matrix[MAX_OFDM_TONES_UNWRAP];
    double unwrappedMag[MAX_OFDM_TONES_UNWRAP] = {0};
    double unwrappedPhase[MAX_OFDM_TONES_UNWRAP] = {0};

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const CSIData &data);

class AbstractPicoScenesFrameSegment {
public:
    char segId[2]{'S', 'G'};
    std::string segIdString;
    uint32_t segLength = 0;
    std::shared_ptr<uint8_t> segData = nullptr;

    AbstractPicoScenesFrameSegment(const std::string &segIdString, uint32_t segLength, const std::shared_ptr<uint8_t> &segData, bool copyData);

    static std::optional<AbstractPicoScenesFrameSegment> fromBuffer(uint16_t segLength, const uint8_t *buffer);

    std::pair<uint32_t, std::shared_ptr<uint8_t>> toBuffer();

    virtual std::string toString();

    virtual ~AbstractPicoScenesFrameSegment();
    // TODO to finish this class
};

class PicoScenesRxFrameStructure {
public:
    PicoScenesDeviceType deviceType = PicoScenesDeviceType::QCA9300;
    RxSBasic rxs_basic = {};
    ExtraInfo rxExtraInfo;
    std::vector<std::complex<float>> basebandSignals;
    std::vector<std::complex<float>> preEqualizedDataSymbols;
    std::vector<std::complex<float>> nonHTCSI;
    CSIData csi;
    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> PicoScenesHeader;
    std::optional<ExtraInfo> txExtraInfo;
    std::optional<std::map<std::string, std::pair<uint32_t, std::shared_ptr<uint8_t>>>> segmentMap;
    std::shared_ptr<uint8_t> rawBuffer;
    std::optional<std::pair<std::shared_ptr<uint8_t>, uint32_t>> msduBuffer;
    uint32_t posMSDU;
    std::optional<uint32_t> posPicoScenesHeader;
    std::optional<uint32_t> posExtraInfo;
    std::optional<uint32_t> posSegments;
    uint32_t rawBufferLength;

    static bool isOldRXSEnhancedFrame(const uint8_t bufferHead[6]);

    static std::optional<PicoScenesRxFrameStructure> fromBuffer(const uint8_t *buffer, uint32_t bufferLength, RXSParsingLevel parsingLevel = RXSParsingLevel::EXTRA_NOCSI);

    static std::optional<PicoScenesRxFrameStructure> concatenateFragmentedPicoScenesRxFrames(const std::vector<PicoScenesRxFrameStructure> &frameQueue);

    static PicoScenesRxFrameStructure fromRXSEnhancedBuffer(const uint8_t *buffer);

    int addOrReplaceSegment(const std::pair<std::string, std::pair<uint32_t, std::shared_ptr<uint8_t>>> &newSegment);

    bool operator==(const PicoScenesRxFrameStructure &rhs) const;

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const PicoScenesRxFrameStructure &rxframe);

class PicoScenesFrameTxParameters {
public:
    bool useHTRate;
    double mcs;
    bool useShortGI;
    bool useLDPC;
    //TODO 改成枚举class
    bool channelBonding;
    bool forceSounding;
    double numExtraSounding;
    double actualNumTxAnts;
    double idleTime;
    double scramblerState;
    bool maxPowerScaleTo1;
    double txIQAmplitudeImbalance_dB;
    double txIQPhaseImbalance_rad;

    PicoScenesFrameTxParameters() {
        useHTRate = true;
        mcs = 0;
        useShortGI = true;
        useLDPC = true;
        channelBonding = false;
        forceSounding = true;
        numExtraSounding = 0;
        actualNumTxAnts = 1;
        idleTime = 20e-6;
        scramblerState = 39;
        maxPowerScaleTo1 = 1;
        txIQAmplitudeImbalance_dB = 0;
        txIQPhaseImbalance_rad = 0;
    }

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameTxParameters &parameters);

class PicoScenesTxFrameStructure {
public:
    ieee80211_mac_frame_header standardHeader;
    PicoScenesFrameHeader frameHeader;
    PicoScenesFrameTxParameters txParameters;
    std::optional<ExtraInfo> extraInfo;
    std::map<std::string, std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH>> segmentBuffer;
    std::map<std::string, uint32_t> segmentLength;

    void reset();

    [[nodiscard]] uint32_t totalLength() const;

    int toBuffer(uint8_t *buffer, std::optional<uint16_t> bufferLength = std::nullopt) const;

    [[nodiscard]] std::shared_ptr<uint8_t> toBuffer() const;

    PicoScenesTxFrameStructure &addExtraInfo(const ExtraInfo &txExtraInfo);

    PicoScenesTxFrameStructure &addSegmentBuffer(const std::string &identifier, const uint8_t *buffer, uint32_t length);

    PicoScenesTxFrameStructure &addSegmentBuffer(const std::string &identifier, const std::array<uint8_t, PICOSCENES_FRAME_SEGMENT_MAX_LENGTH> &bufferArray, uint32_t length);

    PicoScenesTxFrameStructure &setMoreFrags();

    PicoScenesTxFrameStructure &setFragNumber(uint8_t fragNumber);

    PicoScenesTxFrameStructure &setRetry();

    PicoScenesTxFrameStructure &setTaskId(uint16_t taskId);

    PicoScenesTxFrameStructure &setRandomTaskId();

    PicoScenesTxFrameStructure &setPicoScenesFrameType(uint8_t frameType);

    PicoScenesTxFrameStructure &setChannelBonding(bool useChannelBonding);

    PicoScenesTxFrameStructure &setSGI(bool useSGI);

    PicoScenesTxFrameStructure &setMCS(uint8_t mcs);

    PicoScenesTxFrameStructure &setDestinationAddress(const uint8_t macAddr[6]);

    PicoScenesTxFrameStructure &setSourceAddress(const uint8_t macAddr[6]);

    PicoScenesTxFrameStructure &set3rdAddress(const uint8_t macAddr[6]);

    PicoScenesTxFrameStructure &setForceSounding(bool forceSounding);

    PicoScenesTxFrameStructure &setNumExtraSounding(uint8_t numExtraSounding);

    PicoScenesTxFrameStructure &useLDPC(bool useLDPC);

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const PicoScenesTxFrameStructure &txframe);

#endif //PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
