//
// Created by Zhiping Jiang on 2020/11/5.
//

#ifndef PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX
#define PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX

#include "PicoScenesCommons.hxx"
#include "DynamicFieldInterpretation.hxx"

/**
 * @brief PicoScenes Frame Segment is a typed, versioned and named data structure used to carrier various of data from the Tx end, PicoScenes driver or SDR baseband to the PicoScenes Platform.
 * @details AbstractPicoScenesFrameSegment is the root class for various of Frame segment classes.
 */
class AbstractPicoScenesFrameSegment {
public:
    std::string segmentName; ///< The name of the segment
    uint16_t segmentVersionId; ///< The version of the segment
    std::vector<uint8_t> segmentPayload; ///< The actual payload part of the segment
    std::vector<uint8_t> rawBuffer; ///< Raw buffer contains the segment header and the payload
    bool rawBufferSynced{false}; ///< Indicating if the rawBuffer is in-sync with the segment meta Info AND the segmentPayload

    /**
     * Extract the SegmentName, SegmentLength (except the 4 bytes of the field itself), 2-byte version, and offset of the segment payload
     * @return
     */
    static std::tuple<std::string, uint32_t, uint16_t, uint32_t> extractSegmentMetaData(const uint8_t *buffer, const uint32_t bufferLength) {
        uint32_t rxPos = 0;
        uint32_t segmentLength = *reinterpret_cast<const uint32_t *>(buffer + rxPos);
        rxPos += 4;
        if (segmentLength > bufferLength)
            throw std::runtime_error("corrupted segment...");
        const auto segNameLength = *(buffer + rxPos++);
        auto segmentName = std::string(reinterpret_cast<const char *>(buffer + rxPos), *reinterpret_cast<const char *>(buffer + rxPos + segNameLength - 1) == '\0' ? reinterpret_cast<const char *>(buffer + rxPos + segNameLength - 1) : reinterpret_cast<const char *>(buffer + rxPos + segNameLength));
        rxPos += segNameLength;
        uint16_t segmentVersionId = *reinterpret_cast<const uint16_t *>(buffer + rxPos);
        rxPos += 2;

        return std::make_tuple(segmentName, segmentLength, segmentVersionId, rxPos);
    }

    AbstractPicoScenesFrameSegment(std::string &&segmentNameV, const uint16_t segmentVersionId) : segmentName(std::move(segmentNameV)), segmentVersionId(segmentVersionId) {}

    AbstractPicoScenesFrameSegment(const uint8_t *buffer, const size_t bufferLength) {
        auto [segmentNameV, segmentLengthV, versionIdV, offset] = extractSegmentMetaData(buffer, bufferLength);
        if (segmentLengthV + 4 > bufferLength)
            throw std::underflow_error("Cannot parse the segment [" + segmentNameV + "] version [" + std::to_string(versionIdV) + "[with less than " + std::to_string(segmentLengthV + 4) + "B.");

        segmentName = std::move(segmentNameV);
        segmentVersionId = versionIdV;
        segmentPayload.reserve(bufferLength - offset + 1);
        std::copy(buffer + offset, buffer + bufferLength, std::back_inserter(segmentPayload));
        std::copy_n(buffer, bufferLength, std::back_inserter(rawBuffer));
        rawBufferSynced = true;
    }

    [[nodiscard]] const std::vector<uint8_t> &getSegmentPayload() const {
        return segmentPayload;
    }

    [[nodiscard]] const std::vector<uint8_t> &getRawBuffer() const {
        return rawBuffer;
    }

    [[nodiscard]] const std::vector<uint8_t> &getSyncedRawBuffer() {
        if (!rawBufferSynced)
            rebuildRawBuffer();
        return rawBuffer;
    }

    /**
     * @brief Change the payload data, the rawBuffer is also updated.
     * @param payload
     */
    void setSegmentPayload(const std::vector<uint8_t> &payload) {
        segmentPayload = payload;
        rawBufferSynced = false;
    }

