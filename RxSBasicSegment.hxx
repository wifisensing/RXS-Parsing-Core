//
// Created by Zhiping Jiang on 2020/11/6.
//

#ifndef PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX
#define PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX

#include "macros.hxx"
#include "AbstractPicoScenesFrameSegment.hxx"
#include "PicoScenesCommons.hxx"

/**
 * @brief Basic structure for received signal information
 *
 * @details This structure contains basic information about a received signal,
 * including device type, timestamps, frequencies, bandwidth, packet format,
 * and signal strength measurements. It is used to store and process received
 * signal data in the PicoScenes framework.
 */
PACKED(struct RxSBasic {
    uint16_t deviceType;    /**< Device type code */
    uint64_t tstamp;        /**< Hardware assigned timestamp */
    uint64_t systemTime;    /**< System nanosecond time via ktime_get_real() */
    int16_t centerFreq;     /**< Primary RF center frequency */
    int16_t centerFreq2;    /**< Second RF center frequency, mainly used for multi-channel splitting, e.g., 80 + 80 mode */
    int16_t controlFreq;    /**< Control channel frequency */
    uint16_t cbw;           /**< Channel bandwidth [20, 40, 80, 160, 320] */
    uint8_t packetFormat;   /**< 0 for NonHT, 1 for HT, 2 for VHT, 3 for HE-SU, 4 for HE-MU, 5 for EHT-MU */
    uint16_t pkt_cbw;       /**< Packet CBW [20, 40, 80, 160, 320] */
    uint16_t guardInterval; /**< 400/800/1600/3200ns */
    uint8_t mcs;            /**< Modulation and Coding Scheme */
    uint8_t numSTS;         /**< Number of Space-Time Stream */
    uint8_t numESS;         /**< Number of Extra Spatial Sounding (an 802.11n only feature) */
    uint8_t numRx;          /**< Number of Rx antenna */
    uint8_t numUser;        /**< Number of users in multi-user transmission */
    uint8_t userIndex;      /**< User index in multi-user transmission */
    float noiseFloor;       /**< Noise floor */
    float rssi;             /**< Rx frame RSSI */
    PACKED(union {
        PACKED(float rssAntenna[8]);  /**< Array of RSSI values for each antenna */
        struct {
            float rssi_ctl0;   /**< Rx frame RSSI [ctl, chain 0] */
            float rssi_ctl1;   /**< Rx frame RSSI [ctl, chain 1] */
            float rssi_ctl2;   /**< Rx frame RSSI [ctl, chain 2] */
            float rssi_ctl3;   /**< Rx frame RSSI [ctl, chain 3] */
            float rssi_ctl4;   /**< Rx frame RSSI [ctl, chain 4] */
            float rssi_ctl5;   /**< Rx frame RSSI [ctl, chain 5] */
            float rssi_ctl6;   /**< Rx frame RSSI [ctl, chain 6] */
            float rssi_ctl7;   /**< Rx frame RSSI [ctl, chain 7] */
        };
    });

    /**
     * @brief Convert the structure to a string representation
     *
     * @return std::string String representation of the structure
     */
    [[nodiscard]] std::string toString() const;

    /**
     * @brief Convert the structure to a binary buffer
     *
     * @return std::vector<uint8_t> Binary representation of the structure
     */
    std::vector<uint8_t> toBuffer() const;

});

/**
 * @brief Stream insertion operator for RxSBasic
 *
 * @param os Output stream
 * @param rxSBasic RxSBasic object to insert
 * @return std::ostream& Reference to the output stream
 */
std::ostream &operator<<(std::ostream &os, const RxSBasic &rxSBasic);


/**
 * @brief Segment class for basic received signal information
 *
 * @details This class represents a segment of a PicoScenes frame that contains
 * basic information about a received signal. It inherits from AbstractPicoScenesFrameSegment
 * and provides methods to access and manipulate the RxSBasic structure.
 */
class RxSBasicSegment : public AbstractPicoScenesFrameSegment {

public:
    /**
     * @brief Default constructor
     */
    RxSBasicSegment();

    /**
     * @brief Constructor from binary buffer
     *
     * @param buffer Pointer to the binary buffer
     * @param bufferLength Length of the binary buffer
     */
    RxSBasicSegment(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Constructor from RxSBasic structure
     *
     * @param rvBasic RxSBasic structure to initialize from
     */
    explicit RxSBasicSegment(const RxSBasic &rvBasic);

    /**
     * @brief Get the basic received signal information
     *
     * @return const RxSBasic& Reference to the RxSBasic structure
     */
    [[nodiscard]] const RxSBasic &getBasic() const;

    /**
     * @brief Set the basic received signal information
     *
     * @param basicV RxSBasic structure to set
     */
    void setBasic(const RxSBasic &basicV);

    /**
     * @brief Convert the segment to a string representation
     *
     * @return std::string String representation of the segment
     */
    [[nodiscard]] std::string toString() const override;

private:
    /**
     * @brief Map of version-specific deserialization functions
     *
     * @details This map associates version numbers with functions that can
     * deserialize RxSBasic structures from binary buffers.
     */
    static std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> versionedSolutionMap;

    /**
     * @brief Initialize the version-specific deserialization map
     *
     * @return std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> Initialized map
     */
    static std::map<uint16_t, std::function<RxSBasic(const uint8_t *, uint32_t)>> initializeSolutionMap() noexcept;

    /**
     * @brief Basic received signal information
     */
    RxSBasic basic{};
};

#endif //PICOSCENES_PLATFORM_RXSBASICSEGMENT_HXX
