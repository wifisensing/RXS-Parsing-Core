//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
#define PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX


#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class BasebandSignalSegment : AbstractPicoScenesFrameSegment {
public:
    BasebandSignalSegment();

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    std::vector<uint8_t> toBuffer() const override;
};


#endif //PICOSCENES_PLATFORM_BASEBANDSIGNALSEGMENT_HXX
