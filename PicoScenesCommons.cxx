//
// Created by 蒋志平 on 2020/11/6.
//

#include "PicoScenesCommons.hxx"

bool isIntelMVMTypeNIC(PicoScenesDeviceType psdt) {
    return psdt == PicoScenesDeviceType::IWLMVM_AX200 || psdt == PicoScenesDeviceType::IWLMVM_AX210;
}

bool isCOTSNIC(PicoScenesDeviceType psdt) {
    return isIntelMVMTypeNIC(psdt) || psdt == PicoScenesDeviceType::QCA9300 || psdt == PicoScenesDeviceType::IWL5300 || psdt == PicoScenesDeviceType::MAC80211Compatible;
}

bool isSDR(PicoScenesDeviceType psdt) {
    return psdt == PicoScenesDeviceType::USRP || psdt == PicoScenesDeviceType::SoapySDR;
}

std::string DeviceType2String(PicoScenesDeviceType type) {
    switch (type) {
        case PicoScenesDeviceType::QCA9300:
            return "QCA9300";
        case PicoScenesDeviceType::IWL5300:
            return "IWL5300";
        case PicoScenesDeviceType::IWLMVM_AX200:
            return "AX200";
        case PicoScenesDeviceType::IWLMVM_AX210:
            return "AX210";
        case PicoScenesDeviceType::MAC80211Compatible:
            return "MAC80211 Compatible NIC";
        case PicoScenesDeviceType::USRP:
            return "USRP(SDR)";
        case PicoScenesDeviceType::VirtualSDR:
            return "Virtual(SDR)";
        case PicoScenesDeviceType::SoapySDR:
            return "SoapySDR Compatible SDR";
        case PicoScenesDeviceType::Unknown:
            return "Unknown";
        default:
            throw std::runtime_error("unrecognized PicoScenesDeviceType.");
    }
}

std::string DeviceSubtype2String(PicoScenesDeviceSubtype subtype) {
    switch (subtype) {
        case PicoScenesDeviceSubtype::Unknown:
            return "Unknown";
        case PicoScenesDeviceSubtype::QCA9300:
            return "QCA9300";
        case PicoScenesDeviceSubtype::IWL5300:
            return "IWL5300";
        case PicoScenesDeviceSubtype::AX200:
            return "AX200";
        case PicoScenesDeviceSubtype::AX210:
            return "AX210";
        case PicoScenesDeviceSubtype::AX211:
            return "AX211";
        case PicoScenesDeviceSubtype::USRP_N2x0:
            return "USRP N2x0";
        case PicoScenesDeviceSubtype::USRP_B2x0:
            return "USRP B2x0";
        case PicoScenesDeviceSubtype::USRP_E3x0:
            return "USRP E3x0";
        case PicoScenesDeviceSubtype::USRP_N3x0:
            return "USRP N3x0";
        case PicoScenesDeviceSubtype::USRP_X3x0:
            return "USRP X3x0";
        case PicoScenesDeviceSubtype::USRP_X4x0:
            return "USRP X4x0";
        case PicoScenesDeviceSubtype::HackRFOne:
            return "HackRF One";
        case PicoScenesDeviceSubtype::LimeSDR:
            return "LimeSDR";
        case PicoScenesDeviceSubtype::BladeRF:
            return "BladeRF";
        case PicoScenesDeviceSubtype::SOAPYSDR_UHD:
            return "SoapySDR_UHD";
        default:
            throw std::runtime_error("unrecognized PicoScenesDeviceSubtype.");
    }
}

