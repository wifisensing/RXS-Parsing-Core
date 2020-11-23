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
//    uint8_t numCSIGroup;
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
//    uint8_t numCSIGroup;
//    uint16_t numTone;
//    uint8_t numSTS;        /* number of Spatial-time Stream */
//    uint8_t numESS;        /* number of Extra Spatial-time Stream */
//    uint8_t numRx;
//    uint8_t antSel;
//    uint32_t rawDataLength;
//} __attribute__ ((__packed__));

void parseQCA9300CSIData(std::complex<double> *outputArray, const uint8_t *csiData, int ntx, int nrx, int num_tones) {

    auto parse10bitsValues = [](const uint8_t rawByte[5], int outputArray[4]) {
        static uint16_t negativeSignBit = (1 << (10 - 1));
        static uint16_t minNegativeValue = (1 << 10);
        outputArray[0] = ((rawByte[0] & 0xffU) >> 0U) + ((rawByte[1] & 0x03u) << 8u);
        outputArray[1] = ((rawByte[1] & 0xfcU) >> 2U) + ((rawByte[2] & 0x0fU) << 6U);
        outputArray[2] = ((rawByte[2] & 0xf0U) >> 4U) + ((rawByte[3] & 0x3fU) << 4U);
        outputArray[3] = ((rawByte[3] & 0xc0U) >> 6U) + ((rawByte[4] & 0xffU) << 2U);
        for (auto i = 0; i < 4; i++) {
            if (outputArray[i] & negativeSignBit)
                outputArray[i] -= minNegativeValue;
        }
    };

    int valuePos, pos, rxIndex, txIndex, toneIndex, totalTones = nrx * ntx * num_tones;
    int tempArray[4];
    for (auto i = 0; i < totalTones / 2; i++) {
        parse10bitsValues(csiData + i * 5, tempArray);

        valuePos = i * 2;
        rxIndex = valuePos % nrx;
        txIndex = (valuePos / nrx) % ntx;
        toneIndex = valuePos / (nrx * ntx);
        pos = rxIndex * (ntx * num_tones) + txIndex * num_tones + toneIndex;
        outputArray[pos].real(tempArray[1]);
        outputArray[pos].imag(tempArray[0]);

        valuePos = i * 2 + 1;
        rxIndex = valuePos % nrx;
        txIndex = (valuePos / nrx) % ntx;
        toneIndex = valuePos / (nrx * ntx);
        pos = rxIndex * (ntx * num_tones) + txIndex * num_tones + toneIndex;
        outputArray[pos].real(tempArray[3]);
        outputArray[pos].imag(tempArray[2]);
    }
}

void parseIWL5300CSIData(std::complex<double> *csi_matrix, const uint8_t *payload, int ntx, int nrx, uint8_t ant_sel) {

    auto positionComputationWRTPermutation = [](int ntx, int num_tones, int ntxIdx, int nrxIdx, int subcarrierIdx, std::optional<Uint8Vector> ant_sel) -> int {
        auto new_nrxIdx = nrxIdx;
        if (ant_sel && ant_sel->size() > 1) {
            auto ant_sel_copy = *ant_sel;
            auto sorted_indexes = sort_indexes(ant_sel_copy);
            sorted_indexes = sort_indexes(sorted_indexes); // double sort, shit but works !
            new_nrxIdx = sorted_indexes[nrxIdx];
        }

        return new_nrxIdx * (ntx * num_tones) + ntxIdx * num_tones + subcarrierIdx;
    };

    std::vector<uint8_t> antSelVector = [](uint8_t ant_sel) {
        return std::vector<uint8_t>{static_cast<unsigned char>(((unsigned) ant_sel & 0x1U) + 1),
                                    static_cast<unsigned char>((((unsigned) ant_sel >> 0x2U) & 0x3U) + 1),
                                    static_cast<unsigned char>((((unsigned) ant_sel >> 0x4U) & 0x3U) + 1)
        };
    }(ant_sel);

    uint32_t index = 0;
    uint8_t remainder;

    for (auto subcarrierIdx = 0; subcarrierIdx < 30; subcarrierIdx++) {
        index += 3;
        remainder = index % 8;

        for (auto nrxIdx = 0; nrxIdx < nrx; nrxIdx++) {
            for (auto ntxIdx = 0; ntxIdx < ntx; ntxIdx++) {
                auto position = positionComputationWRTPermutation(nrx, 30, ntxIdx, nrxIdx, subcarrierIdx, antSelVector);
                char tmp1 = (payload[index / 8] >> remainder) | (payload[index / 8 + 1] << (8 - remainder));
                char tmp2 = (payload[index / 8 + 1] >> remainder) | (payload[index / 8 + 2] << (8 - remainder));
                csi_matrix[position].real((double) tmp1);
                csi_matrix[position].imag((double) tmp2);
                index += 16;
            }
        }
    }
}

