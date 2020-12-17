//
// Created by 蒋志平 on 2020/11/5.
//

#include <algorithm>
#include <utility>
#include <deque>
#include "CSISegment.hxx"
#include "preprocessor/generated/CSIPreprocessor.h"

// The following definition is for reference use.
//struct QCA9300CSISegmentContentDescriptorV1 {
//    uint16_t deviceType;
//    uint8_t packetFormat;
//    uint16_t cbw;
//uint64_t carrierFreq;
//uint64_t samplingRate;
//uint32_t subcarrierBandwidth;
//    uint16_t numTone;
//    uint8_t numSTS;        /* number of Spatial-time Stream */
//    uint8_t numESS;        /* number of Extra Spatial-time Stream */
//    uint8_t numRx;
//    uint8_t antSel;
//    uint32_t rawDataLength;
//} __attribute__ ((__packed__));
//
//struct IWL5300CSISegmentContentDescriptorV1 {
//    uint16_t deviceType;
//    uint8_t packetFormat;
//    uint16_t cbw;
//uint64_t carrierFreq;
//uint64_t samplingRate;
//uint32_t subcarrierBandwidth;
//    uint16_t numTone;
//    uint8_t numSTS;        /* number of Spatial-time Stream */
//    uint8_t numESS;        /* number of Extra Spatial-time Stream */
//    uint8_t numRx;
//    uint8_t antSel;
//    uint32_t rawDataLength;
//} __attribute__ ((__packed__));


SignalMatrix<std::complex<double>> parseQCA9300CSIData(const uint8_t *csiData, int ntx, int nrx, int num_tones) {
    std::vector<std::complex<double>> CSIArray(ntx * nrx * num_tones);
    parseQCA9300CSIData(CSIArray.begin(), csiData, ntx, nrx, num_tones);
    return SignalMatrix(CSIArray, std::vector<int32_t>{num_tones, ntx, nrx}, SignalMatrixStorageMajority::ColumnMajor);
}

SignalMatrix<std::complex<double>> parseIWL5300CSIData(const uint8_t *payload, int ntx, int nrx, uint8_t ant_sel) {
    std::vector<std::complex<double>> CSIArray(ntx * nrx * 30);
    parseIWL5300CSIData(CSIArray.begin(), payload, ntx, nrx, ant_sel);
    return SignalMatrix(CSIArray, std::vector<int32_t>{30, ntx, nrx}, SignalMatrixStorageMajority::ColumnMajor);
}

std::vector<int16_t> CSI::QCA9300SubcarrierIndices_CBW20 = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>();
    for (auto i = -28; i <= -1; i++)
        indices.emplace_back(i);
    for (auto i = 1; i <= 28; i++)
        indices.emplace_back(i);

    return indices;
}();

std::vector<int16_t> CSI::QCA9300SubcarrierIndices_CBW20_HT40MINUS = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>();
    for (auto i = -60; i <= -33; i++)
        indices.emplace_back(i);
    for (auto i = -31; i <= -4; i++)
        indices.emplace_back(i);

    return indices;
}();

std::vector<int16_t> CSI::QCA9300SubcarrierIndices_CBW20_HT40PLUS = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>();
    for (auto i = 4; i <= 31; i++)
        indices.emplace_back(i);
    for (auto i = 33; i <= 60; i++)
        indices.emplace_back(i);

    return indices;
}();

std::vector<int16_t> CSI::QCA9300SubcarrierIndices_CBW40 = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>();
    for (auto i = -58; i <= -2; i++)
        indices.emplace_back(i);
    for (auto i = 2; i <= 58; i++)
        indices.emplace_back(i);
    return indices;
}();

std::vector<int16_t> CSI::IWL5300SubcarrierIndices_CBW20 = []() noexcept -> std::vector<int16_t> {
    return std::vector<int16_t>{-28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, -1, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 28};
}();

std::vector<int16_t> CSI::IWL5300SubcarrierIndices_CBW20_HT40MINUS = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>{-28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, -1, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 28};
    for(auto & index: indices)
        index -= 32;
    return indices;
}();

std::vector<int16_t> CSI::IWL5300SubcarrierIndices_CBW20_HT40PLUS = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>{-28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, -1, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 28};
    for(auto & index: indices)
        index += 32;
    return indices;
}();

std::vector<int16_t> CSI::IWL5300SubcarrierIndices_CBW40 = []() noexcept -> std::vector<int16_t> {
    return std::vector<int16_t>{-58, -54, -50, -46, -42, -38, -34, -30, -26, -22, -18, -14, -10, -6, -2, 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58};
}();

