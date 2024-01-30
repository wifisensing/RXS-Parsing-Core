//
// Created by Zhiping Jiang on 4/20/22.
//

#include "CargoSegment.hxx"

std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> CargoCompression::compressor;
std::function<std::optional<std::vector<uint8_t>>(const uint8_t *, size_t)> CargoCompression::decompressor;

class PayloadCargoV1 {
public:
    uint16_t taskId{0};
    uint8_t numSegments{0};
    uint8_t sequence{0};
    uint8_t totalParts{0};
    bool compressed{false};
    uint32_t payloadLength{0};
    std::vector<uint8_t> payloadData;
};

size_t PayloadCargo::totalLength() const {
    return sizeof(taskId) + sizeof(numSegments) + sizeof(totalParts) + sizeof(sequence) + +sizeof(compressed) + sizeof(payloadLength) + payloadData.size();
}

std::vector<uint8_t> PayloadCargo::toBuffer() const {
    auto buffer = std::vector<uint8_t>();
    uint32_t cargoLength = totalLength();
    std::copy_n(reinterpret_cast<const uint8_t *>(&cargoLength), sizeof(cargoLength), std::back_inserter(buffer));
    std::copy_n(reinterpret_cast<const uint8_t *>(&taskId), sizeof(taskId), std::back_inserter(buffer));
    std::copy_n(&numSegments, sizeof(numSegments), std::back_inserter(buffer));
    std::copy_n(&sequence, sizeof(sequence), std::back_inserter(buffer));
    std::copy_n(&totalParts, sizeof(totalParts), std::back_inserter(buffer));
    std::copy_n(reinterpret_cast<const uint8_t *>(&compressed), sizeof(compressed), std::back_inserter(buffer));
    std::copy_n(reinterpret_cast<const uint8_t *>(&payloadLength), sizeof(payloadLength), std::back_inserter(buffer));
    std::copy(payloadData.cbegin(), payloadData.cend(), std::back_inserter(buffer));

    return buffer;
}

std::shared_ptr<PayloadCargo> PayloadCargo::fromBuffer(const uint8_t *buffer, const uint32_t bufferLength) {
    if (uint32_t cargoLength = *reinterpret_cast<const uint32_t *>(buffer); cargoLength != bufferLength - 4)[[unlikely]] {
        throw std::runtime_error("cargo segment length inconsistent");
    }

    auto pos = 4;
    auto cargo = std::make_shared<PayloadCargo>();
    cargo->taskId = *reinterpret_cast<const decltype(taskId) *>(buffer + pos);
    pos += sizeof(taskId);
    cargo->numSegments = *(buffer + pos);
    pos += sizeof(numSegments);
    cargo->sequence = *(buffer + pos);
    pos += sizeof(sequence);
    cargo->totalParts = *(buffer + pos);
    pos += sizeof(totalParts);
    cargo->compressed = *reinterpret_cast<const bool *>(buffer + pos);
    pos += sizeof(compressed);
    cargo->payloadLength = *reinterpret_cast<const uint32_t *>(buffer + pos);
    pos += sizeof(payloadLength);
    std::copy(buffer + pos, buffer + bufferLength, std::back_inserter(cargo->payloadData));

    return cargo;
}

std::shared_ptr<PayloadCargo> PayloadCargo::fromBuffer(const std::vector<uint8_t> &buffer) {
    return fromBuffer(buffer.data(), buffer.size());
}

U8Vector PayloadCargo::mergeAndValidateCargo(const std::vector<std::shared_ptr<PayloadCargo>>& cargos) {
    U8Vector rawPayload, decompressedPayload;
    std::for_each(cargos.cbegin(), cargos.cend(), [&](const std::shared_ptr<PayloadCargo> &cargo) {
        std::copy(cargo->payloadData.cbegin(), cargo->payloadData.cend(), std::back_inserter(rawPayload));
    });

    if (cargos.front()->compressed && CargoCompression::isAlgorithmRegistered()) {
        decompressedPayload = CargoCompression::getDecompressor()(rawPayload.data(), rawPayload.size()).value_or(U8Vector());
    } else {
        decompressedPayload = rawPayload;
    }

    auto pos = 0, numSegment = 0;
    while (pos < decompressedPayload.size()) {
        auto [segmentName, segmentLength, versionId, contentOffset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(decompressedPayload.data() + pos, decompressedPayload.size() - pos);
        pos += segmentLength + 4;
        numSegment++;
    }

    if (pos != decompressedPayload.size() && numSegment != cargos.front()->numSegments) [[unlikely]] {
        throw std::runtime_error("incorrect cargo length");
    }

    return decompressedPayload;
}

static auto v1Parser = [](const uint8_t *buffer, const uint32_t bufferLength) -> std::shared_ptr<PayloadCargo> {
    auto cargo = PayloadCargo::fromBuffer(buffer, bufferLength);
    return cargo;
};

std::map<uint16_t, std::function<std::shared_ptr<PayloadCargo>(const uint8_t *, uint32_t)>> CargoSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<std::shared_ptr<PayloadCargo>(const uint8_t *, uint32_t)>> CargoSegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<std::shared_ptr<PayloadCargo>(const uint8_t *, uint32_t)>>{{0x1U, v1Parser}};
}

CargoSegment::CargoSegment() : AbstractPicoScenesFrameSegment("Cargo", 0x1U) {}

CargoSegment::CargoSegment(const std::shared_ptr<PayloadCargo>& cargoV): AbstractPicoScenesFrameSegment("Cargo", 0x1U), cargo(cargoV) {
    setSegmentPayload(std::move(cargo->toBuffer()));
}

CargoSegment::CargoSegment(const uint8_t *buffer, const uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "Cargo")
        throw std::runtime_error("CargoSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.contains(segmentVersionId)) {
        throw std::runtime_error("CargoSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");
    }
    cargo = versionedSolutionMap.at(segmentVersionId)(segmentPayload.data(), segmentPayload.size());
}

std::shared_ptr<PayloadCargo> CargoSegment::getCargo() const {
    return cargo;
}

void CargoSegment::setCargo(const std::shared_ptr<PayloadCargo> &cargoV) {
    cargo = cargoV;
    setSegmentPayload(std::move(cargo->toBuffer()));
}

std::string CargoSegment::toString() const {
    std::stringstream ss;
    ss << segmentName << ":[taskId=" << cargo->taskId << ", numSeg=" << static_cast<uint16_t>(cargo->numSegments)
       << ", seq=" << static_cast<uint16_t>(cargo->sequence) << ", total_part=" << static_cast<uint16_t>(cargo->totalParts)
       << ", compressed=" << cargo->compressed << ", total_length=" << cargo->payloadLength
       << ", part_len=" << cargo->payloadData.size() << "]";
    return ss.str();
}