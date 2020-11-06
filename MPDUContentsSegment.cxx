//
// Created by 蒋志平 on 2020/11/6.
//

#include "MPDUContentsSegment.hxx"

MPDUContentsSegment::MPDUContentsSegment() : AbstractPicoScenesFrameSegment("MPDU", 0x1F), isAMPDU(false) {}

void MPDUContentsSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {

}
