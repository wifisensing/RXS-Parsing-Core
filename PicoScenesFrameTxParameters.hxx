//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
#define PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX

#include "PicoScenesCommons.hxx"

class TxPrecodingParameters {
public:
    bool precodingEnabled = false;

    SignalMatrix<std::complex<double>> userSpecifiedSpatialMappingMatrix;

    SignalMatrix<std::complex<double>> rxCSI4TxBeamforming; /// N_{sc}-by-N_{tx_sts}-by-N_{rx}
    uint8_t txNumSTS4TxBeamforming = 1;

    std::vector<std::array<double, 3>> antennaPositions{};
    double txSteeringAngles4PhasedArray = 0;
    double txCarrierFrequency4PhasedArray = 0;
    double subcarrierBandwidth4PhasedArray = 312.5e3;
    std::vector<int16_t> subcarrierIndices4PhasedArray{};
};

class PicoScenesFrameTxParameters {
public:
    FrontEndModePreset preset = FrontEndModePreset::Customized;
    std::optional<double> preciseTxTime = std::nullopt;
    bool NDPFrame;
    PacketFormatEnum frameType;
    std::vector<uint8_t> mcs;
    std::vector<uint8_t> numSTS;
    double numAntenna;
    uint16_t txcm;
    ChannelBandwidthEnum cbw;
    GuardIntervalEnum guardInterval;
    std::vector<ChannelCodingEnum> coding;
    double numExtraSounding;
    bool forceSounding;
    double prefixPaddingTime;
    double idleTime;
    double postfixPaddingTime;
    double scramblerState;
    double txIQAmplitudeImbalance_dB;
    double txIQPhaseImbalance_rad;
    bool maxPowerScaleTo1;
    double actualSamplingRate;
    double samplingRateOffset;
    double carrierFrequencyOffset;
    double resampleRatio;
    double vhtGroupId;
    std::vector<uint8_t> heAllocationIndex;
    bool txHEExtendedRange;
    bool heHighDoppler;
    double heMidamblePeriodicity;
    double heLTFType;
    TxPrecodingParameters precodingParameters;

    PicoScenesFrameTxParameters() {
        reset();
    }

    void reset() {
        NDPFrame = false;
        frameType = PacketFormatEnum::PacketFormat_HT;
        mcs = std::vector<uint8_t>(1, 0);
        numSTS = std::vector<uint8_t>(1, 1);
        numAntenna = 1;
        txcm = 1;
        cbw = ChannelBandwidthEnum::CBW_20;
        guardInterval = GuardIntervalEnum::GI_800;
        coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
        numExtraSounding = 0;
        forceSounding = true;
        prefixPaddingTime = 4e-6;
        idleTime = 4e-6;
        postfixPaddingTime = 0;
        scramblerState = 39;
        txIQAmplitudeImbalance_dB = 0;
        txIQPhaseImbalance_rad = 0;
        maxPowerScaleTo1 = true;
        actualSamplingRate = 20e6;
        samplingRateOffset = 0;
        carrierFrequencyOffset = 0;
        resampleRatio = 1.0;
        vhtGroupId = 0;
        heAllocationIndex = std::vector<uint8_t>(1, 0);
        txHEExtendedRange = false;
        heHighDoppler = false;
        heMidamblePeriodicity = 10;
        heLTFType = 4;
    }

    void applyPreset(const std::string &presetName) {
        const auto &names = FrontEndModePreset2Strings();
        if (auto found = std::find(names.cbegin(), names.cend(), presetName); found != names.cend()) {
            auto presetIndex = std::distance(names.cbegin(), found);
            auto preset = getAllFrontEndModePresets().at(presetIndex);
            applyPreset(preset);
        } else
            throw std::invalid_argument("invalid frontend mode preset name: " + presetName + "\n" + printHelpContentForFrontEndModePreset());
    }

    void applyPreset(FrontEndModePreset presetV) {
        preset = presetV;

        switch (preset) {
            case FrontEndModePreset::TX_CBW_160_HESU:
                actualSamplingRate = 200e6;
                cbw = ChannelBandwidthEnum::CBW_160;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_160_VHT_LDPC:
                actualSamplingRate = 200e6;
                cbw = ChannelBandwidthEnum::CBW_160;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_160_VHT:
                actualSamplingRate = 200e6;
                cbw = ChannelBandwidthEnum::CBW_160;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_80_HESU:
                actualSamplingRate = 100e6;
                cbw = ChannelBandwidthEnum::CBW_80;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_80_VHT:
                actualSamplingRate = 100e6;
                cbw = ChannelBandwidthEnum::CBW_80;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_80_VHT_LDPC:
                actualSamplingRate = 100e6;
                cbw = ChannelBandwidthEnum::CBW_80;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_HESU:
                actualSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT:
                actualSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT_LDPC:
                actualSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT:
                actualSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TX_CBW_40_HESU:
                actualSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::TX_CBW_40_VHT:
                actualSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::TX_CBW_40_VHT_LDPC:
                actualSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::RX_CBW_160:
                [[fallthrough]];
            case FrontEndModePreset::RX_CBW_80:
                [[fallthrough]];
            case FrontEndModePreset::RX_CBW_40:
                throw std::invalid_argument("PicoScenesFrameTxParameters::applyPreset method doesn't support Rx presets.");
            case FrontEndModePreset::Customized:
                break;
        }
    }

    void validate() const;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameTxParameters &parameters);

#endif //PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
