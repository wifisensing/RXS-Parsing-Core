//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
#define PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX

#include "PicoScenesCommons.hxx"
#include "FrontEndModePreset.hxx"

/**
 * @brief Parameters for transmit precoding operations
 */
class TxPrecodingParameters {
public:
   SignalMatrix<> userSpecifiedSpatialMappingMatrix;  ///< User-defined spatial mapping matrix
    double simulateTxDistance{0};                      ///< Simulated transmission distance
    SignalMatrix<> rxCSI4TxBeamforming;               ///< CSI matrix for transmit beamforming (N_{sc}-by-N_{tx_sts}-by-N_{rx})
    uint8_t txNumSTS4TxBeamforming{1};                ///< Number of spatial streams for transmit beamforming
    std::vector<std::array<double, 3>> antennaPositions; ///< 3D positions of antennas
    double txSteeringAngles4PhasedArray{0};           ///< Steering angles for phased array
    double carrierFrequency{0};                       ///< Carrier frequency
};

/**
 * @brief Frame transmission parameters for PicoScenes
 * @details Contains all parameters needed to configure frame transmission,
 *          including PHY layer settings and advanced features
 */
struct PicoScenesFrameTxParameters {
   std::shared_ptr<FrontEndModePreset> preset{FrontEndModePreset::DEFAULT}; ///< Frontend mode preset
    bool NDPFrame{false};                             ///< Null Data Packet flag
    PacketFormatEnum frameType{PacketFormatEnum::PacketFormat_HT}; ///< Frame format type
    std::vector<uint8_t> mcs{0};                     ///< Modulation and Coding Scheme
    std::vector<uint8_t> numSTS{1};                  ///< Number of Space-Time Streams
    double numAntenna{1};                            ///< Number of antennas
    uint16_t txcm{1};                                ///< Transmit chain mask
    ChannelBandwidthEnum cbw{ChannelBandwidthEnum::CBW_20}; ///< Channel bandwidth
    bool preferAMPDU{false};                         ///< Prefer A-MPDU aggregation
    GuardIntervalEnum guardInterval{GuardIntervalEnum::GI_800}; ///< Guard interval
    std::vector<ChannelCodingEnum> coding{ChannelCodingEnum::BCC}; ///< Channel coding scheme
    uint8_t numExtraSounding{0};                     ///< Number of extra sounding symbols
    bool forceSounding{true};                        ///< Force sounding
    double prefixPaddingTime{8e-6};                  ///< Prefix padding time in seconds
    double inBasebandPostfixPaddingTime{4e-6};       ///< In-baseband postfix padding time
    double postfixPaddingTime{0};                    ///< Postfix padding time
    uint8_t scramblerState{0x27};                    ///< Initial scrambler state
    double txIQAmplitudeImbalance_dB{0};            ///< TX I/Q amplitude imbalance in dB
    double txIQPhaseImbalance_rad{0};               ///< TX I/Q phase imbalance in radians
    std::optional<double> fixedPowerScale{std::nullopt}; ///< Fixed power scaling factor
    double samplingRateOffset{0};                    ///< Sampling rate offset
    double carrierFrequencyOffset{0};                ///< Carrier frequency offset
    double resampleRatio{1.0};                       ///< Resampling ratio
    double simulateTxDistance{0};                    ///< Simulated transmission distance
    bool splitHighLow{false};                        ///< Split high/low frequency components
    uint8_t vhtGroupId{0};                          ///< VHT group ID
    std::vector<uint8_t> heAllocationIndex{0};       ///< HE resource unit allocation index
    bool txHEExtendedRange{false};                   ///< HE extended range mode
    bool heHighDoppler{false};                       ///< HE high Doppler mode
    uint8_t heMidamblePeriodicity{10};              ///< HE midamble periodicity
    uint8_t heLTFType{4};                           ///< HE LTF type
    bool hePreHESpatialMapping{false};               ///< Pre-HE spatial mapping
    bool ehtDupMode{false};                         ///< EHT duplicate mode
    uint8_t ehtLTFType{4};                          ///< EHT LTF type
    std::optional<TxPrecodingParameters> precodingParameters = std::nullopt; ///< Precoding parameters

    /**
     * @brief Apply preset configuration by name
     * @param presetName Name of the preset to apply
     * @throw std::invalid_argument if preset name is invalid
     */
    void applyPreset(const std::string &presetName) {
        if (!FrontEndModePreset::getPresetMap().contains(presetName))
            throw std::invalid_argument("invalid frontend mode preset name: " + presetName + "\n" + FrontEndModePreset::printHelpContentForFrontEndModePreset());
        applyPreset(FrontEndModePreset::getPresetMap().at(presetName));
    }

    /**
     * @brief Apply preset configuration
     * @param presetV Preset configuration to apply
     */
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

    /**
     * @brief Validate transmission parameters
     * @throw std::invalid_argument if parameters are invalid
     * @details Checks compatibility of parameters based on frame type and standards
     */
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

    /**
     * @brief Get string representation of parameters
     * @return String describing the transmission parameters
     */
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

/**
 * @brief Stream output operator for transmission parameters
 * @param os Output stream
 * @param parameters Parameters to output
 * @return Reference to output stream
 */
inline std::ostream& operator<<(std::ostream&os, const PicoScenesFrameTxParameters&parameters) {
    os << parameters.toString();
    return os;
}

#endif //PICOSCENES_PLATFORM_PICOSCENESFRAMETXPARAMETERS_HXX
