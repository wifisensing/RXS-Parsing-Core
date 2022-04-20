//
// Created by csi on 4/20/22.
//

#include "CargoSegment.hxx"

size_t PayloadCargo::totalLength() const {
    return sizeof(sequence) + payloadData.size();
}

std::vector<uint8_t> PayloadCargo::toBuffer() const {
    auto buffer = std::vector<uint8_t>();
    uint32_t cargoLength = totalLength();
    std::copy((uint8_t *) &cargoLength, (uint8_t *) &cargoLength + sizeof(cargoLength), std::back_inserter(buffer));
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