std::string PacketFormat2String(PacketFormatEnum format) {
    switch (format) {
        case PacketFormatEnum::PacketFormat_NonHT:
            return "NonHT";
        case PacketFormatEnum::PacketFormat_HT:
            return "HT";
        case PacketFormatEnum::PacketFormat_VHT:
            return "VHT";
        case PacketFormatEnum::PacketFormat_HESU:
            return "HE-SU";
        case PacketFormatEnum::PacketFormat_HEMU:
            return "HE-MU";
        case PacketFormatEnum::PacketFormat_HETB:
            return "HE-TB";
        case PacketFormatEnum::PacketFormat_EHTMU:
            return "EHT-MU";
        case PacketFormatEnum::PacketFormat_EHTTB:
            return "EHT-TB";
        case PacketFormatEnum::PacketFormat_Unknown:
            return "Unknown";
    }

    throw std::runtime_error("Unsupported packet format.");
}

std::string ChannelBandwidth2String(ChannelBandwidthEnum cbw) {
    switch (cbw) {
        case ChannelBandwidthEnum::CBW_20:
            return "20";
        case ChannelBandwidthEnum::CBW_40:
            return "40";
        case ChannelBandwidthEnum::CBW_80:
            return "80";
        case ChannelBandwidthEnum::CBW_160:
            return "160";
        case ChannelBandwidthEnum::CBW_320:
            return "320";
        default:
            return "Unknown Channel Bandwidth (CBW)";
    }
}

std::string channelModel2String(ChannelModeEnum mode) {
    switch (mode) {
        case ChannelModeEnum::HT40_PLUS:
            return "HT40_PLUS";
        case ChannelModeEnum::HT40_MINUS:
            return "HT40_MINUS";
        case ChannelModeEnum::HT20:
            return "HT20";
    }
    return "channel mode error.";
}

std::string GuardInterval2String(GuardIntervalEnum gi) {
    switch (gi) {
        case GuardIntervalEnum::GI_400:
            return "0.4us";
        case GuardIntervalEnum::GI_800:
            return "0.8us";
        case GuardIntervalEnum::GI_1600:
            return "1.6us";
        case GuardIntervalEnum::GI_3200:
            return "3.2us";
        default:
            throw std::runtime_error("Unsupported GuardIntervalEnum...");
    }
}

std::string ChannelCoding2String(ChannelCodingEnum coding) {
    switch (coding) {
        case ChannelCodingEnum::LDPC:
            return "LDPC";
        case ChannelCodingEnum::BCC:
            return "BCC";
        default:
            return "Unknown";
    }
}

std::ostream &operator<<(std::ostream &os, const PicoScenesDeviceType &deviceType) {
    os << DeviceType2String(deviceType);
    return os;
}

std::ostream &operator<<(std::ostream &os, const PacketFormatEnum &format) {
    os << PacketFormat2String(format);
    return os;
}

std::ostream &operator<<(std::ostream &os, const ChannelBandwidthEnum &cbw) {
    os << ChannelBandwidth2String(cbw);
    return os;
}

std::ostream &operator<<(std::ostream &os, const ChannelModeEnum &channelMode) {
    os << channelModel2String(channelMode);
    return os;
}

std::ostream &operator<<(std::ostream &os, const GuardIntervalEnum &gi) {
    os << GuardInterval2String(gi);
    return os;
}

std::ostream &operator<<(std::ostream &os, const ChannelCodingEnum &coding) {
    os << ChannelCoding2String(coding);
    return os;
}

std::ostream &operator<<(std::ostream &os, const PicoScenesDeviceSubtype &deviceSubtype) {
    os << DeviceSubtype2String(deviceSubtype);
    return os;
}