CSI CSI::fromQCA9300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, ChannelBandwidthEnum cbw) {
    auto csi = CSI{.deviceType = PicoScenesDeviceType::QCA9300,
            .packetFormat = PacketFormatEnum::PacketFormat_HT,
            .cbw = cbw,
            .dimensions = CSIDimension{.numTones = numTones, .numTx = numTx, .numRx = numRx},
            .antSel = 0,
            .CSIArray = parseQCA9300CSIData(buffer, numTx, numRx, numTones),
    };
    std::copy(buffer, buffer + bufferLength, std::back_inserter(csi.rawCSIData));
    if (csi.cbw == ChannelBandwidthEnum::CBW_20) {
        csi.subcarrierIndices = CSI::QCA9300SubcarrierIndices_CBW20;
    } else if (csi.cbw == ChannelBandwidthEnum::CBW_40) {
        csi.subcarrierIndices = CSI::QCA9300SubcarrierIndices_CBW40;
    }

    return csi;
}

CSI CSI::fromIWL5300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, ChannelBandwidthEnum cbw, uint8_t ant_sel) {
    auto csi = CSI{.deviceType = PicoScenesDeviceType::IWL5300,
            .packetFormat=PacketFormatEnum::PacketFormat_HT,
            .cbw = cbw,
            .dimensions = CSIDimension{.numTones = numTones, .numTx = numTx, .numRx = numRx, .numESS = numLTF},
            .antSel = ant_sel,
            .CSIArray = parseIWL5300CSIData(buffer, numTx, numRx, ant_sel),
    };
    std::copy(buffer, buffer + bufferLength, std::back_inserter(csi.rawCSIData));
    if (csi.cbw == ChannelBandwidthEnum::CBW_20) {
        csi.subcarrierIndices = CSI::IWL5300SubcarrierIndices_CBW20;
    } else if (csi.cbw == ChannelBandwidthEnum::CBW_40)
        csi.subcarrierIndices = CSI::IWL5300SubcarrierIndices_CBW40;

    return csi;
}

void CSI::interpolateCSI() {
    coder::array<creal_T, 2U> CSI;
    coder::array<creal_T, 2U> newCSI;
    coder::array<double, 2U> newMag;
    coder::array<double, 2U> newPhase;
    coder::array<short, 1U> interpedIndex_int16;
    coder::array<short, 1U> subcarrierIndex_int16;

    CSI.set_size(dimensions.numTones, dimensions.numTx * dimensions.numRx);
    for (auto toneIndex = 0; toneIndex < CSI.size(0); toneIndex++) {
        for (auto txTrIndex = 0; txTrIndex < CSI.size(1); txTrIndex++) {
            auto pos = toneIndex + txTrIndex * CSI.size(0);
            CSI[pos] = *(creal_T *) (&CSIArray.array[pos]);
        }
    }

    subcarrierIndex_int16.set_size(subcarrierIndices.size());
    for (auto scIndex = 0; scIndex < subcarrierIndex_int16.size(0); scIndex++) {
        subcarrierIndex_int16[scIndex] = subcarrierIndices[scIndex];
    }

    CSIPreprocessor(CSI, subcarrierIndex_int16, newCSI, newMag, newPhase, interpedIndex_int16);

    CSIArray.array.clear();
    std::copy((std::complex<double> *) newCSI.data(), (std::complex<double> *) newCSI.data() + newCSI.numel(), std::back_inserter(CSIArray.array));

    magnitudeArray.array.clear();
    std::copy((double *) newMag.data(), (double *) newMag.data() + newMag.numel(), std::back_inserter(magnitudeArray.array));

    phaseArray.array.clear();
    std::copy((double *) newPhase.data(), (double *) newPhase.data() + newPhase.numel(), std::back_inserter(phaseArray.array));

    subcarrierIndices.clear();
    std::copy((int16_t *) interpedIndex_int16.data(), (int16_t *) interpedIndex_int16.data() + interpedIndex_int16.numel(), std::back_inserter(subcarrierIndices));
    dimensions.numTones = subcarrierIndices.size();
}

