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

    [[nodiscard]] const SignalMatrix<std::complex<float>> &getFloat32SignalMatrix() const;

    void setSignalMatrix(const SignalMatrix<std::complex<float>> &bbsignalsV);

    void setSignalMatrix(SignalMatrix<std::complex<float>> &&bbsignalsV);

    [[nodiscard]] std::vector<uint8_t> toBuffer() const override;

    [[nodiscard]] std::string toString() const override;

private:
    SignalMatrix<std::complex<float>> bbsignalsFloat32;
};

#endif //PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
