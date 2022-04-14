//
// Created by 蒋志平 on 2020/11/6.
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
#include "SignalMatrix.hxx"

#ifdef BUILD_WITH_MEX
#include "mex.h"
#define printf mexPrintf
#endif

using ComplexData = std::complex<double>;
using ComplexFloatData = std::complex<float>;
using ComplexArray = std::vector<ComplexData>;
using ComplexFloatArray = std::vector<ComplexData>;
using Uint8Vector = std::vector<uint8_t>;

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
 * Check the specified device type is Intel MVM-based NIC (AX200 or AX210)
 * @return
 */
bool isIntelMVMTypeNIC(PicoScenesDeviceType psdt);

/**
 * Return the device type string for the given PicoScenesDeviceType type
 * @return
 */
std::string DeviceType2String(PicoScenesDeviceType type);

std::ostream &operator<<(std::ostream &os, const PicoScenesDeviceType &deviceType);

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
};

/**
 * Return the device sub-type string
 * @param subtype
 * @return A string with identical enum name
 */
std::string DeviceSubtype2String(PicoScenesDeviceSubtype subtype);

std::ostream &operator<<(std::ostream &os, const PicoScenesDeviceSubtype &deviceSubtype);

/**
 * PicoScenes supported packet format
 */
enum class PacketFormatEnum : int8_t {
    PacketFormat_NonHT = 0, ///< 11a or 11g
    PacketFormat_HT = 1, ///< 11n or Wi-Fi 4
    PacketFormat_VHT = 2, ///< 11ac or Wi-Fi 5
    PacketFormat_HESU = 3, ///< 11ax Single-User, or Wi-Fi 6
    PacketFormat_HEMU = 4, ///< 11ax Multi-User, or Wi-Fi 6
    PacketFormat_Unknown = -1
};


std::string PacketFormat2String(PacketFormatEnum format);

std::ostream &operator<<(std::ostream &os, const PacketFormatEnum &format);

/**
 * Tx/Rx Channel Bandwidth (CBW) parameter
 * @note CBW doesn't necessarily equals to hardware sampling rate.
 */
enum class ChannelBandwidthEnum : uint16_t {
    CBW_20 = 20, ///< 20 MHz
    CBW_40 = 40, ///< 40 MHz
    CBW_80 = 80, ///< 80 MHz
    CBW_160 = 160, ///< 160 MHz
};

std::string ChannelBandwidth2String(ChannelBandwidthEnum cbw);

std::ostream &operator<<(std::ostream &os, const ChannelBandwidthEnum &cbw);

/**
 * 802.11n Channel Mode
 * @deprecated
 */
enum class ChannelModeEnum : uint8_t {
    HT20 = 8,
    HT40_MINUS = 24,
    HT40_PLUS = 40,
};

std::string channelModel2String(ChannelModeEnum mode);

std::ostream &operator<<(std::ostream &os, const ChannelModeEnum &channelMode);

/**
 * The guarding interval (ns) between successive OFDM symbols
 */
enum class GuardIntervalEnum : uint16_t {
    GI_400 = 400, ///< 802.11n/ac "Short Guard Interval"
    GI_800 = 800,
    GI_1600 = 1600,
    GI_3200 = 3200
};


std::string GuardInterval2String(GuardIntervalEnum gi);

std::ostream &operator<<(std::ostream &os, const GuardIntervalEnum &gi);

/**
 * Wi-Fi Coding schemes, BCC or LDPC
 */
enum class ChannelCodingEnum : uint8_t {
    BCC = 0,
    LDPC = 1,
};

std::string ChannelCoding2String(ChannelCodingEnum coding);

std::ostream &operator<<(std::ostream &os, const ChannelCodingEnum &coding);


enum class FrontEndModePreset : uint8_t {
    TX_CBW_160_HESU, ///< 160 MHz CBW, 200 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HESU format, LDPC coding
    TX_CBW_160_VHT, ///< 160 MHz CBW, 200 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, BCC coding
    TX_CBW_160_VHT_LDPC, ///< 160 MHz CBW, 200 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, LDPC coding
    TX_CBW_80_HESU, ///< 80 MHz CBW, 100 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HESU format, LDPC coding
    TX_CBW_80_VHT, ///< 80 MHz CBW, 100 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, BCC coding
    TX_CBW_80_VHT_LDPC, ///< 80 MHz CBW, 100 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, LDPC coding
    TX_CBW_40_RESAMPLE_HESU, ///< 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HESU format, LDPC coding
    TX_CBW_40_RESAMPLE_VHT, ///< 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, BCC coding
    TX_CBW_40_RESAMPLE_VHT_LDPC, ///< 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, VHT format, LDPC coding
    TX_CBW_40_RESAMPLE_HT, ///< 40 MHz CBW, 50 MHz actual Tx sampling rate, 1.25 Tx resample ratio, HT format, BCC coding
    TX_CBW_40_HESU, ///< 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, HESU format, LDPC coding
    TX_CBW_40_VHT, ///< 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, VHT format, BCC coding
    TX_CBW_40_VHT_LDPC, ///< 40 MHz CBW, 40 MHz actual Tx sampling rate, 1.0 Tx resample ratio, VHT format, LDPC coding

    RX_CBW_160, ///< 160 MHz Rx CBW, 200 MHz actual Rx sampling rate, 1.25 Rx resample ratio
    RX_CBW_80,  //< 80 MHz Rx CBW, 100 MHz actual Rx sampling rate, 1.25 Rx resample ratio
    RX_CBW_40,  //< 40 MHz Rx CBW, 40 MHz actual Rx sampling rate, 1.0 Rx resample ratio

    Customized,
};

std::string FrontEndModePreset2String(FrontEndModePreset preset);

std::string FrontEndModePreset2DescriptiveString(FrontEndModePreset preset);

std::string printHelpContentForFrontEndModePreset();

std::ostream &operator<<(std::ostream &os, const FrontEndModePreset &preset);

const std::vector<std::string> &FrontEndModePreset2Strings();

const std::vector<FrontEndModePreset> &getAllFrontEndModePresets();

#endif //PICOSCENES_PLATFORM_PICOSCENESCOMMONS_HXX