    /**
     * The move semantic version of setSegmentPayload()
     * @param payload
     */
    void setSegmentPayload(std::vector<uint8_t> &&payload) {
        segmentPayload = std::move(payload);
        rawBufferSynced = false;
    }

    [[nodiscard]] DynamicFieldInterpreter getDynamicInterpreter() const {
        return DynamicFieldInterpreter{segmentName, segmentVersionId, segmentPayload.data()};
    }

    /**
     * @brief Return the total length of the segment buffer
     * 
     * @return uint32_t 
     */
    [[nodiscard]] uint32_t totalLength() const {
        /**
         * 1B segment name length;
         * segmentName.size() + 1B, for segment name and the ending \0
         * 2B of version Id
         */
        return 1 + segmentName.size() + 1 + sizeof(segmentVersionId) + segmentPayload.size();
    }

    /**
     * @return totalLength() with the leading 4-byte length
     */
    [[nodiscard]] inline uint32_t totalLengthIncludingLeading4ByteLength() const {
        return totalLength() + 4;
    }

    /**
     * @brief ModularPicoScenesTxFrame and ModularPicoScenesRxFrame call this method to get the whole data of this segment
     * @details By default, this method performs 3 things:
     * 1. call totalLength method to obtain the length of the raw buffer;
     * 2. pre-allocate a std::vector<uint8_t> buffer;
     * 3. fill the buffer with the other AbstractPicoScenesFrameSegment#toBuffer method
     *
     */
     [[deprecated("use getSyncedRawBuffer instead")]]
    [[nodiscard]] std::vector<uint8_t> toBuffer() const {
        if (rawBufferSynced)
            return rawBuffer;

        std::vector<uint8_t> result(totalLengthIncludingLeading4ByteLength());
        toBuffer(result.data(), result.size());
        return result;
    }

    void rebuildRawBuffer() {
        rawBuffer.clear();
        rawBuffer.resize( totalLengthIncludingLeading4ByteLength());
        toBuffer(rawBuffer.data(), rawBuffer.size());
        rawBufferSynced = true;
    }

    uint32_t toBuffer(uint8_t *buffer, const std::optional<uint32_t> bufferLength = std::nullopt) const {
        const uint32_t totalLengthV = totalLength();

        if (bufferLength && bufferLength < totalLengthV)
            throw std::invalid_argument("buffer not long enough...");

        uint8_t segNameLength = segmentName.length() + 1;
        uint32_t pos{0};
        std::copy_n(reinterpret_cast<const uint8_t *>(&totalLengthV), sizeof(totalLengthV), buffer + pos);
        pos += sizeof(totalLengthV);
        std::copy_n(&segNameLength, sizeof(segNameLength), buffer + pos);
        pos += sizeof(segNameLength);
        std::copy_n(reinterpret_cast<const uint8_t *>(segmentName.data()), segmentName.size() + 1, buffer + pos);
        pos += segmentName.size() + 1;
        std::copy_n(reinterpret_cast<const uint8_t *>(&segmentVersionId), sizeof(segmentVersionId), buffer + pos);
        pos += sizeof(segmentVersionId);
        std::copy_n(segmentPayload.data(), segmentPayload.size(), buffer + pos);
        pos += segmentPayload.size();

        return pos;
    }

    /**
     * @brief Return a short description for this segment
     */
    [[nodiscard]] virtual std::string toString() const {
        std::stringstream ss;
        ss << segmentName << "(" << segmentVersionId << "): " << totalLengthIncludingLeading4ByteLength() << "B";
        return ss.str();
    }

    virtual ~AbstractPicoScenesFrameSegment() = default;
};

inline std::ostream &operator<<(std::ostream &os, const AbstractPicoScenesFrameSegment &segment) {
    os << segment.toString();
    return os;
}


#endif //PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX
