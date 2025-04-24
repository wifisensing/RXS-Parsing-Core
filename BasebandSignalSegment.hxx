//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
#define PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

/**
 * @brief Class for handling baseband signal segments in PicoScenes frames
 *
 * This class extends AbstractPicoScenesFrameSegment to provide specialized handling
 * of baseband signals stored as complex float matrices. It supports both version 1
 * and version 2 formats of baseband signal data, with automatic conversion and
 * storage management.
 */
class BasebandSignalSegment : public AbstractPicoScenesFrameSegment {
public:
    /**
     * @brief Default constructor
     *
     * Creates a new BasebandSignalSegment with name "BasebandSignal" and version 0x2
     */
    BasebandSignalSegment();

    /**
     * @brief Constructor for parsing existing segment data
     *
     * @param buffer Pointer to raw segment data
     * @param bufferLength Length of the buffer in bytes
     * @throw std::runtime_error If segment name is not "BasebandSignal" or version is not 1 or 2
     */
    BasebandSignalSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Constructor from existing signal matrix
     *
     * @param signalsV Reference to complex float signal matrix
     */
    BasebandSignalSegment(const SignalMatrix<std::complex<float>> & signalsV);

    /**
     * @brief Move constructor from signal matrix
     *
     * @param signalsV Rvalue reference to complex float signal matrix
     */
    BasebandSignalSegment(SignalMatrix<std::complex<float>> && signalsV);

    /**
     * @brief Get the stored baseband signals
     *
     * @return Const reference to the complex float signal matrix
     */
    [[nodiscard]] const SignalMatrix<std::complex<float>> &getSignals() const;

    /**
     * @brief Set new baseband signals
     *
     * Updates the internal signal matrix and rebuilds the segment payload
     *
     * @param bbsignalsV Reference to new complex float signal matrix
     */
    void setSignals(const SignalMatrix<std::complex<float>> &bbsignalsV);

    /**
     * @brief Set new baseband signals using move semantics
     *
     * Updates the internal signal matrix and rebuilds the segment payload
     *
     * @param bbsignalsV Rvalue reference to new complex float signal matrix
     */
    void setSignals(SignalMatrix<std::complex<float>> &&bbsignalsV);

    /**
     * @brief Get string representation of the segment
     *
     * @return String containing segment name and signal matrix dimensions
     */
    [[nodiscard]] std::string toString() const override;

private:
    /** @brief Matrix storing the complex float baseband signals */
    SignalMatrix<std::complex<float>> signals;
};

#endif //PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
