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

class EQDataSymbolsSegment final: public AbstractPicoScenesFrameSegment {
public:
    EQDataSymbolsSegment();

    EQDataSymbolsSegment(const uint8_t *buffer, uint32_t bufferLength);

    EQDataSymbolsSegment(const SignalMatrix<std::complex<float>> & symbolsV, uint8_t MCS);

    EQDataSymbolsSegment(SignalMatrix<std::complex<float>> && symbolsV, uint8_t MCS);

    [[nodiscard]] const SignalMatrix<std::complex<float>> &getSymbols() const;

    void setSymbolsAndMCS(SignalMatrix<std::complex<float>> &&symbolsV, uint8_t MCS);

    [[nodiscard]] const std::vector<std::complex<float>> &getReferenceConstellationMap() const;

    [[nodiscard]] uint8_t getMCS() const;

    [[nodiscard]] std::string toString() const override;

private:
    SignalMatrix<std::complex<float>> symbols;
    
    uint8_t MCS{0};
};

#endif //PICOSCENES_PLATFORM_EQDATASYMBOLSSEGMENT_HXX 