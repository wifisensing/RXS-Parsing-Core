//
// Created by 蒋志平 on 9/9/21.
//

#include "MVMExtraSegment.hxx"

std::map<std::string, std::tuple<std::string, size_t, size_t, bool>> IntelMVMCSIHeaderDefinition::fieldMapping;
std::vector<std::pair<std::string, std::tuple<std::string, size_t, size_t, bool>>> IntelMVMCSIHeaderDefinition::fieldList;
std::unordered_map<std::type_index, std::string> IntelMVMCSIHeaderDefinition::typeNames;

void IntelMVMCSIHeaderDefinition::ensureTypeNameMapReady() {
    if (typeNames.empty()) {
        typeNames[std::type_index(typeid(int8_t))] = "int8";
        typeNames[std::type_index(typeid(uint8_t))] = "uint8";
        typeNames[std::type_index(typeid(int16_t))] = "int16";
        typeNames[std::type_index(typeid(uint16_t))] = "uint16";
        typeNames[std::type_index(typeid(int32_t))] = "int32";
        typeNames[std::type_index(typeid(uint32_t))] = "uint32";
        typeNames[std::type_index(typeid(int64_t))] = "int64";
        typeNames[std::type_index(typeid(uint64_t))] = "uint64";
    }
}

void IntelMVMCSIHeaderDefinition::buildDefaultFieldMapping() {
    ensureTypeNameMapReady();
    fieldList.clear();
    fieldMapping.clear();
    fieldList.emplace_back(makeField<uint32_t, 0, 1>("IQDataSize", false));
    fieldList.emplace_back(makeField<uint32_t, 8, 1>("FTMClock", true));
    fieldList.emplace_back(makeField<uint32_t, 12 + 10 * 4, 1>("NumTone", false));
    std::copy(fieldList.cbegin(), fieldList.cend(), std::inserter(fieldMapping, fieldMapping.begin()));
}

const std::map<std::string, std::tuple<std::string, size_t, size_t, bool>> &IntelMVMCSIHeaderDefinition::getCurrentFieldMapping() {
    if (fieldMapping.empty()) [[unlikely]] {
        buildDefaultFieldMapping();
    }

    return fieldMapping;
}

const std::vector<std::pair<std::string, std::tuple<std::string, size_t, size_t, bool>>> &IntelMVMCSIHeaderDefinition::getCurrentFields() {
    if (fieldList.empty()) [[unlikely]] {
        buildDefaultFieldMapping();
    }

    return fieldList;
}

void IntelMVMCSIHeaderDefinition::setNewFieldMapping(const std::vector<std::pair<std::string, std::tuple<std::string, size_t, size_t, bool>>> &newFields) {
    fieldList.clear();
    fieldMapping.clear();
    std::copy(newFields.cbegin(), newFields.cend(), std::back_inserter(fieldList));
    std::copy(newFields.cbegin(), newFields.cend(), std::inserter(fieldMapping, fieldMapping.begin()));
}

IntelMVMParsedCSIHeader::IntelMVMParsedCSIHeader() {
    memset(this, 0, sizeof(IntelMVMParsedCSIHeader));
}

bool IntelMVMParsedCSIHeader::operator==(const IntelMVMParsedCSIHeader &rhs) const {
    return std::memcmp(this, &rhs, sizeof(IntelMVMParsedCSIHeader));
}

bool IntelMVMParsedCSIHeader::operator!=(const IntelMVMParsedCSIHeader &rhs) const {
    return !(rhs == *this);
}

IntelRateNFlag IntelMVMParsedCSIHeader::getRateNFlagInterpretation() const {
    return IntelRateNFlag(rateNflag);
}

