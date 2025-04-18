//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
#define PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX

#include "PicoScenesCommons.hxx"
#include "FrontEndModePreset.hxx"

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
    double inBasebandPostfixPaddingTime{4e-6};
    double postfixPaddingTime{0};
    uint8_t scramblerState{0x27};
    double txIQAmplitudeImbalance_dB{0};
    double txIQPhaseImbalance_rad{0};
    std::optional<double> fixedPowerScale{std::nullopt};
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
        if (!FrontEndModePreset::getPresetMap().contains(presetName))
            throw std::invalid_argument("invalid frontend mode preset name: " + presetName + "\n" + FrontEndModePreset::printHelpContentForFrontEndModePreset());
        applyPreset(FrontEndModePreset::getPresetMap().at(presetName));
    }

    void applyPreset(const std::shared_ptr<FrontEndModePreset> &presetV) {
        preset = presetV;
        if (preset->txCBW) {
            cbw = static_cast<ChannelBandwidthEnum>(preset->txCBW.value());
            frameType = preset->format;
            coding = std::vector{preset->coding};
            if (frameType == PacketFormatEnum::PacketFormat_HESU ||
                frameType == PacketFormatEnum::PacketFormat_HEMU ||
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

            if (const auto requiredNumHTLTFs = [&] {
                switch (numSTS[0]) {
                    case 1:
                        return 1;
                    case 2:
                        return 2;
                    case 3:
                        [[fallthrough]];
                    default:
                        return 4;
                }
            }() + [&] {
                switch (numExtraSounding) {
                    case 0:
                        return 0;
                    case 1:
                        return 1;
                    case 2:
                        return 2;
                    case 3:
                        [[fallthrough]];
                    case 4:
                        return 4;
                    default:
                        return static_cast<int>(numExtraSounding);
                }
            }(); requiredNumHTLTFs > 5) {
                throw std::invalid_argument("Invalid number of Extra HT-LTF: " + std::to_string(numExtraSounding) + " for HT-format frame with " + std::to_string(numSTS[0]) + " STS.");
            }

            if (guardInterval > GuardIntervalEnum::GI_800)
                throw std::invalid_argument("Invalid Tx GI: " + GuardInterval2String(guardInterval) + " for HT frame.");
        } else if (frameType == PacketFormatEnum::PacketFormat_VHT) {
            if (mcs[0] > 9)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for VHT frame.");

            if (numSTS[0] > 8)
                throw std::invalid_argument("Invalid Tx numSTS: " + std::to_string(numSTS[0]) + " for VHT frame.");

            if (guardInterval > GuardIntervalEnum::GI_800)
                throw std::invalid_argument("Invalid Tx GI: " + GuardInterval2String(guardInterval) + " for VHT frame.");
        } else if (frameType == PacketFormatEnum::PacketFormat_HESU || frameType == PacketFormatEnum::PacketFormat_HEMU) {
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
        } else if (frameType == PacketFormatEnum::PacketFormat_EHTSU) {
            if (coding[0] == ChannelCodingEnum::BCC && cbw > ChannelBandwidthEnum::CBW_20)
                throw std::invalid_argument(">20 MHz CBW EHT-SU format requires the LDPC coding.");

            if (coding[0] == ChannelCodingEnum::BCC && numSTS[0] >= 4)
                throw std::invalid_argument(">4 STS EHT-SU format requires the LDPC coding.");

            if (coding[0] == ChannelCodingEnum::BCC && mcs[0] > 9)
                throw std::invalid_argument(" >9 MCS EHT-SU format requires the LDPC coding.");

            if (mcs[0] > 13)
                throw std::invalid_argument("Invalid Tx MCS: " + std::to_string(mcs[0]) + " for EHT-SU frame.");

            if (const auto expectedNumEHTLTF = [&] {
                switch (numSTS[0]) {
                    case 1:
                        return 1;
                    case 2:
                        return 2;
                    case 3:
                        [[fallthrough]];
                    case 4:
                        return 4;
                    case 5:
                        [[fallthrough]];
                    case 6:
                        return 6;
                    case 7:
                        [[fallthrough]];
                    default:
                        return 8;
                }
            }() + numExtraSounding; numExtraSounding > 0 && (expectedNumEHTLTF == 3 || expectedNumEHTLTF == 5 || expectedNumEHTLTF == 7 || expectedNumEHTLTF > 8)) {
                    throw std::invalid_argument("Invalid number of Extra EHT-LTF: " + std::to_string(numExtraSounding) + " for EHT-format frame with " + std::to_string(numSTS[0]) + " STS.");
            }

            if (guardInterval == GuardIntervalEnum::GI_400)
                throw std::invalid_argument("Invalid GI " + std::to_string(static_cast<uint16_t>(guardInterval)) + " for EHT-SU frame. Must be 800, 1600 or 3200ns.");

            if (ehtLTFType == 1)
                throw std::invalid_argument("Invalid EHT-LTF compression mode " + std::to_string(ehtLTFType) + " for EHT-SU frame. Must be 2 or 4.");

            if (ehtLTFType == 4 && guardInterval == GuardIntervalEnum::GI_1600)
                throw std::invalid_argument("Invalid GI " + std::to_string(static_cast<uint16_t>(guardInterval)) + " for EHT-SU EHT-LTF compression mode 4. Must be 800 or 3200ns.");

            if (ehtLTFType == 2 && guardInterval == GuardIntervalEnum::GI_3200)
                throw std::invalid_argument("Invalid GI " + std::to_string(static_cast<uint16_t>(guardInterval)) + " for EHT-SU EHT-LTF compression mode 1 or 2. Must be 800 or 1600ns.");
        }
    }

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "tx_param[preset=" << (preset ? preset->presetName : "NULL") << ", type=" << PacketFormat2String(frameType) << ", CBW=" << ChannelBandwidth2String(cbw) << ", MCS=" << static_cast<int16_t>(mcs[0]) << ", numSTS=" << static_cast<int16_t>(numSTS[0]) << ", Coding=" << ChannelCoding2String(coding[0]) << ", GI=" << GuardInterval2String(guardInterval);
        if (frameType == PacketFormatEnum::PacketFormat_HT) {
            ss << ", numESS=" << static_cast<int>(numExtraSounding) << ", sounding(11n)=" << forceSounding;
        } else if (frameType == PacketFormatEnum::PacketFormat_HESU) {
            ss << ", HiDoppler=" << (heHighDoppler ? static_cast<int>(heMidamblePeriodicity) : 0) << ", ER=" << txHEExtendedRange;
        } else if (frameType == PacketFormatEnum::PacketFormat_EHTSU) {
            ss << ", numESS=" << static_cast<int>(numExtraSounding);
        }
        ss <<", prefix=" << prefixPaddingTime * 1e6 <<"us, postfix=" << postfixPaddingTime * 1e6 << "us]";
        return ss.str();
    }
};

inline std::ostream& operator<<(std::ostream&os, const PicoScenesFrameTxParameters&parameters) {
    os << parameters.toString();
    return os;
}

#endif //PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
