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
#include "PicoScenesFrameTxParameters.hxx"

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

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header);

struct PicoScenesFrameHeader {
    uint32_t magicValue = 0x20150315;
    uint32_t version = 0x20201110;
    PicoScenesDeviceType deviceType = PicoScenesDeviceType::QCA9300;
    uint8_t numSegments = 0;
    uint8_t frameType = 0;
    uint16_t taskId = 0;
    uint16_t txId = 0;

    static std::optional<PicoScenesFrameHeader> fromBuffer(const uint8_t *buffer);

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameHeader &frameHeader);


class ModularPicoScenesRxFrame {
public:
    // Rx side segments
    RxSBasicSegment rxSBasicSegment;
    ExtraInfoSegment rxExtraInfoSegment;
    CSISegment csiSegment;
    BasebandSignalSegment basebandSignalSegment;

    // Tx side header and segments
    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> PicoScenesHeader;
    std::optional<ExtraInfoSegment> txExtraInfoSegment;
    std::optional<CSISegment> txCSISegment;

    std::map<std::string, std::vector<uint8_t>> rxUnknownSegmentMap;
    std::map<std::string, std::vector<uint8_t>> txUnknownSegmentMap;
    std::vector<Uint8Vector> nonPicoScenesMSDUContent;
    Uint8Vector rawBuffer;

    static std::optional<ModularPicoScenesRxFrame> fromBuffer(const uint8_t *buffer, uint32_t bufferLength, bool interpolateCSI = false);

    static std::optional<ModularPicoScenesRxFrame> concatenateFragmentedPicoScenesRxFrames(const std::vector<ModularPicoScenesRxFrame> &frameQueue);

    bool operator==(const ModularPicoScenesRxFrame &rhs) const;

    [[nodiscard]] std::string toString() const;

protected:
    bool isSynthesized = false;

    Uint8Vector toBuffer();
};

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesRxFrame &parameters);

class ModularPicoScenesTxFrame {
public:
    ieee80211_mac_frame_header standardHeader;
    PicoScenesFrameHeader frameHeader;
    PicoScenesFrameTxParameters txParameters;
    std::vector<std::shared_ptr<AbstractPicoScenesFrameSegment>> segments;

    void addSegments(const std::shared_ptr<AbstractPicoScenesFrameSegment> &segment);

    void reset();

    [[nodiscard]] uint32_t totalLength() const;

    int toBuffer(uint8_t *buffer, uint32_t bufferLength) const;

    [[nodiscard]] Uint8Vector toBuffer() const;

    ModularPicoScenesTxFrame &setMoreFrags();

    ModularPicoScenesTxFrame &setFragNumber(uint8_t fragNumber);

    ModularPicoScenesTxFrame &setRetry();

    ModularPicoScenesTxFrame &setTaskId(uint16_t taskId);

    ModularPicoScenesTxFrame &setTxId(uint16_t txId);

    ModularPicoScenesTxFrame &setRandomTaskId();

    ModularPicoScenesTxFrame &setPicoScenesFrameType(uint8_t frameType);

    [[maybe_unused]] ModularPicoScenesTxFrame &setFrameFormat(PacketFormatEnum format);

    ModularPicoScenesTxFrame &setChannelBandwidth(const ChannelBandwidthEnum &cbw);

    ModularPicoScenesTxFrame &setGuardInterval(GuardIntervalEnum guardInterval);

    ModularPicoScenesTxFrame &setMCS(uint8_t mcs);

    [[maybe_unused]] ModularPicoScenesTxFrame &setMCS(const std::vector<uint8_t> &mcs);

    ModularPicoScenesTxFrame &setNumSTS(uint8_t numSTS);

    ModularPicoScenesTxFrame &setNumSTS(const std::vector<uint8_t> &numSTSs);

    ModularPicoScenesTxFrame &setDestinationAddress(const uint8_t macAddr[6]);

    ModularPicoScenesTxFrame &setSourceAddress(const uint8_t macAddr[6]);

    ModularPicoScenesTxFrame &set3rdAddress(const uint8_t macAddr[6]);

    ModularPicoScenesTxFrame &setForceSounding(bool forceSounding);

    ModularPicoScenesTxFrame &setNumExtraSounding(uint8_t numExtraSounding);

    ModularPicoScenesTxFrame &setChannelCoding(ChannelCodingEnum codings);

    [[maybe_unused]] ModularPicoScenesTxFrame &setChannelCoding(const std::vector<ChannelCodingEnum> &codings);

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesTxFrame &txframe);


#endif //PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
