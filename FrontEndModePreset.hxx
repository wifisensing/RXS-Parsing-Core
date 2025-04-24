//
// Created by Zhiping Jiang on 11/27/22.
//

#ifndef PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX
#define PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX

#include "PicoScenesCommons.hxx"
#include "RxSParsingCoreDLLImport.hxx"

/**
 * @brief Class for managing front-end mode presets in PicoScenes
 *
 * @details This class provides predefined configurations for different wireless standards
 * and channel bandwidths. It supports:
 * - Various IEEE 802.11 standards (NonHT, HT, VHT, HESU, EHTSU)
 * - Different channel bandwidths (20MHz, 40MHz, 80MHz, 160MHz, 320MHz)
 * - Different coding schemes (BCC, LDPC)
 * - Separate TX and RX configurations
 * - Combined TX/RX (TR) configurations
 */
class FrontEndModePreset {
public:
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_320_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 320". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HT; ///< Equivalent to "--format ht --coding bcc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_EHTSU_LDPC; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HESU_LDPC; ///< Equivalent to "--format hesu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HT; ///< Equivalent to "--format ht --coding bcc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_NonHT; ///< Equivalent to "--format nonht --coding bcc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_320; ///< Equivalent to "--rx-cbw 320". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_160; ///< Equivalent to "--rx-cbw 160". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_80; ///< Equivalent to "--rx-cbw 80". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_40; ///< Equivalent to "--rx-cbw 40". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_20; ///< Equivalent to "--rx-cbw 20". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_320_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 320 --rx-cbw 320". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HT; ///< Equivalent to "--format ht --coding bcc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_EHTSU_LDPC; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_EHTSU; ///< Equivalent to "--format ehtsu --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HESU_LDPC; ///< Equivalent to "--format hesu --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HT; ///< Equivalent to "--format ht --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_NonHT; ///< Equivalent to "--format nonht --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> DEFAULT; ///< The default profile, equivalent to TR_CBW_20_HT. Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.

    /**
     * @brief Get list of all available presets
     * @return Reference to vector containing all preset instances
     */
    static const std::vector<std::shared_ptr<FrontEndModePreset>> &getPresetList();

    /**
     * @brief Get map of presets indexed by name
     * @return Reference to map of preset name to preset instance
     */
    static const std::map<std::string, std::shared_ptr<FrontEndModePreset>> &getPresetMap();

    /**
     * @brief Get help content describing all available presets
     * @return Formatted string containing preset descriptions
     */
    static std::string printHelpContentForFrontEndModePreset();

    FrontEndModePreset() = delete;

    bool operator==(const FrontEndModePreset &rhs) const;

    bool operator<(const FrontEndModePreset &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const FrontEndModePreset &preset);

    const std::string presetName{};                    ///< Name identifier for the preset
    const PacketFormatEnum format{PacketFormatEnum::PacketFormat_Unknown};  ///< Packet format (NonHT/HT/VHT/HESU/EHTSU)
    const ChannelCodingEnum coding{ChannelCodingEnum::BCC};  ///< Channel coding scheme (BCC/LDPC)
    const std::optional<double> txCBW{std::nullopt};   ///< Transmit channel bandwidth in MHz
    const std::optional<double> rxCBW{std::nullopt};   ///< Receive channel bandwidth in MHz
    const std::string description{};                   ///< Human-readable description of the preset

private:

    /**
     * @brief Private constructor for creating preset instances
     *
     * @param presetName Name identifier for the preset
     * @param format Packet format enumeration
     * @param coding Channel coding scheme
     * @param txCbw Transmit channel bandwidth (optional)
     * @param rxCbw Receive channel bandwidth (optional)
     * @param description Human-readable description
     */
    FrontEndModePreset(std::string presetName, PacketFormatEnum format, ChannelCodingEnum coding, const std::optional<double> &txCbw, const std::optional<double> &rxCbw, std::string description);
};


#endif //PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX
