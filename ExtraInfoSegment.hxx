//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
#define PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "RXSExtraInfo.hxx"

class ExtraInfoSegment : public AbstractPicoScenesFrameSegment {
public:
    ExtraInfoSegment();

    explicit ExtraInfoSegment(const ExtraInfo &extraInfoV);

    explicit ExtraInfoSegment(ExtraInfo &&extraInfoV);

    ExtraInfoSegment(const uint8_t *buffer, uint32_t bufferLength);

    [[nodiscard]] const ExtraInfo &getExtraInfo() const;

    void setExtraInfo(const ExtraInfo &extraInfoV);

    [[nodiscard]] std::string toString() const override;

private:
    static std::map<uint16_t, std::function<ExtraInfo(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<ExtraInfo(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    ExtraInfo extraInfo;
};

#endif //PICOSCENES_PLATFORM_EXTRAINFOSEGMENT_HXX
