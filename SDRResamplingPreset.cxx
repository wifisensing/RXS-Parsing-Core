//
// Created by csi on 11/27/22.
//

#include <mutex>
#include <sstream>
#include "SDRResamplingPreset.hxx"

SDRResamplingPreset SDRResamplingPreset::Preset_TX_20_20 = SDRResamplingPreset("Tx_20_20", 20, 20, std::nullopt, std::nullopt, "Equivalent to \"--rate 20e6 --tx-resample-ratio 1.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_20_25 = SDRResamplingPreset("Tx_20_25", 20, 25, std::nullopt, std::nullopt, "Equivalent to \"--rate 25e6 --tx-resample-ratio 1.25\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_20_40 = SDRResamplingPreset("Tx_20_40", 20, 40, std::nullopt, std::nullopt, "Equivalent to \"--rate 40e6 --tx-resample-ratio 2.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_20_50 = SDRResamplingPreset("Tx_20_50", 20, 50, std::nullopt, std::nullopt, "Equivalent to \"--rate 50e6 --tx-resample-ratio 2.5\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_20_100 = SDRResamplingPreset("Tx_20_100", 20, 100, std::nullopt, std::nullopt, "Equivalent to \"--rate 100e6 --tx-resample-ratio 5\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_20_200 = SDRResamplingPreset("Tx_20_200", 20, 200, std::nullopt, std::nullopt, "Equivalent to \"--rate 200e6 --tx-resample-ratio 10\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_40_40 = SDRResamplingPreset("Tx_40_40", 40, 40, std::nullopt, std::nullopt, "Equivalent to \"--rate 40e6 --tx-resample-ratio 1.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_40_50 = SDRResamplingPreset("Tx_40_50", 40, 50, std::nullopt, std::nullopt, "Equivalent to \"--rate 40e6 --tx-resample-ratio 1.25\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_40_100 = SDRResamplingPreset("Tx_40_100", 40, 100, std::nullopt, std::nullopt, "Equivalent to \"--rate 40e6 --tx-resample-ratio 2.5\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_40_200 = SDRResamplingPreset("Tx_40_200", 40, 200, std::nullopt, std::nullopt, "Equivalent to \"--rate 40e6 --tx-resample-ratio 5.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_80_100 = SDRResamplingPreset("Tx_80_100", 80, 100, std::nullopt, std::nullopt, "Equivalent to \"--rate 100e6 --tx-resample-ratio 1.25\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_80_200 = SDRResamplingPreset("Tx_80_200", 80, 200, std::nullopt, std::nullopt, "Equivalent to \"--rate 200e6 --tx-resample-ratio 2.5\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TX_160_200 = SDRResamplingPreset("Tx_160_200", 160, 200, std::nullopt, std::nullopt, "Equivalent to \"--rate 200e6 --tx-resample-ratio 1.25\"");

SDRResamplingPreset SDRResamplingPreset::Preset_RX_20_20 = SDRResamplingPreset("RX_20_20", std::nullopt, std::nullopt, 20, 20, "Equivalent to \"--rate 20e6 --rx-resample-ratio 1.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_20_25 = SDRResamplingPreset("RX_20_25", std::nullopt, std::nullopt, 20, 25, "Equivalent to \"--rate 25e6 --rx-resample-ratio 0.8\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_20_40 = SDRResamplingPreset("RX_20_40", std::nullopt, std::nullopt, 20, 40, "Equivalent to \"--rate 40e6 --rx-resample-ratio 0.5\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_20_50 = SDRResamplingPreset("RX_20_50", std::nullopt, std::nullopt, 20, 50, "Equivalent to \"--rate 50e6 --rx-resample-ratio 0.4\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_20_100 = SDRResamplingPreset("RX_20_100", std::nullopt, std::nullopt, 20, 100, "Equivalent to \"--rate 100e6 --rx-resample-ratio 0.2\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_20_200 = SDRResamplingPreset("RX_20_200", std::nullopt, std::nullopt, 20, 200, "Equivalent to \"--rate 200e6 --rx-resample-ratio 0.1\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_40_40 = SDRResamplingPreset("RX_40_40", std::nullopt, std::nullopt, 40, 40, "Equivalent to \"--rate 40e6 --rx-resample-ratio 1.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_40_50 = SDRResamplingPreset("RX_40_50", std::nullopt, std::nullopt, 40, 50, "Equivalent to \"--rate 50e6 --rx-resample-ratio 0.8\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_40_100 = SDRResamplingPreset("RX_40_100", std::nullopt, std::nullopt, 40, 100, "Equivalent to \"--rate 100e6 --rx-resample-ratio 0.4\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_40_200 = SDRResamplingPreset("RX_40_200", std::nullopt, std::nullopt, 40, 200, "Equivalent to \"--rate 200e6 --rx-resample-ratio 0.2\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_80_100 = SDRResamplingPreset("RX_80_100", std::nullopt, std::nullopt, 80, 100, "Equivalent to \"--rate 100e6 --rx-resample-ratio 0.8\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_80_200 = SDRResamplingPreset("RX_80_200", std::nullopt, std::nullopt, 80, 200, "Equivalent to \"--rate 200e6 --rx-resample-ratio 0.4\"");
SDRResamplingPreset SDRResamplingPreset::Preset_RX_160_200 = SDRResamplingPreset("RX_160_200", std::nullopt, std::nullopt, 160, 200, "Equivalent to \"--rate 200e6 --rx-resample-ratio 0.8\"");

