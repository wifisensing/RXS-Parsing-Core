//
// Created by Zhiping Jiang on 2018/11/10.
//

#ifndef PICOSCENES_PLATFROM_RXSEXTRAINFO_H
#define PICOSCENES_PLATFROM_RXSEXTRAINFO_H

#include "PicoScenesCommons.hxx"

/**
 * @brief Enumeration for Atheros carrier frequency tuning policies
 *
 * @details Defines different methods for tuning the carrier frequency in Atheros wireless cards.
 * These policies determine how the hardware configures its radio frequency settings.
 */
enum AtherosCFTuningPolicy : uint8_t {
    CFTuningByChansel = 30,      /**< Tune by channel selection value */
    CFTuningByFastCC,            /**< Tune by fast channel change */
    CFTuningByHardwareReset,     /**< Tune by hardware reset */
    CFTuningByDefault,           /**< Tune by default settings */
};

/**
 * @brief Convert a tuning policy value to its string representation
 *
 * @param policy The tuning policy value to convert
 * @return std::string String representation of the policy
 * @throw std::runtime_error If the policy value is unknown
 */
inline std::string TuningPolicy2String(const uint8_t policy) {
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
 * @brief Feature code bit flags for ExtraInfo structure
 *
 * @details These macros define the bit positions in the featureCode field of the ExtraInfo structure.
 * Each bit indicates the presence of a specific field in the ExtraInfo structure.
 */
#define PICOSCENES_EXTRAINFO_HASLENGTH                 0x00000001U  /**< Bit 0: Has length field */
#define PICOSCENES_EXTRAINFO_HASVERSION                0x00000002U  /**< Bit 1: Has version field */
#define PICOSCENES_EXTRAINFO_HASMACCUR                 0x00000004U  /**< Bit 2: Has current MAC address */
#define PICOSCENES_EXTRAINFO_HASMACROM                 0x00000008U  /**< Bit 3: Has ROM MAC address */
#define PICOSCENES_EXTRAINFO_HASCHANSEL                0x00000010U  /**< Bit 4: Has channel selection */
#define PICOSCENES_EXTRAINFO_HASBMODE                  0x00000020U  /**< Bit 5: Has band mode */
#define PICOSCENES_EXTRAINFO_HASEVM                    0x00000040U  /**< Bit 6: Has EVM data */
#define PICOSCENES_EXTRAINFO_HASTXCHAINMASK            0x00000080U  /**< Bit 7: Has TX chain mask */
#define PICOSCENES_EXTRAINFO_HASRXCHAINMASK            0x00000100U  /**< Bit 8: Has RX chain mask */
#define PICOSCENES_EXTRAINFO_HASTXPOWER                0x00000200U  /**< Bit 9: Has TX power */
#define PICOSCENES_EXTRAINFO_HASCF                     0x00000400U  /**< Bit 10: Has carrier frequency */
#define PICOSCENES_EXTRAINFO_HASTXTSF                  0x00000800U  /**< Bit 11: Has TX TSF */
#define PICOSCENES_EXTRAINFO_HASLASTHWTXTSF            0x00001000U  /**< Bit 12: Has last hardware TX TSF */
#define PICOSCENES_EXTRAINFO_HASCHANNELFLAGS           0x00002000U  /**< Bit 13: Has channel flags */
#define PICOSCENES_EXTRAINFO_HASTXNESS                 0x00004000U  /**< Bit 14: Has TX ness */
#define PICOSCENES_EXTRAINFO_HASTUNINGPOLICY           0x00008000U  /**< Bit 15: Has tuning policy */
#define PICOSCENES_EXTRAINFO_HASPLLRATE                0x00010000U  /**< Bit 16: Has PLL rate */
#define PICOSCENES_EXTRAINFO_HASPLLREFDIV              0x00020000U  /**< Bit 17: Has PLL reference divider */
#define PICOSCENES_EXTRAINFO_HASPLLCLKSEL              0x00040000U  /**< Bit 18: Has PLL clock select */
#define PICOSCENES_EXTRAINFO_HASAGC                    0x00080000U  /**< Bit 19: Has AGC */
#define PICOSCENES_EXTRAINFO_HASANTENNASELECTION       0x00100000U  /**< Bit 20: Has antenna selection */
#define PICOSCENES_EXTRAINFO_HASSAMPLINGRATE           0x00200000U  /**< Bit 21: Has sampling rate */
#define PICOSCENES_EXTRAINFO_HASCFO                    0x00400000U  /**< Bit 22: Has carrier frequency offset */
#define PICOSCENES_EXTRAINFO_HASSFO                    0x00800000U  /**< Bit 23: Has sampling frequency offset */
#define PICOSCENES_EXTRAINFO_HASTEMPERATURE            0x01000000U  /**< Bit 24: Has temperature */

/**
 * @brief Structure for storing extra information about received signals
 *
 * @details This structure contains various fields that provide additional information
 * about received wireless signals, such as device identifiers, channel settings,
 * signal quality metrics, and hardware configuration parameters.
 */
struct ExtraInfo {
    uint32_t featureCode;        /**< Bit flags indicating which fields are present */
    bool hasLength;              /**< Whether the length field is present */
    bool hasVersion;             /**< Whether the version field is present */
    bool hasMacAddr_cur;         /**< Whether the current MAC address is present */
    bool hasMacAddr_rom;         /**< Whether the ROM MAC address is present */
    bool hasChansel;             /**< Whether the channel selection is present */
    bool hasBMode;               /**< Whether the band mode is present */
    bool hasEVM;                 /**< Whether EVM data is present */
    bool hasTxChainMask;         /**< Whether TX chain mask is present */
    bool hasRxChainMask;         /**< Whether RX chain mask is present */
    bool hasTxpower;             /**< Whether TX power is present */
    bool hasCF;                  /**< Whether carrier frequency is present */
    bool hasTxTSF;               /**< Whether TX TSF is present */
    bool hasLastHWTxTSF;         /**< Whether last hardware TX TSF is present */
    bool hasChannelFlags;        /**< Whether channel flags are present */
    bool hasTxNess;              /**< Whether TX ness is present */
    bool hasTuningPolicy;        /**< Whether tuning policy is present */
    bool hasPLLRate;             /**< Whether PLL rate is present */
    bool hasPLLRefDiv;           /**< Whether PLL reference divider is present */
    bool hasPLLClkSel;           /**< Whether PLL clock select is present */
    bool hasAGC;                 /**< Whether AGC is present */
    bool hasAntennaSelection;    /**< Whether antenna selection is present */
    bool hasSamplingRate;        /**< Whether sampling rate is present */
    bool hasCFO;                 /**< Whether carrier frequency offset is present */
    bool hasSFO;                 /**< Whether sampling frequency offset is present */
    bool hasTemperature;         /**< Whether temperature is present */

    uint16_t length;             /**< Length of the extra info data */
    uint64_t version;            /**< Version of the extra info format */
    uint8_t macaddr_rom[6];      /**< ROM MAC address (permanent) */
    uint8_t macaddr_cur[6];      /**< Current MAC address */
    uint32_t chansel;            /**< Channel selection value */
    uint8_t bmode;               /**< Band mode (0 for 5GHz, 1 for 2.4GHz) */
    int8_t evm[20];              /**< Error Vector Magnitude data */
    uint8_t txChainMask;         /**< TX chain mask */
    uint8_t rxChainMask;         /**< RX chain mask */
    uint8_t txpower;             /**< TX power level */
    uint64_t cf;                 /**< Carrier frequency */
    uint64_t txTSF;              /**< TX TSF (Time Synchronization Function) */
    uint64_t lastHwTxTSF;        /**< Last hardware TX TSF */
    uint16_t channelFlags;       /**< Channel flags */
    uint8_t tx_ness;             /**< TX ness value */
    AtherosCFTuningPolicy tuningPolicy; /**< Carrier frequency tuning policy */
    uint16_t pll_rate;           /**< PLL rate */
    uint8_t pll_refdiv;          /**< PLL reference divider */
    uint8_t pll_clock_select;    /**< PLL clock select */
    uint8_t agc;                 /**< Automatic Gain Control value */
    uint8_t ant_sel[3];          /**< Antenna selection values */
    uint64_t samplingRate;       /**< Sampling rate */
    int32_t cfo;                 /**< Carrier Frequency Offset */
    int32_t sfo;                 /**< Sampling Frequency Offset */
    int8_t temperature;          /**< Temperature reading */

    /**
     * @brief Default constructor
     */
    ExtraInfo();

    /**
     * @brief Calculate the buffer length needed to store this ExtraInfo
     *
     * @return uint16_t The calculated buffer length
     */
    [[nodiscard]] uint16_t calculateBufferLength() const;

    /**
     * @brief Update the length field based on the current state
     */
    void updateLength();

    /**
     * @brief Parse binary data into an ExtraInfo structure
     *
     * @param extraInfoPtr Pointer to the binary data
     * @param extraInfo Pointer to the ExtraInfo structure to populate
     * @param suppliedFeatureCode Optional feature code to use instead of reading from binary data
     * @return int Number of bytes parsed
     */
    static int fromBinary(const uint8_t *extraInfoPtr, struct ExtraInfo *extraInfo, uint32_t suppliedFeatureCode = 0);

    /**
     * @brief Create an ExtraInfo structure from binary data
     *
     * @param extraInfoPtr Pointer to the binary data
     * @param suppliedFeatureCode Optional feature code to use instead of reading from binary data
     * @return std::optional<ExtraInfo> The parsed ExtraInfo structure, or nullopt if parsing failed
     */
    static std::optional<ExtraInfo> fromBuffer(const uint8_t *extraInfoPtr, uint32_t suppliedFeatureCode = 0);

    /**
     * @brief Convert this ExtraInfo to a binary buffer
     *
     * @param buffer Pointer to the buffer to write to
     * @return int Number of bytes written
     */
    int toBuffer(uint8_t *buffer) const;

    /**
     * @brief Convert this ExtraInfo to a binary buffer
     *
     * @return std::vector<uint8_t> The binary representation
     */
    [[nodiscard]] std::vector<uint8_t> toBuffer() const;

    /**
     * @brief Set the length field
     *
     * @param length The length value to set
     */
    void setLength(uint16_t length);

    /**
     * @brief Set the version field
     *
     * @param version The version value to set
     */
    void setVersion(uint64_t version);

    /**
     * @brief Set the ROM MAC address
     *
     * @param addr_rom The MAC address to set
     */
    void setMacaddr_rom(const uint8_t addr_rom[6]);

    /**
     * @brief Set the current MAC address
     *
     * @param addr_cur The MAC address to set
     */
    void setMacaddr_cur(const uint8_t addr_cur[6]);

    /**
     * @brief Set the channel selection
     *
     * @param chansel The channel selection value to set
     */
    void setChansel(uint32_t chansel);

    /**
     * @brief Set the band mode
     *
     * @param bmode The band mode to set (0 for 5GHz, 1 for 2.4GHz)
     */
    void setBmode(uint8_t bmode);

    /**
     * @brief Set the TX chain mask
     *
     * @param txChainMask The TX chain mask to set
     */
    void setTxChainMask(uint8_t txChainMask);

    /**
     * @brief Set the RX chain mask
     *
     * @param rxChainMaskV The RX chain mask to set
     */
    void setRxChainMask(uint8_t rxChainMaskV);

    /**
     * @brief Set the TX power
     *
     * @param txpowerV The TX power to set
     */
    void setTxpower(uint8_t txpowerV);

    /**
     * @brief Set the carrier frequency
     *
     * @param cf The carrier frequency to set
     */
    void setCf(uint64_t cf);

    /**
     * @brief Set the TX TSF
     *
     * @param txTsf The TX TSF to set
     */
    void setTxTsf(uint64_t txTsf);

    /**
     * @brief Set the last hardware TX TSF
     *
     * @param lastHwTxTsf The last hardware TX TSF to set
     */
    void setLastHwTxTsf(uint64_t lastHwTxTsf);

    /**
     * @brief Set the channel flags
     *
     * @param channelFlags The channel flags to set
     */
    void setChannelFlags(uint16_t channelFlags);

    /**
     * @brief Set the TX ness
     *
     * @param txNess The TX ness to set
     */
    void setTxNess(uint8_t txNess);

    /**
     * @brief Set the tuning policy
     *
     * @param tuningPolicy The tuning policy to set
     */
    void setTuningPolicy(uint8_t tuningPolicy);

    /**
     * @brief Set the PLL rate
     *
     * @param pllRate The PLL rate to set
     */
    void setPllRate(uint16_t pllRate);

    /**
     * @brief Set the PLL reference divider
     *
     * @param pllRefdiv The PLL reference divider to set
     */
    void setPllRefdiv(uint8_t pllRefdiv);

    /**
     * @brief Set the PLL clock select
     *
     * @param pllClockSelect The PLL clock select to set
     */
    void setPllClockSelect(uint8_t pllClockSelect);

    /**
     * @brief Set the AGC value
     *
     * @param agc The AGC value to set
     */
    void setAgc(uint8_t agc);

    /**
     * @brief Set the antenna selection
     *
     * @param ant_sel The antenna selection values to set
     */
    void setAntennaSelection(const uint8_t ant_sel[3]);

    /**
     * @brief Set the sampling rate
     *
     * @param sf The sampling rate to set
     */
    void setSamplingRate(double sf);

    /**
     * @brief Set the carrier frequency offset
     *
     * @param cfo The carrier frequency offset to set
     */
    void setCFO(int32_t cfo);

    /**
     * @brief Set the sampling frequency offset
     *
     * @param sfo The sampling frequency offset to set
     */
    void setSFO(int32_t sfo);

    /**
     * @brief Set the temperature
     *
     * @param temperature The temperature to set
     */
    void setTemperature(int8_t temperature);

    /**
     * @brief Convert this ExtraInfo to a string representation
     *
     * @return std::string String representation of this ExtraInfo
     */
    [[nodiscard]] std::string toString() const;
};

/**
 * @brief Stream insertion operator for ExtraInfo
 *
 * @param os Output stream
 * @param extraInfo ExtraInfo object to insert
 * @return std::ostream& Reference to the output stream
 */
std::ostream &operator<<(std::ostream &os, const ExtraInfo &extraInfo);

/**
 * @brief Test the presence of version field in a feature code
 *
 * @param featureCode The 32-bit feature code
 * @return true if the version field is present, false otherwise
 */
inline bool extraInfoHasVersion(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 0u & 0x1U);
}

