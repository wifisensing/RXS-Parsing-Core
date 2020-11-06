//
// Created by 蒋志平 on 2020/11/6.
//

#include "ModularPicoScenesFrame.hxx"
#include <boost/algorithm/string.hpp>

std::optional<ModularPicoScenesRxFrame> ModularPicoScenesRxFrame::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {
    uint32_t totalLength = 0, pos = 0;

    // first 4 bytes should be the buffer length
    if (*(uint32_t *) buffer + 4 == bufferLength) {
        totalLength = *(uint32_t *) buffer;
        pos += 4;
    } else {
        return {};
    }

    // next 4 bytes should be the magic number
    if (*(uint32_t *) (buffer + pos) != 0x20150315)
        return {};
    pos += 4;

    // next 2 bytes is the frame version
    uint16_t frameVersion = *(uint16_t *) (buffer + pos);
    if (frameVersion != 0x1U)
        return {};

    auto frame = ModularPicoScenesRxFrame();
    while (pos < bufferLength) {
        auto[segmentName, segmentLength, versionId, offset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(buffer + pos, bufferLength - pos);
        frame.rxSegmentMap.emplace(segmentName, Uint8Vector(buffer + pos, buffer + pos + segmentLength));
    }

    frame.standardHeader = *((ieee80211_mac_frame_header *) (buffer + pos));
    pos += sizeof(ieee80211_mac_frame_header);

    if (auto PSHeader = PicoScenesFrameHeader::fromBuffer(buffer + pos)) {
        frame.PicoScenesHeader = *PSHeader;
        pos += sizeof(PicoScenesFrameHeader);

        for (auto i = 0; i < frame.PicoScenesHeader->numSegments; i++) {
            auto[segmentName, segmentLength, versionId, offset] = AbstractPicoScenesFrameSegment::extractSegmentMetaData(buffer + pos, bufferLength);
            frame.txSegmentMap.emplace(segmentName, Uint8Vector(buffer + pos, buffer + pos + segmentLength));
        }
    } else {
        uint32_t msduLength = bufferLength - pos;
        auto msduBuffer = std::shared_ptr<uint8_t>(new uint8_t[msduLength], std::default_delete<uint8_t[]>());
        std::copy(buffer + pos, buffer + bufferLength, frame.nonPicoScenesMSDUContent.begin());
    }

    std::copy(buffer, buffer + pos, frame.rawBuffer.begin());

    return frame;
}
