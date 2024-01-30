//
// Created by Zhiping Jiang on 2020/11/6.
//

#include <random>
#include <cassert>
#include "ModularPicoScenesFrame.hxx"

std::string ieee80211_mac_frame_header_frame_control_field::getTypeString() const {
    if (type == 0) {
        switch (subtype) {
            case 0:
                return "[MF]Assoc. Req.";
            case 1:
                return "[MF]Assoc. Resp.";
            case 2:
                return "[MF]Re-assoc. Req.";
            case 3:
                return "[MF]Re-assoc. Resp.";
            case 4:
                return "[MF]Probe Req.";
            case 5:
                return "[MF]Probe Resp.";
            case 8:
                return "[MF]Beacon";
            case 9:
                return "[MF]ATIM";
            case 10:
                return "[MF]Disassoc.";
            case 11:
                return "[MF]Authen.";
            case 12:
                return "[MF]Deauthen.";
            case 13:
                return "[MF]Action";
            default:
                return "[MF]Reserved_" + std::to_string(subtype);
        }
    } else if (type == 1) {
        switch (subtype) {
            case 2:
                return "[CF]Trigger";
            case 5:
                return "[CF]VHT/HE NDP Ann.";
            case 7:
                return "[CF]Wrapper";
            case 8:
                return "[CF]BA Req.";
            case 9:
                return "[CF]BA";
            case 10:
                return "[CF]PS-Poll";
            case 11:
                return "[CF]RTS";
            case 12:
                return "[CF]CTS";
            case 13:
                return "[CF]ACK";
            case 14:
                return "[CF]CF-End";
            case 15:
                return "[CF]CF-End+CF-ACK";
            default:
                return "[CF]Reserved_" + std::to_string(subtype);
        }
    } else if (type == 2) {
        switch (subtype) {
            case 0:
                return "[DF]Data";
            case 1:
                return "[DF]Data+CF-ACK";
            case 2:
                return "[DF]Data+CF-Poll";
            case 3:
                return "[DF]Data-CF-ACK+CF-Poll";
            case 4:
                return "[DF]Null";
            case 5:
                return "[DF]CF-ACK";
            case 6:
                return "[DF]CF-Poll";
            case 7:
                return "[DF]CF-ACK+CF-Poll";
            case 8:
                return "[DF]QoS Data";
            case 9:
                return "[DF]QoS Data+CF-ACK";
            case 10:
                return "[DF]QoS Data+CF-Poll";
            case 11:
                return "[DF]QoS Data+CF-ACK+CF-Poll";
            case 12:
                return "[DF]QoS Null";
            case 13:
                return "[DF]Reserved13";
            case 14:
                return "[DF]QoS+CF-Poll(Null)";
            case 15:
                return "[DF]QoS+CF-ACK(Null)";
            default:
                return "[DF]Reserved_" + std::to_string(subtype);
        }
    }

    return "[" + std::to_string(type) + "]Reserved_" + std::to_string(subtype);
}


std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header_frame_control_field &fc) {
    os << fc.getTypeString();
    return os;
}

ieee80211_mac_frame_header ieee80211_mac_frame_header::createFromBuffer(const uint8_t *buffer, std::optional<uint32_t> bufferLength) {
    ieee80211_mac_frame_header result{};
    uint32_t pos = 0;
    result.fc = *reinterpret_cast<const ieee80211_mac_frame_header_frame_control_field *>(buffer + pos);

    if (result.fc.type == 0 || result.fc.type == 2 || result.fc.type == 3) { // Management Frames, Data Frames and all type=3 reserved
        result = *reinterpret_cast<const ieee80211_mac_frame_header *>(buffer);
        return result;
    }

    pos += sizeof(ieee80211_mac_frame_header_frame_control_field);
    result.dur = *reinterpret_cast<const uint16_t *>(buffer + pos);
    pos += 2;
    std::copy_n(buffer + pos, 6, result.addr1.begin());
    pos += 6;

    switch (result.fc.subtype) {
        case 2:
        case 8:
        case 9:
        case 10:
        case 11:
        case 15:
            std::copy_n(buffer + pos, 6, result.addr2.begin());
            break;
        default:
            break;
    }

    return result;
}

