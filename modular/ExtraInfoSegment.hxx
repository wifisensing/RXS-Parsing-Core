//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
#define PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "../RXSExtraInfo.h"

class ExtraInfoSegment : AbstractPicoScenesFrameSegment {
public:
    ExtraInfoSegment();

    ExtraInfoSegment(const ExtraInfo &extraInfo);

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

private:
    ExtraInfo extraInfo;

    void updateFieldMap() override;
};


#endif //PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
