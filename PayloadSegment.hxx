//
// Created by Zhiping Jiang on 12/13/20.
//

#ifndef PICOSCENES_PLATFORM_PAYLOADSEGMENT_HXX
#define PICOSCENES_PLATFORM_PAYLOADSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

/**
 * @brief Enumeration of payload data types
 */
enum class PayloadDataType : uint8_t {
    RawData = 0,              ///< Raw binary data
    SegmentData,             ///< Segment-specific data
    SignalMatrix,            ///< Signal matrix data
    CSIData,                 ///< Channel State Information data
    FullMSDUPacket,         ///< Complete MAC Service Data Unit packet
    FullPicoScenesPacket,   ///< Complete PicoScenes packet
};

std::ostream &operator<<(std::ostream &os, const PayloadDataType &payloadDataType);

/**
 * @brief Container class for payload data
 * @details Holds the payload type, description, and actual data content
 */
class PayloadData {
public:
   PayloadDataType dataType = PayloadDataType::RawData;  ///< Type of payload data
    std::string payloadDescription;                        ///< Description of the payload
    std::vector<uint8_t> payloadData;                     ///< Actual payload data

    /**
     * @brief Serialize payload data to buffer
     * @return Vector containing serialized data
     */
    std::vector<uint8_t> toBuffer();

    /**
     * @brief Create PayloadData from buffer
     * @param buffer Input buffer
     * @param bufferLength Length of buffer
     * @return Constructed PayloadData object
     * @throw std::runtime_error if payload length is inconsistent
     */
    static PayloadData fromBuffer(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Create PayloadData from vector buffer
     * @param buffer Input buffer as vector
     * @return Constructed PayloadData object
     */
    static PayloadData fromBuffer(const std::vector<uint8_t> &buffer);
};

/**
 * @brief Segment class for handling payload data in PicoScenes frames
 * @details Implements payload management with version-specific parsing support
 */
class PayloadSegment : public AbstractPicoScenesFrameSegment {

public:
    /**
     * @brief Default constructor
     */
    PayloadSegment();

    /**
     * @brief Construct from buffer
     * @param buffer Input buffer
     * @param bufferLength Length of buffer
     * @throw std::runtime_error if segment name or version is not supported
     */
    PayloadSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Construct with vector payload
     * @param description Payload description
     * @param payload Vector containing payload data
     * @param payloadType Optional payload type (defaults to RawData)
     */
    PayloadSegment(const std::string &description, const std::vector<uint8_t> &payload,
                   std::optional<PayloadDataType> payloadType = std::nullopt);

    /**
     * @brief Construct with buffer payload
     * @param description Payload description
     * @param payloadBuffer Buffer containing payload data
     * @param bufferLength Length of buffer
     * @param payloadType Optional payload type (defaults to RawData)
     */
    PayloadSegment(const std::string &description, const uint8_t *payloadBuffer,
                   size_t bufferLength, std::optional<PayloadDataType> payloadType = std::nullopt);

    /**
     * @brief Construct with string payload
     * @param description Payload description
     * @param stringPayload String containing payload data
     * @param payloadType Optional payload type (defaults to RawData)
     */
    PayloadSegment(const std::string &description, const std::string &stringPayload,
                   std::optional<PayloadDataType> payloadType = std::nullopt);

    /**
     * @brief Get payload data
     * @return Const reference to PayloadData
     */
    [[nodiscard]] const PayloadData &getPayloadData() const;

    /**
     * @brief Set payload data
     * @param payload New payload data to set
     */
    void setPayloadData(const PayloadData &payload);

    /**
     * @brief Get string representation
     * @return String describing the segment
     */
    [[nodiscard]] std::string toString() const override;

private:
    /// Map of version-specific parsing solutions
    static std::map<uint16_t, std::function<PayloadData(const uint8_t *, uint32_t)>> versionedSolutionMap;

    /**
     * @brief Initialize solution map
     * @return Map of version-specific parsing functions
     */
    static std::map<uint16_t, std::function<PayloadData(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    /// Payload data storage
    PayloadData payloadData;
};

#endif //PICOSCENES_PLATFORM_PAYLOADSEGMENT_HXX