std::string ieee80211_mac_frame_header::toString() const {
    std::stringstream ss;
    ss << "MACHeader:[type=" << fc << ", ";
    ss << "dest=" << std::nouppercase << std::setfill('0') << std::setw(2) << std::right << std::hex << static_cast<int>(addr1[0]) << ":" << static_cast<int>(addr1[1]) << ":" << static_cast<int>(addr1[2]) << ":" << static_cast<int>(addr1[3]) << ":" << static_cast<int>(addr1[4]) << ":" << static_cast<int>(addr1[5]) << ", ";
    ss << "src=" << std::nouppercase << std::setfill('0') << std::setw(2) << std::right << std::hex << static_cast<int>(addr2[0]) << ":" << static_cast<int>(addr2[1]) << ":" << static_cast<int>(addr2[2]) << ":" << static_cast<int>(addr2[3]) << ":" << static_cast<int>(addr2[4]) << ":" << static_cast<int>(addr2[5]) << ", ";
    ss << "seq=" << std::dec << seq << ", frag=" << frag << ", ";
    ss << "mfrags=" << std::to_string(fc.moreFrags) << "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const ieee80211_mac_frame_header &header) {
    os << header.toString();
    return os;
}

std::optional<PicoScenesFrameHeader> PicoScenesFrameHeader::fromBuffer(const uint8_t *buffer) {
    if (const auto magicValue = *reinterpret_cast<const uint32_t *>(buffer); magicValue == 0x20150315) {
        auto frameHeader = *reinterpret_cast<const PicoScenesFrameHeader *>(buffer);
        return frameHeader;
    }
    return std::nullopt;
}

std::string PicoScenesFrameHeader::toString() const {
    std::stringstream ss;
    ss << "PSFHeader:[ver=0x" << std::hex << version << std::dec << ", device=" << deviceType << ", numSegs=" << static_cast<int>(numSegments) << ", type=" << static_cast<int>(frameType) << ", taskId=" << static_cast<int>(taskId) << ", txId=" << static_cast<int>(txId) << "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameHeader &frameHeader) {
    os << frameHeader.toString();
    return os;
}