/**
 * @brief Test the presence of length field in a feature code
 *
 * @details Please note: in the ExtraInfo struct, the length value does not include the length field itself.
 *
 * @param featureCode the 32-bit feature code
 * @return true if the length field is present, false otherwise
 */
inline bool extraInfoHasLength(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 1U & 0x1U);
}

/**
 * @brief Test the presence of "current MAC Address" field in a feature code
 *
 * @details The "current" MAC address is the mac address attached to the working interface,
 * this MAC address can be modified using bash commands.
 *
 * @param featureCode the 32-bit feature code
 * @return true if the current MAC address field is present, false otherwise
 *
 * @see extraInfoHasMacAddress_Rom
 */
inline bool extraInfoHasMacAddress_Current(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 2U & 0x1U);
}

/**
 * @brief Test the presence of "Permanent MAC Address" field in a feature code
 *
 * @details "Permanent MAC Address" is the mac address stored in the H/W's EEPROM, which cannot be modified.
 *
 * @param featureCode the 32-bit feature code
 * @return true if the permanent MAC address field is present, false otherwise
 *
 * @see extraInfoHasMacAddress_Current
 */
inline bool extraInfoHasMacAddress_Rom(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 3U & 0x1U);
}

/**
 * @brief Test the presence of "Channel Selection" field in a feature code
 *
 * @details Chansel value is used in Ath9k driver to configure the carrier frequency
 *
 * @param featureCode the 32-bit feature code
 * @return true if the channel selection field is present, false otherwise
 *
 * @see extraInfoHasBMode
 */
