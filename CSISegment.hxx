//
// Created by Zhiping Jiang on 2020/11/5.
//

#ifndef PICOSCENES_PLATFORM_CSISEGMENT_HXX
#define PICOSCENES_PLATFORM_CSISEGMENT_HXX

#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

/**
 * @brief Parse CSI data from QCA9300 NIC
 *
 * @tparam Iterator Output iterator type for CSI data
 * @param outputArray Output array for parsed CSI data
 * @param csiData Raw CSI data buffer
 * @param nSTS Number of spatial streams
 * @param nRx Number of receive antennas
 * @param nTones Number of OFDM subcarriers
 */
template<typename Iterator>
void parseQCA9300CSIData(Iterator outputArray, const uint8_t *csiData, const int nSTS, const int nRx, const int nTones) {

    auto parse10bitsValues = [](const uint8_t rawByte[5], int outputArray[4]) {
        static uint16_t negativeSignBit = (1 << (10 - 1));
        static uint16_t minNegativeValue = (1 << 10);
        outputArray[0] = ((rawByte[0] & 0xffU) >> 0U) + ((rawByte[1] & 0x03u) << 8u);
        outputArray[1] = ((rawByte[1] & 0xfcU) >> 2U) + ((rawByte[2] & 0x0fU) << 6U);
        outputArray[2] = ((rawByte[2] & 0xf0U) >> 4U) + ((rawByte[3] & 0x3fU) << 4U);
        outputArray[3] = ((rawByte[3] & 0xc0U) >> 6U) + ((rawByte[4] & 0xffU) << 2U);
        for (auto i = 0; i < 4; i++) {
            if (outputArray[i] & negativeSignBit)
                outputArray[i] -= minNegativeValue;
        }
    };

    int totalTones = nRx * nSTS * nTones;
    for (auto i = 0; i < totalTones / 2; i++) {
        int tempArray[4];
        parse10bitsValues(csiData + i * 5, tempArray);

        int valuePos = i * 2;
        int rxIndex = valuePos % nRx;
        int txIndex = (valuePos / nRx) % nSTS;
        int toneIndex = valuePos / (nRx * nSTS);
        int pos = rxIndex * (nSTS * nTones) + txIndex * nTones + toneIndex;
        outputArray[pos].real(tempArray[1]);
        outputArray[pos].imag(tempArray[0]);

        valuePos = i * 2 + 1;
        rxIndex = valuePos % nRx;
        txIndex = (valuePos / nRx) % nSTS;
        toneIndex = valuePos / (nRx * nSTS);
        pos = rxIndex * (nSTS * nTones) + txIndex * nTones + toneIndex;
        outputArray[pos].real(tempArray[3]);
        outputArray[pos].imag(tempArray[2]);
    }
}

/**
 * @brief Sort indices based on values
 *
 * @tparam T Value type to sort
 * @param v Vector of values
 * @return Vector of sorted indices
 */
