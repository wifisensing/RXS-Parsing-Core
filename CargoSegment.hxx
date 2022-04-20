//
// Created by csi on 4/20/22.
//

#ifndef PICOSCENES_PLATFORM_CARGOSEGMENT_HXX
#define PICOSCENES_PLATFORM_CARGOSEGMENT_HXX

#include <functional>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class PayloadCargo {
public:
    uint8_t sequence{0};
    std::vector<uint8_t> payloadData;

    size_t totalLength() const;

    [[nodiscard]] std::vector<uint8_t> toBuffer() const;

    static PayloadCargo fromBuffer(const uint8_t *buffer, uint32_t bufferLength);

    static PayloadCargo fromBuffer(const std::vector<uint8_t> &buffer);
};

class CargoSegment : public AbstractPicoScenesFrameSegment {

};


#endif //PICOSCENES_PLATFORM_CARGOSEGMENT_HXX