std::string FrontEndModePreset2String(FrontEndModePreset preset) {
    switch (preset) {
        case FrontEndModePreset::TX_CBW_160_HESU:
            return "TX_CBW_160_HESU";
        case FrontEndModePreset::TX_CBW_160_VHT:
            return "TX_CBW_160_VHT";
        case FrontEndModePreset::TX_CBW_160_VHT_LDPC:
            return "TX_CBW_160_VHT_LDPC";
        case FrontEndModePreset::TX_CBW_80_HESU:
            return "TX_CBW_80_HESU";
        case FrontEndModePreset::TX_CBW_80_VHT:
            return "TX_CBW_80_VHT";
        case FrontEndModePreset::TX_CBW_80_VHT_LDPC:
            return "TX_CBW_80_VHT_LDPC";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_HESU:
            return "TX_CBW_40_RESAMPLE_HESU";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT:
            return "TX_CBW_40_RESAMPLE_VHT";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT_LDPC:
            return "TX_CBW_40_RESAMPLE_VHT_LDPC";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT_LDPC:
            return "TX_CBW_40_RESAMPLE_HT_LDPC";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT:
            return "TX_CBW_40_RESAMPLE_HT";
        case FrontEndModePreset::TX_CBW_40_HESU:
            return "TX_CBW_40_HESU";
        case FrontEndModePreset::TX_CBW_40_VHT:
            return "TX_CBW_40_VHT";
        case FrontEndModePreset::TX_CBW_40_VHT_LDPC:
            return "TX_CBW_40_VHT_LDPC";
        case FrontEndModePreset::TX_CBW_40_HT:
            return "TX_CBW_40_HT";
        case FrontEndModePreset::TX_CBW_40_HT_LDPC:
            return "TX_CBW_40_HT_LDPC";
        case FrontEndModePreset::RX_CBW_160:
            return "RX_CBW_160";
        case FrontEndModePreset::RX_CBW_80:
            return "RX_CBW_80";
        case FrontEndModePreset::RX_CBW_40_RESAMPLE:
            return "RX_CBW_40_RESAMPLE";
        case FrontEndModePreset::RX_CBW_40:
            return "RX_CBW_40";
        case FrontEndModePreset::TR_CBW_160_HESU:
            return "TR_CBW_160_HESU";
        case FrontEndModePreset::TR_CBW_160_VHT:
            return "TR_CBW_160_VHT";
        case FrontEndModePreset::TR_CBW_160_VHT_LDPC:
            return "TR_CBW_160_VHT_LDPC";
        case FrontEndModePreset::TR_CBW_80_HESU:
            return "TR_CBW_80_HESU";
        case FrontEndModePreset::TR_CBW_80_VHT:
            return "TR_CBW_80_VHT";
        case FrontEndModePreset::TR_CBW_80_VHT_LDPC:
            return "TR_CBW_80_VHT_LDPC";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_HESU:
            return "TR_CBW_40_RESAMPLE_HESU";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT:
            return "TR_CBW_40_RESAMPLE_VHT";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT_LDPC:
            return "TR_CBW_40_RESAMPLE_VHT_LDPC";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_HT:
            return "TR_CBW_40_RESAMPLE_HT";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_HT_LDPC:
            return "TR_CBW_40_RESAMPLE_HT_LDPC";
        case FrontEndModePreset::TR_CBW_40_HESU:
            return "TR_CBW_40_HESU";
        case FrontEndModePreset::TR_CBW_40_VHT:
            return "TR_CBW_40_VHT";
        case FrontEndModePreset::TR_CBW_40_VHT_LDPC:
            return "TR_CBW_40_VHT_LDPC";
        case FrontEndModePreset::TR_CBW_40_HT:
            return "TR_CBW_40_HT";
        case FrontEndModePreset::TR_CBW_40_HT_LDPC:
            return "TR_CBW_40_HT_LDPC";
        default:
            throw std::invalid_argument("Unknown Tx parameter reset");
    }
}

