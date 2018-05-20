//
// Created by Zhiping Jiang
//

#ifndef RXS_ENHANCED_H_
#define RXS_ENHANCED_H_

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <memory>
#include <cstdint>
#include <complex>
#include <bitset>

#define TONE_20M                56 ///< Ath9k NIC returns 56 subcarrier in 20MHz bandwidth
#define TONE_40M                114 ///< Ath9k NIC returns 114 subcarrier in 40MHz bandwidth
#define TONE_20M_UNWRAP         57
#define TONE_40M_UNWRAP         117
#define MAX_OFDM_TONES          (TONE_40M * 3 * 3) ///< in 3x3 MIMO + 40MHZ case
#define MAX_OFDM_TONES_UNWRAP   (TONE_40M_UNWRAP * 3 * 3) ///< in 3x3 MIMO + 40MHZ case
#define BITS_PER_BYTE           8
#define BITS_PER_COMPLEX_SYMBOL (2 * BITS_PER_SYMBOL)
#define BITS_PER_SYMBOL         10

#define AR_CSI_EXTRAINFO_HASLENGTH                 0x00000001
#define AR_CSI_EXTRAINFO_HASVERSION                0x00000002
#define AR_CSI_EXTRAINFO_HASMACCUR                 0x00000004
#define AR_CSI_EXTRAINFO_HASMACROM                 0x00000008
#define AR_CSI_EXTRAINFO_HASCHANSEL                0x00000010
#define AR_CSI_EXTRAINFO_HASBMODE                  0x00000020
#define AR_CSI_EXTRAINFO_HASEVM                    0x00000040
#define AR_CSI_EXTRAINFO_HASTXCHAINMASK            0x00000080
#define AR_CSI_EXTRAINFO_HASRXCHAINMASK            0x00000100
#define AR_CSI_EXTRAINFO_HASTXPOWER                0x00000200
#define AR_CSI_EXTRAINFO_HASCF                     0x00000400
#define AR_CSI_EXTRAINFO_HASTXTSF                  0x00000800
#define AR_CSI_EXTRAINFO_HASLASTHWTXTSF            0x00001000
#define AR_CSI_EXTRAINFO_HASCHANNELFLAGS           0x00002000
#define AR_CSI_EXTRAINFO_HASPLLSLOPE               0x00004000
#define AR_CSI_EXTRAINFO_HASTUNINGPOLICY           0x00008000

/**
 * Test the presence of version field.
 * @param featureCode The 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasVersion(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 0 & 0x1);
}

/**
 * Test the presence of length field
 *
 * Please note: in the ExtraInfo struct, the length value does not include the length field itself.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasLength(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 1 & 0x1);
}

/**
 * Test the presence of "current MAC Address" field
 *
 * The "current" MAC address is the mac address attached to the working interface, this MAC address can be modified using bash commands.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasMacAddress_Rom
 */
inline bool extraInfoHasMacAddress_Current(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 2 & 0x1);
}

/**
 * Test the presence of "Permenant MAC Address" field
 *
 * "Permenant MAC Address" is the mac address store the in the H/W's EEPROM, which cannot be modified.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasMacAddress_Current
 */
inline bool extraInfoHasMacAddress_Rom(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 3 & 0x1);
}

/**
 * Test the presence of "Channel Selection" field
 *
 * Chansel value is used in Ath9k driver to configure the carrier frequency
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasBMode
 */
inline bool extraInfoHasChansel(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 4 & 0x1);
}

/**
 * Test the presence of "Bmode" field
 *
 * Bmode value is used together with Chansel value in Ath9k driver to configure the carrier frequency.
 *
 * 1 for 2.4GHz band, 0 for 5GHz band.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 *
 * @see extraInfoHasChansel
 */
inline bool extraInfoHasBMode(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 5 & 0x1);
/**
 * Test the presence of EVM data
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 */
}
inline bool extraInfoHasEVM(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 6 & 0x1);
}

/**
 * Test the presence of Tx ChainMask
 *
 * Tx/Rx chainmask is a 3-bit value. Each bit stands for a radio chain.
 * 1 for first chain, 4 for last chain.
 * The chainmask bits combination MUST be in low to high order.
 *
 * @param featureCode the 32-bit feature code
 * @return true for the presence, and false for not.
 */
