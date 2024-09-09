//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_PICOSCENESCOMMONS_HXX
#define PICOSCENES_PLATFORM_PICOSCENESCOMMONS_HXX

#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <string>
#include <complex>
#include <numeric>
#include <mutex>
#include <bitset>
#include <iomanip>
#include "SignalMatrix.hxx"
#include "SDRResamplingPreset.hxx"

#ifdef BUILD_WITH_MEX
#include "mex.h"
#define printf mexPrintf
#endif

using ComplexData = std::complex<double>;
using ComplexFloatData = std::complex<float>;
using CS16 = std::complex<int16_t>;
using ComplexArray = std::vector<ComplexData>;
using ComplexFloatArray = std::vector<ComplexData>;
using CS16Vector = std::vector<std::complex<int16_t>>;
using U8Vector = std::vector<uint8_t>;

/**
 * IWL5300 can measure CSI for this MAC address in monitor mode. A pure magic MAC address
 */
static constexpr std::array<uint8_t, 6> MagicIntel123456{0x00, 0x16, 0xea, 0x12, 0x34, 0x56};

static constexpr std::array<uint8_t, 6> BroadcastFFMAC{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/**
 * @brief PicoScenes supported device type
 * @see PicoScenesDeviceSubtype
 */
enum class PicoScenesDeviceType : uint16_t {
    QCA9300 = 0x9300, ///< QCA9300 NIC
    IWL5300 = 0x5300, ///< IWL5300 NIC
    IWLMVM_AX200 = 0x2000, ///< AX200 NIC
    IWLMVM_AX210 = 0x2100, ///< AX210 NIC
    MAC80211Compatible = 0x802, ///< All packet-injectable NIC models
    USRP = 0x1234, ///< USRP
    VirtualSDR = 0x1000, ///< Virtual SDR
    SoapySDR = 0x24D2, ///< SoapySDR supported SDR devices
    Unknown = 0x404, ///< Unknown
};

/**
 * PicoScenes supported device sub-type
 * @see PicoScenesDeviceType
 * @note Never try to alter the subtype order
 */
enum class PicoScenesDeviceSubtype : uint16_t {
    Unknown = 0,
    QCA9300,
    IWL5300,
    AX200,
    AX210,
    VirtualSDR,
    USRP_N2x0, // usrp2
    USRP_B100, // b100
    USRP_B2x0, // b200
    USRP_E3x0, // e3x0
    USRP_N3x0, // n3xx
    USRP_X3x0, // x300
    USRP_X4x0, // x4xx
    HackRFOne,
    LimeSDR,
    BladeRF,
    SOAPYSDR_UHD,
    AX211,
};

/**
 * PicoScenes supported packet format
 */
enum class PacketFormatEnum : int8_t {
    PacketFormat_NonHT = 0, ///< 11a or 11g
    PacketFormat_HT = 1, ///< 11n or Wi-Fi 4
    PacketFormat_VHT = 2, ///< 11ac or Wi-Fi 5
    PacketFormat_HESU = 3, ///< 11ax Single-User
    PacketFormat_HEMU = 4, ///< 11ax Multi-User
    PacketFormat_EHTSU = 5, ///< 11be Single-User (a special case of Non-OFDMA EHT-MU)
    PacketFormat_EHTMU = 6, ///< 11be Multi-User
    PacketFormat_Unknown = -1
};

/**
 * Tx/Rx Channel Bandwidth (CBW) parameter
 * @note CBW doesn't necessarily equals to hardware sampling rate.
 */
enum class ChannelBandwidthEnum : uint16_t {
    CBW_20 = 20, ///< 20 MHz
    CBW_40 = 40, ///< 40 MHz
    CBW_80 = 80, ///< 80 MHz
    CBW_160 = 160, ///< 160 MHz
    CBW_320 = 320, ///< 320 MHz
};

/**
 * 802.11n Channel Mode
 * @deprecated
 */
enum class ChannelModeEnum : uint8_t {
    HT20 = 8,
    HT40_MINUS = 24,
    HT40_PLUS = 40,
};

/**
 * The guarding interval (ns) between successive OFDM symbols
 */
enum class GuardIntervalEnum : uint16_t {
    GI_400 = 400, ///< 802.11n/ac "Short Guard Interval"
    GI_800 = 800,
    GI_1600 = 1600,
    GI_3200 = 3200
};

/**
 * Wi-Fi Coding schemes, BCC or LDPC
 */
enum class ChannelCodingEnum : uint8_t {
    BCC = 0,
    LDPC = 1,
};

/**
 * Check the specified device type is Intel MVM-based NIC (AX200 or AX210)
 * @return
 */
inline bool isIntelMVMTypeNIC(const PicoScenesDeviceType psdt) {
    return psdt == PicoScenesDeviceType::IWLMVM_AX200 || psdt == PicoScenesDeviceType::IWLMVM_AX210;
}

/**
 * Check the specified device type is COTS Wi-Fi NICS (210/200/9300/5300/802)
 * @return
 */
inline bool isCOTSNIC(const PicoScenesDeviceType psdt) {
    return isIntelMVMTypeNIC(psdt) || psdt == PicoScenesDeviceType::QCA9300 || psdt == PicoScenesDeviceType::IWL5300 || psdt == PicoScenesDeviceType::MAC80211Compatible;
}

/**
 * Check the specified device type is SDR (USRP or SoapySDR)
 * @param psdt
 * @return
 */
inline bool isSDR(const PicoScenesDeviceType psdt) {
    return psdt == PicoScenesDeviceType::USRP || psdt == PicoScenesDeviceType::SoapySDR || psdt == PicoScenesDeviceType::VirtualSDR;
}

inline std::string DeviceType2String(const PicoScenesDeviceType type) {
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

inline std::string DeviceSubtype2String(const PicoScenesDeviceSubtype subtype) {
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
        case PicoScenesDeviceSubtype::VirtualSDR:
            return "VirtualSDR";
        default:
            throw std::runtime_error("unrecognized PicoScenesDeviceSubtype.");
    }
}

inline std::string PacketFormat2String(const PacketFormatEnum format) {
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
        case PacketFormatEnum::PacketFormat_EHTSU:
            return "EHT-SU";
        case PacketFormatEnum::PacketFormat_EHTMU:
            return "EHT-MU";
        case PacketFormatEnum::PacketFormat_Unknown:
            return "Unknown";
    }

    throw std::runtime_error("Unsupported packet format.");
}

