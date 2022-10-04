//
// Created by csi on 10/4/22.
//

#ifndef PICOSCENES_PLATFORM_DYNAMICSEGMENT_HXX
#define PICOSCENES_PLATFORM_DYNAMICSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"

class DynamicSegment : public AbstractPicoScenesFrameSegment {
public:
    DynamicSegment(const uint8_t *buffer, uint32_t bufferLength);

private:

};


#endif //PICOSCENES_PLATFORM_DYNAMICSEGMENT_HXX