inline bool extraInfoHasChansel(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 4U & 0x1U);
}

/**
 * @brief Test the presence of "Bmode" field in a feature code
 *
 * @details Bmode value is used together with Chansel value in Ath9k driver to configure the carrier frequency.
 * 1 for 2.4GHz band, 0 for 5GHz band.
 *
 * @param featureCode the 32-bit feature code
 * @return true if the band mode field is present, false otherwise
 *
 * @see extraInfoHasChansel
 */
inline bool extraInfoHasBMode(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 5U & 0x1U);
}

/**
 * @brief Test the presence of EVM data in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the EVM data field is present, false otherwise
 */
inline bool extraInfoHasEVM(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 6U & 0x1U);
}

/**
 * @brief Test the presence of Tx ChainMask in a feature code
 *
 * @details Tx/Rx chainmask is a 3-bit value. Each bit stands for a radio chain.
 * 1 for first chain, 4 for last chain.
 * The chainmask bits combination MUST be in low to high order.
 *
 * @param featureCode the 32-bit feature code
 * @return true if the TX chain mask field is present, false otherwise
 */
inline bool extraInfoHasTxChainMask(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 7U & 0x1U);
}

/**
 * @brief Test the presence of Rx ChainMask in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the RX chain mask field is present, false otherwise
 */