inline std::string ChannelBandwidth2String(const ChannelBandwidthEnum cbw) {
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

inline std::string channelModel2String(const ChannelModeEnum mode) {
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

inline std::string GuardInterval2String(const GuardIntervalEnum gi) {
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

inline std::string ChannelCoding2String(const ChannelCodingEnum coding) {
    switch (coding) {
        case ChannelCodingEnum::LDPC:
            return "LDPC";
        case ChannelCodingEnum::BCC:
            return "BCC";
        default:
            return "Unknown";
    }
}

inline std::ostream& operator<<(std::ostream& os, const PicoScenesDeviceType& deviceType) {
    os << DeviceType2String(deviceType);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const PacketFormatEnum& format) {
    os << PacketFormat2String(format);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ChannelBandwidthEnum& cbw) {
    os << ChannelBandwidth2String(cbw);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ChannelModeEnum& channelMode) {
    os << channelModel2String(channelMode);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const GuardIntervalEnum& gi) {
    os << GuardInterval2String(gi);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ChannelCodingEnum& coding) {
    os << ChannelCoding2String(coding);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const PicoScenesDeviceSubtype& deviceSubtype) {
    os << DeviceSubtype2String(deviceSubtype);
    return os;
}

#endif //PICOSCENES_PLATFORM_PICOSCENESCOMMONS_HXX
