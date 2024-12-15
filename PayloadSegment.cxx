//
// Created by Zhiping Jiang on 12/13/20.
//

#include "PayloadSegment.hxx"

std::ostream &operator<<(std::ostream &os, const PayloadDataType &payloadDataType) {
    switch (payloadDataType) {
        case PayloadDataType::RawData:
            os << "RawData";
            break;
        case PayloadDataType::SignalMatrix:
            os << "Signal";
            break;
        case PayloadDataType::CSIData:
            os << "CSI";
            break;
        case PayloadDataType::SegmentData:
            os << "Segment";
            break;
        case PayloadDataType::FullMSDUPacket:
            os << "MSDU";
            break;
        case PayloadDataType::FullPicoScenesPacket:
            os << "FullPicoScenes";
            break;
    }
    return os;
}

std::vector<uint8_t> PayloadData::toBuffer() {
    auto buffer = std::vector<uint8_t>();
    uint32_t totalLength = sizeof(dataType) + 1 + payloadDescription.length() + 4 + payloadData.size();
    std::copy_n(reinterpret_cast<uint8_t *>(&totalLength), sizeof(totalLength), std::back_inserter(buffer));
    std::copy_n(reinterpret_cast<uint8_t *>(&dataType), sizeof(dataType), std::back_inserter(buffer));
    uint8_t descriptionLength = payloadDescription.length();
    std::copy_n(&descriptionLength, sizeof(descriptionLength), std::back_inserter(buffer));
    std::copy_n(reinterpret_cast<uint8_t *>(payloadDescription.data()), payloadDescription.length(), std::back_inserter(buffer));
    uint32_t payloadLength = payloadData.size();
    std::copy_n(reinterpret_cast<uint8_t *>(&payloadLength), sizeof(payloadLength), std::back_inserter(buffer));
    std::copy(payloadData.cbegin(), payloadData.cend(), std::back_inserter(buffer));

    return buffer;
}

PayloadData PayloadData::fromBuffer(const uint8_t *buffer, const uint32_t bufferLength) {
    if (const uint32_t payloadTotalLength = *reinterpret_cast<const int32_t *>(buffer); payloadTotalLength != bufferLength - 4)
        throw std::runtime_error("Payload length inconsistent");

    auto pos = 4;
    PayloadData payload;
    payload.dataType = *reinterpret_cast<const PayloadDataType *>(buffer + pos);
    pos += sizeof(PayloadDataType);
    const auto descriptionLength = *(buffer + pos++);
    payload.payloadDescription = std::string(reinterpret_cast<const char *>(buffer + pos), reinterpret_cast<const char *>(buffer + pos + descriptionLength));
    pos += descriptionLength;
    const auto payloadLength = *reinterpret_cast<const uint32_t *>(buffer + pos);
    pos += 4;
    std::copy_n(buffer + pos, payloadLength, std::back_inserter(payload.payloadData));
    return payload;
}

PayloadData PayloadData::fromBuffer(const std::vector<uint8_t> &buffer) {
    return fromBuffer(buffer.data(), buffer.size());
}

static auto v1Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> PayloadData {
    return PayloadData::fromBuffer(buffer, bufferLength);
};

std::map<uint16_t, std::function<PayloadData(const uint8_t *, uint32_t)>> PayloadSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<PayloadData(const uint8_t *, uint32_t)>> PayloadSegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<PayloadData(const uint8_t *, uint32_t)>>{{0x1U, v1Parser}};
}

PayloadSegment::PayloadSegment() : AbstractPicoScenesFrameSegment("Payload", 0x1U) {}

PayloadSegment::PayloadSegment(const std::string &description, const std::vector<uint8_t> &payload, const std::optional<PayloadDataType> payloadType) : PayloadSegment() {
    setPayloadData({
                           .dataType = payloadType.value_or(PayloadDataType::RawData),
                           .payloadDescription = description,
                           .payloadData = payload
                   });
}

PayloadSegment::PayloadSegment(const std::string &description, const uint8_t *payloadBuffer, const size_t bufferLength, const std::optional<PayloadDataType> payloadType) : PayloadSegment() {
    const auto payload = std::vector<uint8_t>(payloadBuffer, payloadBuffer + bufferLength);
    setPayloadData({
                           .dataType = payloadType.value_or(PayloadDataType::RawData),
                           .payloadDescription = description,
                           .payloadData = payload
                   });
}

PayloadSegment::PayloadSegment(const std::string &description, const std::string &stringPayload, const std::optional<PayloadDataType> payloadType) : PayloadSegment() {
    const auto payload = std::vector<uint8_t>(stringPayload.cbegin(), stringPayload.cend());
    setPayloadData({
                           .dataType = payloadType.value_or(PayloadDataType::RawData),
                           .payloadDescription = description,
                           .payloadData = payload
                   });
}

PayloadSegment::PayloadSegment(const uint8_t *buffer, const uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "Payload")
        throw std::runtime_error("PayloadSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.contains(segmentVersionId))
        throw std::runtime_error("PayloadSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    payloadData = versionedSolutionMap.at(segmentVersionId)(segmentPayload.data(), segmentPayload.size());
}

const PayloadData &PayloadSegment::getPayloadData() const {
    return payloadData;
}

void PayloadSegment::setPayloadData(const PayloadData &payload) {
    payloadData = payload;
    setSegmentPayload(payloadData.toBuffer());
}

std::string PayloadSegment::toString() const {
    std::stringstream ss;
    ss << segmentName << ":[Type=" << payloadData.dataType << ", Description=" << payloadData.payloadDescription << ", length=" << payloadData.payloadData.size() << "B]";
    return ss.str();
}