inline bool extraInfoHasRxChainMask(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 8U & 0x1U);
}

/**
 * @brief Test the presence of Tx Power in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the TX power field is present, false otherwise
 */
inline bool extraInfoHasTxPower(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 9U & 0x1U);
}

/**
 * @brief Test the presence of Carrier Frequency in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the carrier frequency field is present, false otherwise
 */
inline bool extraInfoHasCF(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 10U & 0x1U);
}

/**
 * @brief Test the presence of Tx TSF in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the TX TSF field is present, false otherwise
 */
inline bool extraInfoHasTxTSF(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 11U & 0x1U);
}

/**
 * @brief Test the presence of Last Hardware Tx TSF in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the last hardware TX TSF field is present, false otherwise
 */
inline bool extraInfoHasLastHWTxTSF(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 12U & 0x1U);
}

/**
 * @brief Test the presence of Channel Flags in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the channel flags field is present, false otherwise
 */
inline bool extraInfoHasChannelFlags(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 13U & 0x1U);
}

/**
 * @brief Test the presence of Tx Ness in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the TX ness field is present, false otherwise
 */
inline bool extraInfoHasTxNess(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 14U & 0x1U);
}

/**
 * @brief Test the presence of Tuning Policy in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the tuning policy field is present, false otherwise
 */
