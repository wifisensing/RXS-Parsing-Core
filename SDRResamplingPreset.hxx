//
// Created by csi on 11/27/22.
//

#ifndef PICOSCENES_PLATFORM_SDRRESAMPLINGPRESET_HXX
#define PICOSCENES_PLATFORM_SDRRESAMPLINGPRESET_HXX


#include <optional>
#include <iostream>
#include <map>
#include <vector>

class SDRResamplingPreset {
public:

    static SDRResamplingPreset Preset_TX_20_20;
    static SDRResamplingPreset Preset_TX_20_25;
    static SDRResamplingPreset Preset_TX_20_40;
    static SDRResamplingPreset Preset_TX_20_50;
    static SDRResamplingPreset Preset_TX_20_100;
    static SDRResamplingPreset Preset_TX_20_200;
    static SDRResamplingPreset Preset_TX_40_40;
    static SDRResamplingPreset Preset_TX_40_50;
    static SDRResamplingPreset Preset_TX_40_100;
    static SDRResamplingPreset Preset_TX_40_200;
    static SDRResamplingPreset Preset_TX_80_100;
    static SDRResamplingPreset Preset_TX_80_200;
    static SDRResamplingPreset Preset_TX_160_200;

    static SDRResamplingPreset Preset_RX_20_20;
    static SDRResamplingPreset Preset_RX_20_25;
    static SDRResamplingPreset Preset_RX_20_40;
    static SDRResamplingPreset Preset_RX_20_50;
    static SDRResamplingPreset Preset_RX_20_100;
    static SDRResamplingPreset Preset_RX_20_200;
    static SDRResamplingPreset Preset_RX_40_40;
    static SDRResamplingPreset Preset_RX_40_50;
    static SDRResamplingPreset Preset_RX_40_100;
    static SDRResamplingPreset Preset_RX_40_200;
    static SDRResamplingPreset Preset_RX_80_100;
    static SDRResamplingPreset Preset_RX_80_200;
    static SDRResamplingPreset Preset_RX_160_200;

    static SDRResamplingPreset Preset_TR_20_20;
    static SDRResamplingPreset Preset_TR_20_25;
    static SDRResamplingPreset Preset_TR_20_40;
    static SDRResamplingPreset Preset_TR_20_50;
    static SDRResamplingPreset Preset_TR_20_100;
    static SDRResamplingPreset Preset_TR_20_200;
    static SDRResamplingPreset Preset_TR_40_40;
    static SDRResamplingPreset Preset_TR_40_50;
    static SDRResamplingPreset Preset_TR_40_100;
    static SDRResamplingPreset Preset_TR_40_200;
    static SDRResamplingPreset Preset_TR_80_100;
    static SDRResamplingPreset Preset_TR_80_200;
    static SDRResamplingPreset Preset_TR_160_200;

    bool operator==(const SDRResamplingPreset &rhs) const;

    bool operator<(const SDRResamplingPreset &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const SDRResamplingPreset &preset);

    static const std::vector<SDRResamplingPreset> &getPresetList();

    static const std::map<std::string, SDRResamplingPreset> &getPresetMap();

    static std::string printHelpContentForSDRResamplingPreset();

private:
    explicit SDRResamplingPreset(std::string presetName, const std::optional<double> &txCbw = std::nullopt, const std::optional<double> &txSamplingRate = std::nullopt, const std::optional<double> &rxCbw = std::nullopt, const std::optional<double> &rxSamplingRate = std::nullopt, std::string description = "");

    const std::string presetName{};
    const std::optional<double> txCBW{std::nullopt};
    const std::optional<double> txSamplingRate{std::nullopt};
    const std::optional<double> rxCBW{std::nullopt};
    const std::optional<double> rxSamplingRate{std::nullopt};
    const std::string description{};
};


#endif //PICOSCENES_PLATFORM_SDRRESAMPLINGPRESET_HXX