inline bool extraInfoHasTxChainMask(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 7 & 0x1);
}
inline bool extraInfoHasRxChainMask(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 8 & 0x1);
}
inline bool extraInfoHasTxPower(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 9 & 0x1);
}
inline bool extraInfoHasCF(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 10 & 0x1);
}
inline bool extraInfoHasTxTSF(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 11 & 0x1);
}
inline bool extraInfoHasLastHWTxTSF(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 12 & 0x1);
}
inline bool extraInfoHasChannelFlags(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 13 & 0x1);
}
inline bool extraInfoHasPLLSlope(uint32_t featureCode) {
	return static_cast<bool>(featureCode >> 14 & 0x1);
}
inline bool extraInfoHasTuningPolicy(uint32_t featureCode) {
    return static_cast<bool>(featureCode >> 15 & 0x1);
}

enum RXSParsingLevel: uint8_t {
    BASIC_NOEXTRA_NOCSI = 10,
    EXTRA_NOCSI,
    EXTRA_CSI,
    EXTRA_CSI_UNWRAP
};

enum ChannelMode : uint8_t {
    NON_HT5 = 10,
    NON_HT10,
    NON_HT20,
    HT20,
    HT40_MINUS,
    HT40_PLUS,
};

enum AtherosCFTuningPolicy: uint8_t {
    CFTuningByFastCC,
    CFTuningByChansel,
    CFTuningByHardwareReset,
};

/**
 * The Rx Status Basic Informations
 */
struct ath_rx_status_basic {
    uint64_t   tstamp;      ///< h/w assigned RX timestamp
    uint16_t   csi_len;     ///< csi length
    uint16_t   channel;     ///< receiving channel frequency, unit is MHz, e.g, 2437
    uint8_t	   sgi;	        ///< short GI, 1 for Short

    int8_t     noise;       ///< noise floor
    uint8_t	   rate;	    ///< MCS index
    uint8_t    chanBW;      ///< receiving channel bandwidth, 0 for 20MHz, 1 for 40MHz
    uint8_t    num_tones;   ///< number of tones (subcarriers), should be 56 or 114
    uint8_t    nrx;         ///< number of receiving antennas, 1~3
    uint8_t    ntx;         ///< number of transmitting anteannas, 1~3

    uint8_t    rssi;        ///< rx frame RSSI
    uint8_t    rssi0;       ///< rx frame RSSI [ctl, chain 0]
    uint8_t    rssi1;       ///< rx frame RSSI [ctl, chain 1]
    uint8_t    rssi2;       ///< rx frame RSSI [ctl, chain 2]
} __attribute__ ((__packed__));

/**
 Atheros Tx Status structure
 */
struct ath_tx_status {
	uint32_t ts_tstamp; ///< timestamp of the TX event, in microsecond
	uint16_t ts_seqnum;
	uint8_t ts_status;
	uint8_t ts_rateindex;
	int8_t ts_rssi;
	uint8_t ts_shortretry;
	uint8_t ts_longretry;
	uint8_t ts_virtcol;
	uint8_t ts_flags;
	int8_t ts_rssi_ctl0;
	int8_t ts_rssi_ctl1;
	int8_t ts_rssi_ctl2;
	int8_t ts_rssi_ext0;
	int8_t ts_rssi_ext1;
	int8_t ts_rssi_ext2;
    uint8_t qid;
	uint16_t desc_id;
    uint8_t tid;
	uint32_t ba_low;
	uint32_t ba_high;
	uint32_t evm0;
	uint32_t evm1;
	uint32_t evm2;
	uint32_t duration; ///< Tx duration in microsecond
} __attribute__ ((__packed__));

struct ieee80211_packet_header_info {
    uint8_t version        :2,
			isShortHeader  :1,
			needACK        :1,
			moreIsComming  :1, ///< indicating that this packet is segmented and Tx-ed in several sub-packets.
			isReTX         :1, ///< is re-transmission packet
			hasTxExtraInfo :1, ///< contains Tx ExtraInfo @see ExtraInfo
			hasChronosInfo :1; ///< contains ChronosInfo @see ChronosInfo
    uint8_t frameType;
    uint16_t taskId;
} __attribute__ ((__packed__));