std::vector<uint8_t> CSI::toBuffer() {
    if (!rawCSIData.empty()) {
        return rawCSIData;
    }

    auto buffer = std::vector<uint8_t>();
    std::copy((uint8_t *) &deviceType, (uint8_t *) &deviceType + sizeof(deviceType), std::back_inserter(buffer));
    std::copy((uint8_t *) &packetFormat, (uint8_t *) &packetFormat + sizeof(packetFormat), std::back_inserter(buffer));
    std::copy((uint8_t *) &cbw, (uint8_t *) &cbw + sizeof(cbw), std::back_inserter(buffer));
    std::copy((uint8_t *) &carrierFreq, (uint8_t *) &carrierFreq + sizeof(carrierFreq), std::back_inserter(buffer));
    std::copy((uint8_t *) &samplingRate, (uint8_t *) &samplingRate + sizeof(samplingRate), std::back_inserter(buffer));
    std::copy((uint8_t *) &subcarrierBandwidth, (uint8_t *) &subcarrierBandwidth + sizeof(subcarrierBandwidth), std::back_inserter(buffer));
    std::copy((uint8_t *) &dimensions.numTones, (uint8_t *) &dimensions.numTones + sizeof(dimensions.numTones), std::back_inserter(buffer));
    std::copy((uint8_t *) &dimensions.numTx, (uint8_t *) &dimensions.numTx + sizeof(dimensions.numTx), std::back_inserter(buffer));
    std::copy((uint8_t *) &dimensions.numRx, (uint8_t *) &dimensions.numRx + sizeof(dimensions.numRx), std::back_inserter(buffer));
    std::copy((uint8_t *) &dimensions.numESS, (uint8_t *) &dimensions.numESS + sizeof(dimensions.numESS), std::back_inserter(buffer));
    std::copy((uint8_t *) &antSel, (uint8_t *) &antSel + sizeof(antSel), std::back_inserter(buffer));
    if (deviceType == PicoScenesDeviceType::IWL5300 || deviceType == PicoScenesDeviceType::QCA9300) {
        std::copy(rawCSIData.cbegin(), rawCSIData.cend(), std::back_inserter(buffer));
    } else if (deviceType == PicoScenesDeviceType::USRP) {
        std::vector<uint8_t> subcarrierIndicesBuffer;
        for (const auto &subcarrierIndex: subcarrierIndices) {
            std::copy((uint8_t *) &subcarrierIndex, (uint8_t *) &subcarrierIndex + sizeof(subcarrierIndex), std::back_inserter(subcarrierIndicesBuffer));
        }
        auto csiBuffer = CSIArray.toBuffer();

        uint32_t csiBufferLength = subcarrierIndicesBuffer.size() + csiBuffer.size();
        std::copy((uint8_t *) &csiBufferLength, (uint8_t *) &csiBufferLength + sizeof(csiBufferLength), std::back_inserter(buffer));
        std::copy(subcarrierIndicesBuffer.cbegin(), subcarrierIndicesBuffer.cend(), std::back_inserter(buffer));
        std::copy(csiBuffer.cbegin(), csiBuffer.cend(), std::back_inserter(buffer));
    }

    return buffer;
}

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> CSI {
    uint32_t pos = 0;

    auto deviceType = (PicoScenesDeviceType) *(uint16_t *) (buffer + pos);
    pos += sizeof(PicoScenesDeviceType);
    PacketFormatEnum packetFormat = *(PacketFormatEnum *) (buffer + pos);
    pos += sizeof(PacketFormatEnum);
    ChannelBandwidthEnum cbw = *(ChannelBandwidthEnum *) (buffer + pos);
    pos += sizeof(ChannelBandwidthEnum);
    auto carrierFreq = *(uint64_t *) (buffer + pos);
    pos += sizeof(uint64_t);
    auto samplingRate = *(uint64_t *) (buffer + pos);
    pos += sizeof(uint64_t);
    auto subcarrierBandwidth = *(uint32_t *) (buffer + pos);
    pos += sizeof(uint32_t);
    uint16_t numTone = *(uint16_t *) (buffer + pos);
    pos += 2;
    uint8_t numSTS = *(uint8_t *) (buffer + pos++);
    uint8_t numRx = *(uint8_t *) (buffer + pos++);
    uint8_t numESS = *(uint8_t *) (buffer + pos++);
    uint8_t antSelByte = *(uint8_t *) (buffer + pos++);
    uint32_t CSIBufferLength = *(uint32_t *) (buffer + pos);
    pos += 4;

    if (deviceType == PicoScenesDeviceType::QCA9300) {
        auto csi = CSI::fromQCA9300(buffer + pos, CSIBufferLength, numSTS, numRx, numSTS + numESS, numTone, cbw);
        csi.carrierFreq = carrierFreq;
        csi.samplingRate = samplingRate;
        csi.subcarrierBandwidth = subcarrierBandwidth;
        return csi;
    } else if (deviceType == PicoScenesDeviceType::IWL5300) {
        auto csi = CSI::fromIWL5300(buffer + pos, CSIBufferLength, numSTS, numRx, numSTS + numESS, numTone, cbw, antSelByte);
        csi.carrierFreq = carrierFreq;
        csi.samplingRate = samplingRate;
        csi.subcarrierBandwidth = subcarrierBandwidth;
        return csi;
    } else if (deviceType == PicoScenesDeviceType::USRP) {
        auto csiBufferStart = pos;
        std::vector<int16_t> subcarrierIndices;
        for (auto i = 0; i < numTone; i++) {
            subcarrierIndices.emplace_back(*(uint16_t *) (buffer + pos));
            pos += 2;
        }
        uint32_t csiArrayLength = CSIBufferLength - 2 * numTone;
        CSI csi{.deviceType = PicoScenesDeviceType::USRP,
                .packetFormat = packetFormat,
                .cbw = cbw,
                .carrierFreq = carrierFreq,
                .samplingRate = samplingRate,
                .subcarrierBandwidth = subcarrierBandwidth,
                .dimensions = CSIDimension{.numTones = numTone, .numTx = numSTS, .numRx = numRx, .numESS = numESS},
                .antSel = 0,
                .subcarrierIndices = subcarrierIndices,
                .CSIArray = SignalMatrix<std::complex<double>>::fromBuffer(buffer + pos, buffer + pos + csiArrayLength, SignalMatrixStorageMajority::RowMajor)
        };
        auto csiBufferEnd = pos + csiArrayLength;
        std::copy(buffer + csiBufferStart, buffer + csiBufferEnd, std::back_inserter(csi.rawCSIData));
        return csi;
    }

    throw std::runtime_error("CSISegment cannot decode the given buffer.");
};

