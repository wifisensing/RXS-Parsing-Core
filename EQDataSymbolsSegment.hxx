#ifndef PICOSCENES_PLATFORM_EQDATASYMBOLSSEGMENT_HXX
#define PICOSCENES_PLATFORM_EQDATASYMBOLSSEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

/**
 * @brief This class stores the equalized data symbols, MCS, and reference constellation map. Mainly used for Constellation plot & analysis.
 * 
 * The EQDataSymbolsSegment segment contains the equalized constellation points of data subcarriers
 * after channel equalization. These symbols can be used for constellation visualization
 * and signal quality analysis. The Modulation and Coding Scheme (MCS) information is also
 * stored to provide context about the modulation format used. Additionally, it maintains
 * a reference constellation map that can be used for visual comparison with the actual
 * received symbols to assess signal quality and distortion.
 */


class EQDataSymbols {
public:
    [[nodiscard]] const SignalMatrix<std::complex<float>> &getSymbols() const;

    [[nodiscard]] uint8_t getMCS() const;

    [[nodiscard]] const std::vector<std::complex<float>> &getReferenceConstellationMap() const;

    std::vector<uint8_t> toBuffer() const;

    SignalMatrix<std::complex<float>> symbols;

    uint8_t MCS{0};
};

class EQDataSymbolsSegment final: public AbstractPicoScenesFrameSegment {
public:
    EQDataSymbolsSegment();

    EQDataSymbolsSegment(const uint8_t *buffer, uint32_t bufferLength);

    EQDataSymbolsSegment(const EQDataSymbols &symbols);

    EQDataSymbolsSegment(EQDataSymbols &&symbols);

    const EQDataSymbols &getSymbols() const;

    EQDataSymbols &getSymbols();

    [[nodiscard]] std::string toString() const override;

private:
    EQDataSymbols symbols;
};

#endif //PICOSCENES_PLATFORM_EQDATASYMBOLSSEGMENT_HXX 