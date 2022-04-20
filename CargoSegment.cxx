//
// Created by csi on 4/20/22.
//

#include "CargoSegment.hxx"

class PayloadCargoV1 {
public:
    uint16_t taskId{0};
    uint8_t sequence{0};
    std::vector<uint8_t> payloadData;
};

size_t PayloadCargo::totalLength() const {
    return sizeof(taskId) + sizeof(sequence) + payloadData.size();
}

std::vector<uint8_t> PayloadCargo::toBuffer() const {
    auto buffer = std::vector<uint8_t>();
    uint32_t cargoLength = totalLength();
    std::copy((uint8_t *) &cargoLength, (uint8_t *) &cargoLength + sizeof(cargoLength), std::back_inserter(buffer));
    std::copy((uint8_t *) &taskId, (uint8_t *) &taskId + sizeof(taskId), std::back_inserter(buffer));
    std::copy((uint8_t *) &sequence, (uint8_t *) &sequence + sizeof(sequence), std::back_inserter(buffer));
    std::copy(payloadData.cbegin(), payloadData.cend(), std::back_inserter(buffer));

    return buffer;
}

PayloadCargo PayloadCargo::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    uint32_t cargoLength = *(uint32_t *) buffer;

    if (cargoLength != bufferLength - 4)[[unlikely]] {
        throw std::runtime_error("cargo segment length inconsistent");
    }

    auto pos = 4;
    PayloadCargo cargo;
    cargo.sequence = *(decltype(sequence) *) (buffer + pos);
    pos += sizeof(sequence);
    std::copy(buffer + pos, buffer + bufferLength, std::back_inserter(cargo.payloadData));

    return cargo;
}

PayloadCargo PayloadCargo::fromBuffer(const std::vector<uint8_t> &buffer) {
    return fromBuffer(buffer.data(), buffer.size());
}

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> PayloadCargo {
    auto cargo = PayloadCargo::fromBuffer(buffer, bufferLength);
    return cargo;
};

std::map<uint16_t, std::function<PayloadCargo(const uint8_t *, uint32_t)>> CargoSegment::versionedSolutionMap = initializeSolutionMap();

CargoSegment::CargoSegment() : AbstractPicoScenesFrameSegment("Cargo", 0x1U) {}

std::map<uint16_t, std::function<PayloadCargo(const uint8_t *, uint32_t)>> CargoSegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<PayloadCargo(const uint8_t *, uint32_t)>>{{0x1U, v1Parser}};
}

const PayloadCargo &CargoSegment::getCargo() const {
    return cargo;
}

void CargoSegment::setCargo(const PayloadCargo &cargoV) {
    cargo = cargoV;
    addField("core", cargo.toBuffer());
}

std::vector<uint8_t> CargoSegment::toBuffer() const {
    return AbstractPicoScenesFrameSegment::toBuffer(true);
}

CargoSegment CargoSegment::createByBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    CargoSegment segment;
    segment.fromBuffer(buffer, bufferLength);
    return segment;
}

void CargoSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto[segmentName, segmentLength, versionId, offset] = extractSegmentMetaData(buffer, bufferLength);
    if (segmentName != "Cargo")
        throw std::runtime_error("CargoSegment cannot parse the segment named " + segmentName + ".");
    if (segmentLength + 4 > bufferLength)
        throw std::underflow_error("CargoSegment cannot parse the segment with less than " + std::to_string(segmentLength + 4) + "B.");
    if (!versionedSolutionMap.count(versionId)) {
        throw std::runtime_error("CargoSegment cannot parse the segment with version v" + std::to_string(versionId) + ".");
    }

    cargo = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    std::copy(buffer, buffer + bufferLength, std::back_inserter(rawBuffer));
    this->segmentLength = segmentLength;
    successfullyDecoded = true;
}

std::string CargoSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[taskId=" + std::to_string(cargo.taskId) + ", seq=" + std::to_string(cargo.sequence) + ", payload_len=" + std::to_string(cargo.payloadData.size()) + "]";
    auto temp = ss.str();
    return temp;
}

std::ostream &operator<<(std::ostream &os, const CargoSegment &cargoSegment) {
    os << cargoSegment.toString();
    return os;
}
