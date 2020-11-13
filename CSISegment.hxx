//
// Created by 蒋志平 on 2020/11/5.
//

#ifndef PICOSCENES_PLATFORM_CSISEGMENT_HXX
#define PICOSCENES_PLATFORM_CSISEGMENT_HXX

#include <functional>
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

class CSIDimension {
public:
    uint16_t numTones = 1;
    uint8_t numTx = 1;
    uint8_t numRx = 1;
    uint8_t numLTF = 1;
};

class CSI {
public:
    PicoScenesDeviceType deviceType;
    PacketFormatEnum packetFormat;
    CSIDimension dimensions;
    ComplexArray CSIArrays;
    Uint8Vector rawCSIData;

    static CSI fromQCA9300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones);

    static CSI fromIWL5300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, std::optional<Uint8Vector> ant_sel);

};

class CSISegment : public AbstractPicoScenesFrameSegment {
public:
    CSISegment();

    static CSISegment createByBuffer(const uint8_t *buffer, uint32_t bufferLength);

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    void addCSI(const CSI &perUserCSI);

    std::vector<CSI> muCSI;
private:
    static std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;
};


template<typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {

    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(),
              [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

    return idx;
}

#endif //PICOSCENES_PLATFORM_CSISEGMENT_HXX
