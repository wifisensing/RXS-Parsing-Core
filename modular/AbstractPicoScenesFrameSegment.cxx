//
// Created by 蒋志平 on 2020/11/5.
//

#include <utility>

#include "AbstractPicoScenesFrameSegment.hxx"

AbstractPicoScenesFrameSegment::AbstractPicoScenesFrameSegment(std::string segmentName, uint16_t segmentVersionId) : segmentName(std::move(segmentName)), segmentVersionId(segmentVersionId) {
}

void AbstractPicoScenesFrameSegment::addField(const std::string &fieldName, const std::vector<uint8_t> &data) {
    fieldMap[fieldName] = data;
}

void AbstractPicoScenesFrameSegment::addField(const std::string &fieldName, const uint8_t *buffer, uint32_t bufferLength) {
    fieldMap[fieldName] = std::vector<uint8_t>(buffer, buffer + bufferLength);
}

void AbstractPicoScenesFrameSegment::addField(const std::string &fieldName, const std::pair<const uint8_t *, uint32_t> &buffer) {
    fieldMap[fieldName] = std::vector<uint8_t>(buffer.first, buffer.first + buffer.second);
}

void AbstractPicoScenesFrameSegment::addField(const std::string &fieldName, const std::pair<std::shared_ptr<uint8_t>, uint32_t> &buffer) {
    fieldMap[fieldName] = std::vector<uint8_t>(buffer.first.get(), buffer.first.get() + buffer.second);
}

void AbstractPicoScenesFrameSegment::removeField(const std::string &fieldName) {
    fieldMap.erase(fieldMap.find(fieldName));
}

uint32_t AbstractPicoScenesFrameSegment::totalLength(bool includeItself) {
    uint32_t length = includeItself ? 4 : 0;
    length += segmentName.size();
    length += 1;
    length += sizeof(segmentVersionId);
    for (const auto &field: fieldMap) {
        length += field.second.size();
    }
    segmentLength = length;

    return length;
}

std::vector<uint8_t> AbstractPicoScenesFrameSegment::toBuffer(bool totalLengthIncluded) {
    auto result = std::vector<uint8_t>(totalLength(true));
    toBuffer(totalLengthIncluded, &result[0], result.size());
    return result;
}

uint32_t AbstractPicoScenesFrameSegment::toBuffer(bool totalLengthIncluded, uint8_t *buffer, std::optional<uint32_t> capacity) {
    updateFieldMap();
    uint32_t finalTotalLength = totalLength(false) + (totalLengthIncluded ? 4 : 0);
    if (capacity && *capacity < finalTotalLength)
        throw std::runtime_error("buffer capacity not enough for PicoScenes frame segment [" + segmentName + " (" + std::to_string(finalTotalLength) + "B)]");

    uint32_t pos = 0;
    // copy totalLength
    if (totalLengthIncluded) {
        *((uint32_t *) (buffer + pos)) = totalLength(false);
        pos += 4;
    }

    // length of the segment name
    *(buffer + pos++) = segmentName.size() + 1;
    // copy the segment name
    std::memcpy(buffer + pos, segmentName.data(), segmentName.size() + 1);
    pos += segmentName.size() + 1;
    // version Id
    std::memcpy(buffer + pos, &segmentVersionId, sizeof(segmentVersionId));
    pos += sizeof(segmentVersionId);
    // copy fields
    for (const auto &field: fieldMap) {
        std::copy(field.second.cbegin(), field.second.cend(), buffer + pos);
        pos += field.second.size();
    }

    return pos;
}

std::string AbstractPicoScenesFrameSegment::toString() {
    return "";
}

std::vector<uint8_t> AbstractPicoScenesFrameSegment::getField(const std::string &fieldName) {
    const auto &field = fieldMap.at(fieldName);
    auto result = std::vector<uint8_t>(field.size());
    getField(fieldName, &result[0], result.size());
    return result;
}

uint32_t AbstractPicoScenesFrameSegment::getField(const std::string &fieldName, uint8_t *buffer, std::optional<uint32_t> capacity) {
    const auto &field = fieldMap.at(fieldName);
    if (capacity && *capacity < field.size())
        throw std::runtime_error("buffer capacity not enough for copying segment field [" + fieldName + " (" + std::to_string(field.size()) + "B)]");

    std::copy(field.cbegin(), field.cend(), buffer);
    return field.size();
}

std::tuple<std::string, uint32_t, uint16_t, uint32_t> AbstractPicoScenesFrameSegment::extractSegmentMetaData(const uint8_t *buffer, uint32_t bufferLength) {
    uint32_t rxPos = 0;
    uint32_t segmentLength = *(uint32_t *) (buffer + rxPos);
    if (segmentLength > bufferLength)
        throw std::runtime_error("corrected segment...");

    rxPos += 4;
    auto segNameLength = *(buffer + rxPos++);
    auto segmentName = std::string((char *) (buffer + rxPos));
    rxPos += segNameLength;
    uint16_t segmentVersionId = *(uint16_t *) (buffer + rxPos);
    rxPos += 2;

    return std::make_tuple(segmentName, segmentLength, segmentVersionId, rxPos);
}
