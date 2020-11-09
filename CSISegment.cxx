//
// Created by 蒋志平 on 2020/11/5.
//

#include <algorithm>
#include <utility>
#include "CSISegment.hxx"


void parseQCA9300CSIData(std::complex<double> *outputArray, const uint8_t *csiData, int ntx, int nrx, int num_tones) {

    auto parse10bitsValues = [](const uint8_t *rawByte, int outputArray[4]) {
        static auto negativeSignBit = (1 << (10 - 1));
        static auto minNegativeValue = (1 << 10);
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
        outputArray[pos].real(tempArray[1]);
        outputArray[pos].imag(tempArray[0]);
    }
}

void parseIWL5300CSIData(std::complex<double> *csi_matrix, const uint8_t *payload, int ntx, int nrx, std::optional<Uint8Vector> ant_sel = std::nullopt) {

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

    uint32_t index = 0;
    uint8_t remainder;

    for (auto subcarrierIdx = 0; subcarrierIdx < 30; subcarrierIdx++) {
        index += 3;
        remainder = index % 8;

        for (auto nrxIdx = 0; nrxIdx < nrx; nrxIdx++)
            for (auto ntxIdx = 0; ntxIdx < ntx; ntxIdx++) {
                auto position = positionComputationWRTPermutation(nrx, 30, ntxIdx, nrxIdx, subcarrierIdx, ant_sel);
                char tmp1 = (payload[index / 8] >> remainder) | (payload[index / 8 + 1] << (8 - remainder));
                char tmp2 = (payload[index / 8 + 1] >> remainder) | (payload[index / 8 + 2] << (8 - remainder));
                csi_matrix[position].real((double) tmp1);
                csi_matrix[position].imag((double) tmp2);
                index += 16;
                //                printf("pos=%3d, subcarrierIdx=%2d, nrxIdx=%d, ntxIdx=%d, real=%f, imag=%f\n", position, subcarrierIdx, nrxIdx, ntxIdx, csi_matrix[position].real(), csi_matrix[position].imag());
            }
    }
}

CSI CSI::fromQCA9300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones) {
    auto csi = CSI{.deviceType = PicoScenesDeviceType::QCA9300, .packetFormat = PacketFormatEnum::PacketFormat_HT, .dimensions = CSIDimension{.numTones = numTones, .numTx = numTx, .numRx = numRx}};
    csi.CSIArrays.resize(numTx * numRx * numLTF * numTones);
    parseQCA9300CSIData(&csi.CSIArrays[0], buffer, numTx, numRx, numTones);
    std::copy(buffer, buffer + bufferLength, csi.rawCSIData.begin());

    return csi;
}

CSI CSI::fromIWL5300(const uint8_t *buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numLTF, uint8_t numTones, std::optional<Uint8Vector> ant_sel) {
    auto csi = CSI{.deviceType = PicoScenesDeviceType::IWL5300, .packetFormat=PacketFormatEnum::PacketFormat_HT, .dimensions = CSIDimension{.numTones = numTones, .numTx = numTx, .numRx = numRx, .numLTF = numLTF}};
    csi.CSIArrays.resize(numTx * numRx * numLTF * numTones);
    parseIWL5300CSIData(&csi.CSIArrays[0], buffer, numTx, numRx, std::move(ant_sel));
    std::copy(buffer, buffer + bufferLength, csi.rawCSIData.begin());

    return csi;
}


static auto v1Parser = [](const uint8_t *buffer, uint32_t bufferLength) -> CSI {
    uint32_t pos = 0;

    throw std::runtime_error("ExtraInfoSegment cannot decode the given buffer.");
};

std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> CSISegment::versionedSolutionMap = initializeSolutionMap();

std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> CSISegment::initializeSolutionMap() noexcept {
    return std::map<uint16_t, std::function<CSI(const uint8_t *, uint32_t)>> {{0x1U, v1Parser}};
}

CSISegment::CSISegment() : AbstractPicoScenesFrameSegment("CSI", 0x1u) {

}

void CSISegment::addCSI(const CSI &perUserCSI) {
    muCSI.emplace_back(perUserCSI);
}

void CSISegment::updateFieldMap() {
    fieldMap.clear();
    addField("numCSIGroup", Uint8Vector(1, fieldMap.size()));

    auto groupCount = 0;
    for (const auto &csi : muCSI) {
        addField("deviceType" + std::to_string(groupCount), csi.deviceType);
        addField("numTone" + std::to_string(groupCount), csi.dimensions.numTones);
        addField("numTx" + std::to_string(groupCount), csi.dimensions.numTx);
        addField("numRx" + std::to_string(groupCount), csi.dimensions.numRx);
        addField("numLTF" + std::to_string(groupCount), csi.dimensions.numLTF);
        addField("rawDataLength" + std::to_string(groupCount), csi.rawCSIData.size());
        addField("rawData" + std::to_string(groupCount), csi.rawCSIData);
        groupCount++;
    }

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

    auto csi = versionedSolutionMap.at(versionId)(buffer + offset, bufferLength - offset);
    addCSI(csi);
}
