//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX

#include "macros.hxx"
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
#include "EQDataSymbolsSegment.hxx"

/**
 * @brief IEEE 802.11 MAC frame control field structure
 */
PACKED(struct ieee80211_mac_frame_header_frame_control_field {
    uint16_t version: 2,     ///< Protocol version
            type: 2,         ///< Frame type
            subtype: 4,      ///< Frame subtype
            toDS: 1,         ///< To Distribution System
            fromDS: 1,       ///< From Distribution System
            moreFrags: 1,    ///< More fragments
            retry: 1,        ///< Retry
            power_mgmt: 1,   ///< Power management
            more: 1,         ///< More data
            protect: 1,      ///< Protected frame
            order: 1;        ///< Strictly ordered

     /**
     * @brief Default constructor initializing for Action Frame NO ACK
     * @note Supported by both QCA9300 and IWL5300
     */
    ieee80211_mac_frame_header_frame_control_field() : version(0), type(0), subtype(0xe), toDS(0), fromDS(0), moreFrags(0), retry(0), power_mgmt(0), more(0), protect(0), order(1) {}

    /**
     * @brief Get string representation of frame type
     * @return String describing the frame type
     */
    [[nodiscard]] std::string getTypeString() const;

});

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header_frame_control_field &fc);

/**
 * @brief IEEE 802.11 MAC Protocol Data Unit (MPDU) header structure
 * @note This structure is verified in Wireshark
 */
PACKED(struct ieee80211_mac_frame_header {
    ieee80211_mac_frame_header_frame_control_field fc;  ///< Frame control field
    uint16_t dur = 0;                                   ///< Duration
    std::array<uint8_t, 6> addr1;                      ///< Address 1 (Destination)
    std::array<uint8_t, 6> addr2;                      ///< Address 2 (Source)
    std::array<uint8_t, 6> addr3;                      ///< Address 3
    uint16_t frag: 4,                                  ///< Fragment number
            seq: 12;                                    ///< Sequence number

    /**
     * @brief Default constructor
     */
    ieee80211_mac_frame_header() : frag(0), seq(0) {};

    /**
     * @brief Create header from buffer
     * @param buffer Input buffer
     * @param bufferLength Optional buffer length
     * @return Constructed MAC frame header
     */
    static ieee80211_mac_frame_header createFromBuffer(const uint8_t *buffer, std::optional<uint32_t> bufferLength);

    /**
     * @brief Get string representation of header
     * @return String describing the header
     */
    [[nodiscard]] std::string toString() const;

});

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header);

/**
 * @brief PicoScenes frame header structure
 */
PACKED(struct PicoScenesFrameHeader {
    uint32_t magicValue = 0x20150315;                  ///< Magic number for validation
    uint32_t version = 0x20201110;                     ///< Version identifier
    PicoScenesDeviceType deviceType;                   ///< Device type
    uint8_t numSegments = 0;                          ///< Number of segments
    uint8_t frameType = 0;                            ///< Frame type
    uint16_t taskId = 0;                              ///< Task identifier
    uint16_t txId = 0;                                ///< Transmission identifier

    /**
     * @brief Create header from buffer
     * @param buffer Input buffer
     * @return Optional containing the header if valid
     */
    static std::optional<PicoScenesFrameHeader> fromBuffer(const uint8_t *buffer);

    /**
     * @brief Get string representation of header
     * @return String describing the header
     */
    [[nodiscard]] std::string toString() const;

});

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameHeader &frameHeader);

/**
 * @brief PicoScenes receive frame header structure
 */
PACKED(class ModularPicoScenesRxFrameHeader {
public:
    uint32_t frameLength = 0;                          ///< Total frame length
    uint32_t magicWord = 0x20150315;                  ///< Magic number for validation
    uint16_t frameVersion = 0x2U;                     ///< Frame version
    uint8_t numRxSegments = 0;                        ///< Number of receive segments
    uint16_t numMPDU = 1;                             ///< Number of MPDUs (A-MPDU size)

    /**
     * @brief Initialize header with default values
     * @return Reference to initialized header
     */
    ModularPicoScenesRxFrameHeader &initialize2Default() {
        magicWord = 0x20150315;
        frameVersion = 0x2U;
        numRxSegments = 0;
        numMPDU = 1;
        return *this;
    }

});

class ModularPicoScenesRxFrame {
public:
    ModularPicoScenesRxFrameHeader rxFrameHeader;      ///< Receive frame header
    std::shared_ptr<RxSBasicSegment> rxSBasicSegment; ///< Basic receive segment
    std::shared_ptr<ExtraInfoSegment> rxExtraInfoSegment; ///< Extra info segment
    std::shared_ptr<CSISegment> csiSegment;           ///< CSI segment
    std::shared_ptr<MVMExtraSegment> mvmExtraSegment; ///< MVM extra segment
    std::shared_ptr<SDRExtraSegment> sdrExtraSegment; ///< SDR extra segment
    std::shared_ptr<CSISegment> legacyCSISegment;     ///< Legacy CSI segment
    std::shared_ptr<BasebandSignalSegment> basebandSignalSegment; ///< Baseband signal segment
    std::shared_ptr<EQDataSymbolsSegment> eqDataSymbolsSegment; ///< Equalized data symbols segment
    std::map<std::string, std::shared_ptr<AbstractPicoScenesFrameSegment>> rxUnknownSegments; ///< Unknown receive segments

    ieee80211_mac_frame_header standardHeader;         ///< Standard 802.11 header
    std::vector<U8Vector> mpdus;                      ///< MPDUs
    bool isNDP{false};                                ///< Null Data Packet flag

