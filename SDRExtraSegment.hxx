//
// Created by csi on 10/23/22.
//

#ifndef PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX

#include <typeindex>
#include <unordered_map>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

struct SDRExtra {
    double CFO;
    int8_t scramblerInit;
    uint64_t packetStartInternal;
    double lastTxTime;

    [[nodiscard]] std::string toString() const;

    std::vector<uint8_t> toBuffer();

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const SDRExtra &sdrExtra);


class SDRExtraSegment : public AbstractPicoScenesFrameSegment {
public:
    SDRExtraSegment();

    SDRExtraSegment(const uint8_t *buffer, uint32_t bufferLength);

    SDRExtraSegment(const SDRExtra &sdrExtra);

    const SDRExtra &getSdrExtra() const;

    void setSdrExtra(const SDRExtra &sdrExtra);

    std::string toString() const override;

private:
    static std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    SDRExtra sdrExtra{};

};


#endif //PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX
