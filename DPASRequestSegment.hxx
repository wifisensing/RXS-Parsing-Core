//
// Created by Zhiping Jiang on 5/22/21.
//

#ifndef PICOSCENES_PLATFORM_DPASREQUESTSEGMENT_HXX
#define PICOSCENES_PLATFORM_DPASREQUESTSEGMENT_HXX

#include <functional>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

struct DPASRequest {
    uint8_t  requestMode;
    uint16_t batchId;
    uint8_t  stage;
    uint32_t batchLength;
    uint32_t sequenceId;
    uint32_t intervalTime;
    uint32_t intervalStep;
    PicoScenesDeviceType deviceType;
    uint16_t deviceSubtype;
    uint64_t carrierFrequency;
    uint32_t samplingFrequency;

    std::vector<uint8_t> toBuffer();
} __attribute__ ((__packed__));


class DPASRequestSegment : public AbstractPicoScenesFrameSegment {
public:
    DPASRequestSegment();

    explicit DPASRequestSegment(const DPASRequest &syncRequest);

    static DPASRequestSegment createByBuffer(const uint8_t *buffer, uint32_t bufferLength);

    DPASRequest getRequest() const;

    void setRequest(const DPASRequest &requestV);

    [[nodiscard]] std::vector<uint8_t> toBuffer() const override;

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    [[nodiscard]] std::string toString() const override;

private:
    static std::map<uint16_t, std::function<DPASRequest(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<DPASRequest(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    DPASRequest dpasRequest;
};

std::ostream &operator<<(std::ostream &os, const DPASRequestSegment &dpasRequestSegment);


#endif //PICOSCENES_PLATFORM_DPASREQUESTSEGMENT_HXX
