//
// Created by 蒋志平 on 9/9/21.
//

#ifndef PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class IntelMVMParsedCSIHeader {
public:
    uint32_t iqDataSize{};
    uint32_t reserved4;
    uint32_t ftmClock;
    uint8_t reserved12_52[40];
    uint32_t numTones;
    uint32_t reserved56;
    uint32_t rssi1;
    uint32_t rssi2;
    uint8_t sourceAddress[6];
    uint16_t addressPadding;
    uint8_t csiSequence;
    uint8_t reserved77[11];
    uint32_t muClock; // 88
    uint32_t rate_n_flags; // 92
    union {
        uint8_t blockedSection[176] __attribute__ ((__packed__));
        struct {
            uint8_t chain0Info96[20];
            uint8_t chain1Info96[20];
            uint8_t chain2Info96[20];
            uint8_t chain3Info96[20];
            uint32_t value176;
            uint32_t value180;
            uint32_t value184;
            uint8_t reserved188_198[10];
            uint16_t value198;
            uint64_t timeValue200;
            uint8_t reserved208_240[32];
            uint8_t chainInfo240[4];
            uint8_t chainInfo244[4];
            uint8_t chainInfo248[4];
            uint8_t chainInfo252[4];
            uint16_t value256;
            uint16_t value258;
            uint16_t value260;
            uint16_t value262;
            uint16_t value264;
            uint16_t value266;
            uint16_t value268;
            uint16_t value270;
        } __attribute__ ((__packed__));
    };


    IntelMVMParsedCSIHeader();

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

    static bool isAdvancedPropertiesBlocked();

    static void setBlockingAdvancedProperties(bool block);

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    std::vector<uint8_t> toBuffer() const override;

    [[nodiscard]] std::string toString() const override;

    const IntelMVMExtrta &getMvmExtra() const;

private:
    static std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<IntelMVMExtrta(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    static bool blockAdvancedProperties;

    IntelMVMExtrta mvmExtra;
};

std::ostream &operator<<(std::ostream &os, const MVMExtraSegment &mvmSegment);


#endif //PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
