//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
#define PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class BasebandSignalSegment : public AbstractPicoScenesFrameSegment {
public:
    BasebandSignalSegment();

    BasebandSignalSegment(const uint8_t *buffer, uint32_t bufferLength);

    BasebandSignalSegment(const SignalMatrix<std::complex<float>> & signalsV);

    BasebandSignalSegment(SignalMatrix<std::complex<float>> && signalsV);

    [[nodiscard]] const SignalMatrix<std::complex<float>> &getSignals() const;

    void setSignals(const SignalMatrix<std::complex<float>> &bbsignalsV);

    void setSignals(SignalMatrix<std::complex<float>> &&bbsignalsV);

    [[nodiscard]] std::string toString() const override;

private:
    SignalMatrix<std::complex<float>> signals;
};

#endif //PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
