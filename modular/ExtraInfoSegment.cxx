//
// Created by 蒋志平 on 2020/11/6.
//

#include "ExtraInfoSegment.hxx"


ExtraInfoSegment::ExtraInfoSegment() : AbstractPicoScenesFrameSegment("EI", 0x1U) {}

ExtraInfoSegment::ExtraInfoSegment(const ExtraInfo &extraInfo) : AbstractPicoScenesFrameSegment("EI", 0x1U), extraInfo(extraInfo) {}

void ExtraInfoSegment::updateFieldMap() {
    uint8_t array[500];
    memset(array, 0, sizeof(array));
    auto length = extraInfo.toBuffer(array);
    fieldMap.clear();
    addField("EI", array, length);
}

void ExtraInfoSegment::fromBuffer(const uint8_t *buffer, uint32_t bufferLength) {

}