struct ieee80211_packet_header {
    uint16_t  fc;
	uint16_t dur; // Atheros NIC will over-write this value, so leave alone.
    uint8_t	 addr1[6];
	uint8_t	 addr2[6];
	uint8_t	 addr3[6];
    uint16_t seq;
    struct ieee80211_packet_header_info header_info;
} __attribute__ ((__packed__));

/**
 The mininum set of ExtraInfo used for Tx fabrication.
 
 @see ExtraInfo
 */
struct TxExtraInfoMinSet {
    uint32_t txExtraInfoFeatureCode;
    uint16_t txExtraInfoLength;
    uint8_t txExtraInfoVersion;
    uint8_t txExtraInfoMacAddr_cur[6];
    uint8_t txExtraInfoMacAddr_rom[6];

    int getTxTSFPos();
} __attribute__ ((__packed__));

struct ExtraInfo {
    bool hasLength;
    bool hasVersion;
    bool hasMacAddr_cur;
    bool hasMacAddr_rom;
    bool hasChansel;
    bool hasBMode;
    bool hasEVM;
    bool hasTxChainMask;
    bool hasRxChainMask;
    bool hasTxpower;
    bool hasCF;
    bool hasTxTSF;
	bool hasLastHWTxTSF;
    bool hasChannelFlags;
	bool hasPLLSlope;
	bool hasTuningPolicy;
    uint16_t length;
    uint8_t version;
    uint8_t macaddr_rom[6];
    uint8_t macaddr_cur[6];
    uint32_t chansel;
    uint8_t bmode;
    int8_t  evm[18];
    uint8_t txChainMask;
    uint8_t rxChainMask;
    uint8_t txpower;
    uint64_t cf;
    uint32_t txTSF;
	uint32_t lastHwTxTSF;
    uint16_t channelFlags;
	double pllSlope;
	uint8_t tuningPolicy;

    static int fromBinary(const uint8_t *extraInfoPtr, struct ExtraInfo * extraInfo, uint32_t suppliedFeatureCode = 0);
    static int toBinary(void * extraInfoPtr);
};

struct ChronosInfo {
	uint16_t length;
	uint8_t  version;
	uint8_t  ackRequestType; // 0 for NO ACK, 1 for ACK with simple Injection, 2 for ACK with Injection containing TxRXS.
	uint8_t  ackInjectionType;
    int8_t   ackMCS;         // 0 to 23 are OK, negative means use default (maybe mcs 0).
	int8_t   ackBandWidth;   // 0 for 20MHz, 1 for 40MHz, negative means use default (maybe 20MHz).
	int8_t   ackSGI;         // 0 for LGI, 1 for SGI, negative means use default (maybe LGI).
    int8_t   ackTxChainMask; // negative means use default
	int8_t   ackRxChainMask; // negative means use default
	int8_t   ackTxpower;     // negative means use default
	uint64_t frequency;
    int32_t  ackExpectedDelay_us;
	uint16_t TxRXSLength;    // 0 means no TxRXS
} __attribute__ ((__packed__));

struct RXS_enhanced {
    struct ath_rx_status_basic rxs_basic;
	uint16_t   csi_pos;     /* CSI data position in raw buffer, used for re-parsing.*/
	uint16_t   payloadLength;
    uint16_t   rawBufferLength;
    uint16_t   txDuration;
    struct ExtraInfo rxExtraInfo;
    struct ieee80211_packet_header txHeader;
    struct ExtraInfo txExtraInfo;
    struct ChronosInfo chronosInfo;
    std::complex<double> csi_matrix[MAX_OFDM_TONES_UNWRAP];
    double unwrappedMag[MAX_OFDM_TONES_UNWRAP];
    double unwrappedPhase[MAX_OFDM_TONES_UNWRAP];
    uint8_t  chronosACKBody[3000];
    uint8_t  rawBuffer[6000];
	RXSParsingLevel parsingLevel;
};

