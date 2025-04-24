//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
#define PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "RXSExtraInfo.hxx"

/**
 * @brief Class for managing extra information in PicoScenes frames
 *
 * @details This class extends AbstractPicoScenesFrameSegment to handle additional
 * information that may be attached to PicoScenes frames. It provides functionality
 * for storing, retrieving, and managing extra metadata and parameters that don't
 * fit into other standard segment types.
 */
class ExtraInfoSegment : public AbstractPicoScenesFrameSegment {
public:
    /**
     * @brief Default constructor
     *
     * Initializes a new ExtraInfoSegment with version 0x2
     */
    ExtraInfoSegment();

    /**
     * @brief Constructor from existing ExtraInfo object
     *
     * @param extraInfoV Reference to ExtraInfo object to copy from
     */
    explicit ExtraInfoSegment(const ExtraInfo &extraInfoV);

    /**
     * @brief Move constructor from ExtraInfo object
     *
     * @param extraInfoV ExtraInfo object to move from
     */
    explicit ExtraInfoSegment(ExtraInfo &&extraInfoV);

    /**
     * @brief Constructor from raw buffer
     *
     * @param buffer Pointer to raw data buffer
     * @param bufferLength Length of the buffer in bytes
     * @throw std::runtime_error If segment name is not "ExtraInfo" or version is unsupported
     */
    ExtraInfoSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Get the stored extra information
     *
     * @return Const reference to the ExtraInfo object
     */
    [[nodiscard]] const ExtraInfo &getExtraInfo() const;

    /**
     * @brief Set new extra information
     *
     * @param extraInfoV New ExtraInfo object to store
     */
    void setExtraInfo(const ExtraInfo &extraInfoV);

    /**
     * @brief Get string representation of the segment
     *
     * @return String representation of the contained ExtraInfo
     */
    [[nodiscard]] std::string toString() const override;

private:
    /**
     * @brief Map of version-specific parsing solutions
     *
     * Maps version numbers to corresponding parsing functions that can handle
     * that version's format of extra information
     */
    static std::map<uint16_t, std::function<ExtraInfo(const uint8_t *, uint32_t)>> versionedSolutionMap;

    /**
     * @brief Initialize the version-specific solution map
     *
     * @return Map containing parsing functions for each supported version
     * @note Currently supports versions 0x1 and 0x2
     */
    static std::map<uint16_t, std::function<ExtraInfo(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    ExtraInfo extraInfo;  ///< Storage for the extra information
};

#endif //PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
