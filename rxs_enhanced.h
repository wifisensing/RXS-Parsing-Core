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
#include <iostream>
#include <optional>
#include "RXSExtraInfo.h"
#include "CSIMatrixParser.h"
#include "CSIPreprocess.h"
#ifdef BUILD_WITH_MEX
    #include "mex.h"
    #define printf mexPrintf
#endif


#define TONE_20M                56 ///< Ath9k NIC returns 56 subcarrier in 20MHz bandwidth
#define TONE_40M                114 ///< Ath9k NIC returns 114 subcarrier in 40MHz bandwidth
#define TONE_20M_UNWRAP         57
#define TONE_40M_UNWRAP         117
#define MAX_OFDM_TONES          (TONE_40M * 4 * 4) ///< in 4x4 MIMO + 40MHZ case
#define MAX_OFDM_TONES_UNWRAP   (TONE_40M_UNWRAP * 4 * 4) ///< in 4x4 MIMO + 40MHZ case
#define BITS_PER_BYTE           8
#define BITS_PER_COMPLEX_SYMBOL (2 * BITS_PER_SYMBOL)
#define BITS_PER_SYMBOL         10

/**
 * Shared RxStatus basic information for Qualcomm Atheros AR9300 series and Intel 5300
 */
PACK(struct rx_status_basic {
    uint64_t   tstamp;      ///< h/w assigned RX timestamp
    uint16_t   csi_len;     ///< csi length
    uint16_t   channel;     ///< receiving channel frequency, unit is MHz, e.g, 2437
    uint8_t	   sgi;	        ///< short GI, 1 for Short

    int8_t     noise;       ///< noise floor
    uint8_t	   rate;	    ///< MCS index
    uint8_t    channelBonding; ///< receiving channel bandwidth, 0 for 20MHz, 1 for 40MHz
    uint8_t    num_tones;   ///< number of tones (subcarriers), should be 56 or 114
    uint8_t    nrx;         ///< number of receiving antennas, 1~3
    uint8_t    ntx;         ///< number of transmitting antennas, 1~3
	uint8_t    nltf;        ///< number of LTF field, 1~3
	uint8_t    nss;         ///< number of CSI measurement groups

    uint8_t    rssi;        ///< rx frame RSSI
    uint8_t    rssi0;       ///< rx frame RSSI [ctl, chain 0]
    uint8_t    rssi1;       ///< rx frame RSSI [ctl, chain 1]
    uint8_t    rssi2;       ///< rx frame RSSI [ctl, chain 2]
});

/**
 Atheros Tx Status structure
 */
PACK(struct ath_tx_status {
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
});

struct ieee80211_packet_header_frame_control {
    uint16_t version         :2,
             type            :2,
             subtype         :4,
             toDS            :1,
             fromDS          :1,
             moreFrags       :1,
             retry           :1,
             power_mgmt      :1,
             more            :1,
             protect         :1,
             order           :1;
};

#ifdef __GNUC__
struct ieee80211_packet_header_info {
    uint8_t version        :2,
			isShortHeader  :1,
			needACK        :1,
			moreIsComming  :1, ///< indicating that this packet is segmented and Tx-ed in several sub-packets.
			isReTX         :1, ///< is re-transmission packet
			hasTxExtraInfo :1, ///< contains Tx ExtraInfo @see ExtraInfo
			hasEchoProbeInfo :1; ///< contains EchoProbeInfo @see EchoProbeInfo
    uint8_t frameType;
    uint16_t taskId;
} __attribute__ ((__packed__));
#endif

#ifdef _MSC_VER
__pragma( pack(push, 1) ) struct ieee80211_packet_header_info __pragma( pack(pop)) {
    uint8_t version        :2,
            isShortHeader  :1,
            needACK        :1,
            moreIsComming  :1, ///< indicating that this packet is segmented and Tx-ed in several sub-packets.
            isReTX         :1, ///< is re-transmission packet
            hasTxExtraInfo :1, ///< contains Tx ExtraInfo @see ExtraInfo
            hasEchoProbeInfo :1; ///< contains EchoProbeInfo @see EchoProbeInfo
    uint8_t frameType;
    uint16_t taskId;
};
#endif

