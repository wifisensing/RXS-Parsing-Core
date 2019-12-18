//
// Created by 蒋志平 on 2018/11/10.
//

#ifndef PICOSCENES_PLATFROM_RXSEXTRAINFO_H
#define PICOSCENES_PLATFROM_RXSEXTRAINFO_H

#include <cstdlib>
#include <cstdio>
#include <complex>
#include <bitset>

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#define PICOSCENES_EXTRAINFO_HASLENGTH                 0x00000001U
#define PICOSCENES_EXTRAINFO_HASVERSION                0x00000002U
#define PICOSCENES_EXTRAINFO_HASMACCUR                 0x00000004U
#define PICOSCENES_EXTRAINFO_HASMACROM                 0x00000008U
#define PICOSCENES_EXTRAINFO_HASCHANSEL                0x00000010U
#define PICOSCENES_EXTRAINFO_HASBMODE                  0x00000020U
#define PICOSCENES_EXTRAINFO_HASEVM                    0x00000040U
#define PICOSCENES_EXTRAINFO_HASTXCHAINMASK            0x00000080U
#define PICOSCENES_EXTRAINFO_HASRXCHAINMASK            0x00000100U
#define PICOSCENES_EXTRAINFO_HASTXPOWER                0x00000200U
#define PICOSCENES_EXTRAINFO_HASCF                     0x00000400U
#define PICOSCENES_EXTRAINFO_HASTXTSF                  0x00000800U
#define PICOSCENES_EXTRAINFO_HASLASTHWTXTSF            0x00001000U
#define PICOSCENES_EXTRAINFO_HASCHANNELFLAGS           0x00002000U
#define PICOSCENES_EXTRAINFO_HASTXNESS                 0x00004000U
#define PICOSCENES_EXTRAINFO_HASTUNINGPOLICY           0x00008000U
#define PICOSCENES_EXTRAINFO_HASPLLRATE                0x00010000U
#define PICOSCENES_EXTRAINFO_HASPLLREFDIV              0x00020000U
#define PICOSCENES_EXTRAINFO_HASPLLCLKSEL              0x00040000U
#define PICOSCENES_EXTRAINFO_HASAGC                    0x00080000U
#define PICOSCENES_EXTRAINFO_HASANTENNASELECTION       0x00100000U


/**
 The mininum set of ExtraInfo used for Tx fabrication.

 @see ExtraInfo
 */
PACK(struct TxExtraInfoMinSet {
         uint32_t txExtraInfoFeatureCode;
         uint16_t txExtraInfoLength;
         uint64_t txExtraInfoVersion;
         uint8_t txExtraInfoMacAddr_cur[6];
         uint8_t txExtraInfoMacAddr_rom[6];

         int getTxTSFPos();
     });

struct ExtraInfo {
    uint32_t featureCode = 0x0;
    bool hasLength;
    bool hasVersion;
    bool hasMacAddr_cur;
    bool hasMacAddr_rom;
    bool hasChansel;
    bool hasBMode;
    bool hasEVM;
    bool hasTxChainMask;
    bool hasRxChainMask;
    bool hasTxpower;
    bool hasCF;
    bool hasTxTSF;
    bool hasLastHWTxTSF;
    bool hasChannelFlags;
    bool hasTxNess;
    bool hasTuningPolicy;
    bool hasPLLRate;
    bool hasPLLRefDiv;
    bool hasPLLClkSel;
    bool hasAGC;
    bool hasAntennaSelection;
    uint16_t length;
    uint64_t version;
    uint8_t macaddr_rom[6];
    uint8_t macaddr_cur[6];
    uint32_t chansel;
    uint8_t bmode;
    int8_t evm[20];
    uint8_t txChainMask;
    uint8_t rxChainMask;
    uint8_t txpower;
    uint64_t cf;
    uint32_t txTSF;
    uint32_t lastHwTxTSF;
    uint16_t channelFlags;
    uint8_t tx_ness;
    uint8_t tuningPolicy;
    uint16_t pll_rate;
    uint8_t pll_refdiv;
    uint8_t pll_clock_select;
    uint8_t agc;
    uint8_t ant_sel[3];

    uint16_t getLength();

    static int fromBinary(const uint8_t *extraInfoPtr, struct ExtraInfo *extraInfo, uint32_t suppliedFeatureCode = 0);

    int toBinary(uint8_t *buffer);

    [[maybe_unused]] void setLength(uint16_t length);

    [[maybe_unused]] void setVersion(uint64_t version);

    [[maybe_unused]] void setMacaddr_rom(const uint8_t addr_rom[6]);

    [[maybe_unused]] void setMacaddr_cur(const uint8_t addr_cur[6]);

    [[maybe_unused]] void setChansel(uint32_t chansel);

    [[maybe_unused]] void setBmode(uint8_t bmode);

    [[maybe_unused]] void setTxChainMask(uint8_t txChainMask);

    [[maybe_unused]] void setRxChainMask(uint8_t rxChainMaskV);

    [[maybe_unused]] void setTxpower(uint8_t txpowerV);

    [[maybe_unused]] void setCf(uint64_t cf);

    [[maybe_unused]] void setTxTsf(uint32_t txTsf);

    [[maybe_unused]] void setLastHwTxTsf(uint32_t lastHwTxTsf);

    [[maybe_unused]] void setChannelFlags(uint16_t channelFlags);

    [[maybe_unused]] void setTxNess(uint8_t txNess);

    [[maybe_unused]] void setTuningPolicy(uint8_t tuningPolicy);

    [[maybe_unused]] void setPllRate(uint16_t pllRate);

    [[maybe_unused]] void setPllRefdiv(uint8_t pllRefdiv);

