//
// Created by Zhiping Jiang on 11/27/22.
//

#ifndef PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX
#define PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX

#include "PicoScenesCommons.hxx"
#include "RxSParsingCoreDLLImport.hxx"

class FrontEndModePreset {
public:
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_320_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 400e6 --tx-resample-ratio 1.25 --cbw 320"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_VHT; ///< Equivalent to "--format vht --coding bcc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_160_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_VHT; ///< Equivalent to "--format vht --coding bcc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_80_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_VHT; ///< Equivalent to "--format vht --coding bcc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HT; ///< Equivalent to "--format ht --coding bcc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_40_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_EHTSU_LDPC; ///< Equivalent to "--format ehtsu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HESU_LDPC; ///< Equivalent to "--format hesu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_VHT; ///< Equivalent to "--format vht --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HT; ///< Equivalent to "--format ht --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TX_CBW_20_NonHT; ///< Equivalent to "--format nonht --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_320; ///< Equivalent to "--rate 200e6 --rx-resample-ratio 0.8 --rx-cbw 320"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_160; ///< Equivalent to "--rate 200e6 --rx-resample-ratio 0.8 --rx-cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_80; ///< Equivalent to "--rate 100e6 --rx-resample-ratio 0.8 --rx-cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_40; ///< Equivalent to "--rate 40e6 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> RX_CBW_20; ///< Equivalent to "--rate 20e6 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_320_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 400e6 --tx-resample-ratio 1.25 --cbw 320 --rx-resample-ratio 0.8 --rx-cbw 320"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160 --rx-resample-ratio 0.8 --rx-cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160 --rx-resample-ratio 0.8 --rx-cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_VHT; ///< Equivalent to "--format vht --coding bcc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160 --rx-resample-ratio 0.8 --rx-cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_160_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 200e6 --tx-resample-ratio 1.25 --cbw 160 --rx-resample-ratio 0.8 --rx-cbw 160"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80 --rx-resample-ratio 0.8 --rx-cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80 --rx-resample-ratio 0.8 --rx-cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_VHT; ///< Equivalent to "--format vht --coding bcc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80 --rx-resample-ratio 0.8 --rx-cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_80_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 100e6 --tx-resample-ratio 1.25 --cbw 80 --rx-resample-ratio 0.8 --rx-cbw 80"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_EHTSU; ///< Equivalent to "--format ehtsu --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_VHT; ///< Equivalent to "--format vht --coding bcc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HT; ///< Equivalent to "--format ht --coding bcc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_40_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --rate 40e6 --tx-resample-ratio 1.0 --cbw 40 --rx-resample-ratio 1.0 --rx-cbw 40"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_EHTSU_LDPC; ///< Equivalent to "--format ehtsu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_EHTSU; ///< Equivalent to "--format ehtsu --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HESU; ///< Equivalent to "--format hesu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HESU_LDPC; ///< Equivalent to "--format hesu --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_VHT; ///< Equivalent to "--format vht --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_VHT_LDPC; ///< Equivalent to "--format vht --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HT; ///< Equivalent to "--format ht --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_HT_LDPC; ///< Equivalent to "--format ht --coding ldpc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> TR_CBW_20_NonHT; ///< Equivalent to "--format nonht --coding bcc --rate 20e6 --tx-resample-ratio 1.0 --cbw 20 --rx-resample-ratio 1.0 --rx-cbw 20"
    RXS_PARSING_CORE_API static const std::shared_ptr<FrontEndModePreset> DEFAULT; ///< The default profile, equivalent to TR_CBW_20_HT
 
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
    const std::optional<double> txSamplingRate{std::nullopt};
    const std::optional<double> rxCBW{std::nullopt};
    const std::optional<double> rxSamplingRate{std::nullopt};
    const std::string description{};
    const std::optional<double> txResampleRatio{std::nullopt};
    const std::optional<double> rxResampleRatio{std::nullopt};

private:

    FrontEndModePreset(std::string presetName, PacketFormatEnum format, ChannelCodingEnum coding, const std::optional<double> &txCbw, const std::optional<double> &txSamplingRate, const std::optional<double> &rxCbw, const std::optional<double> &rxSamplingRate, std::string description);
};


#endif //PICOSCENES_PLATFORM_FRONTENDMODEPRESET_HXX
