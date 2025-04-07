//
// Created by Zhiping Jiang on 11/27/22.
//

#ifndef PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX
#define PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX

#include "PicoScenesCommons.hxx"

class FrontEndModePreset {
public:
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_320_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 320". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 160". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 80". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HT; ///< Equivalent to "--format ht --coding bcc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 40". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_EHTSU_LDPC; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HESU_LDPC; ///< Equivalent to "--format hesu --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HT; ///< Equivalent to "--format ht --coding bcc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_NonHT; ///< Equivalent to "--format nonht --coding bcc --cbw 20". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> RX_CBW_320; ///< Equivalent to "--rx-cbw 320". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> RX_CBW_160; ///< Equivalent to "--rx-cbw 160". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> RX_CBW_80; ///< Equivalent to "--rx-cbw 80". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> RX_CBW_40; ///< Equivalent to "--rx-cbw 40". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> RX_CBW_20; ///< Equivalent to "--rx-cbw 20". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_320_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 320 --rx-cbw 320". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 160 --rx-cbw 160". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 80 --rx-cbw 80". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HT; ///< Equivalent to "--format ht --coding bcc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 40 --rx-cbw 40". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_EHTSU_LDPC; ///< Equivalent to "--format ehtsu --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_EHTSU; ///< Equivalent to "--format ehtsu --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HESU; ///< Equivalent to "--format hesu --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HESU_LDPC; ///< Equivalent to "--format hesu --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_VHT; ///< Equivalent to "--format vht --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HT; ///< Equivalent to "--format ht --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_NonHT; ///< Equivalent to "--format nonht --coding bcc --cbw 20 --rx-cbw 20". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.
    static const std::shared_ptr<FrontEndModePreset> DEFAULT; ///< The default profile, equivalent to TR_CBW_20_HT. Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices.

    static const std::vector<std::shared_ptr<FrontEndModePreset>> &getPresetList();

    static const std::map<std::string, std::shared_ptr<FrontEndModePreset>> &getPresetMap();

    static std::string printHelpContentForFrontEndModePreset();

    FrontEndModePreset() = delete;

    bool operator==(const FrontEndModePreset &rhs) const;

    bool operator<(const FrontEndModePreset &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const FrontEndModePreset &preset);

    const std::string presetName{};
    const PacketFormatEnum format{PacketFormatEnum::PacketFormat_Unknown};
    const ChannelCodingEnum coding{ChannelCodingEnum::BCC};
    const std::optional<double> txCBW{std::nullopt};
    const std::optional<double> rxCBW{std::nullopt};
    const std::string description{};

private:

    FrontEndModePreset(std::string presetName, PacketFormatEnum format, ChannelCodingEnum coding, const std::optional<double> &txCbw, const std::optional<double> &rxCbw, std::string description);
};


#endif //PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX
