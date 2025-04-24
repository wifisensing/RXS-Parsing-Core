//
// Created by Zhiping Jiang on 2020/11/5.
//

#ifndef PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX
#define PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX

#include "PicoScenesCommons.hxx"
#include "DynamicFieldInterpretation.hxx"

/**
 * @brief Abstract base class for PicoScenes frame segments
 *
 * PicoScenes Frame Segment is a typed, versioned and named data structure used to carry
 * various types of data from the Tx end, PicoScenes driver or SDR baseband to the PicoScenes Platform.
 * This class serves as the root class for various frame segment classes like CSISegment,
 * PayloadSegment, BasebandSignalSegment etc.
 */
struct AbstractPicoScenesFrameSegment {
    std::string segmentName{}; ///< The name identifier of the segment
    uint16_t segmentVersionId{}; ///< Version identifier of the segment format
    std::vector<uint8_t> segmentPayload{}; ///< The actual payload data of the segment
    mutable std::vector<uint8_t> rawBuffer{}; ///< Raw buffer containing both segment header and payload
    mutable bool rawBufferSynced{false}; ///< Flag indicating if rawBuffer is synchronized with segment metadata and payload

    /**
     * @brief Extract metadata from a segment buffer
     *
     * @param buffer Pointer to the buffer containing segment data
     * @param bufferLength Length of the buffer
     * @return tuple containing:
     *         - segment name (string)
     *         - segment length excluding the 4-byte length field (uint32_t)
     *         - version ID (uint16_t)
     *         - offset to payload data (uint32_t)
     * @throw std::runtime_error if segment is corrupted
     */
    static std::tuple<std::string, uint32_t, uint16_t, uint32_t> extractSegmentMetaData(const uint8_t* buffer, const uint32_t bufferLength) {
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

    /**
     * @brief Constructor for creating a new segment
     * @param segmentNameV Name of the segment
     * @param segmentVersionId Version ID of the segment
     */
    AbstractPicoScenesFrameSegment(std::string&& segmentNameV, const uint16_t segmentVersionId) : segmentName(std::move(segmentNameV)), segmentVersionId(segmentVersionId) {}

    /**
     * @brief Constructor for parsing existing segment from buffer
     * @param buffer Pointer to buffer containing segment data
     * @param bufferLength Length of the buffer
     * @throw std::underflow_error if buffer is too small
     */
    AbstractPicoScenesFrameSegment(const uint8_t* buffer, const size_t bufferLength) {
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

    /**
     * @brief Get the segment payload data
     * @return Const reference to payload data vector
     */
    [[nodiscard]] const std::vector<uint8_t>& getSegmentPayload() const {
        return segmentPayload;
    }

    /**
     * @brief Get the synchronized raw buffer
     * @return Const reference to raw buffer
     * @note Will rebuild buffer if not synchronized
     */
    [[nodiscard]] const std::vector<uint8_t>& getSyncedRawBuffer() const {
        if (!rawBufferSynced)
            rebuildRawBuffer();
        return rawBuffer;
    }

    /**
     * @brief Set new payload data for the segment
     * @tparam T Type of payload data (defaults to vector<uint8_t>)
     * @param payload New payload data
     * @note Marks raw buffer as out of sync
     */
    template<typename T = std::vector<uint8_t>>
    void setSegmentPayload(T&& payload) {
        segmentPayload = std::forward<T>(payload);
        rawBufferSynced = false;
    }

    /**
     * @brief Get dynamic field interpreter for this segment
     * @return DynamicFieldInterpreter instance
     */
    [[nodiscard]] DynamicFieldInterpreter getDynamicInterpreter() const {
        return DynamicFieldInterpreter{segmentName, segmentVersionId, segmentPayload.data()};
    }

    /**
     * @brief Calculate total length of the segment
     * @return Total length in bytes including:
     *         - 1 byte for segment name length
     *         - segment name length + 1 byte for null terminator
     *         - 2 bytes for version ID
     *         - payload length
     */
    [[nodiscard]] uint32_t totalLength() const {
        return 1 + segmentName.size() + 1 + sizeof(segmentVersionId) + segmentPayload.size();
    }

    /**
     * @brief Calculate total length including leading 4-byte length field
     * @return Total length plus 4 bytes
     */
    [[nodiscard]] inline uint32_t totalLengthIncludingLeading4ByteLength() const {
        return totalLength() + 4;
    }

    /**
     * @brief Rebuild the raw buffer from current segment data
     * @note Marks raw buffer as synchronized after rebuild
     */
    void rebuildRawBuffer() const {
        rawBuffer = U8Vector(totalLengthIncludingLeading4ByteLength());
        toBuffer(rawBuffer.data(), rawBuffer.size());
        rawBufferSynced = true;
    }

    /**
     * @brief Serialize segment data to a buffer
     * @param buffer Target buffer to write to
     * @param bufferLength Optional buffer length for validation
     * @return Number of bytes written
     * @throw std::invalid_argument if buffer is too small
     */
    uint32_t toBuffer(uint8_t* buffer, const std::optional<uint32_t> bufferLength = std::nullopt) const {
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
     * @brief Get string representation of the segment
     * @return String describing segment name, version and size
     */
    [[nodiscard]] virtual std::string toString() const {
        std::stringstream ss;
        ss << segmentName << "(" << segmentVersionId << "): " << totalLengthIncludingLeading4ByteLength() << "B";
        return ss.str();
    }

    virtual ~AbstractPicoScenesFrameSegment() = default;
};

/**
 * @brief Stream output operator for AbstractPicoScenesFrameSegment
 * @param os Output stream
 * @param segment Segment to output
 * @return Reference to output stream
 */
inline std::ostream& operator<<(std::ostream& os, const AbstractPicoScenesFrameSegment& segment) {
    os << segment.toString();
    return os;
}


#endif //PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX
