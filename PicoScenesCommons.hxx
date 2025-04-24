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
#include <functional>
#include <optional>
#include <memory>
#include "SignalMatrix.hxx"

/**
 * @brief Type definitions for commonly used data types
 */
using ComplexData = std::complex<double>;        ///< Complex double precision data
using ComplexFloatData = std::complex<float>;    ///< Complex single precision data
using CS16 = std::complex<int16_t>;             ///< Complex 16-bit integer data
using ComplexArray = std::vector<ComplexData>;   ///< Array of complex double precision data
using ComplexFloatArray = std::vector<ComplexData>; ///< Array of complex single precision data
using CS16Vector = std::vector<std::complex<int16_t>>; ///< Vector of complex 16-bit integer data
using U8Vector = std::vector<uint8_t>;          ///< Vector of unsigned 8-bit integer data

/**
 * @brief Magic MAC address for IWL5300 CSI measurement in monitor mode
 */
static constexpr std::array<uint8_t, 6> MagicIntel123456{0x00, 0x16, 0xea, 0x12, 0x34, 0x56};

/**
 * @brief Broadcast MAC address
 */
static constexpr std::array<uint8_t, 6> BroadcastFFMAC{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/**
 * @brief PicoScenes supported device types
 * @see PicoScenesDeviceSubtype
 */
enum class PicoScenesDeviceType : uint16_t {
    QCA9300 = 0x9300,           ///< Qualcomm Atheros QCA9300 NIC
    IWL5300 = 0x5300,          ///< Intel Wireless Link 5300 NIC
    IWLMVM_AX200 = 0x2000,     ///< Intel Wi-Fi 6 AX200
    IWLMVM_AX210 = 0x2100,     ///< Intel Wi-Fi 6E AX210
    MAC80211Compatible = 0x802, ///< Generic MAC80211 compatible NICs
    USRP = 0x1234,             ///< Universal Software Radio Peripheral
    VirtualSDR = 0x1000,       ///< Virtual Software Defined Radio
    SoapySDR = 0x24D2,         ///< SoapySDR compatible devices
    Unknown = 0x404            ///< Unknown device type
};

/**
 * @brief PicoScenes device subtypes
 * @note The order of subtypes must not be altered
 */
enum class PicoScenesDeviceSubtype : uint16_t {
    Unknown = 0,               ///< Unknown subtype
    QCA9300,                  ///< QCA9300 NIC
    IWL5300,                  ///< IWL5300 NIC
    AX200,                    ///< Intel AX200
    AX210,                    ///< Intel AX210
    VirtualSDR,               ///< Virtual SDR
    USRP_N2x0,               ///< USRP N200/N210
    USRP_B100,               ///< USRP B100
    USRP_B2x0,               ///< USRP B200/B210
    USRP_E3x0,               ///< USRP E310/E320
    USRP_N3x0,               ///< USRP N300/N310/N320/N321
    USRP_X3x0,               ///< USRP X300/X310
    USRP_X4x0,               ///< USRP X410/X440
    HackRFOne,               ///< HackRF One
    LimeSDR,                 ///< LimeSDR
    BladeRF,                 ///< BladeRF
    SOAPYSDR_UHD,           ///< SoapySDR UHD devices
    AX211,                   ///< Intel AX211
    USRP_E310 = 1310,       ///< USRP E310
    USRP_E320 = 1320,       ///< USRP E320
    USRP_N300_N310 = 310,   ///< USRP N300/N310
    USRP_N320_N321 = 320,   ///< USRP N320/N321
    USRP_X410 = 410,        ///< USRP X410
    USRP_X440 = 440,        ///< USRP X440
};

/**
 * @brief IEEE 802.11 packet formats
 */
enum class PacketFormatEnum : int8_t {
    PacketFormat_NonHT = 0,   ///< 802.11a/g (Legacy)
    PacketFormat_HT = 1,      ///< 802.11n (Wi-Fi 4)
    PacketFormat_VHT = 2,     ///< 802.11ac (Wi-Fi 5)
    PacketFormat_HESU = 3,    ///< 802.11ax Single-User (Wi-Fi 6)
    PacketFormat_HEMU = 4,    ///< 802.11ax Multi-User
    PacketFormat_EHTSU = 5,   ///< 802.11be Single-User (Wi-Fi 7)
    PacketFormat_EHTMU = 6,   ///< 802.11be Multi-User
    PacketFormat_Unknown = -1 ///< Unknown format
};


/**
 * @brief Channel bandwidth enumeration
 * @note CBW doesn't necessarily equal hardware sampling rate
 */
enum class ChannelBandwidthEnum : uint16_t {
    CBW_20 = 20,    ///< 20 MHz bandwidth
    CBW_40 = 40,    ///< 40 MHz bandwidth
    CBW_80 = 80,    ///< 80 MHz bandwidth
    CBW_160 = 160,  ///< 160 MHz bandwidth
    CBW_320 = 320   ///< 320 MHz bandwidth
};

/**
 * @brief 802.11n channel modes
 * @deprecated Use ChannelBandwidthEnum instead
 */
enum class ChannelModeEnum : uint8_t {
    HT20 = 8,           ///< 20 MHz HT mode
    HT40_MINUS = 24,    ///< 40 MHz HT mode (extension channel below)
    HT40_PLUS = 40,     ///< 40 MHz HT mode (extension channel above)
};

/**
 * @brief Guard interval durations
 */
enum class GuardIntervalEnum : uint16_t {
    GI_400 = 400,   ///< 0.4 μs (Short GI in 802.11n/ac)
    GI_800 = 800,   ///< 0.8 μs (Normal GI)
    GI_1600 = 1600, ///< 1.6 μs
    GI_3200 = 3200  ///< 3.2 μs
};

/**
 * @brief Channel coding schemes
 */
enum class ChannelCodingEnum : uint8_t {
    BCC = 0,    ///< Binary Convolutional Coding
    LDPC = 1,   ///< Low-Density Parity-Check
};

/**
 * @brief Check if device is Intel MVM-based NIC
 * @param psdt Device type to check
 * @return true if device is AX200 or AX210
 */
inline bool isIntelMVMTypeNIC(const PicoScenesDeviceType psdt) {
    return psdt == PicoScenesDeviceType::IWLMVM_AX200 || psdt == PicoScenesDeviceType::IWLMVM_AX210;
}

/**
 * @brief Check if device is COTS Wi-Fi NIC
 * @param psdt Device type to check
 * @return true if device is commercial off-the-shelf Wi-Fi NIC
 */
inline bool isCOTSNIC(const PicoScenesDeviceType psdt) {
    return isIntelMVMTypeNIC(psdt) || psdt == PicoScenesDeviceType::QCA9300 || psdt == PicoScenesDeviceType::IWL5300 || psdt == PicoScenesDeviceType::MAC80211Compatible;
}

/**
 * @brief Check if device is SDR
 * @param psdt Device type to check
 * @return true if device is USRP, SoapySDR, or Virtual SDR
 */
inline bool isSDR(const PicoScenesDeviceType psdt) {
    return psdt == PicoScenesDeviceType::USRP || psdt == PicoScenesDeviceType::SoapySDR || psdt == PicoScenesDeviceType::VirtualSDR;
}

// String conversion functions
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
        case PicoScenesDeviceSubtype::USRP_E310:
            return "USRP E310";
        case PicoScenesDeviceSubtype::USRP_E320:
            return "USRP E320";
        case PicoScenesDeviceSubtype::USRP_N300_N310:
            return "USRP N300/N310";
        case PicoScenesDeviceSubtype::USRP_N320_N321:
            return "USRP N320/N321";
        case PicoScenesDeviceSubtype::USRP_X410:
            return "USRP X410";
        case PicoScenesDeviceSubtype::USRP_X440:
            return "USRP X440";
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

// Stream operators
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
