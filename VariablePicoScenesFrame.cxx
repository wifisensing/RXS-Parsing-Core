//
// Created by csi on 12/17/19.
//

#include "VariablePicoScenesFrame.hxx"


bool PicoScenesRxFrameStructure::isOldRXSEnhancedFrame(const uint8_t bufferHead[6]) {
    auto magicValue = *((uint32_t *) (bufferHead + 2));
    return (magicValue == 0x20150315 || magicValue == 0x20120930);
}

std::optional<PicoScenesFrameHeader> PicoScenesFrameHeader::fromBuffer(const uint8_t *buffer) {
    auto magicValue = *((uint32_t *) (buffer));
    if (magicValue == 0x20150315 || magicValue == 0x20120930) {
        auto frameHeader = *((PicoScenesFrameHeader *) (buffer));
        return frameHeader;
    }
    return std::nullopt;
}

std::optional<PicoScenesRxFrameStructure> PicoScenesRxFrameStructure::fromBuffer(const uint8_t *buffer, uint32_t start, enum RXSParsingLevel parsingLevel) {
    uint16_t totalLength = 2;
    uint16_t pos = start;
    PicoScenesRxFrameStructure rxFrame;
    totalLength += *((uint16_t *) (buffer + pos));
    pos += 2;

    rxFrame.rxs_basic = *((struct rx_status_basic *) (buffer + pos));
    pos += sizeof(struct rx_status_basic);
    if (rxFrame.rxs_basic.nrx <= 0 || rxFrame.rxs_basic.nrx > 3 || rxFrame.rxs_basic.ntx <= 0 || rxFrame.rxs_basic.ntx > 3) {
        printf("RXS Parser: Impossible values in nrx (%d), ntx (%d), or num_tones (%d). Error occurs in file format or parsing.\n", rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.ntx, rxFrame.rxs_basic.num_tones);
        return std::nullopt;
    }

    ExtraInfo::fromBinary(buffer + pos, &rxFrame.rxExtraInfo);
    pos += rxFrame.rxExtraInfo.length + 2;
    if (rxFrame.rxExtraInfo.hasEVM) {
        for (auto &evm : rxFrame.rxExtraInfo.evm) {
            evm += rxFrame.rxs_basic.noise;
            if (evm > 0) {
                evm = -128;
            }
        }
    }

    if (parsingLevel >= EXTRA_CSI) {
        if (rxFrame.deviceType == PicoScenesDeviceType::QCA9300)
            ar_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.csi.csi_matrix);
        else
            iwl_parse_csi_matrix(buffer + pos, rxFrame.rxs_basic.ntx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nltf, rxFrame.rxs_basic.num_tones, rxFrame.rxExtraInfo, rxFrame.csi.csi_matrix);

        // commit the following two lines to acquire raw csi matrix
        auto new_tones_num = phaseUnwrapAroundDC(rxFrame.csi.csi_matrix, rxFrame.csi.unwrappedMag, rxFrame.csi.unwrappedPhase, rxFrame.rxs_basic.nss / rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.nrx, rxFrame.rxs_basic.num_tones, rxFrame.rxs_basic.channelBonding);
        rxFrame.rxs_basic.num_tones = new_tones_num;
    }
    pos += rxFrame.rxs_basic.csi_len;

    rxFrame.standardHeader = *((ieee80211_mac_frame_header *) (buffer + pos));
    pos += sizeof(ieee80211_mac_frame_header);

    rxFrame.PicoScenesHeader = PicoScenesFrameHeader::fromBuffer(buffer + pos);
    pos += sizeof(PicoScenesFrameHeader);

    if (rxFrame.PicoScenesHeader) {
        rxFrame.frameSegmentStarts = std::vector<uint16_t>(rxFrame.PicoScenesHeader->segments);
        rxFrame.segmentMap = std::map<std::string, std::shared_ptr<uint8_t>>();
        char identifier[2];
        for (auto i = 0; i < rxFrame.PicoScenesHeader->segments; i++) {
            identifier[0] = *((char *) (buffer + pos));
            pos++;
            identifier[1] = *((char *) (buffer + pos));
            pos++;
            auto identifierString = std::string(identifier);

            if (identifierString == "EI") { // Tx ExtraInfo is a special case.
                ExtraInfo txExtraInfo{};
                ExtraInfo::fromBinary(buffer + pos, &txExtraInfo);
                rxFrame.txExtraInfo = txExtraInfo;
                pos += txExtraInfo.length + 2;
                continue;
            }

            auto segmentLength = *((uint16_t *) (buffer + pos));
            auto segmentBuffer = std::shared_ptr<uint8_t>(new uint8_t[segmentLength], std::default_delete<uint8_t[]>());
            memcpy(segmentBuffer.get(), buffer + pos, segmentLength);
            pos += segmentLength;
            rxFrame.segmentMap->emplace(std::make_pair(identifierString, segmentBuffer));
        }
    }

    return rxFrame;
}

void PicoScenesTxFrameStructure::addSegmentBuffer(const std::string &identifier, const uint8_t *buffer, uint16_t length) {
    auto bufferArray = std::array<uint8_t, 2048>();
    memcpy(bufferArray.data(), buffer, length);
    addSegmentBuffer(identifier, bufferArray, length);
    frameHeader.segments = segmentLength.size() + extraInfo ? 1 : 0;
}

void PicoScenesTxFrameStructure::addSegmentBuffer(const std::string &identifier, const std::array<uint8_t, 2048> &bufferArray, uint16_t length) {
    if (segmentLength.find(identifier) != segmentLength.end())
        throw std::runtime_error("Duplicated segment buffer identifier: " + identifier);

    segmentBuffer.emplace(std::make_pair(identifier, bufferArray));
    segmentLength.emplace(std::make_pair(identifier, length));
    frameHeader.segments = segmentLength.size() + extraInfo ? 1 : 0;
}

void PicoScenesTxFrameStructure::addExtraInfo(const ExtraInfo &txExtraInfo) {
    this->extraInfo = txExtraInfo;
    frameHeader.segments = segmentLength.size() + 1;
}

int PicoScenesTxFrameStructure::toBuffer(uint8_t *buffer) {
    uint16_t pos = 0;
    memcpy(buffer + pos, &standardHeader, sizeof(ieee80211_mac_frame_header));
    pos += sizeof(ieee80211_mac_frame_header);
    memcpy(buffer + pos, &frameHeader, sizeof(PicoScenesFrameHeader));
    if (extraInfo) {
        *((char *) (buffer + pos++)) = 'E';
        *((char *) (buffer + pos++)) = 'I';
        auto extraLength = extraInfo->toBinary(buffer + pos);
        pos += extraLength;
    }

    for (const auto &segmentPair : segmentLength) {
        *((char *) (buffer + pos++)) = segmentPair.first.c_str()[0];
        *((char *) (buffer + pos++)) = segmentPair.first.c_str()[1];
        pos += 2;
        memcpy(buffer + pos, segmentBuffer.at(segmentPair.first).data(), segmentPair.second);
        pos += segmentPair.second;
    }
    return pos;
}