inline ChannelMode channelFlags2ChannelMode(uint16_t channelFlags) {
    std::bitset<16> channelFlagSet(channelFlags);

    if  (channelFlagSet.test(2)) {
        return NON_HT5;
    }

    if  (channelFlagSet.test(1)) {
        return NON_HT10;
    }

    if  (channelFlagSet.test(3) && channelFlagSet.test(4)) {
        return HT40_PLUS;
    }

    if  (channelFlagSet.test(3) && channelFlagSet.test(5)) {
        return HT40_MINUS;
    }

    if  (channelFlagSet.test(3)) {
        return HT20;
    }

    return NON_HT20;
}

inline std::string channelModel2String(ChannelMode mode) {
    switch (mode) {
        case HT40_PLUS:
            return "HT40_PLUS";
        case HT40_MINUS:
            return "HT40_MINUS";
        case HT20:
            return "HT20";
        case NON_HT20:
            return "NON_HT20";
        case NON_HT5:
            return "NON_HT5";
        case NON_HT10:
            return "NON_HT10";
    }
}

/**
 * calculate the over-the-air Tx duration for a packet
 * @param length packet length
 * @param mcs  Tx MCS value
 * @param wide40BW  bandwidth 20 or 40MHz
 * @param usingSGI  short or long GI
 * @param lengthWithoutFCS length not include FCS ?
 * @return over-the-air tx duration
 */
uint16_t pkt_duration(uint16_t length, uint8_t mcs, bool wide40BW, bool usingSGI = false, bool lengthWithoutFCS = true);

/**
 * Parse 32-bit feature code into has* values of struct ExtraInfo
 * @param featureCode the input 32-bit feature code
 * @param extraInfo The ExtraInfo to be modified to reflect the feature code
 */
void featureCodeInterpretation(uint32_t featureCode, struct ExtraInfo * extraInfo);

/**
 * Parse the raw RxS data
 *
 * This method calls parse_rxs_enhanced for the core parsing task.
 *
 * @param inBytes pointer to the raw data head
 * @param parsingLevel parsing level (if parse CSI, or just head)
 * @return Parsed RxS struct
 *
 * @see parse_rxs_enhanced
 * @see RXS_enhanced
 * @see RXSParsingLevel
 */
struct RXS_enhanced parseRXS(const uint8_t * inBytes, enum RXSParsingLevel parsingLevel);

/**
 * Parse the RxS raw data into RXS_enhanced struct.
 *
 * @param inBytes pointer to the raw data head
 * @param rxs return RXS_enhanced struct
 * @param parsingLevel parsing level
 * @return 0 if success
 *
 * @see parseRXS
 * @see RXS_enhanced
 * @see RXSParsingLevel
 */
int parse_rxs_enhanced(const uint8_t * inBytes, struct RXS_enhanced *rxs, enum RXSParsingLevel parsingLevel);

/**
 * Parse the CSI data for an existing RXS_enhanced struct.
 *
 * The method will first check whether the parsingLevel field is EXTRA_CSI.
 * If not, it will parse the CSI data.
 *
 * @param rxs the RXS_enhanced struct to be parsed again
 * @return 0 if success
 */
int reparseCSIMatrixInRXS(struct RXS_enhanced *rxs);

/**
 * Directly inject (in-place add) an ExtraInfo Item into the raw RxS data.
 *
 * The transparency to the upper RxS parsing process makes this method very useful in case of adding some platform-calculated value into RXS struct.
 * Currently, this method is used to inject clock slope value into RXS
 *
 * @param rxs_raw the raw rxs data
 * @param featureCode_added the feature code for the adding value
 * @param data pointer to the value to be added
 * @param length value length
 */
void inplaceAddRxExtraInfo(uint8_t *rxs_raw, uint32_t featureCode_added, uint8_t * data, int length);

void inplaceAddTxChronosData(RXS_enhanced * rxs, uint8_t *dataBuffer, int bufferLength);

/**
 * a signature value for a mac-address, used for hashing.
 * @param macaddr 6-byte mac address
 * @return 
 */
inline uint64_t MACAddressSignature(const uint8_t *macaddr) {
    return macaddr[0] + macaddr[1] * (1UL << 8) + macaddr[2] * ( 1UL << 16) + macaddr[3] * ( 1UL << 24) + macaddr[4] * ( 1UL << 32) + macaddr[5] * ( 1UL << 40);
}

#endif // RXS_ENHANCED_H_