    std::optional<PicoScenesFrameHeader> PicoScenesHeader; ///< PicoScenes header
    std::shared_ptr<ExtraInfoSegment> txExtraInfoSegment; ///< Transmit extra info segment
    std::vector<std::shared_ptr<PayloadSegment>> payloadSegments; ///< Payload segments
    std::vector<std::shared_ptr<CargoSegment>> cargoSegments; ///< Cargo segments
    std::map<std::string, std::shared_ptr<AbstractPicoScenesFrameSegment>> txUnknownSegments; ///< Unknown transmit segments

    /**
     * @brief Create frame from buffer
     * @param inputBuffer Input buffer
     * @param bufferLength Buffer length
     * @param interpolateCSI Whether to interpolate CSI
     * @return Optional containing the frame if valid
     */
    static std::optional<ModularPicoScenesRxFrame> fromBuffer(const uint8_t *inputBuffer, uint32_t bufferLength, bool interpolateCSI = false);

     /**
     * @brief Concatenate fragmented frames
     * @param frameQueue Vector of fragmented frames
     * @return Optional containing the concatenated frame if successful
     */
    static std::optional<ModularPicoScenesRxFrame> concatenateFragmentedPicoScenesRxFrames(const std::vector<ModularPicoScenesRxFrame> &frameQueue);

    /**
     * @brief Get string representation of frame
     * @return String describing the frame
     */
    [[nodiscard]] std::string toString() const;

    /**
     * @brief Rebuild raw buffer from frame components
     */
    void rebuildRawBuffer();

    /**
     * @brief Convert frame to buffer
     * @return Vector containing frame data
     */
    [[nodiscard]] U8Vector toBuffer() const;

private:
    U8Vector rawBuffer;                               ///< Raw buffer storage
};

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesRxFrame &rxframe);

/**
 * @brief PicoScenes transmit frame class
 * @details Two MUTUALLY EXCLUSIVE approaches to specify the frame content:
 *  1. Through PicoScenes segment-based structure (frameHeader, segments, etc.)
 *  2. The direct arbitraryAMPDUContent, which directly specifies the bytes of a whole AMPDU
 *
 * @note If arbitraryAMPDUContent is not std::nullopt, the segment-based approach will be skipped!
 */
class ModularPicoScenesTxFrame {
public:
    PicoScenesFrameTxParameters txParameters;         ///< Transmission parameters
    ieee80211_mac_frame_header standardHeader;        ///< Standard 802.11 header
    std::optional<PicoScenesFrameHeader> frameHeader; ///< PicoScenes header
    std::vector<std::shared_ptr<AbstractPicoScenesFrameSegment>> segments; ///< Frame segments
    std::vector<ModularPicoScenesTxFrame> additionalAMPDUFrames; ///< Additional A-MPDU frames
    std::optional<std::vector<U8Vector>> arbitraryAMPDUContent; ///< Arbitrary A-MPDU content
    std::vector<CS16Vector> prebuiltSignals;         ///< Prebuilt baseband signals

    /**
     * @brief Append frame to A-MPDU
     * @param frame Frame to append
     * @return Reference to this frame
     */
    ModularPicoScenesTxFrame & appendAMPDUFrame(const ModularPicoScenesTxFrame &frame);

    /**
     * @brief Add segment to frame
     * @param segment Segment to add
     * @return Reference to this frame
     */
    ModularPicoScenesTxFrame &  addSegment(const std::shared_ptr<AbstractPicoScenesFrameSegment> &segment);

    /**
     * @brief Get segment by name
     * @param querySegmentName Name of segment to get
     * @return Shared pointer to segment if found
     */
    std::shared_ptr<AbstractPicoScenesFrameSegment> getSegment(const std::string &querySegmentName);

    /**
     * @brief Get typed segment by name
     * @tparam SegmentT Type of segment
     * @param querySegmentName Name of segment to get
     * @return Shared pointer to typed segment if found
     */
    template<typename SegmentT, class = typename std::enable_if<std::is_base_of<AbstractPicoScenesFrameSegment, SegmentT>::value>::type>
    std::shared_ptr<SegmentT> getTypedSegment(const std::string &querySegmentName) {
        auto basePointer = getSegment(querySegmentName);
        return basePointer ? std::dynamic_pointer_cast<SegmentT>(basePointer) : nullptr;
    }

    /**
     * @brief Reset frame to default state
     */
    void reset();

    /**
     * @brief Convert frame to buffer
     * @return Vector of buffers (for A-MPDU)
     */
    [[nodiscard]] std::vector<U8Vector> toBuffer() const;

    /**
     * @brief Auto-split frame into multiple frames
     * @param maxSegmentBuffersLength Maximum segment buffer length
     * @param firstSegmentCappingLength Optional length cap for first segment
     * @param maxNumMPDUInAMPDU Optional maximum number of MPDUs in A-MPDU
     * @return Vector of split frames
     */
    [[nodiscard]] std::vector<ModularPicoScenesTxFrame> autoSplit(int16_t maxSegmentBuffersLength = 1400, std::optional<uint16_t> firstSegmentCappingLength = std::nullopt, std::optional<uint16_t> maxNumMPDUInAMPDU = std::nullopt) const;

    // Setter methods for frame parameters
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

    /**
     * @brief Get string representation of frame
     * @return String describing the frame
     */
    [[nodiscard]] std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesTxFrame &txframe);


#endif //PICOSCENES_PLATFORM_MODULARPICOSCENESFRAME_HXX
