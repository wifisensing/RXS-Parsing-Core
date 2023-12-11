//
// Created by Zhiping Jiang on 2020/11/6.
//

#include "BasebandSignalSegment.hxx"

static auto v2Parser = [](const uint8_t *buffer, uint32_t bufferLength, void *bbsignal) {
    *(SignalMatrix<std::complex<float>> *) bbsignal = SignalMatrix<std::complex<float>>::fromBuffer(buffer, buffer + bufferLength, SignalMatrixStorageMajority::ColumnMajor);
};

BasebandSignalSegment::BasebandSignalSegment() : AbstractPicoScenesFrameSegment("BasebandSignal", 0x2U) {}

BasebandSignalSegment::BasebandSignalSegment(const uint8_t *buffer, uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "BasebandSignal")
        throw std::runtime_error("BasebandSignalSegment cannot parse the segment named " + segmentName + ".");
    if (segmentVersionId != 1 && segmentVersionId != 2)
        throw std::runtime_error("BasebandSignalSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    v2Parser(segmentPayload.data(), segmentPayload.size(), (void *) &bbsignalsFloat32);
}

[[maybe_unused]] const SignalMatrix<std::complex<float>> &BasebandSignalSegment::getFloat32SignalMatrix() const {
    return bbsignalsFloat32;
}

void BasebandSignalSegment::setSignalMatrix(const SignalMatrix<std::complex<float>> &bbsignalsV) {
    bbsignalsFloat32 = bbsignalsV;
}

void BasebandSignalSegment::setSignalMatrix(SignalMatrix<std::complex<float>> &&bbsignalsV) {
    bbsignalsFloat32 = std::move(bbsignalsV);
    setSegmentPayload(std::move(bbsignalsFloat32.toBuffer()));
}

//std::vector<uint8_t> BasebandSignalSegment::toBuffer() const {
//    auto signalBuffer = bbsignalsFloat32.toBuffer();
//
//    uint32_t totalLengthV = totalLengthIncludingLeading4ByteLength() + signalBuffer.size();
//    std::vector<uint8_t> result;
//    result.reserve(totalLengthV);
//
//    uint8_t segNameLength = segmentName.length() + 1;
//    std::copy((uint8_t *) &totalLengthV, (uint8_t *) &totalLengthV + sizeof(totalLengthV), std::back_inserter(result));
//    std::copy((uint8_t *) &segNameLength, (uint8_t *) &segNameLength + sizeof(segNameLength), std::back_inserter(result));
//    std::copy((uint8_t *) segmentName.data(), (uint8_t *) (segmentName.data() + segmentName.size() + 1), std::back_inserter(result));
//    std::copy((uint8_t *) &segmentVersionId, (uint8_t *) &segmentVersionId + sizeof(segmentVersionId), std::back_inserter(result));
//    std::copy(signalBuffer.cbegin(), signalBuffer.cend(), std::back_inserter(result));
//
//    return result;
//}

std::string BasebandSignalSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[(float) " + std::to_string(bbsignalsFloat32.dimensions[0]) + "x" + std::to_string(bbsignalsFloat32.dimensions[1]) + "]";
    return ss.str();
}