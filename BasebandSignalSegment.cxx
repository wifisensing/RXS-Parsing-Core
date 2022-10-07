//
// Created by 蒋志平 on 2020/11/6.
//

#include "BasebandSignalSegment.hxx"


static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> SignalMatrix<std::complex<double>> {
    auto signal = SignalMatrix<std::complex<double>>::fromBuffer(buffer, buffer + bufferLength, SignalMatrixStorageMajority::ColumnMajor);
    return signal;
};

std::map<uint16_t, std::function<SignalMatrix<std::complex<double>>(const uint8_t *, uint32_t)>> BasebandSignalSegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<SignalMatrix<std::complex<double>>(const uint8_t *, uint32_t)>>{{0x1U, v1Parser}};
}

std::map<uint16_t, std::function<SignalMatrix<std::complex<double>>(const uint8_t *, uint32_t)>> BasebandSignalSegment::versionedSolutionMap = initializeSolutionMap();


BasebandSignalSegment::BasebandSignalSegment() : AbstractPicoScenesFrameSegment("BasebandSignal", 0x1U) {}

BasebandSignalSegment::BasebandSignalSegment(const uint8_t *buffer, uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "BasebandSignal")
        throw std::runtime_error("BasebandSignalSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.count(segmentVersionId))
        throw std::runtime_error("BasebandSignalSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    bbsignals = versionedSolutionMap.at(segmentVersionId)(segmentPayload.data(), segmentPayload.size());
}

[[maybe_unused]] const SignalMatrix<std::complex<double>> &BasebandSignalSegment::getSignalMatrix() const {
    return bbsignals;
}

void BasebandSignalSegment::setSignalMatrix(const SignalMatrix<std::complex<double>> &bbsignalsV) {
    bbsignals = bbsignalsV;
    setSegmentPayload(bbsignals.toBuffer());
}

std::string BasebandSignalSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[" + std::to_string(bbsignals.dimensions[0]) + "x" + std::to_string(bbsignals.dimensions[1]) + "]";
    auto temp = ss.str();
    return temp;
}

std::ostream &operator<<(std::ostream &os, const BasebandSignalSegment &basebandSignalSegment) {
    os << basebandSignalSegment.toString();
    return os;
}