auto ModularPicoScenesRxFrame::fromBuffer(const uint8_t *inputBuffer, const uint32_t bufferLength, const bool interpolateCSI) -> std::optional<ModularPicoScenesRxFrame> {
    uint32_t pos = 0;
    auto rxFrameHeader = *reinterpret_cast<const ModularPicoScenesRxFrameHeader *>(inputBuffer);
    if (rxFrameHeader.frameLength + 4 != bufferLength || rxFrameHeader.magicWord != 0x20150315) {
        return {};
    }

    if (rxFrameHeader.frameVersion == 1) { // for compatibility
        pos = sizeof(ModularPicoScenesRxFrameHeader) - sizeof(ModularPicoScenesRxFrameHeader::numMPDU);
        rxFrameHeader.numMPDU = 1;
    } else {
        pos += sizeof(ModularPicoScenesRxFrameHeader);
    }

    auto frame = ModularPicoScenesRxFrame();
    try {
        std::copy_n(inputBuffer, bufferLength, std::back_inserter(frame.rawBuffer));
        auto *buffer = frame.rawBuffer.data();
        frame.rxFrameHeader = rxFrameHeader;
        for (auto i = 0; i < frame.rxFrameHeader.numRxSegments; i++) {
            auto [segmentName, segmentLength, versionId, offset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(buffer + pos, bufferLength - pos);
            if (segmentName == "RxSBasic") {
                frame.rxSBasicSegment = std::make_shared<RxSBasicSegment>(buffer + pos, segmentLength + 4);
            } else if (segmentName == "ExtraInfo") {
                frame.rxExtraInfoSegment = std::make_shared<ExtraInfoSegment>(buffer + pos, segmentLength + 4);
            } else if (segmentName == "MVMExtra") {
                frame.mvmExtraSegment = std::make_shared<MVMExtraSegment>(buffer + pos, segmentLength + 4);
                std::copy(frame.mvmExtraSegment->getSyncedRawBuffer().cbegin(), frame.mvmExtraSegment->getSyncedRawBuffer().cend(), frame.rawBuffer.begin() + pos);
            } else if (segmentName == "SDRExtra") {
                frame.sdrExtraSegment = std::make_shared<SDRExtraSegment>(buffer + pos, segmentLength + 4);
            } else if (segmentName == "CSI") {
                frame.csiSegment = std::make_shared<CSISegment>(buffer + pos, segmentLength + 4);
                if (interpolateCSI) {
                    frame.csiSegment->getCSI()->removeCSDAndInterpolateCSI();
                }
            } else if (segmentName == "LegacyCSI") {
                frame.legacyCSISegment = std::make_shared<CSISegment>(buffer + pos, segmentLength + 4);
                if (interpolateCSI) {
                    frame.legacyCSISegment->getCSI()->removeCSDAndInterpolateCSI();
                }
            } else if (segmentName == "BasebandSignal") {
                frame.basebandSignalSegment = std::make_shared<BasebandSignalSegment>(buffer + pos, segmentLength + 4);
            } else if (segmentName == "PreEQSymbols" || segmentName == "PilotCSI" || segmentName == "RawCSI" || segmentName == "RawLegacyCSI") {
                // Do nothing for compatibility
//                frame.preEQSymbolsSegment = PreEQSymbolsSegment(buffer + pos, segmentLength + 4);
            } else {
                frame.rxUnknownSegments.emplace(segmentName, std::make_shared<AbstractPicoScenesFrameSegment>(buffer + pos, segmentLength + 4));
            }
            pos += (segmentLength + 4);
        }

        frame.mpdus.resize(frame.rxFrameHeader.numMPDU);
        for (auto mpduIndex = 0; mpduIndex < frame.rxFrameHeader.numMPDU; mpduIndex++) {
            uint32_t currentMPDUStart = frame.rxFrameHeader.frameVersion == 1 ? pos : pos + sizeof(uint32_t);
            uint32_t currentMPDULength = frame.rxFrameHeader.frameVersion == 1 ? (bufferLength - pos) : *reinterpret_cast<const uint32_t *>(buffer + pos); // length without the 4-byte of itself
            std::copy_n(buffer + currentMPDUStart, currentMPDULength, std::back_inserter(frame.mpdus[mpduIndex]));
            pos = currentMPDUStart + currentMPDULength;

            const auto *mpduBuffer = frame.mpdus[mpduIndex].data();
            if (mpduIndex == 0 && currentMPDULength >= sizeof(ieee80211_mac_frame_header)) {
                frame.standardHeader = ieee80211_mac_frame_header::createFromBuffer(mpduBuffer, currentMPDULength);
            }

            if (currentMPDULength >= sizeof(ieee80211_mac_frame_header) + sizeof(PicoScenesFrameHeader)) {
                if (auto PSHeader = PicoScenesFrameHeader::fromBuffer(frame.mpdus[mpduIndex].data() + sizeof(ieee80211_mac_frame_header))) {
                    frame.PicoScenesHeader = PSHeader;
                    uint32_t mpduPos = sizeof(ieee80211_mac_frame_header) + sizeof(PicoScenesFrameHeader);

                    for (auto segmentIndex = 0; segmentIndex < frame.PicoScenesHeader->numSegments; segmentIndex++) {
                        auto [segmentName, segmentLength, versionId, offset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(mpduBuffer + mpduPos, frame.mpdus[mpduIndex].size() - mpduPos);
                        if (segmentName == "ExtraInfo") {
                            frame.txExtraInfoSegment = std::make_shared<ExtraInfoSegment>(mpduBuffer + mpduPos, segmentLength + 4);
                        } else if (segmentName == "Payload") {
                            frame.payloadSegments.emplace_back(std::make_shared<PayloadSegment>(mpduBuffer + mpduPos, segmentLength + 4));
                        } else if (segmentName == "Cargo") {
                            frame.cargoSegments.emplace_back(std::make_shared<CargoSegment>(mpduBuffer + mpduPos, segmentLength + 4));
                        } else {
                            frame.txUnknownSegments.emplace(segmentName, std::make_shared<AbstractPicoScenesFrameSegment>(mpduBuffer + mpduPos, segmentLength + 4));
                        }
                        mpduPos += segmentLength + 4;
                    }
                }
            }
        }
    } catch (const std::exception &exception) {
        std::cout << "Error occurs during Rx frame parsing:" << exception.what() << ". Error skipped" << std::endl;
        return {};
    }

    return frame;
}

std::string ModularPicoScenesRxFrame::toString() const {
    std::stringstream ss;
    ss << "RxFrame:{";
    ss << *rxSBasicSegment;
    ss << ", Rx" << *rxExtraInfoSegment;
    if (mvmExtraSegment)
        ss << ", " << *mvmExtraSegment;
    if (sdrExtraSegment)
        ss << ", " << *sdrExtraSegment;
    ss << ", " << "(" << PacketFormat2String(csiSegment->getCSI()->packetFormat) << ")" << *csiSegment;
    if (legacyCSISegment)
        ss << ", " << *legacyCSISegment;
    if (basebandSignalSegment)
        ss << ", " << *basebandSignalSegment;
    if (!rxUnknownSegments.empty()) {
        std::stringstream segss;
        segss << "RxSegments:(";
        for (const auto &[name, segment]: rxUnknownSegments) {
            segss << name << ":" << segment->totalLengthIncludingLeading4ByteLength() << "B, ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }

    if (!isNDP)
        ss << ", " << standardHeader;
    else
        ss << ", NDP frame";
    if (PicoScenesHeader)
        ss << ", " << *PicoScenesHeader;
    if (txExtraInfoSegment)
        ss << ", Tx" << *txExtraInfoSegment;
    if (!payloadSegments.empty()) {
        std::stringstream segss;
        segss << "Payloads:(";
        for (const auto &segment: payloadSegments) {
            segss << *segment << ", ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }
    if (!cargoSegments.empty()) {
        ss << ", Cargos (" << cargoSegments.size()  << "): {";
        for (auto i = 0; i < cargoSegments.size() - 1; i++) {
            ss << *cargoSegments[i] << ", ";
        }
        ss << *cargoSegments.back() << "}";
    }
    if (!txUnknownSegments.empty()) {
        std::stringstream segss;
        segss << "TxSegments:(";
        for (const auto &[name, segment]: txUnknownSegments) {
            segss << *segment << ", ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }
    if (!isNDP)
        ss << ", MPDU:" << "[num=" << mpdus.size() << ", total=" << std::accumulate(mpdus.cbegin(), mpdus.cend(), 0, [](size_t acc, const auto &mpdu) {
            return acc + mpdu.size();
        }) << "B]";
    ss << "}";
    return ss.str();
}

std::optional<ModularPicoScenesRxFrame> ModularPicoScenesRxFrame::concatenateFragmentedPicoScenesRxFrames(const std::vector<ModularPicoScenesRxFrame> &frameQueue) {
    ModularPicoScenesRxFrame baseFrame = frameQueue.front();
    std::vector<std::shared_ptr<PayloadCargo>> cargos;
    for (const auto &frame: frameQueue)
        for (const auto &segment: frame.cargoSegments)
            cargos.emplace_back(segment->getCargo());
    auto mergedPayload = PayloadCargo::mergeAndValidateCargo(cargos);
    if (mergedPayload.empty()) // in case of decompression failure
        return std::nullopt;

    auto pos = 0, numSegment = 0;
    while (pos < mergedPayload.size()) {
        auto [segmentName, segmentLength, versionId, contentOffset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(mergedPayload.data() + pos, mergedPayload.size() - pos);
        if (segmentName == "ExtraInfo") {
            baseFrame.txExtraInfoSegment = std::make_shared<ExtraInfoSegment>(mergedPayload.data() + pos, segmentLength + 4);
        } else if (segmentName == "Payload") {
            baseFrame.payloadSegments.emplace_back(std::make_shared<PayloadSegment>(mergedPayload.data() + pos, segmentLength + 4));
        } else if (segmentName == "Cargo") {
            baseFrame.cargoSegments.emplace_back(std::make_shared<CargoSegment>(mergedPayload.data() + pos, segmentLength + 4));
        } else {
            baseFrame.txUnknownSegments.emplace(segmentName, std::make_shared<AbstractPicoScenesFrameSegment>(mergedPayload.data() + pos, segmentLength + 4));
        }
        pos += segmentLength + 4;
        numSegment++;
    }
    baseFrame.cargoSegments.clear();
    baseFrame.PicoScenesHeader->numSegments = numSegment;
    baseFrame.rebuildRawBuffer();

    return baseFrame;
}

Uint8Vector ModularPicoScenesRxFrame::toBuffer() const {
    if (!rawBuffer.empty())
        return rawBuffer;

    // Rx segments
    auto modularFrameHeader = ModularPicoScenesRxFrameHeader().initialize2Default();
    modularFrameHeader.numMPDU = mpdus.size();

    auto reservedLength = rxSBasicSegment ? rxSBasicSegment->totalLengthIncludingLeading4ByteLength() : 0;
    reservedLength += rxExtraInfoSegment ? rxExtraInfoSegment->totalLengthIncludingLeading4ByteLength() : 0;
    reservedLength += csiSegment ? csiSegment->totalLengthIncludingLeading4ByteLength() : 0;
    reservedLength += mvmExtraSegment ? mvmExtraSegment->totalLengthIncludingLeading4ByteLength() : 0;
    reservedLength += sdrExtraSegment ? sdrExtraSegment->totalLengthIncludingLeading4ByteLength() : 0;
    reservedLength += legacyCSISegment ? legacyCSISegment->totalLengthIncludingLeading4ByteLength() : 0;
    reservedLength += basebandSignalSegment ? basebandSignalSegment->totalLengthIncludingLeading4ByteLength() : 0;
    for (const auto &[segName, segment]: rxUnknownSegments) {
        reservedLength += segment->totalLengthIncludingLeading4ByteLength();
    }
    Uint8Vector rxSegmentBuffer;
    rxSegmentBuffer.reserve(reservedLength);

    if (rxSBasicSegment) {
        std::copy(rxSBasicSegment->getSyncedRawBuffer().cbegin(), rxSBasicSegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (rxExtraInfoSegment) {
        std::copy(rxExtraInfoSegment->getSyncedRawBuffer().cbegin(), rxExtraInfoSegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (csiSegment) {
        std::copy(csiSegment->getSyncedRawBuffer().cbegin(), csiSegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (mvmExtraSegment) {
        std::copy(mvmExtraSegment->getSyncedRawBuffer().cbegin(), mvmExtraSegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (sdrExtraSegment) {
        std::copy(sdrExtraSegment->getSyncedRawBuffer().cbegin(), sdrExtraSegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (legacyCSISegment) {
        std::copy(legacyCSISegment->getSyncedRawBuffer().cbegin(), legacyCSISegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (basebandSignalSegment) {
        std::copy(basebandSignalSegment->getSyncedRawBuffer().cbegin(), basebandSignalSegment->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    for (const auto &[name, segmentPtr]: rxUnknownSegments) {
        std::copy(segmentPtr->getSyncedRawBuffer().cbegin(), segmentPtr->getSyncedRawBuffer().cend(), std::back_inserter(rxSegmentBuffer));
        modularFrameHeader.numRxSegments++;
    }

    if (rxSegmentBuffer.size() != reservedLength) {
        throw std::runtime_error("RxSegmentBuffer reservation fails.");
    }

    // Assembly the full buffer
    Uint8Vector frameBuffer;
    modularFrameHeader.frameLength = sizeof(modularFrameHeader) + rxSegmentBuffer.size() + std::accumulate(mpdus.cbegin(), mpdus.cend(), 0, [](size_t acc, const auto &mpdu) {
        return acc + mpdu.size() + sizeof(uint32_t);
    }) - 4;
    frameBuffer.reserve(modularFrameHeader.frameLength + 4);
    std::copy_n(reinterpret_cast<const uint8_t *>(&modularFrameHeader), sizeof(ModularPicoScenesRxFrameHeader), std::back_inserter(frameBuffer));
    std::copy_n(rxSegmentBuffer.data(), rxSegmentBuffer.size(), std::back_inserter(frameBuffer));
    for (const auto &mpdu: mpdus) {
        uint32_t mpduSize = mpdu.size();
        std::copy_n(reinterpret_cast<const uint8_t *>(&mpduSize), sizeof(uint32_t), std::back_inserter(frameBuffer));
        std::copy_n(mpdu.data(), mpdu.size(), std::back_inserter(frameBuffer));
    }

    //// for in-situ validation
//    {
//        auto recovered = ModularPicoScenesRxFrame::fromBuffer(frameBuffer.data(), frameBuffer.size());
//        std::cout << "recovered:" << *recovered << std::endl;
//    }

    return frameBuffer;
}

void ModularPicoScenesRxFrame::rebuildRawBuffer() {
    rawBuffer.clear();

    if (PicoScenesHeader) {
        mpdus.assign(1, Uint8Vector{});

        std::copy_n(reinterpret_cast<const uint8_t *>(&standardHeader), sizeof(standardHeader), std::back_inserter(mpdus[0]));
        std::copy_n(reinterpret_cast<const uint8_t *>(&PicoScenesHeader.value()), sizeof(PicoScenesFrameHeader), std::back_inserter(mpdus[0]));

        if (txExtraInfoSegment) {
            std::copy(txExtraInfoSegment->getSyncedRawBuffer().cbegin(), txExtraInfoSegment->getSyncedRawBuffer().cend(), std::back_inserter(mpdus[0]));
        }
        for (auto &payloadSegment: payloadSegments) {
            std::copy(payloadSegment->getSyncedRawBuffer().cbegin(), payloadSegment->getSyncedRawBuffer().cend(), std::back_inserter(mpdus[0]));
        }

        for (auto &cargo: cargoSegments) {
            std::copy(cargo->getSyncedRawBuffer().cbegin(), cargo->getSyncedRawBuffer().cend(), std::back_inserter(mpdus[0]));
        }

        for (auto &[name, segmentPtr]: txUnknownSegments) {
            std::copy(segmentPtr->getSyncedRawBuffer().cbegin(), segmentPtr->getSyncedRawBuffer().cend(), std::back_inserter(mpdus[0]));
        }
    }

    rawBuffer = toBuffer();
}

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesRxFrame &rxframe) {
    os << rxframe.toString();
    return os;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::addSegment(const std::shared_ptr<AbstractPicoScenesFrameSegment> &segment) {
    segments.emplace_back(segment);
    if (!frameHeader)
        frameHeader = PicoScenesFrameHeader();
    frameHeader->numSegments = segments.size();
    return *this;
}

std::shared_ptr<AbstractPicoScenesFrameSegment> ModularPicoScenesTxFrame::getSegment(const std::string &querySegmentName) {
    auto resultIt = std::find_if(segments.begin(), segments.end(), [=](const auto &eachSegment) {
        return eachSegment->segmentName == querySegmentName;
    });

    return resultIt == segments.end() ? nullptr : *resultIt;
}

std::vector<Uint8Vector> ModularPicoScenesTxFrame::toBuffer() const {
    if (txParameters.NDPFrame)
        return {};

    // If arbitraryAMPDUContent is not empty, return the arbitraryAMPDUContent
    if (arbitraryAMPDUContent && !arbitraryAMPDUContent->empty() && !arbitraryAMPDUContent.value()[0].empty()) {
        return *arbitraryAMPDUContent;
    }

    // Else in PicoScenes Segment-based Tx frame style
    {
        std::vector<Uint8Vector> ampduOutput{};

        // use a pointer vector to flatten the AMPDU hierarchy
        std::vector framePtrs{this};
        for(const auto & frame : additionalAMPDUFrames) {
            framePtrs.emplace_back(&frame);
        }

        std::transform(framePtrs.cbegin(), framePtrs.cend(), std::back_inserter(ampduOutput), [&] (const auto * framePtr) -> Uint8Vector {
            auto mpduContent = Uint8Vector{};
            std::copy_n(reinterpret_cast<const uint8_t *>(&framePtr->standardHeader), sizeof(ieee80211_mac_frame_header), std::back_inserter(mpduContent));
            if (frameHeader) {
                if (frameHeader->numSegments != segments.size())
                    throw std::overflow_error("ModularPicoScenesTxFrame toBuffer method segment number in-consistent!");
                std::copy_n(reinterpret_cast<const uint8_t *>(&frameHeader), sizeof(PicoScenesFrameHeader), std::back_inserter(mpduContent));
                for(const auto &segment: framePtr->segments) {
                    std::copy(segment->getSyncedRawBuffer().cbegin(), segment->getSyncedRawBuffer().cend(), std::back_inserter(mpduContent));
                }
            }

            return mpduContent;
        });

        return ampduOutput;
    }
}

std::vector<ModularPicoScenesTxFrame> ModularPicoScenesTxFrame::autoSplit(const int16_t maxSegmentBuffersLength, const std::optional<uint16_t> firstSegmentCappingLength, std::optional<uint16_t> maxNumMPDUInAMPDU) const {
    if (!frameHeader)
        return std::vector<ModularPicoScenesTxFrame>{1, *this};

    auto segmentsLength = 0;
    for (const auto &segment: segments) {
        segmentsLength += segment->totalLengthIncludingLeading4ByteLength();
    }

    if (segmentsLength < maxSegmentBuffersLength)
        return std::vector<ModularPicoScenesTxFrame>{1, *this};

    auto pos = 0;
    Uint8Vector allSegmentBuffer(segmentsLength), compressedBuffer;
    for (const auto &segment: segments) {
        segment->toBuffer(allSegmentBuffer.data() + pos);
        pos += segment->totalLengthIncludingLeading4ByteLength();
    }

    Uint8Vector *bufferPtr;
    size_t bufferLength{0};
    bool usingCompression{false};
    if (CargoCompression::isAlgorithmRegistered()) {
        compressedBuffer = CargoCompression::getCompressor()(allSegmentBuffer.data(), allSegmentBuffer.size()).value_or(Uint8Vector());
        bufferLength = compressedBuffer.size();
        bufferPtr = &compressedBuffer;
        usingCompression = true;

//        auto decompressedPayload = CargoCompression::getDecompressor()(compressedBuffer.data(), compressedBuffer.size()).value_or(Uint8Vector());
//        std::cout << "identical:" << (decompressedPayload == allSegmentBuffer) << std::endl;
    } else {
        bufferPtr = &allSegmentBuffer;
        bufferLength = allSegmentBuffer.size();
    }

    uint8_t numCargos = std::ceil(1.0 * bufferLength / maxSegmentBuffersLength);
    if (firstSegmentCappingLength) {
        auto remainLength = bufferLength - firstSegmentCappingLength.value();
        numCargos = std::ceil(1.0 * remainLength / maxSegmentBuffersLength) + 1;
    }
    const auto avgStepLength = size_t(std::ceil(1.0 * bufferLength / numCargos));
    auto cargos = std::vector<std::shared_ptr<PayloadCargo>>();

    pos = 0;
    uint8_t sequence = 0;
    while (pos < bufferLength) {
        auto stepLength = pos + avgStepLength < bufferLength ? avgStepLength : bufferLength - pos;
        if (pos == 0 && firstSegmentCappingLength)
            stepLength = pos + *firstSegmentCappingLength < bufferLength ? *firstSegmentCappingLength : bufferLength - pos;
        cargos.emplace_back(std::make_shared<PayloadCargo>(PayloadCargo{
            .taskId = frameHeader->taskId,
            .numSegments = frameHeader->numSegments,
            .sequence = sequence++,
            .totalParts = numCargos,
            .compressed = usingCompression,
            .payloadLength = static_cast<uint32_t>(bufferLength),
            .payloadData = Uint8Vector(bufferPtr->data() + pos, bufferPtr->data() + pos + stepLength)
        }));
        pos += stepLength;
    }

    auto cargoFrames = std::vector<ModularPicoScenesTxFrame>();
    for (auto i = 0; i < cargos.size(); i++) {
        const auto &cargo = cargos[i];
        auto txframe = *this;
        txframe.standardHeader.seq = i;
        txframe.segments.clear();
        txframe.txParameters.postfixPaddingTime = 2e-3;
        txframe.frameHeader = this->frameHeader;
        txframe.addSegment(std::make_shared<CargoSegment>(cargo));
        cargoFrames.emplace_back(txframe);
    }

    if (!txParameters.preferAMPDU)
        return cargoFrames;

    if (!maxNumMPDUInAMPDU)
        maxNumMPDUInAMPDU = 3; // default value

    std::vector<ModularPicoScenesTxFrame> ampduFrames;
    for (auto baseIndex = 0; baseIndex < cargoFrames.size(); baseIndex += *maxNumMPDUInAMPDU) {
        for (auto i = 1; i < *maxNumMPDUInAMPDU; i++) {
            if (baseIndex + i < cargoFrames.size())
                cargoFrames[baseIndex].appendAMPDUFrame(cargoFrames[baseIndex + i]);
        }
        ampduFrames.emplace_back(cargoFrames[baseIndex]);
    }

    return ampduFrames;
}

void ModularPicoScenesTxFrame::reset() {
    standardHeader = ieee80211_mac_frame_header();
    frameHeader = PicoScenesFrameHeader();
    txParameters = PicoScenesFrameTxParameters();
    arbitraryAMPDUContent = std::nullopt;
    additionalAMPDUFrames.clear();
    prebuiltSignals.clear();
    segments.clear();
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setSequenceId(const uint16_t sequence) {
    standardHeader.seq = sequence;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setMoreFrags() {
    standardHeader.fc.moreFrags = 1;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setFragNumber(const uint8_t fragNumber) {
    standardHeader.frag = fragNumber;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setRetry() {
    standardHeader.fc.retry = 1;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setDeviceType(const PicoScenesDeviceType deviceType) {
    if (!frameHeader)
        frameHeader = PicoScenesFrameHeader();
    frameHeader->deviceType = deviceType;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setTaskId(const uint16_t taskId) {
    if (!frameHeader)
        frameHeader = PicoScenesFrameHeader();
    frameHeader->taskId = taskId;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setTxId(const uint16_t txId) {
    if (!frameHeader)
        frameHeader = PicoScenesFrameHeader();
    frameHeader->txId = txId;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setRandomTaskId() {
    static std::random_device r;
    static std::default_random_engine randomEngine(r());
    static std::uniform_int_distribution<uint16_t> randomGenerator(10000, UINT16_MAX);
    const auto newValue = randomGenerator(randomEngine);
    setTaskId(newValue);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setPicoScenesFrameType(const uint8_t frameType) {
    if (!frameHeader)
        frameHeader = PicoScenesFrameHeader();
    frameHeader->frameType = frameType;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setTxParameters(const PicoScenesFrameTxParameters &parameters) {
    txParameters = parameters;
    return *this;
}

[[maybe_unused]] ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setFrameFormat(const PacketFormatEnum format) {
    txParameters.frameType = format;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setChannelBandwidth(const ChannelBandwidthEnum &cbw) {
    txParameters.cbw = cbw;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setGuardInterval(const GuardIntervalEnum guardInterval) {
    txParameters.guardInterval = guardInterval;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setMCS(uint8_t mcs) {
    txParameters.mcs.clear();
    txParameters.mcs.emplace_back(mcs);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setNumSTS(uint8_t numSTS) {
    txParameters.numSTS.clear();
    txParameters.numSTS.emplace_back(numSTS);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setNumSTS(const std::vector<uint8_t> &numSTSs) {
    txParameters.numSTS = numSTSs;
    return *this;
}


[[maybe_unused]] ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setMCS(const std::vector<uint8_t> &mcs) {
    txParameters.mcs = mcs;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setDestinationAddress(const uint8_t macAddr[6]) {
    std::copy_n(macAddr, 6, standardHeader.addr1.begin());
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setSourceAddress(const uint8_t macAddr[6]) {
    std::copy_n(macAddr, 6, standardHeader.addr2.begin());
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::set3rdAddress(const uint8_t macAddr[6]) {
    std::copy_n(macAddr, 6, standardHeader.addr3.begin());
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setForceSounding(const bool forceSounding) {
    txParameters.forceSounding = forceSounding;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setNumExtraSounding(const uint8_t numExtraSounding) {
    txParameters.numExtraSounding = numExtraSounding;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setChannelCoding(ChannelCodingEnum codings) {
    txParameters.coding.clear();
    txParameters.coding.emplace_back(codings);
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setChannelCoding(const std::vector<ChannelCodingEnum> &codings) {
    txParameters.coding = codings;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setTxHEExtendedRange(const bool txHEExtendedRange) {
    txParameters.txHEExtendedRange = txHEExtendedRange;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setHEHighDoppler(const bool heHighDoppler) {
    txParameters.heHighDoppler = heHighDoppler;
    return *this;
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::setHEMidamblePeriodicity(const uint8_t heMidamblePeriodicity) {
    txParameters.heMidamblePeriodicity = heMidamblePeriodicity;
    return *this;
}

std::string ModularPicoScenesTxFrame::toString() const {
    std::stringstream ss;
    if (!txParameters.NDPFrame) {
        ss << "TxFrame:{" << standardHeader;
        if (frameHeader)
            ss << ", " << *frameHeader;
    } else {
        ss << "TxFrame:{NDP frame";
    }
    ss << ", " << txParameters;

    if (arbitraryAMPDUContent && !arbitraryAMPDUContent->empty() && !arbitraryAMPDUContent.value()[0].empty()) {
        ss << ", ArbitraryAMPDU:" << std::to_string(arbitraryAMPDUContent->size()) << "Pkts}";
        return ss.str();
    }

    if (!additionalAMPDUFrames.empty()) {
        ss << ", AMPDU (incl. self):" << std::to_string(additionalAMPDUFrames.size() + 1) << " Pkts}";
        return ss.str();
    }

    if (!segments.empty()) {
        std::stringstream segss;
        segss << "Segments:(";
        for (const auto &segment: segments) {
            segss << *segment << ", ";
        }
        auto temp = segss.str();
        temp.erase(temp.end() - 2, temp.end());
        temp.append(")");
        ss << ", " << temp;
    }
    ss << "}";
    return ss.str();
}

ModularPicoScenesTxFrame &ModularPicoScenesTxFrame::appendAMPDUFrame(const ModularPicoScenesTxFrame &frame) {
    additionalAMPDUFrames.emplace_back(frame);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const ModularPicoScenesTxFrame &txframe) {
    os << txframe.toString();
    return os;
}