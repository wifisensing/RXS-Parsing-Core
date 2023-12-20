//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
#define PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX

#include "PicoScenesCommons.hxx"
#include "FrontEndModePreset.hxx"
#include "SDRResamplingPreset.hxx"

class TxPrecodingParameters {
public:
    SignalMatrix<> userSpecifiedSpatialMappingMatrix;

    double simulateTxDistance{0};

    SignalMatrix<> rxCSI4TxBeamforming; /// N_{sc}-by-N_{tx_sts}-by-N_{rx}
    uint8_t txNumSTS4TxBeamforming{1};

    std::vector<std::array<double, 3>> antennaPositions;
    double txSteeringAngles4PhasedArray{0};
    double carrierFrequency{0};
};

struct PicoScenesFrameTxParameters {
    std::shared_ptr<FrontEndModePreset> preset{FrontEndModePreset::DEFAULT};
    bool NDPFrame{false};
    PacketFormatEnum frameType{PacketFormatEnum::PacketFormat_HT};
    std::vector<uint8_t> mcs{0};
    std::vector<uint8_t> numSTS{1};
    double numAntenna{1};
    uint16_t txcm{1};
    ChannelBandwidthEnum cbw{ChannelBandwidthEnum::CBW_20};
    GuardIntervalEnum guardInterval{GuardIntervalEnum::GI_800};
    std::vector<ChannelCodingEnum> coding{ChannelCodingEnum::BCC};
    bool preferAMPDU{false};
    uint8_t numExtraSounding{0};
    bool forceSounding{true};
    double prefixPaddingTime{8e-6};
    double idleTime{4e-6};
    double postfixPaddingTime{0};
    uint8_t scramblerState{0x27};
    double txIQAmplitudeImbalance_dB{0};
    double txIQPhaseImbalance_rad{0};
    double hardwareSamplingRate{20e6};
    double samplingRateOffset{0};
    double carrierFrequencyOffset{0};
    double resampleRatio{1.0};
    double simulateTxDistance{0};
    bool splitHighLow{false};
    uint8_t vhtGroupId{0};
    std::vector<uint8_t> heAllocationIndex{0};
    bool txHEExtendedRange{false};
    bool heHighDoppler{false};
    uint8_t heMidamblePeriodicity{10};
    uint8_t heLTFType{4};
    bool hePreHESpatialMapping{false};
    bool ehtDupMode{false};
    uint8_t ehtLTFType{4};
    std::optional<TxPrecodingParameters> precodingParameters = std::nullopt;

    void applyPreset(const std::string &presetName) {
        if (FrontEndModePreset::getPresetMap().find(presetName) == FrontEndModePreset::getPresetMap().cend())
            throw std::invalid_argument("invalid frontend mode preset name: " + presetName + "\n" + FrontEndModePreset::printHelpContentForFrontEndModePreset());
        applyPreset(FrontEndModePreset::getPresetMap().at(presetName));
    }

    void applyPreset(const std::shared_ptr<FrontEndModePreset> &presetV) {
        preset = presetV;
        if (preset->txSamplingRate && preset->txResampleRatio && preset->txCBW) {
            hardwareSamplingRate = preset->txSamplingRate.value();
            cbw = static_cast<ChannelBandwidthEnum>(preset->txCBW.value());
            frameType = preset->format;
            coding = std::vector<ChannelCodingEnum>{preset->coding};
            resampleRatio = preset->txResampleRatio.value();
            if (frameType == PacketFormatEnum::PacketFormat_HESU ||
                frameType == PacketFormatEnum::PacketFormat_EHTMU ||
                frameType == PacketFormatEnum::PacketFormat_EHTSU ||
                frameType == PacketFormatEnum::PacketFormat_EHTMU) {
                guardInterval = GuardIntervalEnum::GI_3200; // default to 3.2us GI for higher robustness
            }
        }
    }

