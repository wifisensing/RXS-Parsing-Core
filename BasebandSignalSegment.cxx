//
// Created by 蒋志平 on 2020/11/6.
//

#include "BasebandSignalSegment.hxx"

BasebandSignalSegment::BasebandSignalSegment() : AbstractPicoScenesFrameSegment("BBSignal", 0x1U) {}

void BasebandSignalSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {

}