SDRResamplingPreset SDRResamplingPreset::Preset_TR_20_20 = SDRResamplingPreset("TR_20_20", 20, 20, 20, 20, "Equivalent to \"--rate 20e6 --tx-resample-ratio 1.0 --rx-resample-ratio 1.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_20_25 = SDRResamplingPreset("TR_20_25", 20, 25, 20, 25, "Equivalent to \"--rate 25e6 --tx-resample-ratio 1.25 --rx-resample-ratio 0.8\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_20_40 = SDRResamplingPreset("TR_20_40", 20, 40, 20, 40, "Equivalent to \"--rate 40e6 --tx-resample-ratio 2.0 --rx-resample-ratio 0.5\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_20_50 = SDRResamplingPreset("TR_20_50", 20, 50, 20, 50, "Equivalent to \"--rate 50e6 --tx-resample-ratio 2.5 --rx-resample-ratio 0.4\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_20_100 = SDRResamplingPreset("TR_20_100", 20, 100, 20, 100, "Equivalent to \"--rate 100e6 --tx-resample-ratio 5 --rx-resample-ratio 0.2\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_20_200 = SDRResamplingPreset("TR_20_200", 20, 200, 20, 200, "Equivalent to \"--rate 200e6 --tx-resample-ratio 10 --rx-resample-ratio 0.1\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_40_40 = SDRResamplingPreset("TR_40_40", 40, 40, 40, 40, "Equivalent to \"--rate 40e6 --tx-resample-ratio 1.0 --rx-resample-ratio 1.0\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_40_50 = SDRResamplingPreset("TR_40_50", 40, 50, 40, 50, "Equivalent to \"--rate 50e6 --tx-resample-ratio 1.25 --rx-resample-ratio 0.8\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_40_100 = SDRResamplingPreset("TR_40_100", 40, 100, 40, 100, "Equivalent to \"--rate 10e6 --tx-resample-ratio 2.5 --rx-resample-ratio 0.4\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_40_200 = SDRResamplingPreset("TR_40_20", 40, 200, 40, 200, "Equivalent to \"--rate 200e6 --tx-resample-ratio 5.0 --rx-resample-ratio 0.2\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_80_100 = SDRResamplingPreset("TR_80_100", 80, 100, 80, 100, "Equivalent to \"--rate 100e6 --tx-resample-ratio 1.25 --rx-resample-ratio 0.8\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_80_200 = SDRResamplingPreset("TR_80_200", 80, 200, 80, 200, "Equivalent to \"--rate 200e6 --tx-resample-ratio 2.5 --rx-resample-ratio 0.4\"");
SDRResamplingPreset SDRResamplingPreset::Preset_TR_160_200 = SDRResamplingPreset("TR_160_200", 160, 200, 160, 200, "Equivalent to \"--rate 200e6 --tx-resample-ratio 1.25 --rx-resample-ratio 0.8\"");

