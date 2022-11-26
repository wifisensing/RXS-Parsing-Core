//
// Created by 蒋志平 on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
#define PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX

#include "PicoScenesCommons.hxx"

class TxPrecodingParameters {
public:
    SignalMatrix<std::complex<double>> userSpecifiedSpatialMappingMatrix;

    double simulateTxDistance{0};

    SignalMatrix<std::complex<double>> rxCSI4TxBeamforming; /// N_{sc}-by-N_{tx_sts}-by-N_{rx}
    uint8_t txNumSTS4TxBeamforming{1};

    std::vector<std::array<double, 3>> antennaPositions;
    double txSteeringAngles4PhasedArray{0};
    double carrierFrequency{0};

    [[nodiscard]] inline bool isActivated() const {
        return !userSpecifiedSpatialMappingMatrix.empty() || simulateTxDistance != 0 || !rxCSI4TxBeamforming.empty() || (txSteeringAngles4PhasedArray != 0 && !antennaPositions.empty());
    }
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
    double hardwareSamplingRate;
    double samplingRateOffset;
    double carrierFrequencyOffset;
    double resampleRatio;
    double simulateTxDistance;
    double vhtGroupId;
    std::vector<uint8_t> heAllocationIndex;
    bool txHEExtendedRange;
    bool heHighDoppler;
    double heMidamblePeriodicity;
    double heLTFType;
    bool hePreHESpatialMapping;
    double additiveNoiseVarianceDb;
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
        hardwareSamplingRate = 20e6;
        samplingRateOffset = 0;
        carrierFrequencyOffset = 0;
        resampleRatio = 1.0;
        simulateTxDistance = 0;
        vhtGroupId = 0;
        heAllocationIndex = std::vector<uint8_t>(1, 0);
        txHEExtendedRange = false;
        heHighDoppler = false;
        heMidamblePeriodicity = 10;
        heLTFType = 4;
        hePreHESpatialMapping = false;
        additiveNoiseVarianceDb = -45;
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
            case FrontEndModePreset::TR_CBW_160_HESU:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_160_HESU:
                hardwareSamplingRate = 200e6;
                cbw = ChannelBandwidthEnum::CBW_160;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_160_VHT_LDPC:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_160_VHT_LDPC:
                hardwareSamplingRate = 200e6;
                cbw = ChannelBandwidthEnum::CBW_160;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_160_VHT:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_160_VHT:
                hardwareSamplingRate = 200e6;
                cbw = ChannelBandwidthEnum::CBW_160;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_80_HESU:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_80_HESU:
                hardwareSamplingRate = 100e6;
                cbw = ChannelBandwidthEnum::CBW_80;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_80_VHT:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_80_VHT:
                hardwareSamplingRate = 100e6;
                cbw = ChannelBandwidthEnum::CBW_80;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_80_VHT_LDPC:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_80_VHT_LDPC:
                hardwareSamplingRate = 100e6;
                cbw = ChannelBandwidthEnum::CBW_80;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_40_RESAMPLE_HESU:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_HESU:
                hardwareSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT:
                hardwareSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_40_RESAMPLE_VHT_LDPC:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_VHT_LDPC:
                hardwareSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_40_RESAMPLE_HT:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT:
                hardwareSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_40_RESAMPLE_HT_LDPC:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_RESAMPLE_HT_LDPC:
                hardwareSamplingRate = 50e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.25;
                break;
            case FrontEndModePreset::TR_CBW_40_HESU:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_HESU:
                hardwareSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HESU;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::TR_CBW_40_VHT:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_VHT:
                hardwareSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::TR_CBW_40_VHT_LDPC:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_VHT_LDPC:
                hardwareSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_VHT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::TR_CBW_40_HT:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_HT:
                hardwareSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::BCC);
                resampleRatio = 1.0;
                break;
            case FrontEndModePreset::TR_CBW_40_HT_LDPC:
                [[fallthrough]];
            case FrontEndModePreset::TX_CBW_40_HT_LDPC:
                hardwareSamplingRate = 40e6;
                cbw = ChannelBandwidthEnum::CBW_40;
                frameType = PacketFormatEnum::PacketFormat_HT;
                coding = std::vector<ChannelCodingEnum>(1, ChannelCodingEnum::LDPC);
                resampleRatio = 1.0;
                break;
            default:
                break;
        }
    }

    void validate() const;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameTxParameters &parameters);

#endif //PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