std::vector<int16_t> CSI::QCA9300SubcarrierIndices_CBW20 = []() noexcept -> std::vector<int16_t> {
    auto indices = std::vector<int16_t>();
    for (auto i = -28; i <= -1; i++)
        indices.emplace_back(i);
    for (auto i = 1; i <= 28; i++)
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
    return std::vector<int16_t>{-28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, -1, 0, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 28};
}();

std::vector<int16_t> CSI::IWL5300SubcarrierIndices_CBW40 = []() noexcept -> std::vector<int16_t> {
    return std::vector<int16_t>{-58, -54, -50, -46, -42, -38, -34, -30, -26, -22, -18, -14, -10, -6, -2, 0, 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58};
}();

CSI CSI::fromQCA9300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, ChannelBandwidthEnum cbw) {
    auto csi = CSI{.deviceType = PicoScenesDeviceType::QCA9300, .packetFormat = PacketFormatEnum::PacketFormat_HT, .cbw = cbw, .dimensions = CSIDimension{.numTones = numTones, .numTx = numTx, .numRx = numRx}, .antSel = 0};
    csi.CSIArrays.resize(numTx * numRx * numLTF * numTones);
    csi.rawCSIData.resize(bufferLength);
    parseQCA9300CSIData(&csi.CSIArrays[0], buffer, numTx, numRx, numTones);
    std::copy(buffer, buffer + bufferLength, csi.rawCSIData.begin());
    if (csi.cbw == ChannelBandwidthEnum::CBW_20) {
        csi.subcarrierIndices = CSI::QCA9300SubcarrierIndices_CBW20;
    } else if (csi.cbw == ChannelBandwidthEnum::CBW_40) {
        csi.subcarrierIndices = CSI::QCA9300SubcarrierIndices_CBW40;
    }

    return csi;
}

CSI CSI::fromIWL5300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, ChannelBandwidthEnum cbw, uint8_t ant_sel) {
    auto csi = CSI{.deviceType = PicoScenesDeviceType::IWL5300, .packetFormat=PacketFormatEnum::PacketFormat_HT, .cbw = cbw, .dimensions = CSIDimension{.numTones = numTones, .numTx = numTx, .numRx = numRx, .numESS = numLTF}, .antSel = ant_sel};
    csi.CSIArrays.resize(numTx * numRx * numLTF * numTones);
    csi.rawCSIData.resize(bufferLength);
    parseIWL5300CSIData(&csi.CSIArrays[0], buffer, numTx, numRx, ant_sel);
    std::copy(buffer, buffer + bufferLength, csi.rawCSIData.begin());
    if (csi.cbw == ChannelBandwidthEnum::CBW_20) {
        csi.subcarrierIndices = CSI::IWL5300SubcarrierIndices_CBW20;
    } else if (csi.cbw == ChannelBandwidthEnum::CBW_40)
        csi.subcarrierIndices = CSI::IWL5300SubcarrierIndices_CBW40;

    return csi;
}

void CSI::interpolateCSI() {
    coder::array<creal_T, 2U> CSI;
    coder::array<creal_T, 2U> newCSI;
    coder::array<short, 1U> interpedIndex_int16;
    coder::array<short, 1U> subcarrierIndex_int16;

    CSI.set_size(dimensions.numTones, dimensions.numTx * dimensions.numRx);
    for (auto toneIndex = 0; toneIndex < CSI.size(0); toneIndex++) {
        for (auto txTrIndex = 0; txTrIndex < CSI.size(1); txTrIndex++) {
            auto pos = toneIndex + txTrIndex * CSI.size(0);
            CSI[pos] = *(creal_T *) (&CSIArrays[pos]);
        }
    }

    subcarrierIndex_int16.set_size(subcarrierIndices.size());
    for (auto scIndex = 0; scIndex < subcarrierIndex_int16.size(0); scIndex++) {
        subcarrierIndex_int16[scIndex] = subcarrierIndices[scIndex];
    }

    CSIPreprocessor(CSI, subcarrierIndex_int16, newCSI, interpedIndex_int16);

    CSIArrays.clear();
    CSIArrays.resize(newCSI.numel());
    std::copy((std::complex<double> *) newCSI.data(), (std::complex<double> *) newCSI.data() + newCSI.numel(), CSIArrays.begin());

    subcarrierIndices.clear();
    subcarrierIndices.resize(interpedIndex_int16.numel());
    std::copy((int16_t *) interpedIndex_int16.data(), (int16_t *) interpedIndex_int16.data() + interpedIndex_int16.numel(), subcarrierIndices.begin());
    dimensions.numTones = subcarrierIndices.size();
}