SDRResamplingPreset::SDRResamplingPreset(std::string presetName, const std::optional<double> &txCbw,
                                         const std::optional<double> &txSamplingRate, const std::optional<double> &rxCbw,
                                         const std::optional<double> &rxSamplingRate, std::string description) : presetName(std::move(presetName)), txCBW(txCbw), txSamplingRate(txSamplingRate), rxCBW(rxCbw),
                                                                                                                 rxSamplingRate(rxSamplingRate), description(std::move(description)),
                                                                                                                 txResampleRatio(txCBW && txSamplingRate ? std::make_optional(*txSamplingRate / *txCBW) : std::nullopt),
                                                                                                                 rxResampleRatio(rxCBW && rxSamplingRate ? std::make_optional(*rxCBW / *rxSamplingRate) : std::nullopt) {}

bool SDRResamplingPreset::operator==(const SDRResamplingPreset &rhs) const {
    return std::tie(presetName, txCBW, txSamplingRate, rxCBW, rxSamplingRate) == std::tie(rhs.presetName, rhs.txCBW, rhs.txSamplingRate, rhs.rxCBW, rhs.rxSamplingRate);
}

bool SDRResamplingPreset::operator<(const SDRResamplingPreset &rhs) const {
    return std::tie(txCBW, txSamplingRate, rxCBW, rxSamplingRate) < std::tie(rhs.txCBW, rhs.txSamplingRate, rhs.rxCBW, rhs.rxSamplingRate);
}

std::ostream &operator<<(std::ostream &os, const SDRResamplingPreset &preset) {
    os << "presetName: " << preset.presetName << " txCBW: " << (preset.txCBW ? std::to_string(*preset.txCBW) : "null") << " txSamplingRate: " << (preset.txSamplingRate ? std::to_string(*preset.txSamplingRate) : "null") << " rxCBW: " << (preset.rxCBW ? std::to_string(*preset.rxCBW) : "null") << " rxSamplingRate: " << (preset.rxSamplingRate ? std::to_string(*preset.rxSamplingRate) : "null");
    return os;
}

const std::vector<SDRResamplingPreset> &SDRResamplingPreset::getPresetList() {
    static auto presetList = std::vector<SDRResamplingPreset>{
            Preset_TX_20_20,
            Preset_TX_20_25,
            Preset_TX_20_40,
            Preset_TX_20_50,
            Preset_TX_20_100,
            Preset_TX_20_200,
            Preset_TX_40_40,
            Preset_TX_40_50,
            Preset_TX_40_100,
            Preset_TX_40_200,
            Preset_TX_80_100,
            Preset_TX_80_200,
            Preset_TX_160_200,

            Preset_RX_20_20,
            Preset_RX_20_25,
            Preset_RX_20_40,
            Preset_RX_20_50,
            Preset_RX_20_100,
            Preset_RX_20_200,
            Preset_RX_40_40,
            Preset_RX_40_50,
            Preset_RX_40_100,
            Preset_RX_40_200,
            Preset_RX_80_100,
            Preset_RX_80_200,
            Preset_RX_160_200,

            Preset_TR_20_20,
            Preset_TR_20_25,
            Preset_TR_20_40,
            Preset_TR_20_50,
            Preset_TR_20_100,
            Preset_TR_20_200,
            Preset_TR_40_40,
            Preset_TR_40_50,
            Preset_TR_40_100,
            Preset_TR_40_200,
            Preset_TR_80_100,
            Preset_TR_80_200,
            Preset_TR_160_200,
    };

    return presetList;
}

const std::map<std::string, SDRResamplingPreset> &SDRResamplingPreset::getPresetMap() {
    static std::map<std::string, SDRResamplingPreset> presetMap;
    static std::once_flag flag;
    std::call_once(flag, [&] {
        for (const auto &preset: getPresetList())
            presetMap.emplace(preset.presetName, preset);
    });

    return presetMap;
}

std::string SDRResamplingPreset::printHelpContentForSDRResamplingPreset() {
    std::stringstream ss;
    ss << "\n\nPicoScenes supports the following SDR resampling presets:\n";
    ss << "-------------------------------------------------------------------------------------\n";
    for (const auto &preset: getPresetList())
        ss << preset.presetName << ": " << preset.description << "\n";
    ss << "-------------------------------------------------------------------------------------\n\n";

    return ss.str();
}