inline bool extraInfoHasTuningPolicy(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 15U & 0x1U);
}

/**
 * @brief Test the presence of PLL Rate in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the PLL rate field is present, false otherwise
 */
inline bool extraInfoHasPLLRate(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 16U & 0x1U);
}

/**
 * @brief Test the presence of PLL Reference Divider in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the PLL reference divider field is present, false otherwise
 */
inline bool extraInfoHasPLLRefDiv(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 17U & 0x1U);
}

/**
 * @brief Test the presence of PLL Clock Select in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the PLL clock select field is present, false otherwise
 */
inline bool extraInfoHasPLLClkSel(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 18U & 0x1U);
}

/**
 * @brief Test the presence of AGC in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the AGC field is present, false otherwise
 */
inline bool extraInfoHasAGC(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 19U & 0x1U);
}

/**
 * @brief Test the presence of Antenna Selection in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the antenna selection field is present, false otherwise
 */
inline bool extraInfoHasAntennaSelection(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 20U & 0x1U);
}

/**
 * @brief Test the presence of Sampling Rate in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the sampling rate field is present, false otherwise
 */
inline bool extraInfoHasSamplingRate(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 21U & 0x1U);
}

/**
 * @brief Test the presence of Carrier Frequency Offset in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the carrier frequency offset field is present, false otherwise
 */
