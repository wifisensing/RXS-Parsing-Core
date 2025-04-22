#include "EQDataSymbolsSegment.hxx"
#include <cmath>

// Function to initialize the MCS to constellation map
static std::unordered_map<uint8_t, std::vector<std::complex<float>>> initializeConstellationMap() {
    std::unordered_map<uint8_t, std::vector<std::complex<float>>> map;
    
    // BPSK (MCS 0)
    map[0] = {
        std::complex<float>(-1.0f, 0.0f),
        std::complex<float>(1.0f, 0.0f)
    };
    
    // QPSK (MCS 1)
    map[1] = {
        std::complex<float>(-1.0f/sqrtf(2.0f), -1.0f/sqrt(2.0f)),
        std::complex<float>(-1.0f/sqrt(2.0f), 1.0f/sqrt(2.0f)),
        std::complex<float>(1.0f/sqrt(2.0f), -1.0f/sqrt(2.0f)),
        std::complex<float>(1.0f/sqrt(2.0f), 1.0f/sqrt(2.0f))
    };
    // QPSK (MCS 2)
    map[2] = map[1];
    
    // 16-QAM (MCS 3)
    std::vector<std::complex<float>> qam16;
    const float scale16 = 1.0f / sqrt(10.0f);
    for (int i = -3; i <= 3; i += 2) {
        for (int j = -3; j <= 3; j += 2) {
            qam16.push_back(std::complex<float>(i * scale16, j * scale16));
        }
    }
    map[3] = qam16;
    map[4] = qam16;
    
    // 64-QAM (MCS 5)
    std::vector<std::complex<float>> qam64;
    const float scale64 = 1.0f / sqrt(42.0f);
    for (int i = -7; i <= 7; i += 2) {
        for (int j = -7; j <= 7; j += 2) {
            qam64.push_back(std::complex<float>(i * scale64, j * scale64));
        }
    }
    map[5] = qam64;
    map[6] = qam64;
    map[7] = qam64;
    
    // 256-QAM (MCS 8)
    std::vector<std::complex<float>> qam256;
    const float scale256 = 1.0f / sqrt(170.0f);
    for (int i = -15; i <= 15; i += 2) {
        for (int j = -15; j <= 15; j += 2) {
            qam256.push_back(std::complex<float>(i * scale256, j * scale256));
        }
    }
    map[8] = qam256;
    map[9] = qam256;
    
    // 1024-QAM (MCS 10)
    std::vector<std::complex<float>> qam1024;
    const float scale1024 = 1.0f / sqrt(682.0f);
    for (int i = -31; i <= 31; i += 2) {
        for (int j = -31; j <= 31; j += 2) {
            qam1024.push_back(std::complex<float>(i * scale1024, j * scale1024));
        }
    }
    map[10] = qam1024;
    map[11] = qam1024;
    
    // 4096-QAM (MCS 12)
    std::vector<std::complex<float>> qam4096;
    const float scale4096 = 1.0f / sqrt(2730.0f);
    for (int i = -63; i <= 63; i += 2) {
        for (int j = -63; j <= 63; j += 2) {
            qam4096.push_back(std::complex<float>(i * scale4096, j * scale4096));
        }
    }
    map[12] = qam4096;
    map[13] = qam4096;

    return map;
}

// Static map to store MCS to reference constellation map pairs
static const std::unordered_map<uint8_t, std::vector<std::complex<float>>> MCS_TO_CONSTELLATION_MAP = initializeConstellationMap();

static auto v1Parser = [](const uint8_t *buffer, const uint32_t bufferLength, void *symbols, uint8_t &MCS) {
    MCS = buffer[0];
    *static_cast<SignalMatrix<std::complex<float>> *>(symbols) = SignalMatrix<std::complex<float>>::fromBuffer(buffer + 1, buffer + bufferLength - 1, SignalMatrixStorageMajority::ColumnMajor);
};

EQDataSymbolsSegment::EQDataSymbolsSegment() : AbstractPicoScenesFrameSegment("EQDataSymbolsSegment", 0x1U) {}

EQDataSymbolsSegment::EQDataSymbolsSegment(SignalMatrix<std::complex<float>>&& symbolsV, uint8_t MCS): AbstractPicoScenesFrameSegment("EQDataSymbolsSegment", 0x1U), symbols(std::move(symbolsV)), MCS(MCS) {
    setSegmentPayload(std::move(symbols.toBuffer()));
}

EQDataSymbolsSegment::EQDataSymbolsSegment(const SignalMatrix<std::complex<float>>& symbolsV, uint8_t MCS): AbstractPicoScenesFrameSegment("EQDataSymbolsSegment", 0x1U), symbols(symbolsV), MCS(MCS) {
    setSegmentPayload(std::move(symbols.toBuffer()));
}

EQDataSymbolsSegment::EQDataSymbolsSegment(const uint8_t *buffer, const uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "EQDataSymbolsSegment")
        throw std::runtime_error("EQDataSymbolsSegment cannot parse the segment named " + segmentName + ".");

    if (segmentVersionId != 1)
        throw std::runtime_error("EQDataSymbolsSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    v1Parser(segmentPayload.data(), segmentPayload.size(), (void *) &symbols, MCS);
}

[[maybe_unused]] const SignalMatrix<std::complex<float>> &EQDataSymbolsSegment::getSymbols() const {
    return symbols;
}

void EQDataSymbolsSegment::setSymbols(const SignalMatrix<std::complex<float>> &symbolsV) {
    symbols = symbolsV;
    setSegmentPayload(std::move(symbols.toBuffer()));
}

void EQDataSymbolsSegment::setSymbols(SignalMatrix<std::complex<float>> &&symbolsV) {
    symbols = std::move(symbolsV);
    setSegmentPayload(std::move(symbols.toBuffer()));
}

void EQDataSymbolsSegment::setMCS(uint8_t MCS) {
    this->MCS = MCS;
    setSegmentPayload(std::move(symbols.toBuffer()));
}

uint8_t EQDataSymbolsSegment::getMCS() const {
    return MCS;
}

const std::vector<std::complex<float>> &EQDataSymbolsSegment::getReferenceConstellationMap() const {
    // If the reference constellation map is empty or the MCS has changed, update it
    if (referenceConstellationMap.empty()) {
        auto it = MCS_TO_CONSTELLATION_MAP.find(MCS);
        if (it != MCS_TO_CONSTELLATION_MAP.end()) {
            referenceConstellationMap = it->second;
        }
    }
    return referenceConstellationMap;
}

std::string EQDataSymbolsSegment::toString() const {
    std::stringstream ss;
    ss << segmentName << ":[MCS=" << static_cast<int>(MCS) << ", (float) " 
       << std::to_string(symbols.dimensions[0]) << "x" << std::to_string(symbols.dimensions[1]);
    if (symbols.dimensions.size() > 2) {
        ss << "x" << std::to_string(symbols.dimensions[2]);
    }
    ss << "]";
    return ss.str();
} 