    void validate() const {
        if (frameType == PacketFormatEnum::PacketFormat_NonHT) {
            if (NDPFrame)
                throw std::invalid_argument("NDP frame is only valid for VHT and HE-SU frame.");

            if (static_cast<int>(cbw) > static_cast<int>(ChannelBandwidthEnum::CBW_20))
                throw std::invalid_argument("Invalid Tx CBW: " + ChannelBandwidth2String(cbw) + " for NonHT frame.");

            if (coding[0] == ChannelCodingEnum::LDPC)
                throw std::invalid_argument("Invalid Tx Coding: " + ChannelCoding2String(coding[0]) + " for NonHT frame.");

            if (mcs[0] > 6)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for NonHT frame.");

            if (numSTS[0] > 1)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for NonHT frame.");

            if (guardInterval != GuardIntervalEnum::GI_800)
                throw std::invalid_argument("Invalid Tx GI: " + GuardInterval2String(guardInterval) + " for NonHT frame.");

            if (preferAMPDU)
                throw std::invalid_argument("AMPDU is not supported by NonHT format.");
        } else if (frameType == PacketFormatEnum::PacketFormat_HT) {
            if (NDPFrame)
                throw std::invalid_argument("NDP frame is only valid for VHT and HE-SU frame.");

            if (static_cast<int>(cbw) > static_cast<int>(ChannelBandwidthEnum::CBW_40))
                throw std::invalid_argument("Invalid Tx CBW: " + ChannelBandwidth2String(cbw) + " for HT frame.");

            if (mcs[0] > 7)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for HT frame.");

            if (numSTS[0] > 4)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for HT frame.");

            if (guardInterval > GuardIntervalEnum::GI_800)
                throw std::invalid_argument("Invalid Tx GI: " + GuardInterval2String(guardInterval) + " for HT frame.");
        } else if (frameType == PacketFormatEnum::PacketFormat_VHT) {
            if (mcs[0] > 9)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for VHT frame.");

            if (numSTS[0] > 8)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for VHT frame.");

            if (guardInterval > GuardIntervalEnum::GI_800)
                throw std::invalid_argument("Invalid Tx GI: " + GuardInterval2String(guardInterval) + " for VHT frame.");
        } else if (frameType == PacketFormatEnum::PacketFormat_HESU) {
            if (coding[0] == ChannelCodingEnum::BCC && cbw > ChannelBandwidthEnum::CBW_20)
                throw std::invalid_argument("Invalid Tx Coding: " + ChannelCoding2String(coding[0]) + " for HE-SU frame.");

            if (mcs[0] > 11)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for HE-SU frame.");

            if (numSTS[0] > 8)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for HE-SU frame.");

            if (txHEExtendedRange && numSTS[0] > 2)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for HE-EXT-SU frame. Must be smaller than 3.");

            if (txHEExtendedRange && mcs[0] > 2)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for HE-EXT-SU frame. Must be smaller than 3.");

            if (heHighDoppler && numSTS[0] > 4)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for HE-SU High-Doppler frame. Must be smaller than 4.");

            if (heHighDoppler && heMidamblePeriodicity != 10 && heMidamblePeriodicity != 20)
                throw std::invalid_argument("Invalid TX mid amble periodicity: " + std::to_string(heMidamblePeriodicity) + " for HE-SU frame. Must be 10 or 20.");

            if (guardInterval == GuardIntervalEnum::GI_400)
                throw std::invalid_argument("Invalid GI " + std::to_string(static_cast<uint16_t>(guardInterval)) + " for HE-SU frame. Must be 800, 1600 or 3200ns.");

            if (heLTFType == 4 && guardInterval == GuardIntervalEnum::GI_1600)
                throw std::invalid_argument("Invalid GI " + std::to_string(static_cast<uint16_t>(guardInterval)) + " for HE-SU HE-LTF compression mode 4. Must be 800 or 3200ns.");

            if ((heLTFType == 2 || heLTFType == 1) && guardInterval == GuardIntervalEnum::GI_3200)
                throw std::invalid_argument("Invalid GI " + std::to_string(static_cast<uint16_t>(guardInterval)) + " for HE-SU HE-LTF compression mode 1 or 2. Must be 800 or 1600ns.");
        } else if (frameType == PacketFormatEnum::PacketFormat_HEMU) {
            // TODO add Wi-Fi 6 MU section
        } else if (frameType == PacketFormatEnum::PacketFormat_EHTSU) {
            // TODO add Wi-Fi 7 section
        } else if (frameType == PacketFormatEnum::PacketFormat_EHTMU) {
            // TODO add Wi-Fi 7 MU section
        }
    }

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "tx_param[preset=" << (preset ? preset->presetName : "NULL") << ", type=" << PacketFormat2String(frameType) << ", CBW=" << ChannelBandwidth2String(cbw) << ", MCS=" << static_cast<int16_t>(mcs[0]) << ", numSTS=" << static_cast<int16_t>(numSTS[0]) << ", Coding=" << ChannelCoding2String(coding[0]) << ", GI=" << GuardInterval2String(guardInterval);
        if (frameType == PacketFormatEnum::PacketFormat_HT) {
            ss << ", numESS=" << numExtraSounding << ", sounding(11n)=" << forceSounding;
        } else if (frameType == PacketFormatEnum::PacketFormat_HESU) {
            ss << ", HiDoppler=" << int(heMidamblePeriodicity) << ", ER=" << txHEExtendedRange;
        } else if (frameType == PacketFormatEnum::PacketFormat_EHTSU) {
            // TODO add Wi-Fi 7 section
        }
        ss << "]";
        return ss.str();
    }
};

inline std::ostream& operator<<(std::ostream&os, const PicoScenesFrameTxParameters&parameters) {
    os << parameters.toString();
    return os;
}

#endif //PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
