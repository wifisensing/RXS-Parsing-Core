//
// Created by Zhiping Jiang on 9/9/21.
//

#ifndef PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
#define PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX

#include "macros.hxx"
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"
#include "IntelRateNFlag.hxx"

PACKED(class IntelMVMParsedCSIHeader {
public:
    union {
        PACKED(uint8_t headerBytes[272]);
        struct {
            uint32_t iqDataSize;
            PACKED(union {
                PACKED(uint32_t blockedSection4[12]);
                PACKED(struct {
                    uint32_t value4;
                    uint32_t ftmClock;
                    uint8_t value12[40];
                });
            };
            uint32_t numTones;
            uint32_t value56[9];
            uint32_t rateNflag;
            uint32_t value96[44];
        });
    };

    IntelMVMParsedCSIHeader();

    [[nodiscard]] IntelRateNFlag getRateNFlagInterpretation() const;

    bool operator==(const IntelMVMParsedCSIHeader &rhs) const;

    bool operator!=(const IntelMVMParsedCSIHeader &rhs) const;

    static void registerDefaultMVMHeaderInterpretation();

});

class IntelMVMExtraInfo {
public:
    uint16_t CSIHeaderLength;
    std::vector<uint8_t> CSIHeader;
    IntelMVMParsedCSIHeader parsedHeader;

    [[nodiscard]] std::vector<uint8_t> toBuffer() const;
};


class MVMExtraSegment : public AbstractPicoScenesFrameSegment {
public:
    MVMExtraSegment();

    MVMExtraSegment(const uint8_t *buffer, uint32_t bufferLength);

    static void manipulateHeader(IntelMVMParsedCSIHeader &header);

    static void setHeaderManipulator(const std::function<void(IntelMVMParsedCSIHeader &)> &headerManipulator);

    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] const IntelMVMExtraInfo &getMvmExtra() const;

    void setMvmExtra(const IntelMVMExtraInfo &mvmExtraV);

private:
    static std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<IntelMVMExtraInfo(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    static std::function<void(IntelMVMParsedCSIHeader &)> headerManipulator;

    IntelMVMExtraInfo mvmExtra;
};

#endif //PICOSCENES_PLATFORM_MVMEXTRASEGMENT_HXX