static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> std::vector<CSI> {
    uint32_t pos = 0;
    uint8_t numCSIGroup = *(uint8_t *) (buffer + pos++);
    std::vector<CSI> results;
    results.reserve(numCSIGroup);
    for (auto i = 0; i < numCSIGroup; i++) {
        PicoScenesDeviceType deviceType = *(PicoScenesDeviceType *) (buffer + pos);
        pos += sizeof(PicoScenesDeviceType);
        PacketFormatEnum packetFormat = *(PacketFormatEnum *) (buffer + pos);
        pos += sizeof(PacketFormatEnum);
        ChannelBandwidthEnum cbw = *(ChannelBandwidthEnum *) (buffer + pos);
        pos += sizeof(ChannelBandwidthEnum);
        uint16_t numTone = *(uint16_t *) (buffer + pos);
        pos += 2;
        uint8_t numSTS = *(uint8_t *) (buffer + pos++);
        uint8_t numRx = *(uint8_t *) (buffer + pos++);
        uint8_t numESS = *(uint8_t *) (buffer + pos++);
        uint8_t antSelByte = *(uint8_t *) (buffer + pos++);
        uint32_t CSIBufferLength = *(uint32_t *) (buffer + pos);
        pos += 4;

        if (deviceType == PicoScenesDeviceType::QCA9300) {
            results.emplace_back(CSI::fromQCA9300(buffer + pos, CSIBufferLength, numSTS, numRx, numSTS + numESS, numTone, cbw));
            continue;
        } else if (deviceType == PicoScenesDeviceType::IWL5300) {
            results.emplace_back(CSI::fromIWL5300(buffer + pos, CSIBufferLength, numSTS, numRx, numSTS + numESS, numTone, cbw, antSelByte));
            continue;
        } else if (deviceType == PicoScenesDeviceType::USRP) {
            //
        } else
            throw std::runtime_error("ExtraInfoSegment cannot decode the given buffer.");
    }

    return results;
};

std::map<uint16_t, std::function<std::vector<CSI>(const uint8_t *, uint32_t)>> CSISegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<std::vector<CSI>(const uint8_t *, uint32_t)>> CSISegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<std::vector<CSI>(const uint8_t *, uint32_t)>>{{0x1U, v1Parser}};
}

CSISegment::CSISegment() : AbstractPicoScenesFrameSegment("CSI", 0x1u) {

}

void CSISegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    auto[segmentName, segmentLength, versionId, offset] = extractSegmentMetaData(buffer, bufferLength);
    if (segmentName != "CSI")
        throw std::runtime_error("CSISegment cannot parse the segment named " + segmentName + ".");
    if (segmentLength + 4 > bufferLength)
        throw std::underflow_error("CSISegment cannot parse the segment with less than " + std::to_string(segmentLength + 4) + "B.");
    if (!versionedSolutionMap.contains(versionId)) {
        throw std::runtime_error("CSISegment cannot parse the segment with version v" + std::to_string(versionId) + ".");
    }

    auto muCSIs = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    csi = muCSIs[0];
    rawBuffer.resize(bufferLength);
    std::copy(buffer, buffer + bufferLength, rawBuffer.begin());
    this->segmentLength = bufferLength - 4;
    isSuccessfullyDecoded = true;
}

CSISegment CSISegment::createByBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    CSISegment csiSegment;
    csiSegment.fromBuffer(buffer, bufferLength);
    return csiSegment;
}

std::string CSISegment::toString() const {
    std::stringstream ss;
    ss << "CSISegments={";
        ss << "(device=" << csi.deviceType << ", format=" << csi.packetFormat << ", CBW=" << csi.cbw << ", dim(nTones,nSTS,nESS,nRx)=(" + std::to_string(csi.dimensions.numTones) + "," + std::to_string(csi.dimensions.numTx) + "," + std::to_string(csi.dimensions.numESS) + "," + std::to_string(csi.dimensions.numRx) + "), raw=" + std::to_string(csi.rawCSIData.size()) + "B)}";
    auto temp = ss.str();
    temp.erase(temp.end() - 2, temp.end());
    temp.append("}");
    return temp;
}

std::ostream &operator<<(std::ostream &os, const CSISegment &csiSegment) {
    os << csiSegment.toString();
    return os;
}