    [[maybe_unused]] void setPllClockSelect(uint8_t pllClockSelect);

    [[maybe_unused]] void setAgc(uint8_t agc);

    [[maybe_unused]] void setAntennaSelection(const uint8_t ant_sel[3]);
};


/**
 * Test the presence of version field.
 * @param featureCode The 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasVersion(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 0 & 0x1);
}

/**
 * Test the presence of length field
 *
 * Please note: in the ExtraInfo struct, the length value does not include the length field itself.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasLength(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 1 & 0x1);
}

/**
 * Test the presence of "current MAC Address" field
 *
 * The "current" MAC address is the mac address attached to the working interface, this MAC address can be modified using bash commands.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasMacAddress_Rom
 */
inline bool extraInfoHasMacAddress_Current(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 2 & 0x1);
}

/**
 * Test the presence of "Permenant MAC Address" field
 *
 * "Permenant MAC Address" is the mac address store the in the H/W's EEPROM, which cannot be modified.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasMacAddress_Current
 */
inline bool extraInfoHasMacAddress_Rom(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 3 & 0x1);
}

/**
 * Test the presence of "Channel Selection" field
 *
 * Chansel value is used in Ath9k driver to configure the carrier frequency
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasBMode
 */
inline bool extraInfoHasChansel(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 4 & 0x1);
}

/**
 * Test the presence of "Bmode" field
 *
 * Bmode value is used together with Chansel value in Ath9k driver to configure the carrier frequency.
 *
 * 1 for 2.4GHz band, 0 for 5GHz band.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasChansel
 */
inline bool extraInfoHasBMode(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 5 & 0x1);
}

/**
 * Test the presence of EVM data
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasEVM(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 6 & 0x1);
}

/**
 * Test the presence of Tx ChainMask
 *
 * Tx/Rx chainmask is a 3-bit value. Each bit stands for a radio chain.
 * 1 for first chain, 4 for last chain.
 * The chainmask bits combination MUST be in low to high order.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasTxChainMask(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 7 & 0x1);
}

inline bool extraInfoHasRxChainMask(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 8 & 0x1);
}

inline bool extraInfoHasTxPower(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 9 & 0x1);
}

inline bool extraInfoHasCF(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 10 & 0x1);
}

inline bool extraInfoHasTxTSF(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 11 & 0x1);
}

inline bool extraInfoHasLastHWTxTSF(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 12 & 0x1);
}

inline bool extraInfoHasChannelFlags(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 13 & 0x1);
}

inline bool extraInfoHasTxNess(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 14 & 0x1);
}

inline bool extraInfoHasTuningPolicy(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 15 & 0x1);
}

inline bool extraInfoHasPLLRate(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 16 & 0x1);
}

inline bool extraInfoHasPLLRefDiv(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 17 & 0x1);
}

inline bool extraInfoHasPLLClkSel(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 18 & 0x1);
}

inline bool extraInfoHasAGC(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 19 & 0x1);
}

inline bool extraInfoHasAntennaSelection(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 20 & 0x1);
}

enum RXSParsingLevel : uint8_t {
    BASIC_NOEXTRA_NOCSI = 10,
    EXTRA_NOCSI,
    EXTRA_CSI,
    EXTRA_CSI_UNWRAP
};

enum ChannelMode : uint8_t {
    HT20 = 8,
    HT40_MINUS = 24,
    HT40_PLUS = 40,
};

enum AtherosCFTuningPolicy : uint8_t {
    CFTuningByChansel = 30,
    CFTuningByFastCC,
    CFTuningByHardwareReset,
    CFTuningByDefault,
};


inline ChannelMode channelFlags2ChannelMode(uint16_t channelFlags) {
    std::bitset<16> channelFlagSet(channelFlags);

    if (channelFlagSet.test(3) && channelFlagSet.test(4)) {
        return HT40_PLUS;
    }

    if (channelFlagSet.test(3) && channelFlagSet.test(5)) {
        return HT40_MINUS;
    }

    if (channelFlagSet.test(3)) {
        return HT20;
    }

    return HT20;
}

inline std::string channelModel2String(ChannelMode mode) {
    switch (mode) {
        case HT40_PLUS:
            return "HT40_PLUS";
        case HT40_MINUS:
            return "HT40_MINUS";
        case HT20:
            return "HT20";
    }
}

inline std::string TuningPolicy2String(uint8_t policy) {
    switch (policy) {
        case CFTuningByChansel:
            return "Chansel";
        case CFTuningByFastCC:
            return "FastCC";
        case CFTuningByHardwareReset:
            return "Reset";
        case CFTuningByDefault:
            return "Default";
        default:
            throw std::runtime_error("[RXS_Enhanced.h] Unknown Tuning Policy for value: " + std::to_string(policy));
    }
}


/**
 * Parse 32-bit feature code into has* values of struct ExtraInfo
 * @param featureCode the input 32-bit feature code
 * @param extraInfo The ExtraInfo to be modified to reflect the feature code
 */
void featureCodeInterpretation(uint32_t featureCode, struct ExtraInfo *extraInfo);


/**
 * Directly inject (in-place add) an ExtraInfo Item into the raw RxS data.
 *
 * The transparency to the upper RxS parsing process makes this method very useful in case of adding some platform-calculated value into RXS struct.
 *
 * @param rxs_raw the raw rxs data
 * @param featureCode_added the feature code for the adding value
 * @param data pointer to the value to be added
 * @param length value length
 */
void inplaceAddRxExtraInfo(uint8_t *rxs_raw, uint32_t featureCode_added, uint8_t *data, int length);


#endif //PICOSCENES_PLATFROM_RXSEXTRAINFO_H