template<typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {

    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(),
              [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

    return idx;
}

/**
 * @brief Parse CSI data from Intel IWL5300 NIC
 *
 * @tparam Iterator Output iterator type for CSI matrix
 * @param csi_matrix Output matrix for parsed CSI data
 * @param payload Raw CSI payload data
 * @param ntx Number of transmit antennas
 * @param nrx Number of receive antennas
 * @param ant_sel Antenna selection configuration
 */
template<typename Iterator>
void parseIWL5300CSIData(Iterator csi_matrix, const uint8_t *payload, const int ntx, const int nrx, const uint8_t ant_sel) {

    auto positionComputationWRTPermutation = [](const int ntx, const int num_tones, const int ntxIdx, const int nrxIdx, const int subcarrierIdx, const std::optional<U8Vector>& ant_sel) -> int {
        auto new_nrxIdx = nrxIdx;
        if (ant_sel && !ant_sel->empty()) {
            auto sorted_indexes = sort_indexes(*ant_sel);
            auto sorted_indexes2 = sort_indexes(sorted_indexes); // double sort, shit but works !
            new_nrxIdx = sorted_indexes2[nrxIdx];
        }

        return new_nrxIdx * (ntx * num_tones) + ntxIdx * num_tones + subcarrierIdx;
    };

    std::vector<uint8_t> antSelVector = [&](uint8_t ant_sel) {
        auto v = std::vector<uint8_t>();
        if (nrx > 1) {
            v.emplace_back(static_cast<unsigned char>((static_cast<unsigned>(ant_sel) & 0x1U) + 1));
            v.emplace_back(static_cast<unsigned char>(((static_cast<unsigned>(ant_sel) >> 0x2U) & 0x3U) + 1));
        };
        if (nrx > 2)
            v.emplace_back(static_cast<unsigned char>(((static_cast<unsigned>(ant_sel) >> 0x4U) & 0x3U) + 1));
        return v;
    }(ant_sel);

    uint32_t index = 0;
    for (auto subcarrierIdx = 0; subcarrierIdx < 30; subcarrierIdx++) {
        index += 3;
        uint8_t remainder = index % 8;

        for (auto nrxIdx = 0; nrxIdx < nrx; nrxIdx++) {
            for (auto ntxIdx = 0; ntxIdx < ntx; ntxIdx++) {
                auto position = positionComputationWRTPermutation(ntx, 30, ntxIdx, nrxIdx, subcarrierIdx, antSelVector);
                char tmp1 = (payload[index / 8] >> remainder) | (payload[index / 8 + 1] << (8 - remainder));
                char tmp2 = (payload[index / 8 + 1] >> remainder) | (payload[index / 8 + 2] << (8 - remainder));
                csi_matrix[position].real(static_cast<double>(tmp1));
                csi_matrix[position].imag(static_cast<double>(tmp2));
                index += 16;
            }
        }
    }
}

/**
 * @brief Describe the dimensions of @see CSI class
 */
class CSIDimension {
public:
    uint16_t numTones = 1; ///< Number of OFDM subcarriers
    uint8_t numTx = 1; ///< Number of Tx spatial streams
    uint8_t numRx = 1; ///< Number of Rx radio chains
    uint8_t numESS = 0; /// Number of Extra Spatial Soudning (ESS) streams
    uint16_t numCSI = 1; /// Number of 3-D CSI matrix, usually 1. However, for LegacyCSI and PilotCSI, since these ARE multiple OFDM symbols, numCSI maybe greater than 1.

    /**
     * @brief Get total number of Tx spatial streams
     * @return Sum of numTx and numESS
     */
    [[nodiscard]] inline uint16_t numTxSpatialStreams() const {
        return numTx + numESS;
    }

    /**
     * @brief Get number of streams per CSI matrix
     * @return Product of total Tx streams and Rx chains
     */
    [[nodiscard]] inline uint16_t numStreamsPerCSI() const {
        return numTxSpatialStreams() * numRx;
    }

    /**
    * @brief Get total number of subcarriers per CSI matrix
    * @return Product of streams per CSI and number of tones
    */
    [[nodiscard]] inline uint16_t numTotalSubcarriersPerCSI() const {
        return numStreamsPerCSI() * numTones;
    }

    /**
     * @brief Get total number of streams across all CSI matrices
     * @return Product of streams per CSI and number of CSI matrices
     */
    [[nodiscard]] inline uint16_t numStreamsAll() const {
        return numStreamsPerCSI() * numCSI;
    }

    /**
     * @brief Get total number of subcarriers across all CSI matrices
     * @return Product of total streams and number of tones
     */
    [[nodiscard]] inline uint16_t numTotalSubcarriersAll() const {
        return numStreamsAll() * numTones;
    }
};

/**
 * @brief Core CSI data class
 *
 * Contains all CSI measurement data including device information,
 * signal parameters, and the actual CSI measurements.
 */
class CSI {
public:
    PicoScenesDeviceType deviceType;      ///< Device type that measured this CSI
    uint8_t firmwareVersion;              ///< Firmware version (AX200/AX210 only)
    PacketFormatEnum packetFormat;        ///< Frame packet format
    ChannelBandwidthEnum cbw;             ///< Channel bandwidth
    uint64_t carrierFreq;                 ///< Primary carrier frequency
    uint64_t carrierFreq2;                ///< Secondary carrier frequency
    bool isMerged;                        ///< High/low dual-channel merge status
    uint64_t samplingRate;                ///< Hardware sampling rate
    uint32_t subcarrierBandwidth;         ///< OFDM subcarrier bandwidth
    CSIDimension dimensions;              ///< CSI dimension information
    uint8_t antSel;                       ///< Antenna selection (IWL5300 only)
    int16_t subcarrierOffset;             ///< Subcarrier index offset
    std::vector<int16_t> subcarrierIndices; ///< CSI subcarrier indices
    std::vector<uint32_t> timingOffsets;  ///< LTF segment timing offsets
    SignalMatrix<std::complex<float>> CSIArray; ///< Core CSI data matrix
    SignalMatrix<float> magnitudeArray;   ///< Magnitude data matrix
    SignalMatrix<float> phaseArray;       ///< Phase data matrix
    SignalMatrix<float> phaseSlope;       ///< Interpolated phase slope
    SignalMatrix<float> phaseIntercept;   ///< Interpolated phase intercept
    U8Vector rawCSIData;                  ///< Raw CSI data

    /**
     * @brief Remove CSD and interpolate CSI data
     *
     * Removes cyclic shift delay from spatial streams and
     * interpolates the 0-th subcarrier while generating
     * magnitude and phase arrays.
     */
    void removeCSDAndInterpolateCSI();

    /**
     * @brief Serialize CSI object to buffer
     * @return Serialized buffer
     */
    std::vector<uint8_t> toBuffer();

    /**
     * @brief Create CSI object from QCA9300 data
     *
     * @param buffer Raw data buffer
     * @param bufferLength Buffer length
     * @param numTx Number of Tx antennas
     * @param numRx Number of Rx antennas
     * @param numTones Number of tones
     * @param cbw Channel bandwidth
     * @param subcarrierIndexOffset Subcarrier index offset
     * @return Shared pointer to created CSI object
     */
    static std::shared_ptr<CSI> fromQCA9300(const uint8_t* buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numTones, ChannelBandwidthEnum cbw, int16_t subcarrierIndexOffset);

    /**
     * @brief Create CSI object from IWL5300 data
     *
     * @param buffer Raw data buffer
     * @param bufferLength Buffer length
     * @param numTx Number of Tx antennas
     * @param numRx Number of Rx antennas
     * @param numTones Number of tones
     * @param cbw Channel bandwidth
     * @param subcarrierIndexOffset Subcarrier index offset
     * @param ant_sel Antenna selection
     * @return Shared pointer to created CSI object
     */
    static std::shared_ptr<CSI> fromIWL5300(const uint8_t* buffer, uint32_t bufferLength, uint8_t numTx, uint8_t numRx, uint8_t numTones, ChannelBandwidthEnum cbw, int16_t subcarrierIndexOffset, uint8_t ant_sel);

    /**
     * @brief Create CSI object from IWL MVM data (AX200/AX210)
     *
     * @param buffer Raw data buffer
     * @param bufferLength Buffer length
     * @param firmwareVersion Firmware version
     * @param numTx Number of Tx antennas
     * @param numRx Number of Rx antennas
     * @param numTones Number of tones
     * @param format Packet format
     * @param cbw Channel bandwidth
     * @param subcarrierIndexOffset Subcarrier index offset
     * @param skipPilotSubcarriers Whether to skip pilot subcarriers
     * @param antSelByte Antenna selection byte
     * @return Shared pointer to created CSI object
     */
    static std::shared_ptr<CSI> fromIWLMVM(const uint8_t* buffer, uint32_t bufferLength, uint8_t firmwareVersion, uint8_t numTx, uint8_t numRx, uint16_t numTones, PacketFormatEnum format, ChannelBandwidthEnum cbw, int16_t subcarrierIndexOffset, bool skipPilotSubcarriers = true, uint8_t antSelByte = 0);

    /**
     * @brief Convert CSI array between different numeric types
     *
     * @tparam OutputValueType Target numeric type
     * @tparam InputValueType Source numeric type
     * @param inputArray Input CSI array
     * @return Converted CSI array
     */
    template<typename OutputValueType, typename InputValueType>
    static std::vector<std::complex<OutputValueType>> convertCSIArrayType(const std::vector<std::complex<InputValueType>> &inputArray) {
        std::vector<std::complex<OutputValueType>> outputArray(inputArray.size());
        for (auto i = 0; i < inputArray.size(); i++) {
            outputArray[i] = std::complex<OutputValueType>(inputArray[i].real(), inputArray[i].imag());
        }
        return outputArray;
    }

    /**
     * @brief Get index of DC (0-th) subcarrier
     * @return Optional containing index if present
     */
    [[nodiscard]] std::optional<int16_t> get0thSubcarrierIndex() const;

    /**
     * @brief Get CSI value for specific parameters
     *
     * @param subcarrierIndex Subcarrier index
     * @param stsIndexStartingFrom0 Spatial stream index
     * @param rxIndexStartingFrom0 Receive antenna index
     * @param csiIndexStartingFrom0 CSI matrix index
     * @return Optional containing CSI value if parameters valid
     */
    [[nodiscard]] std::optional<std::complex<double>> getCSI(int16_t subcarrierIndex, uint8_t stsIndexStartingFrom0 = 0, uint8_t rxIndexStartingFrom0 = 0, uint16_t csiIndexStartingFrom0 = 0) const;

    /**
     * @brief Get magnitude value for specific parameters
     *
     * @param subcarrierIndex Subcarrier index
     * @param stsIndex Spatial stream index
     * @param rxIndex Receive antenna index
     * @param csiIndexStartingFrom0 CSI matrix index
     * @return Optional containing magnitude if parameters valid
     */
    [[nodiscard]] std::optional<double> getMagnitude(int16_t subcarrierIndex, uint8_t stsIndex = 0, uint8_t rxIndex = 0, uint16_t csiIndexStartingFrom0 = 0) const;

    /**
     * @brief Get phase value for specific parameters
     *
     * @param subcarrierIndex Subcarrier index
     * @param stsIndex Spatial stream index
     * @param rxIndex Receive antenna index
     * @param csiIndexStartingFrom0 CSI matrix index
     * @return Optional containing phase if parameters valid
     */
    [[nodiscard]] std::optional<double> getPhase(int16_t subcarrierIndex, uint8_t stsIndex = 0, uint8_t rxIndex = 0, uint16_t csiIndexStartingFrom0 = 0) const;
};

/**
 * @brief Class managing CSI subcarrier indices
 *
 * Provides static methods to access subcarrier indices for different
 * packet formats and channel bandwidths.
 */
class CSISubcarrierIndex {
public:
    /**
     * @brief Get all subcarrier indices for given format and bandwidth
     *
     * @param format Packet format
     * @param cbw Channel bandwidth
     * @return Vector of subcarrier indices
     */
    static const std::vector<int16_t> &getAllSubcarrierIndices(PacketFormatEnum format, ChannelBandwidthEnum cbw);

    /**
     * @brief Get pilot subcarrier indices for given format and bandwidth
     *
     * @param format Packet format
     * @param cbw Channel bandwidth
     * @return Vector of pilot subcarrier indices
     */
    static const std::vector<int16_t> &getPilotSubcarrierIndices(PacketFormatEnum format, ChannelBandwidthEnum cbw);

    /**
     * @brief Get data subcarrier indices for given format and bandwidth
     *
     * @param format Packet format
     * @param cbw Channel bandwidth
     * @return Vector of data subcarrier indices
     */
    static const std::vector<int16_t> &getDataSubcarrierIndices(PacketFormatEnum format, ChannelBandwidthEnum cbw);

    /**
     * @brief Get IWL5300 specific subcarrier indices
     *
     * @param cbw Channel bandwidth
     * @return Vector of subcarrier indices
     */
    static const std::vector<int16_t> &getIWL5300Subcarriers(ChannelBandwidthEnum cbw);

private:
    static const  std::vector<int16_t> NonHT20_52Subcarriers_Indices;
    static const std::vector<int16_t> HTVHT20_56Subcarriers_Indices;
    static const std::vector<int16_t> HTVHT40_114Subcarriers_Indices;
    static const std::vector<int16_t> VHT80_242Subcarriers_Indices;
    static const std::vector<int16_t> VHT160_484Subcarriers_Indices;
    static const std::vector<int16_t> HE20_242Subcarriers_Indices;
    static const std::vector<int16_t> HE40_484Subcarriers_Indices;
    static const std::vector<int16_t> HE80_996Subcarriers_Indices;
    static const std::vector<int16_t> HE160_1992Subcarriers_Indices;

    static const std::vector<int16_t> NonHT20_52Subcarriers_DataIndices;
    static const std::vector<int16_t> HTVHT20_56Subcarriers_DataIndices;
    static const std::vector<int16_t> HTVHT40_114Subcarriers_DataIndices;
    static const std::vector<int16_t> VHT80_242Subcarriers_DataIndices;
    static const std::vector<int16_t> VHT160_484Subcarriers_DataIndices;
    static const std::vector<int16_t> HE20_242Subcarriers_DataIndices;
    static const std::vector<int16_t> HE40_484Subcarriers_DataIndices;
    static const std::vector<int16_t> HE80_996Subcarriers_DataIndices;
    static const std::vector<int16_t> HE160_1992Subcarriers_DataIndices;

    static const std::vector<int16_t> NonHT20_52Subcarriers_PilotIndices;
    static const std::vector<int16_t> HTVHT20_56Subcarriers_PilotIndices;
    static const std::vector<int16_t> HTVHT40_114Subcarriers_PilotIndices;
    static const std::vector<int16_t> VHT80_242Subcarriers_PilotIndices;
    static const std::vector<int16_t> VHT160_484Subcarriers_PilotIndices;
    static const std::vector<int16_t> HE20_242Subcarriers_PilotIndices;
    static const std::vector<int16_t> HE40_484Subcarriers_PilotIndices;
    static const std::vector<int16_t> HE80_996Subcarriers_PilotIndices;
    static const std::vector<int16_t> HE160_1992Subcarriers_PilotIndices;

    static const std::vector<int16_t> IWL5300SubcarrierIndices_CBW20;
    static const std::vector<int16_t> IWL5300SubcarrierIndices_CBW40;
};

/**
 * @brief Class representing a CSI segment in PicoScenes frame
 *
 * Final class that extends AbstractPicoScenesFrameSegment to handle
 * Channel State Information data segments.
 */
class CSISegment final : public AbstractPicoScenesFrameSegment {
public:
    /**
     * @brief Default constructor
     *
     * Creates a new CSI segment with default parameters
     */
    CSISegment();

    /**
     * @brief Construct from buffer
     *
     * @param buffer Raw segment data
     * @param bufferLength Buffer length
     */
    CSISegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
    * @brief Construct from CSI object
    *
    * @param csi Shared pointer to CSI object
    */
    explicit CSISegment(const std::shared_ptr<CSI> & csi);

    /**
     * @brief Get string representation
     * @return String describing the segment
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Get CSI object
     * @return Shared pointer to CSI object
     */
    [[nodiscard]] const std::shared_ptr<CSI> &getCSI() const;

    /**
     * @brief Set CSI object
     * @param csi New CSI object
     */
    void setCSI(const std::shared_ptr<CSI> &csi);

private:
    static std::map<uint16_t, std::function<std::shared_ptr<CSI>(const uint8_t *, uint32_t)>> versionedSolutionMap;

    static std::map<uint16_t, std::function<std::shared_ptr<CSI>(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    std::shared_ptr<CSI> csi{nullptr};
};

#endif //PICOSCENES_PLATFORM_CSISEGMENT_HXX