bool IntelMVMParsedCSIHeader::hasNamedField(const std::string &fieldName) const noexcept {
    return IntelMVMCSIHeaderDefinition::getCurrentFieldMapping().contains(fieldName);
}

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> IntelMVMExtraInfo {
    uint32_t pos = 0;
    auto extra = IntelMVMExtraInfo();
    extra.CSIHeaderLength = *(uint16_t *) (buffer + pos);
    pos += 2;
    if (bufferLength - pos < extra.CSIHeaderLength)
        throw std::runtime_error("MVMExtraSegment v1Parser cannot parse the segment with insufficient buffer length.");

    extra.parsedHeader = *(IntelMVMParsedCSIHeader *) (buffer + pos);
    pos += sizeof(IntelMVMParsedCSIHeader);
    MVMExtraSegment::manipulateHeader(extra.parsedHeader);

    std::copy((uint8_t *) &extra.parsedHeader, (uint8_t *) (&extra.parsedHeader) + sizeof(IntelMVMParsedCSIHeader), std::back_inserter(extra.CSIHeader));
    std::copy(buffer + pos, buffer + bufferLength, std::back_inserter(extra.CSIHeader));
    if (false) {
        IntelMVMParsedCSIHeader regeneratedInstance = *(IntelMVMParsedCSIHeader *) extra.CSIHeader.data();
        if (extra.parsedHeader != regeneratedInstance)
            throw std::runtime_error("failed to validate IntelMVMParsedCSIHeader parser.");
    }

    return extra;
};

std::vector<uint8_t> IntelMVMExtraInfo::toBuffer() const {
    auto buffer = std::vector<uint8_t>();
    std::copy((uint8_t *) &CSIHeaderLength, (uint8_t *) &CSIHeaderLength + sizeof(CSIHeaderLength), std::back_inserter(buffer));
    std::copy(CSIHeader.cbegin(), CSIHeader.cend(), std::back_inserter(buffer));
    return buffer;
}

std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> MVMExtraSegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> MVMExtraSegment::initializeSolutionMap() noexcept {
    std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> map;
    map.emplace(0x1U, v1Parser);
    return map;
}

std::function<void(IntelMVMParsedCSIHeader &)> MVMExtraSegment::headerManipulator{nullptr};

MVMExtraSegment::MVMExtraSegment() : AbstractPicoScenesFrameSegment("MVMExtra", 0x1U) {}

MVMExtraSegment::MVMExtraSegment(const uint8_t *buffer, uint32_t bufferLength) : AbstractPicoScenesFrameSegment(buffer, bufferLength) {
    if (segmentName != "MVMExtra")
        throw std::runtime_error("MVMExtraSegment cannot parse the segment named " + segmentName + ".");
    if (!versionedSolutionMap.count(segmentVersionId))
        throw std::runtime_error("MVMExtraSegment cannot parse the segment with version v" + std::to_string(segmentVersionId) + ".");

    mvmExtra = versionedSolutionMap.at(segmentVersionId)(segmentPayload.data(), segmentPayload.size());
    setMvmExtra(mvmExtra);
}

std::string MVMExtraSegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[";
    ss << "hdr_len=" << mvmExtra.CSIHeaderLength << ", csi_len=" << mvmExtra.parsedHeader.iqDataSize << "B, num_tone=" << mvmExtra.parsedHeader.numTones << ", ftm_clock=" << std::dec << mvmExtra.parsedHeader.ftmClock << "]";
    auto temp = ss.str();
    return temp;
}

std::ostream &operator<<(std::ostream &os, const MVMExtraSegment &mvmSegment) {
    os << mvmSegment.toString();
    return os;
}

const IntelMVMExtraInfo &MVMExtraSegment::getMvmExtra() const {
    return mvmExtra;
}

void MVMExtraSegment::manipulateHeader(IntelMVMParsedCSIHeader &header) {
    if (headerManipulator)
        headerManipulator(header);
}

void MVMExtraSegment::setHeaderManipulator(const std::function<void(IntelMVMParsedCSIHeader &)> &headerManipulatorV) {
    static std::once_flag flag;
    std::call_once(flag, [&] {
        headerManipulator = headerManipulatorV == nullptr ? headerManipulator : headerManipulatorV; // this property can only be set once during the start
    });
}

void MVMExtraSegment::setMvmExtra(const IntelMVMExtraInfo &mvmExtra) {
    MVMExtraSegment::mvmExtra = mvmExtra;
    auto buffer = mvmExtra.toBuffer();
    setSegmentPayload(buffer);
}