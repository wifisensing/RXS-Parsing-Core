//
// Created by  Zhiping Jiang on 10/23/22.
//

#ifndef PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX

#include <unordered_map>
#include "macros.hxx"
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

/**
 * @brief Structure containing extra information for Software Defined Radio (SDR) data
 *
 * @details This structure stores additional metadata related to SDR signal processing,
 * including timing information, signal quality metrics, and hardware configuration parameters.
 */
PACKED(struct SDRExtra {
    int8_t scramblerInit{0};           /**< Initial value of the scrambler */
    int64_t packetStartInternal{0};    /**< Internal timestamp for packet start */
    int64_t hardwareRxSamplingIndex{0}; /**< Hardware sampling index for received signal */
    double preciseRxTime{0};           /**< Precise reception time */
    double lastTxTime{0};              /**< Last transmission time */
    double signalInputSystemTime{0};   /**< System time when signal was input */
    double signalDecodeSystemTime{0};  /**< System time when signal was decoded */
    double sigEVM{0};                  /**< The EVM estimated by the L-SIG field */
    double initialCFO{0};              /**< The CFO estimated by L-STF and L-LTF. This CFO is used to rotate the following L-LTF and Non-L-LTFs, so affecting the CFO measured from CSI. */
    double residualCFO{0};             /**< The CFO estimated by the Pilot subcarriers per-OFDM symbol. This estimation is much more accurate than initialCFO if the PPDU is long, e.g., > 1ms. The reported ExtraInfo::cfo = initialCFO + residualCFO. */

    /**
     * @brief Calculate the decoding delay
     *
     * @return double The time difference between signal decode and input system times
     */
    [[nodiscard]] inline double decodingDelay() const {
        return signalDecodeSystemTime - signalInputSystemTime;
    }

    /**
     * @brief Convert this SDRExtra to a string representation
     *
     * @return std::string String representation of this SDRExtra
     */
    [[nodiscard]] std::string toString() const;

    /**
     * @brief Convert this SDRExtra to a binary buffer
     *
     * @return std::vector<uint8_t> The binary representation
     */
    std::vector<uint8_t> toBuffer();
});

/**
 * @brief Stream insertion operator for SDRExtra
 *
 * @param os Output stream
 * @param sdrExtra SDRExtra object to insert
 * @return std::ostream& Reference to the output stream
 */
std::ostream &operator<<(std::ostream &os, const SDRExtra &sdrExtra);


/**
 * @brief Class representing a Software Defined Radio extra information segment in a PicoScenes frame
 *
 * @details This class extends AbstractPicoScenesFrameSegment to provide functionality for
 * handling SDR-specific extra information in PicoScenes frames. It supports multiple versions
 * of the SDR extra information format and provides methods for parsing, serialization, and
 * string representation.
 */
class SDRExtraSegment : public AbstractPicoScenesFrameSegment {
public:
    /**
     * @brief Default constructor
     *
     * @details Initializes a new SDRExtraSegment with default values
     */
    SDRExtraSegment();

    /**
     * @brief Constructor from binary data
     *
     * @details Creates a new SDRExtraSegment by parsing binary data
     *
     * @param buffer Pointer to the binary data
     * @param bufferLength Length of the binary data
     */
    SDRExtraSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Constructor from SDRExtra structure
     *
     * @details Creates a new SDRExtraSegment from an existing SDRExtra structure
     *
     * @param sdrExtra The SDRExtra structure to use
     */
    explicit SDRExtraSegment(const SDRExtra &sdrExtra);

    /**
     * @brief Get the SDRExtra structure
     *
     * @return const SDRExtra& Reference to the SDRExtra structure
     */
    [[nodiscard]] const SDRExtra &getSdrExtra() const;

    /**
     * @brief Set the SDRExtra structure
     *
     * @param sdrExtra The SDRExtra structure to set
     */
    void setSdrExtra(const SDRExtra &sdrExtra);

    /**
     * @brief Convert this segment to a string representation
     *
     * @return std::string String representation of this segment
     */
    [[nodiscard]] std::string toString() const override;

private:
    /**
     * @brief Map of version IDs to parser functions
     *
     * @details Maps version IDs to functions that parse binary data into SDRExtra structures
     */
    static std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> versionedSolutionMap;

    /**
     * @brief Initialize the versioned solution map
     *
     * @details Creates and returns a map of version IDs to parser functions
     *
     * @return std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> The initialized map
     */
    static std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    /**
     * @brief The SDRExtra structure containing the extra information
     */
    SDRExtra sdrExtra{};
};

#endif //PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX
