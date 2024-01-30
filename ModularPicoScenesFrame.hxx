//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX

#include "PicoScenesCommons.hxx"
#include "PicoScenesFrameTxParameters.hxx"
#include "AbstractPicoScenesFrameSegment.hxx"
#include "CSISegment.hxx"
#include "RxSBasicSegment.hxx"
#include "ExtraInfoSegment.hxx"
#include "MVMExtraSegment.hxx"
#include "SDRExtraSegment.hxx"
#include "BasebandSignalSegment.hxx"
#include "PayloadSegment.hxx"
#include "CargoSegment.hxx"

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

    [[nodiscard]] std::string getTypeString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header_frame_control_field &fc);

/**
 * @brief MPDU header. This structure is verified in Wireshark.
 */
struct ieee80211_mac_frame_header {
    ieee80211_mac_frame_header_frame_control_field fc;
    uint16_t dur = 0;
    std::array<uint8_t, 6> addr1{0x00, 0x16, 0xea, 0x12, 0x34, 0x56};
    std::array<uint8_t, 6> addr2{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    std::array<uint8_t, 6> addr3{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint16_t frag: 4,
            seq: 12;

    ieee80211_mac_frame_header() : frag(0), seq(0) {};

    static ieee80211_mac_frame_header createFromBuffer(const uint8_t *buffer, std::optional<uint32_t> bufferLength);

    [[nodiscard]] std::string toString() const;

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header);

struct PicoScenesFrameHeader {
    [[maybe_unused]] uint32_t magicValue = 0x20150315;
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

class ModularPicoScenesRxFrameHeader {
public:
    uint32_t frameLength = 0;
    uint32_t magicWord = 0x20150315;
    uint16_t frameVersion = 0x2U;
    uint8_t numRxSegments = 0;
    uint16_t numMPDU = 1; // size of A-MPDU

    ModularPicoScenesRxFrameHeader &initialize2Default() {
        magicWord = 0x20150315;
        frameVersion = 0x2U;
        numRxSegments = 0;
        numMPDU = 1;
        return *this;
    }

} __attribute__ ((__packed__));

class ModularPicoScenesRxFrame {
public:
    // Rx side header and segments
    ModularPicoScenesRxFrameHeader rxFrameHeader;
    std::shared_ptr<RxSBasicSegment> rxSBasicSegment;
    std::shared_ptr<ExtraInfoSegment> rxExtraInfoSegment;
    std::shared_ptr<CSISegment> csiSegment;
    std::shared_ptr<MVMExtraSegment> mvmExtraSegment;
    std::shared_ptr<SDRExtraSegment> sdrExtraSegment;
    std::shared_ptr<CSISegment> legacyCSISegment;
    std::shared_ptr<BasebandSignalSegment> basebandSignalSegment;
    std::map<std::string, std::shared_ptr<AbstractPicoScenesFrameSegment>> rxUnknownSegments;

    // Tx side header and segments
    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> PicoScenesHeader;
    std::shared_ptr<ExtraInfoSegment> txExtraInfoSegment;
    std::vector<std::shared_ptr<PayloadSegment>> payloadSegments;
    std::vector<std::shared_ptr<CargoSegment>> cargoSegments;
    std::map<std::string, std::shared_ptr<AbstractPicoScenesFrameSegment>> txUnknownSegments;
    std::vector<U8Vector> mpdus; // unified single-MPDU and A-MPDU

    bool isNDP{false};

    static std::optional<ModularPicoScenesRxFrame> fromBuffer(const uint8_t *inputBuffer, uint32_t bufferLength, bool interpolateCSI = false);

    static std::optional<ModularPicoScenesRxFrame> concatenateFragmentedPicoScenesRxFrames(const std::vector<ModularPicoScenesRxFrame> &frameQueue);

    [[nodiscard]] std::string toString() const;

    void rebuildRawBuffer();

    [[nodiscard]] U8Vector toBuffer() const;

private:
    U8Vector rawBuffer;
};

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesRxFrame &rxframe);

/**
 * \brief TxFrame represent the frame data structure to be transmitted.
 *
 * Two MUTUALLY EXCLUSIVE approaches to specify the frame content:
 *  1. through PicoScenes segment-based structure (frameHeader, segments, etc.)
 *  2. the diract arbitraryAMPDUContent, which directly specifies the bytes of a whole AMPDU. If you want to transmit a single MPDU, just specify the first element of it.
 *
 *  REMINDER: If arbitraryAMPDUContent is not std::nullopt, the segment-based appraoch will be skipped! Two appraoches are mutually exclusive!
 */
class ModularPicoScenesTxFrame {
public:
    // Transmission parameters
    PicoScenesFrameTxParameters txParameters;

    // Content specification Approach 1: PicoScenes Segment-based frame structure, with additional AMPDU
    ieee80211_mac_frame_header standardHeader;
    std::optional<PicoScenesFrameHeader> frameHeader;
    std::vector<std::shared_ptr<AbstractPicoScenesFrameSegment>> segments;
    std::vector<ModularPicoScenesTxFrame> additionalAMPDUFrames;

    // Content specification Approach 2: Arbitrary AMPDU content
    std::optional<std::vector<U8Vector>> arbitraryAMPDUContent;

    // Used for storing prebuilt baseband signals
    std::vector<CS16Vector> prebuiltSignals;

    ModularPicoScenesTxFrame & appendAMPDUFrame(const ModularPicoScenesTxFrame &frame);

    ModularPicoScenesTxFrame &  addSegment(const std::shared_ptr<AbstractPicoScenesFrameSegment> &segment);

    std::shared_ptr<AbstractPicoScenesFrameSegment> getSegment(const std::string &querySegmentName);

    template<typename SegmentT, class = typename std::enable_if<std::is_base_of<AbstractPicoScenesFrameSegment, SegmentT>::value>::type>
    std::shared_ptr<SegmentT> getTypedSegment(const std::string &querySegmentName) {
        auto basePointer = getSegment(querySegmentName);
        return basePointer ? std::dynamic_pointer_cast<SegmentT>(basePointer) : nullptr;
    }

    void reset();

    [[nodiscard]] std::vector<U8Vector> toBuffer() const;

    [[nodiscard]] std::vector<ModularPicoScenesTxFrame> autoSplit(int16_t maxSegmentBuffersLength = 1400, std::optional<uint16_t> firstSegmentCappingLength = std::nullopt, std::optional<uint16_t> maxNumMPDUInAMPDU = std::nullopt) const;

    ModularPicoScenesTxFrame &setMoreFrags();

    ModularPicoScenesTxFrame &setFragNumber(uint8_t fragNumber);

    ModularPicoScenesTxFrame &setSequenceId(uint16_t sequence);

    ModularPicoScenesTxFrame &setRetry();

    ModularPicoScenesTxFrame &setDeviceType(PicoScenesDeviceType deviceType);

    ModularPicoScenesTxFrame &setTaskId(uint16_t taskId);

    ModularPicoScenesTxFrame &setTxId(uint16_t txId);

    ModularPicoScenesTxFrame &setRandomTaskId();

    ModularPicoScenesTxFrame &setPicoScenesFrameType(uint8_t frameType);

    ModularPicoScenesTxFrame &setTxParameters(const PicoScenesFrameTxParameters &parameters);

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

    ModularPicoScenesTxFrame &setTxHEExtendedRange(bool txHEExtendedRange);

    ModularPicoScenesTxFrame &setHEHighDoppler(bool heHighDoppler);

    ModularPicoScenesTxFrame &setHEMidamblePeriodicity(uint8_t heMidamblePeriodicity);

    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesTxFrame &txframe);


#endif //PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
