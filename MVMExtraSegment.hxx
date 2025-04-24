//
// Created by Zhiping Jiang on 9/9/21.
//

#ifndef PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX

#include "macros.hxx"
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "IntelRateNFlag.hxx"

/**
 * @brief Intel MVM CSI header parsing structure
 * @details Contains the parsed CSI header information from Intel MVM devices
 */
PACKED(class IntelMVMParsedCSIHeader {
public:
    union {
        PACKED(uint8_t headerBytes[272]);  ///< Raw header bytes
        struct {
            uint32_t iqDataSize;           ///< Size of IQ data
            PACKED(union {
                PACKED(uint32_t blockedSection4[12]);  ///< Reserved section
                PACKED(struct {
                    uint32_t value4;       ///< Reserved value
                    uint32_t ftmClock;     ///< FTM clock value
                    uint8_t value12[40];   ///< Reserved values
                });
            };
            uint32_t numTones;            ///< Number of tones
            uint32_t value56[9];          ///< Reserved values
            uint32_t rateNflag;           ///< Rate and flag information
            uint32_t value96[44];         ///< Reserved values
        });
    };

    /**
     * @brief Default constructor
     */
    IntelMVMParsedCSIHeader();

    /**
     * @brief Get rate and flag interpretation
     * @return IntelRateNFlag object containing interpreted values
     */
    [[nodiscard]] IntelRateNFlag getRateNFlagInterpretation() const;

    /**
     * @brief Equality comparison operator
     * @param rhs Right-hand side object to compare
     * @return true if objects are equal
     */
    bool operator==(const IntelMVMParsedCSIHeader &rhs) const;

    /**
     * @brief Inequality comparison operator
     * @param rhs Right-hand side object to compare
     * @return true if objects are not equal
     */
    bool operator!=(const IntelMVMParsedCSIHeader &rhs) const;

    /**
     * @brief Register default MVM header interpretation in the system
     */
    static void registerDefaultMVMHeaderInterpretation();
});

/**
 * @brief Intel MVM extra information container
 */
class IntelMVMExtraInfo {
public:
    uint16_t CSIHeaderLength;              ///< Length of CSI header
    std::vector<uint8_t> CSIHeader;        ///< CSI header data
    IntelMVMParsedCSIHeader parsedHeader;  ///< Parsed CSI header

    /**
     * @brief Convert extra info to buffer
     * @return Vector containing serialized data
     */
    [[nodiscard]] std::vector<uint8_t> toBuffer() const;
};

/**
 * @brief MVM extra segment class for handling Intel MVM specific CSI data
 * @details Implements parsing and handling of Intel MVM CSI extra information
 */
class MVMExtraSegment : public AbstractPicoScenesFrameSegment {
public:
    /**
     * @brief Default constructor
     */
    MVMExtraSegment();

    /**
     * @brief Construct from buffer
     * @param buffer Input buffer
     * @param bufferLength Length of buffer
     * @throw std::runtime_error if segment name or version is not supported
     */
    MVMExtraSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Manipulate header using registered manipulator
     * @param header Header to manipulate
     */
    static void manipulateHeader(IntelMVMParsedCSIHeader &header);

    /**
     * @brief Set header manipulator function
     * @param headerManipulator Function to manipulate headers
     * @note Can only be set once during system startup
     */
    static void setHeaderManipulator(const std::function<void(IntelMVMParsedCSIHeader &)> &headerManipulator);

    /**
     * @brief Get string representation
     * @return String describing the segment
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Get MVM extra information
     * @return Const reference to MVM extra info
     */
    [[nodiscard]] const IntelMVMExtraInfo &getMvmExtra() const;

    /**
     * @brief Set MVM extra information
     * @param mvmExtraV New MVM extra info to set
     */
    void setMvmExtra(const IntelMVMExtraInfo &mvmExtraV);

private:
    /// Map of version-specific parsing solutions
    static std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> versionedSolutionMap;

    /**
     * @brief Initialize solution map
     * @return Map of version-specific parsing functions
     */
    static std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    /// Header manipulator function
    static std::function<void(IntelMVMParsedCSIHeader &)> headerManipulator;

    /// MVM extra information storage
    IntelMVMExtraInfo mvmExtra;
};
#endif //PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