std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> CSISegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> CSISegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>>{{0x1U, v1Parser}};
}

CSISegment::CSISegment() : AbstractPicoScenesFrameSegment("CSI", 0x1u) {

}

void CSISegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto[segmentName, segmentLength, versionId, offset] = extractSegmentMetaData(buffer, bufferLength);
    if (segmentName != "CSI" && segmentName != "LegacyCSI")
        throw std::runtime_error("CSISegment cannot parse the segment named " + segmentName + ".");
    if (segmentLength + 4 > bufferLength)
        throw std::underflow_error("CSISegment cannot parse the segment with less than " + std::to_string(segmentLength + 4) + "B.");
    if (!versionedSolutionMap.contains(versionId)) {
        throw std::runtime_error("CSISegment cannot parse the segment with version v" + std::to_string(versionId) + ".");
    }

    csi = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    std::copy(buffer, buffer + bufferLength, std::back_inserter(rawBuffer));
    this->segmentLength = segmentLength;
    isSuccessfullyDecoded = true;
}

CSISegment CSISegment::createByBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    CSISegment csiSegment;
    csiSegment.fromBuffer(buffer, bufferLength);
    return csiSegment;
}

std::string CSISegment::toString() const {
    std::stringstream ss;
    ss << segmentName + ":[";
    ss << "(device=" << csi.deviceType << ", format=" << csi.packetFormat << ", CBW=" << csi.cbw << ", dim(nTones,nSTS,nESS,nRx)=(" + std::to_string(csi.dimensions.numTones) + "," + std::to_string(csi.dimensions.numTx) + "," + std::to_string(csi.dimensions.numESS) + "," + std::to_string(csi.dimensions.numRx) + "), raw=" + std::to_string(csi.rawCSIData.size()) + "B)]";
    auto temp = ss.str();
    temp.erase(temp.end() - 2, temp.end());
    temp.append("]");
    return temp;
}

std::vector<uint8_t> CSISegment::toBuffer() const {
    return AbstractPicoScenesFrameSegment::toBuffer(true);
}

const CSI &CSISegment::getCSI() const {
    return csi;
}

CSI &CSISegment::getCSI() {
    return csi;
}

void CSISegment::setCSI(const CSI &csiV) {
    csi = csiV;
    clearFieldCache();
    addField("core", csi.toBuffer());
}

std::ostream &operator<<(std::ostream &os, const CSISegment &csiSegment) {
    os << csiSegment.toString();
    return os;
}