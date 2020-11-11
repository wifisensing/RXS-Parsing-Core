//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
#define PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX

#include <functional>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "RXSExtraInfo.hxx"

class ExtraInfoSegment : public AbstractPicoScenesFrameSegment {
public:
    ExtraInfoSegment();

    explicit ExtraInfoSegment(const ExtraInfo & extraInfo);

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    ExtraInfo extraInfo;
private:
    static std::map<uint16_t, std::function<ExtraInfo(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<ExtraInfo(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    void updateFieldMap() override;
};


#endif //PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
