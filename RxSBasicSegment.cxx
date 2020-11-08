//
// Created by 蒋志平 on 2020/11/6.
//

#include "RxSBasicSegment.hxx"

std::optional<RxSBasic> RxSBasic::fromBuffer(const uint8_t *buffer) {
    RxSBasic basic = *((RxSBasic *) (buffer));
//    if ((basic.deviceType == (uint16_t) PicoScenesDeviceType::QCA9300 || basic.deviceType == (uint16_t) PicoScenesDeviceType::USRP) && (basic.num_tones != 52 && basic.num_tones != 56 && basic.num_tones != 114)) {
//        printf("RxBasic(QCA9300 or SDR): Impossible values in nrx (%d), ntx (%d), or num_tones (%d).\n", basic.nrx, basic.ntx, basic.num_tones);
//        return std::nullopt;
//    } else if (basic.deviceType == (uint16_t) PicoScenesDeviceType::IWL5300 && basic.num_tones != 30) {
//        printf("RxBasic(IWL5300): Impossible values in nrx (%d), ntx (%d), or num_tones (%d).\n", basic.nrx, basic.ntx, basic.num_tones);
//        return std::nullopt;
//    }

    return basic;
}

std::string RxSBasic::toString() const {
    std::stringstream ss;
//    ss << "RxS:[device=" + DeviceType2String((PicoScenesDeviceType(deviceType))) + ", freq=" + std::to_string(channel) + ", CBW=" + ChannelBandwidth2String(cbw) + ", MCS=" + std::to_string(rate) + ", SGI=" + std::to_string(sgi) + ", CSI=" + std::to_string(csi_len) + "B(" + std::to_string(ntx) + ", " + std::to_string(nrx) + ", " + std::to_string(num_tones) + "), NLTF=" + std::to_string(nltf) + ", NGroup=" + std::to_string(ncsi_group) + ", timestamp=" + std::to_string(tstamp) + ", NF=" +
//          std::to_string(noise) + ", RSS=" + std::to_string(rssi) + "]";
    return ss.str();
}

RxSBasicSegment::RxSBasicSegment() : AbstractPicoScenesFrameSegment("RxSBasic", 0x1U) {}

void RxSBasicSegment::setRxsBasic(const RxSBasic &basicV) {
    basic = basicV;
}

void RxSBasicSegment::updateFieldMap() {
    fieldMap.clear();
    addField("basic", basic);
}

void RxSBasicSegment::fromBuffer(const uint8_t *string, uint32_t length) {

}
