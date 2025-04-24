//
// Created by Zhiping Jiang on 4/20/22.
//

#ifndef PICOSCENES_PLATFORM_CARGOSEGMENT_HXX
#define PICOSCENES_PLATFORM_CARGOSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "RxSParsingCoreDLLImport.hxx"

/**
 * @brief Class managing compression algorithms for cargo data
 *
 * Provides static methods to register and manage compression/decompression algorithms
 * for cargo payload data.
 */
class CargoCompression {
public:
    /**
     * @brief Check if compression algorithm is registered
     * @return true if both compressor and decompressor are registered
     */
    static bool isAlgorithmRegistered() {
        return compressor && decompressor;
    }

    /**
     * @brief Register compression and decompression algorithms
     *
     * @param compressorV Function for compressing data
     * @param decompressorV Function for decompressing data
     * @note Registration is thread-safe and can only be done once
     */
    static void registerAlgorithm(const std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> &compressorV,
                                  const std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> &decompressorV) {
        static std::once_flag onceFlag;
        std::call_once(onceFlag, [&] {
            compressor = compressorV;
            decompressor = decompressorV;
        });
    }

    /**
     * @brief Get the registered compressor function
     * @return Reference to the compressor function
     */
    static const std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> &getCompressor() {
        return compressor;
    }

    /**
     * @brief Get the registered decompressor function
     * @return Reference to the decompressor function
     */
    static const std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> &getDecompressor() {
        return decompressor;
    }

private:
    /** @brief Function pointer for compression algorithm */
    RXS_PARSING_CORE_API static std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> compressor;

    /** @brief Function pointer for decompression algorithm */
    RXS_PARSING_CORE_API static std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> decompressor;
};

/**
 * @brief Class representing a cargo payload
 *
 * Contains metadata and actual payload data for a cargo segment. Supports
 * segmentation for large payloads and optional compression.
 */
class PayloadCargo {
public:
    uint16_t taskId{0};          ///< Task identifier
    uint8_t numSegments{0};      ///< Number of segments in the payload
    uint8_t sequence{0};         ///< Sequence number of this segment
    uint8_t totalParts{0};       ///< Total number of parts
    bool compressed{false};       ///< Whether the payload is compressed
    uint32_t payloadLength{0};   ///< Total length of the payload
    std::vector<uint8_t> payloadData; ///< Actual payload data

    /**
     * @brief Calculate total length of the cargo
     * @return Size in bytes including metadata and payload
     */
    [[nodiscard]] size_t totalLength() const;

    /**
     * @brief Serialize cargo to buffer
     * @return Vector containing serialized cargo data
     */
    [[nodiscard]] std::vector<uint8_t> toBuffer() const;

    /**
     * @brief Create cargo from buffer
     * @param buffer Pointer to buffer containing cargo data
     * @param bufferLength Length of buffer
     * @return Shared pointer to created PayloadCargo
     */
    static std::shared_ptr<PayloadCargo> fromBuffer(const uint8_t* buffer, uint32_t bufferLength);

    /**
     * @brief Create cargo from vector buffer
     * @param buffer Vector containing cargo data
     * @return Shared pointer to created PayloadCargo
     */
    static std::shared_ptr<PayloadCargo> fromBuffer(const std::vector<uint8_t> &buffer);

    /**
     * @brief Merge multiple cargo segments and validate the result
     *
     * @param cargos Vector of cargo segments to merge
     * @return Merged and validated payload data
     * @throw std::runtime_error If cargo validation fails
     */
    static U8Vector mergeAndValidateCargo(const std::vector<std::shared_ptr<PayloadCargo>>&cargos);
};

/**
 * @brief Class representing a cargo segment in PicoScenes frame
 *
 * Extends AbstractPicoScenesFrameSegment to handle cargo data, which can contain
 * multiple segments and support compression.
 */
class CargoSegment : public AbstractPicoScenesFrameSegment {
public:
    /**
     * @brief Default constructor
     *
     * Creates a new cargo segment with name "Cargo" and version 0x1
     */
    CargoSegment();

    /**
     * @brief Constructor with payload
     *
     * @param cargoV Shared pointer to payload cargo
     */
    explicit CargoSegment(const std::shared_ptr<PayloadCargo> &cargoV);

    /**
     * @brief Constructor from buffer
     *
     * @param buffer Pointer to buffer containing segment data
     * @param bufferLength Length of buffer
     * @throw std::runtime_error If segment name is not "Cargo" or version is not supported
     */
    CargoSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Get the cargo payload
     * @return Shared pointer to payload cargo
     */
    [[nodiscard]] std::shared_ptr<PayloadCargo> getCargo() const;

    /**
     * @brief Set new cargo payload
     * @param cargoV Shared pointer to new payload cargo
     */
    void setCargo(const std::shared_ptr<PayloadCargo> &cargoV);

    /**
     * @brief Get string representation of the segment
     * @return String containing segment details
     */
    [[nodiscard]] std::string toString() const override;

private:
    /** @brief Map of version-specific parsing solutions */
    static std::map<uint16_t, std::function<std::shared_ptr<PayloadCargo>(const uint8_t *, uint32_t)>> versionedSolutionMap;

    /**
     * @brief Initialize the version-specific solution map
     * @return Initialized map of parsing solutions
     */
    static std::map<uint16_t, std::function<std::shared_ptr<PayloadCargo>(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    /** @brief Pointer to cargo payload */
    std::shared_ptr<PayloadCargo> cargo{nullptr};
};

#endif //PICOSCENES_PLATFORM_CARGOSEGMENT_HXX
