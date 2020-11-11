//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_MPDUCONTENTSSEGMENT_HXX
#define PICOSCENES_PLATFORM_MPDUCONTENTSSEGMENT_HXX


#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class MPDUContentsSegment : public AbstractPicoScenesFrameSegment {
public:
    MPDUContentsSegment();

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    bool isAMPDU;
    std::vector<Uint8Vector> contents;
};


#endif //PICOSCENES_PLATFORM_MPDUCONTENTSSEGMENT_HXX
