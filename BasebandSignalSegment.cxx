//
// Created by Zhiping Jiang on 2020/11/6.
//

#include "BasebandSignalSegment.hxx"

static auto v1Parser = [](const uint8_t *buffer, const uint32_t bufferLength, void *bbsignal) {
    *static_cast<SignalMatrix<std::complex<float>> *>(bbsignal) = SignalMatrix<>::fromBuffer(buffer, buffer + bufferLength, SignalMatrixStorageMajority::ColumnMajor).convertTo<float>();
};

static auto v2Parser = [](const uint8_t *buffer, const uint32_t bufferLength, void *bbsignal) {
    *static_cast<SignalMatrix<std::complex<float>> *>(bbsignal) = SignalMatrix<std::complex<float>>::fromBuffer(buffer, buffer + bufferLength, SignalMatrixStorageMajority::ColumnMajor);
};

BasebandSignalSegment::BasebandSignalSegment() : AbstractPicoScenesFrameSegment("BasebandSignal", 0x2U) {}

BasebandSignalSegment::BasebandSignalSegment(SignalMatrix<std::complex<float>>&& signalsV): AbstractPicoScenesFrameSegment("BasebandSignal", 0x2U), signals(std::move(signalsV)) {
    setSegmentPayload(std::move(signals.toBuffer()));
}

BasebandSignalSegment::BasebandSignalSegment(const SignalMatrix<std::complex<float>>& signalsV): AbstractPicoScenesFrameSegment("BasebandSignal", 0x2U), signals(signalsV) {
    setSegmentPayload(std::move(signals.toBuffer()));
}

BasebandSignalSegment::BasebandSignalSegment(const uint8_t *buffer, const uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "BasebandSignal")
        throw std::runtime_error("BasebandSignalSegment cannot parse the segment named " + segmentName + ".");
    if (segmentVersionId != 1 && segmentVersionId != 2)
        throw std::runtime_error("BasebandSignalSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    if (segmentVersionId == 1)
        v1Parser(segmentPayload.data(), segmentPayload.size(), (void *) &signals);
    else
        v2Parser(segmentPayload.data(), segmentPayload.size(), (void *) &signals);
}

[[maybe_unused]] const SignalMatrix<std::complex<float>> &BasebandSignalSegment::getSignals() const {
    return signals;
}

void BasebandSignalSegment::setSignals(const SignalMatrix<std::complex<float>> &bbsignalsV) {
    signals = bbsignalsV;
    setSegmentPayload(std::move(signals.toBuffer()));
}

void BasebandSignalSegment::setSignals(SignalMatrix<std::complex<float>> &&bbsignalsV) {
    signals = std::move(bbsignalsV);
    setSegmentPayload(std::move(signals.toBuffer()));
}

std::string BasebandSignalSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[(float) " + std::to_string(signals.dimensions[0]) + "x" + std::to_string(signals.dimensions[1]) + "]";
    return ss.str();
}