inline bool extraInfoHasCFO(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 22U & 0x1U);
}

/**
 * @brief Test the presence of Sampling Frequency Offset in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the sampling frequency offset field is present, false otherwise
 */
inline bool extraInfoHasSFO(const uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 23U & 0x1U);
}

/**
 * @brief Test the presence of Temperature in a feature code
 *
 * @param featureCode the 32-bit feature code
 * @return true if the temperature field is present, false otherwise
 */
inline int8_t extraInfoHasTemperature(const uint32_t featureCode) {
    return static_cast<int8_t>(featureCode >> 24U & 0x1U);
}

/**
 * @brief Enumeration for RxS parsing levels
 *
 * @details Defines different levels of detail for parsing RxS (Received Signal) data
 */
enum class RXSParsingLevel : uint8_t {
    BASIC_NOEXTRA_NOCSI = 10,  /**< Basic parsing without extra info or CSI */
    EXTRA_NOCSI,               /**< Parsing with extra info but without CSI */
    EXTRA_CSI,                 /**< Parsing with extra info and CSI */
    EXTRA_CSI_UNWRAP           /**< Parsing with extra info, CSI, and unwrapped CSI */
};

/**
 * @brief Stream insertion operator for AtherosCFTuningPolicy
 *
 * @param os Output stream
 * @param cfTuningPolicy Tuning policy to insert
 * @return std::ostream& Reference to the output stream
 */
inline std::ostream &operator<<(std::ostream &os, const AtherosCFTuningPolicy &cfTuningPolicy) {
    os << TuningPolicy2String(cfTuningPolicy);
    return os;
}

/**
 * @brief Convert channel flags to channel mode
 *
 * @param channelFlags The channel flags to convert
 * @return ChannelModeEnum The corresponding channel mode
 */
inline ChannelModeEnum channelFlags2ChannelMode(const uint16_t channelFlags) {
    std::bitset<16> channelFlagSet(channelFlags);

    if (channelFlagSet.test(3) && channelFlagSet.test(4)) {
        return ChannelModeEnum::HT40_PLUS;
    }

    if (channelFlagSet.test(3) && channelFlagSet.test(5)) {
        return ChannelModeEnum::HT40_MINUS;
    }

    if (channelFlagSet.test(3)) {
        return ChannelModeEnum::HT20;
    }

    return ChannelModeEnum::HT20;
}

/**
 * @brief Parse 32-bit feature code into has* values of struct ExtraInfo
 *
 * @param featureCode the input 32-bit feature code
 * @param extraInfo The ExtraInfo to be modified to reflect the feature code
 */
void featureCodeInterpretation(uint32_t featureCode, struct ExtraInfo *extraInfo);


/**
 * @brief Directly inject (in-place add) an ExtraInfo Item into the raw RxS data
 *
 * @details The transparency to the upper RxS parsing process makes this method very useful
 * in case of adding some platform-calculated value into RXS struct.
 *
 * @param rxs_raw the raw rxs data
 * @param featureCode_added the feature code for the adding value
 * @param data pointer to the value to be added
 * @param length value length
 */
void inplaceAddRxExtraInfo(uint8_t *rxs_raw, uint32_t featureCode_added, uint8_t *data, int length);


#endif //PICOSCENES_PLATFROM_RXSEXTRAINFO_H