std::string FrontEndModePreset2DescriptiveString(FrontEndModePreset preset) {
    switch (preset) {
        case FrontEndModePreset::TX_CBW_160_HESU:
            return FrontEndModePreset2String(preset) + ": [Tx, 160 MHz CBW, 200 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HESU format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_160_VHT:
            return FrontEndModePreset2String(preset) + ": [Tx, 160 MHz CBW, 200 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, and BCC coding]";
        case FrontEndModePreset::TX_CBW_160_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [Tx, 160 MHz CBW, 200 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_80_HESU:
            return FrontEndModePreset2String(preset) + ": [Tx, 80 MHz CBW, 100 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HESU format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_80_VHT:
            return FrontEndModePreset2String(preset) + ": [Tx, 80 MHz CBW, 100 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, and BCC coding]";
        case FrontEndModePreset::TX_CBW_80_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [Tx, 80 MHz CBW, 100 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_HESU:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HESU format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, and BCC coding]";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HT format, and BCC coding]";
        case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT_LDPC:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HT format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_40_HESU:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, HESU format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_40_VHT:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, VHT format, and BCC coding]";
        case FrontEndModePreset::TX_CBW_40_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, VHT format, and LDPC coding]";
        case FrontEndModePreset::TX_CBW_40_HT:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, HT format, and BCC coding]";
        case FrontEndModePreset::TX_CBW_40_HT_LDPC:
            return FrontEndModePreset2String(preset) + ": [Tx, 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, HT format, and LDPC coding]";
        case FrontEndModePreset::RX_CBW_160:
            return FrontEndModePreset2String(preset) + ": [Rx, 160 MHz Rx CBW, 200 MHz actual Rx sampling rate, 0.8 Rx resample ratio]";
        case FrontEndModePreset::RX_CBW_80:
            return FrontEndModePreset2String(preset) + ": [Rx, 80 MHz Rx CBW, 100 MHz actual Rx sampling rate, 0.8 Rx resample ratio]";
        case FrontEndModePreset::RX_CBW_40_RESAMPLE:
            return FrontEndModePreset2String(preset) + ": [Rx, 40 MHz Rx CBW, 50 MHz actual Rx sampling rate, 0.8 Rx resample ratio]";
        case FrontEndModePreset::RX_CBW_40:
            return FrontEndModePreset2String(preset) + ": [Rx, 40 MHz Rx CBW, 40 MHz actual Rx sampling rate, 1.0 Rx resample ratio]";
        case FrontEndModePreset::TR_CBW_160_HESU:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_160_HESU + RX_CBW_160]";
        case FrontEndModePreset::TR_CBW_160_VHT:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_160_VHT + RX_CBW_160]";
        case FrontEndModePreset::TR_CBW_160_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_160_VHT_LDPC + RX_CBW_160]";
        case FrontEndModePreset::TR_CBW_80_HESU:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_80_HESU + RX_CBW_80]";
        case FrontEndModePreset::TR_CBW_80_VHT:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_80_VHT + RX_CBW_80]";
        case FrontEndModePreset::TR_CBW_80_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_80_VHT_LDPC + RX_CBW_80]";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_HESU:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_RESAMPLE_HESU + RX_CBW_40_RESAMPLE]";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_RESAMPLE_VHT + RX_CBW_40_RESAMPLE]";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_RESAMPLE_VHT_LDPC + RX_CBW_40_RESAMPLE]";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_HT:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_RESAMPLE_HT + RX_CBW_40_RESAMPLE]";
        case FrontEndModePreset::TR_CBW_40_RESAMPLE_HT_LDPC:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_RESAMPLE_HT_LDPC + RX_CBW_40_RESAMPLE]";
        case FrontEndModePreset::TR_CBW_40_HESU:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_HESU + RX_CBW_40]";
        case FrontEndModePreset::TR_CBW_40_VHT:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_VHT + RX_CBW_40]";
        case FrontEndModePreset::TR_CBW_40_VHT_LDPC:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_VHT_LDPC + RX_CBW_40]";
        case FrontEndModePreset::TR_CBW_40_HT:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_HT + RX_CBW_40]";
        case FrontEndModePreset::TR_CBW_40_HT_LDPC:
            return FrontEndModePreset2String(preset) + ": [TX_CBW_40_HT_LDPC + RX_CBW_40]";
        case FrontEndModePreset::Customized:
            return "Customized: [arbitrary Tx profile]";
        default:
            throw std::invalid_argument("Unknown Frontend mode preset");
    }
}

std::ostream &operator<<(std::ostream &os, const FrontEndModePreset &preset) {
    os << FrontEndModePreset2String(preset);
    return os;
}

