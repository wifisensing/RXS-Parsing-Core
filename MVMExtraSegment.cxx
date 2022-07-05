//
// Created by 蒋志平 on 9/9/21.
//

#include "MVMExtraSegment.hxx"
#include <mutex>


IntelMVMParsedCSIHeader::IntelMVMParsedCSIHeader() {
    memset(this, 0, sizeof(IntelMVMParsedCSIHeader));
}

bool IntelMVMParsedCSIHeader::operator==(const IntelMVMParsedCSIHeader &rhs) const {
    return std::memcmp(this, &rhs, sizeof(IntelMVMParsedCSIHeader));
}

bool IntelMVMParsedCSIHeader::operator!=(const IntelMVMParsedCSIHeader &rhs) const {
    return !(rhs == *this);
}

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> IntelMVMExtrta {
    uint32_t pos = 0;
    auto extra = IntelMVMExtrta();
    extra.CSIHeaderLength = *(uint16_t *) (buffer + pos);
    pos += 2;
    if (bufferLength - pos < extra.CSIHeaderLength)
        throw std::runtime_error("MVMExtraSegment v1Parser cannot parse the segment with insufficient buffer length.");

    extra.parsedHeader = *(IntelMVMParsedCSIHeader *) (buffer + pos);
    pos += sizeof(IntelMVMParsedCSIHeader);
    if (MVMExtraSegment::isAdvancedPropertiesBlocked()) {
        std::memset(extra.parsedHeader.blockedSection4, 0, sizeof(extra.parsedHeader.blockedSection4));
        std::memset(extra.parsedHeader.blockedSection56, 0, sizeof(extra.parsedHeader.blockedSection56));
    }
    if (MVMExtraSegment::isReservedPropertiesBlocked()) {
        std::memset(extra.parsedHeader.blockedSection96, 0, sizeof(extra.parsedHeader.blockedSection96));
    }
    std::copy((uint8_t *) &extra.parsedHeader, (uint8_t *) (&extra.parsedHeader) + sizeof(IntelMVMParsedCSIHeader), std::back_inserter(extra.CSIHeader));
    std::copy(buffer + pos, buffer + bufferLength, std::back_inserter(extra.CSIHeader));
    if (false) {
        IntelMVMParsedCSIHeader regeneratedInstance = *(IntelMVMParsedCSIHeader *) extra.CSIHeader.data();
        if (extra.parsedHeader != regeneratedInstance)
            throw std::runtime_error("failed to validate IntelMVMParsedCSIHeader parser.");
    }

    return extra;
};

std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> MVMExtraSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> MVMExtraSegment::initializeSolutionMap() noexcept {
    std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> map;
    map.emplace(0x1U, v1Parser);
    return map;
}

bool MVMExtraSegment::blockAdvancedProperties = false;

MVMExtraSegment::MVMExtraSegment() : AbstractPicoScenesFrameSegment("MVMExtra", 0x1U) {}

void MVMExtraSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto [segmentName, segmentLength, versionId, offset] = extractSegmentMetaData(buffer, bufferLength);
    if (segmentName != "MVMExtra")
        throw std::runtime_error("MVMExtraSegment cannot parse the segment named " + segmentName + ".");
    if (segmentLength + 4 > bufferLength)
        throw std::underflow_error("MVMExtraSegment cannot parse the segment with less than " + std::to_string(segmentLength + 4) + "B.");
    if (!versionedSolutionMap.count(versionId)) {
        throw std::runtime_error("MVMExtraSegment cannot parse the segment with version v" + std::to_string(versionId) + ".");
    }

    mvmExtra = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    rawBuffer.reserve(bufferLength);
    std::copy(buffer, buffer + offset, std::back_inserter(rawBuffer));
    std::copy((uint8_t *) &mvmExtra.CSIHeaderLength, (uint8_t *) &mvmExtra.CSIHeaderLength + sizeof(mvmExtra.CSIHeaderLength), std::back_inserter(rawBuffer));
    std::copy(mvmExtra.CSIHeader.cbegin(), mvmExtra.CSIHeader.cend(), std::back_inserter(rawBuffer));
    this->segmentLength = segmentLength;
    successfullyDecoded = true;
}

MVMExtraSegment MVMExtraSegment::createByBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    MVMExtraSegment mvmExtraSegment;
    mvmExtraSegment.fromBuffer(buffer, bufferLength);
    return mvmExtraSegment;
}

std::vector<uint8_t> MVMExtraSegment::toBuffer() const {
    auto buffer = AbstractPicoScenesFrameSegment::toBuffer(true);
    if (false) {
        auto extraSeg = createByBuffer(buffer.data(), buffer.size());
        std::cout << extraSeg << std::endl;
    }

    return buffer;
}

std::string MVMExtraSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[";
    ss << "hdr_len=" << mvmExtra.CSIHeaderLength << ", csi_len=" << mvmExtra.parsedHeader.iqDataSize << "B, num_tone=" << mvmExtra.parsedHeader.numTones << ", rate_n_flags=0x" << std::hex << mvmExtra.parsedHeader.rate_n_flags << ", ftm_clock=" << std::dec << mvmExtra.parsedHeader.ftmClock << ", mu_clock=" << mvmExtra.parsedHeader.muClock << "]";
    auto temp = ss.str();
    return temp;
}

std::ostream &operator<<(std::ostream &os, const MVMExtraSegment &mvmSegment) {
    os << mvmSegment.toString();
    return os;
}

const IntelMVMExtrta &MVMExtraSegment::getMvmExtra() const {
    return mvmExtra;
}

void MVMExtraSegment::setBlockingAdvancedProperties(bool block) {
    static std::once_flag flag;
    std::call_once(flag, [&] {
        blockAdvancedProperties = block; // this property can only be set once during the start
    });
}

bool MVMExtraSegment::isAdvancedPropertiesBlocked() {
    return blockAdvancedProperties;
}

bool MVMExtraSegment::isReservedPropertiesBlocked() {
    return blockReservedProperties;
}

void MVMExtraSegment::setBlockingReservedProperties(bool block) {
    static std::once_flag flag;
    std::call_once(flag, [&] {
        blockReservedProperties = block; // this property can only be set once during the start
    });
}
