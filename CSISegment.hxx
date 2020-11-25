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
    uint8_t numESS = 0;
};

class CSI {
public:
    PicoScenesDeviceType deviceType;
    PacketFormatEnum packetFormat;
    ChannelBandwidthEnum cbw;
    CSIDimension dimensions;
    uint8_t antSel;
    ComplexArray CSIArrays;
    std::vector<int16_t> subcarrierIndices;
    Uint8Vector rawCSIData;

    void interpolateCSI();

    static CSI fromQCA9300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, ChannelBandwidthEnum cbw);

    static CSI fromIWL5300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, ChannelBandwidthEnum cbw, uint8_t ant_sel);

    template<typename OutputValueType, typename InputValueType>
    static std::vector<std::complex<OutputValueType>> convertCSIArrayType(const std::vector<std::complex<InputValueType>> &inputArray) {
        std::vector<std::complex<OutputValueType>> outputArray(inputArray.size());
        for (auto i = 0; i < inputArray.size(); i++) {
            outputArray[i] = std::complex<OutputValueType>(inputArray[i].real(), inputArray[i].imag());
        }
        return outputArray;
    }

private:
    static std::vector<int16_t> QCA9300SubcarrierIndices_CBW20;
    static std::vector<int16_t> QCA9300SubcarrierIndices_CBW40;
    static std::vector<int16_t> IWL5300SubcarrierIndices_CBW20;
    static std::vector<int16_t> IWL5300SubcarrierIndices_CBW40;
};

class CSISegment : public AbstractPicoScenesFrameSegment {
public:
    CSISegment();

    static CSISegment createByBuffer(const uint8_t *buffer, uint32_t bufferLength);

    void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) override;

    CSI csi;

    [[nodiscard]] std::string toString() const override;

private:
    static std::map<uint16_t, std::function<std::vector<CSI>(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<std::vector<CSI>(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;
};

std::ostream &operator<<(std::ostream &os, const CSISegment &csiSegment);

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
