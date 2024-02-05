//
// Created by  Zhiping Jiang on 10/23/22.
//

#ifndef PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX

#include <unordered_map>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

struct SDRExtra {
    int8_t scramblerInit{0};
    int64_t packetStartInternal{0};
    int64_t hardwareRxSamplingIndex{0};
    double preciseRxTime{0};
    double lastTxTime{0};
    double signalInputSystemTime{0};
    double signalDecodeSystemTime{0};
    double sigEVM{0}; /// The EVM estimated by the L-SIG field
    double initialCFO{0}; ///< The CFO estiamted by L-STF and L-LTF. This CFO is used to rotate the following L-LTF and Non-L-LTFs, so affacting the CFO measured from CSI.
    double residualCFO{0}; ///< The CFO estimtaed by the Pilot subcarriers per-OFDM symbol. This estimation is much much more accurate than initialCFO IFF the PPDU is long, e.g., > 1ms.  The reported ExtraInfo::cfo = initialCFO + residualCFO.

    [[nodiscard]] inline double decodingDelay() const {
        return signalDecodeSystemTime - signalInputSystemTime;
    }

    [[nodiscard]] std::string toString() const;

    std::vector<uint8_t> toBuffer();

} __attribute__ ((__packed__));

std::ostream &operator<<(std::ostream &os, const SDRExtra &sdrExtra);


class SDRExtraSegment : public AbstractPicoScenesFrameSegment {
public:
    SDRExtraSegment();

    SDRExtraSegment(const uint8_t *buffer, uint32_t bufferLength);

    explicit SDRExtraSegment(const SDRExtra &sdrExtra);

    [[nodiscard]] const SDRExtra &getSdrExtra() const;

    void setSdrExtra(const SDRExtra &sdrExtra);

    [[nodiscard]] std::string toString() const override;

private:
    static std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<SDRExtra(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    SDRExtra sdrExtra{};
};

#endif //PICOSCENES_PLATFORM_SDREXTRASEGMENT_HXX