const std::vector<std::string> &FrontEndModePreset2Strings() {
    static auto list = std::vector<std::string>();
    static std::once_flag flag;
    std::call_once(flag, [&] {
        for (const auto &preset: getAllFrontEndModePresets())
            list.emplace_back(FrontEndModePreset2String(preset));
    });

    return list;
}

const std::vector<FrontEndModePreset> &getAllFrontEndModePresets() {
    static auto list = std::vector<FrontEndModePreset>{
            FrontEndModePreset::TX_CBW_160_HESU,
            FrontEndModePreset::TX_CBW_160_VHT,
            FrontEndModePreset::TX_CBW_160_VHT_LDPC,
            FrontEndModePreset::TX_CBW_80_HESU,
            FrontEndModePreset::TX_CBW_80_VHT,
            FrontEndModePreset::TX_CBW_80_VHT_LDPC,
            FrontEndModePreset::TX_CBW_40_RESAMPLE_HESU,
            FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT,
            FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT_LDPC,
            FrontEndModePreset::TX_CBW_40_RESAMPLE_HT,
            FrontEndModePreset::TX_CBW_40_RESAMPLE_HT_LDPC,
            FrontEndModePreset::TX_CBW_40_HESU,
            FrontEndModePreset::TX_CBW_40_VHT,
            FrontEndModePreset::TX_CBW_40_VHT_LDPC,
            FrontEndModePreset::TX_CBW_40_HT,
            FrontEndModePreset::TX_CBW_40_HT_LDPC,

            FrontEndModePreset::RX_CBW_160,
            FrontEndModePreset::RX_CBW_80,
            FrontEndModePreset::RX_CBW_40_RESAMPLE,
            FrontEndModePreset::RX_CBW_40,

            FrontEndModePreset::TR_CBW_160_HESU,
            FrontEndModePreset::TR_CBW_160_VHT,
            FrontEndModePreset::TR_CBW_160_VHT_LDPC,
            FrontEndModePreset::TR_CBW_80_HESU,
            FrontEndModePreset::TR_CBW_80_VHT,
            FrontEndModePreset::TR_CBW_80_VHT_LDPC,
            FrontEndModePreset::TR_CBW_40_RESAMPLE_HESU,
            FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT,
            FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT_LDPC,
            FrontEndModePreset::TR_CBW_40_RESAMPLE_HT,
            FrontEndModePreset::TR_CBW_40_RESAMPLE_HT_LDPC,
            FrontEndModePreset::TR_CBW_40_HESU,
            FrontEndModePreset::TR_CBW_40_VHT,
            FrontEndModePreset::TR_CBW_40_VHT_LDPC,
            FrontEndModePreset::TR_CBW_40_HT,
            FrontEndModePreset::TR_CBW_40_HT_LDPC,

            FrontEndModePreset::Customized,
    };

    return list;
}

std::string printHelpContentForFrontEndModePreset() {
    std::stringstream ss;
    ss << "\n\nPicoScenes supports the following SDR frontend mode presets:\n";
    ss << "-------------------------------------------------------------------------------------\n";
    for (const auto &preset: getAllFrontEndModePresets())
        ss << FrontEndModePreset2DescriptiveString(preset) << "\n";
    ss << "-------------------------------------------------------------------------------------\n\n";

    return ss.str();
}

const std::map<std::string, FrontEndModePreset> &getFrontEndModePresetsMap() {
    static auto presetMap = std::map<std::string, FrontEndModePreset>();
    static std::once_flag flag;
    std::call_once(flag, [&] {
        for (const auto &preset: getAllFrontEndModePresets()) {
            presetMap.emplace(FrontEndModePreset2String(preset), preset);
        }
    });

    return presetMap;
}

std::optional<FrontEndModePreset> frontEndModePresetFromString(const std::string &presetStringInUpperCase) {
    if (getFrontEndModePresetsMap().contains(presetStringInUpperCase))
        return getFrontEndModePresetsMap().at(presetStringInUpperCase);

    return std::nullopt;
}
