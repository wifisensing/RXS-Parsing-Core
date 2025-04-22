#include "EQDataSymbolsSegment.hxx"
#include <cmath>

// Function to initialize the MCS to constellation map
static std::unordered_map<uint8_t, std::vector<std::complex<float>>> initializeConstellationMap() {
    std::unordered_map<uint8_t, std::vector<std::complex<float>>> map;

    // BPSK (MCS 0)
    map[0] = {
        std::complex<float>(-1.0f, 0.0f),
        std::complex<float>(1.0f, 0.0f)};

    // QPSK (MCS 1/2)
    map[1] = {
        std::complex<float>(-1.0f / std::sqrt(2.0f), -1.0f / std::sqrt(2.0f)),
        std::complex<float>(-1.0f / std::sqrt(2.0f), 1.0f / std::sqrt(2.0f)),
        std::complex<float>(1.0f / std::sqrt(2.0f), -1.0f / std::sqrt(2.0f)),
        std::complex<float>(1.0f / std::sqrt(2.0f), 1.0f / std::sqrt(2.0f))};
    map[2] = map[1];

    // 16-QAM (MCS 3/4)
    std::vector<std::complex<float>> qam16;
    const float scale16 = 1.0f / std::sqrt(10.0f);
    for (int i = -3; i <= 3; i += 2) {
        for (int j = -3; j <= 3; j += 2) {
            qam16.emplace_back(i * scale16, j * scale16);
        }
    }
    map[3] = qam16;
    map[4] = qam16;

    // 64-QAM (MCS 5/6/7)
    std::vector<std::complex<float>> qam64;
    const float scale64 = 1.0f / std::sqrt(42.0f);
    for (int i = -7; i <= 7; i += 2) {
        for (int j = -7; j <= 7; j += 2) {
            qam64.emplace_back(i * scale64, j * scale64);
        }
    }
    map[5] = qam64;
    map[6] = qam64;
    map[7] = qam64;

    // 256-QAM (MCS 8/9)
    std::vector<std::complex<float>> qam256;
    const float scale256 = 1.0f / std::sqrt(170.0f);
    for (int i = -15; i <= 15; i += 2) {
        for (int j = -15; j <= 15; j += 2) {
            qam256.emplace_back(i * scale256, j * scale256);
        }
    }
    map[8] = qam256;
    map[9] = qam256;

    // 1024-QAM (MCS 10/11)
    std::vector<std::complex<float>> qam1024;
    const float scale1024 = 1.0f / std::sqrt(682.0f);
    for (int i = -31; i <= 31; i += 2) {
        for (int j = -31; j <= 31; j += 2) {
            qam1024.emplace_back(i * scale1024, j * scale1024);
        }
    }
    map[10] = qam1024;
    map[11] = qam1024;

    // 4096-QAM (MCS 12/13)
    std::vector<std::complex<float>> qam4096;
    const float scale4096 = 1.0f / std::sqrt(2730.0f);
    for (int i = -63; i <= 63; i += 2) {
        for (int j = -63; j <= 63; j += 2) {
            qam4096.emplace_back(i * scale4096, j * scale4096);
        }
    }
    map[12] = qam4096;
    map[13] = qam4096;

    return map;
}

// Static map to store MCS to reference constellation map pairs
static const std::unordered_map<uint8_t, std::vector<std::complex<float>>> MCS_TO_CONSTELLATION_MAP = initializeConstellationMap();

std::vector<uint8_t> EQDataSymbols::toBuffer() const {
    std::vector<uint8_t> buffer;
    buffer.push_back(MCS);
    auto symbolBuffer = symbols.toBuffer();
    std::copy(symbolBuffer.cbegin(), symbolBuffer.cend(), std::back_inserter(buffer));
    return buffer;
}

const SignalMatrix<std::complex<float>> &EQDataSymbols::getSymbols() const {
    return symbols;
}

uint8_t EQDataSymbols::getMCS() const {
    return MCS;
}

const std::vector<std::complex<float>> &EQDataSymbols::getReferenceConstellationMap() const {
    return MCS_TO_CONSTELLATION_MAP.at(MCS);
}

static EQDataSymbols v1Parser(const uint8_t *buffer, const uint32_t bufferLength) {
    const uint8_t MCS = buffer[0];
    auto symbols = SignalMatrix<std::complex<float>>::fromBuffer(buffer + 1, buffer + bufferLength - 1, SignalMatrixStorageMajority::ColumnMajor);
    return EQDataSymbols{
        .symbols = std::move(symbols),
        .MCS = MCS};
};

EQDataSymbolsSegment::EQDataSymbolsSegment() :
    AbstractPicoScenesFrameSegment("EQDataSymbolsSegment", 0x1U) {}

EQDataSymbolsSegment::EQDataSymbolsSegment(const uint8_t *buffer, const uint32_t bufferLength) :
    AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "EQDataSymbolsSegment")
        throw std::runtime_error("EQDataSymbolsSegment cannot parse the segment named " + segmentName + ".");

    if (segmentVersionId != 1)
        throw std::runtime_error("EQDataSymbolsSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    symbols = v1Parser(segmentPayload.data(), segmentPayload.size());
}

EQDataSymbolsSegment::EQDataSymbolsSegment(const EQDataSymbols &symbols) :
    AbstractPicoScenesFrameSegment("EQDataSymbolsSegment", 0x1U), symbols(symbols) {
    setSegmentPayload(std::move(symbols.toBuffer()));
}

EQDataSymbolsSegment::EQDataSymbolsSegment(EQDataSymbols &&symbols) :
    AbstractPicoScenesFrameSegment("EQDataSymbolsSegment", 0x1U), symbols(std::move(symbols)) {
    setSegmentPayload(std::move(symbols.toBuffer()));
}

const EQDataSymbols &EQDataSymbolsSegment::getSymbols() const {
    return symbols;
}

EQDataSymbols &EQDataSymbolsSegment::getSymbols() {
    return symbols;
}

std::string EQDataSymbolsSegment::toString() const {
    std::stringstream ss;
    ss << "EqDataSyms:[MCS=" << static_cast<int>(symbols.getMCS()) << ", (float) " << std::to_string(symbols.getSymbols().dimensions[0]) << "x" << std::to_string(symbols.getSymbols().dimensions[1]);
    if (symbols.getSymbols().dimensions.size() > 2) {
        ss << "x" << std::to_string(symbols.getSymbols().dimensions[2]);
    }
    ss << "]";
    return ss.str();
}