PACK(struct ieee80211_packet_header {
    uint16_t  fc;
	uint16_t dur; // Atheros NIC will over-write this value, so leave alone.
    uint8_t	 addr1[6];
	uint8_t	 addr2[6];
	uint8_t	 addr3[6];
    uint16_t seq;
	uint16_t segment_head_seq;
	struct ieee80211_packet_header_info header_info;
});

PACK(struct EchoProbeInfo {
	uint16_t length;
	uint8_t  version;
	uint8_t  ackRequestType; // 0 for NO ACK, 1 for ACK with simple Injection, 2 for ACK with Injection containing TxRXS.
    int8_t   ackMCS;         // 0 to 23 are OK, negative means use default (maybe mcs 0).
	int8_t   ackBandWidth;   // 0 for 20MHz, 1 for 40MHz, negative means use default (maybe 20MHz).
	int8_t   ackSGI;         // 0 for LGI, 1 for SGI, negative means use default (maybe LGI).
    int8_t   ackTxChainMask; // negative means use default
	int8_t   ackRxChainMask; // negative means use default
	int8_t   ackTxpower;     // negative means use default
	int64_t  frequency;
    int16_t  pll_rate;
	int8_t   pll_refdiv;
	int8_t   pll_clock_select;
    int32_t  ackExpectedDelay_us;
	uint16_t TxRXSLength;    // 0 means no TxRXS
});

struct RXS_enhanced {
	bool isAR9300;
    struct rx_status_basic rxs_basic;
	uint16_t   csi_pos;     /* CSI data position in raw buffer, used for re-parsing.*/
	uint16_t   payload_pos;
	uint16_t   payloadLength;
    uint16_t   rawBufferLength;
	double     basebandFs;
    uint16_t   txDuration;
    struct ExtraInfo rxExtraInfo;
    struct ieee80211_packet_header txHeader;
    struct ExtraInfo txExtraInfo;
    struct EchoProbeInfo echoProbeInfo;
    std::complex<double> csi_matrix[MAX_OFDM_TONES_UNWRAP];
    double unwrappedMag[MAX_OFDM_TONES_UNWRAP];
    double unwrappedPhase[MAX_OFDM_TONES_UNWRAP];
    uint8_t  chronosACKBody[3000];
    uint8_t  rawBuffer[6000];
	RXSParsingLevel parsingLevel;
};

double ath9kPLLBandwidthComputation(double multipler, double devider, double clk_select, bool channelBonding);

/**
 * calculate the over-the-air Tx duration for a packet
 * @param length packet length
 * @param mcs  Tx MCS value
 * @param nltf number of HT-LTF 
 * @param bb_bandwidth baseband bandwidth (Hz)
 * @param channelBonding  802.11n channel bonding feature, true for bonding
 * @param usingSGI  short or long GI
 * @param lengthWithoutFCS length not include FCS ?
 * @return over-the-air tx duration
 */
uint16_t pkt_duration(uint16_t length, uint8_t mcs, uint8_t nltf, double bb_bandwidth, bool channelBonding, bool usingSGI, bool lengthWithoutFCS = true);

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
struct RXS_enhanced parseRXS(const uint8_t *inBytes, enum RXSParsingLevel parsingLevel);

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

void inplaceAddTxChronosData(RXS_enhanced * rxs, uint8_t *dataBuffer, int bufferLength);

/**
 * a signature value for a mac-address, used for hashing.
 * @param macaddr 6-byte mac address
 * @return 
 */
inline uint64_t MACAddressSignature(const uint8_t *macaddr) {
    return macaddr[0] + macaddr[1] * (1UL << 8) + macaddr[2] * ( 1UL << 16) + macaddr[3] * ( 1UL << 24) + macaddr[4] * ( 1ULL << 32) + macaddr[5] * ( 1ULL << 40);
}

#endif // RXS_ENHANCED_H_
