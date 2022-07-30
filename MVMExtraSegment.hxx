//
// Created by 蒋志平 on 9/9/21.
//

#ifndef PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX

#include <typeindex>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class IntelMVMCSIHeaderDefinition {
public:
    IntelMVMCSIHeaderDefinition() = delete;

    static const std::map<std::string, std::tuple<std::string, size_t, size_t, bool>> &getCurrentFieldMapping();

    static void setNewFieldMapping(const std::map<std::string, std::tuple<std::string, size_t, size_t, bool>> &newMapping);

    template<typename FieldType, size_t fieldPosition, size_t numFieldElements = 1>
    static std::pair<std::string, std::tuple<std::string, size_t, size_t, bool>> makeField(std::string fieldName, bool display) {
        ensureTypeNameMapReady();
        return std::make_pair(fieldName, std::make_tuple(typeNames[std::type_index(typeid(FieldType))], fieldPosition, sizeof(FieldType) * numFieldElements, display));
    }

private:

    static std::map<std::string, std::tuple<std::string, size_t, size_t, bool>> fieldMapping;

    static std::unordered_map<std::type_index, std::string> typeNames;

    static void ensureTypeNameMapReady();

    static void buildDefaultFieldMapping();
};

class IntelMVMParsedCSIHeader {
public:
    union {
        uint8_t headerBytes[272]  __attribute__ ((__packed__));;
        struct {
            uint32_t iqDataSize{};
            union {
                uint32_t blockedSection4[12] __attribute__ ((__packed__));
                struct {
                    uint32_t value4;
                    uint32_t ftmClock;
                    uint8_t value12[40];
                } __attribute__ ((__packed__));
            };
            uint32_t numTones;
            uint32_t value56[54];
        }__attribute__ ((__packed__));
    };

    IntelMVMParsedCSIHeader();

    template<typename OutputType>
    OutputType accessField(const std::string &fieldName) const noexcept {
        static std::unordered_map<std::string, size_t> quickMap;
        if (quickMap.contains(fieldName))
            return *(OutputType *) (headerBytes + quickMap[fieldName]);

        if (IntelMVMCSIHeaderDefinition::getCurrentFieldMapping().contains(fieldName)) {
            auto pos = std::get<1>(IntelMVMCSIHeaderDefinition::getCurrentFieldMapping().at(fieldName));
            quickMap[fieldName] = pos;
            return *(OutputType *) (headerBytes + pos);
        }

        return OutputType{};
    }

    bool operator==(const IntelMVMParsedCSIHeader &rhs) const;

    bool operator!=(const IntelMVMParsedCSIHeader &rhs) const;

} __attribute__ ((__packed__));

class IntelMVMExtrta {
public:
    uint16_t CSIHeaderLength;
    std::vector<uint8_t> CSIHeader;
    IntelMVMParsedCSIHeader parsedHeader;
};


class MVMExtraSegment : public AbstractPicoScenesFrameSegment {
public:
    MVMExtraSegment();

    static MVMExtraSegment createByBuffer(const uint8_t *buffer, uint32_t bufferLength);

    static void manipulateHeader(IntelMVMParsedCSIHeader &header);

    static void setHeaderManipulator(const std::function<void(IntelMVMParsedCSIHeader &)> &headerManipulator);

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    std::vector<uint8_t> toBuffer() const override;

    [[nodiscard]] std::string toString() const override;

    const IntelMVMExtrta &getMvmExtra() const;

private:
    static std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    static std::function<void(IntelMVMParsedCSIHeader &)> headerManipulator;

    IntelMVMExtrta mvmExtra;
};

std::ostream &operator<<(std::ostream &os, const MVMExtraSegment &mvmSegment);


#endif //PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
