//
// Created by Zhiping Jiang on 11/27/22.
//

#include "FrontEndModePreset.hxx"

const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_320_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_320_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 320, std::nullopt, "Equivalent to \"--format ehtsu --coding ldpc --cbw 320\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_160_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_160_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 160, std::nullopt, "Equivalent to \"--format ehtsu --coding ldpc --cbw 160\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_160_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_160_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 160, std::nullopt, "Equivalent to \"--format hesu --coding ldpc --cbw 160\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_160_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_160_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 160, std::nullopt, "Equivalent to \"--format vht --coding bcc --cbw 160\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_160_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_160_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 160, std::nullopt, "Equivalent to \"--format vht --coding ldpc --cbw 160\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_80_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_80_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 80, std::nullopt, "Equivalent to \"--format ehtsu --coding ldpc --cbw 80\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_80_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_80_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 80, std::nullopt, "Equivalent to \"--format hesu --coding ldpc --cbw 80\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_80_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_80_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 80, std::nullopt, "Equivalent to \"--format vht --coding bcc --cbw 80\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_80_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_80_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 80, std::nullopt, "Equivalent to \"--format vht --coding ldpc --cbw 80\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_40_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_40_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 40, std::nullopt, "Equivalent to \"--format ehtsu --coding ldpc --cbw 40\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_40_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_40_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 40, std::nullopt, "Equivalent to \"--format hesu --coding ldpc --cbw 40\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_40_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_40_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 40, std::nullopt, "Equivalent to \"--format vht --coding bcc --cbw 40\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_40_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_40_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 40, std::nullopt, "Equivalent to \"--format vht --coding ldpc --cbw 40\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_40_HT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_40_HT", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::BCC, 40, std::nullopt, "Equivalent to \"--format ht --coding bcc --cbw 40\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_40_HT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_40_HT_LDPC", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::LDPC, 40, std::nullopt, "Equivalent to \"--format ht --coding ldpc --cbw 40\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::BCC, 20, std::nullopt, "Equivalent to \"--format ehtsu --coding ldpc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_EHTSU_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_EHTSU_LDPC", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 20, std::nullopt, "Equivalent to \"--format ehtsu --coding ldpc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::BCC, 20, std::nullopt, "Equivalent to \"--format hesu --coding ldpc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_HESU_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_HESU_LDPC", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 20, std::nullopt, "Equivalent to \"--format hesu --coding ldpc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 20, std::nullopt, "Equivalent to \"--format vht --coding bcc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 20, std::nullopt, "Equivalent to \"--format vht --coding ldpc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_HT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_HT", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::BCC, 20, std::nullopt, "Equivalent to \"--format ht --coding bcc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_HT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_HT_LDPC", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::LDPC, 20, std::nullopt, "Equivalent to \"--format ht --coding ldpc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TX_CBW_20_NonHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TX_CBW_20_NONHT", PacketFormatEnum::PacketFormat_NonHT, ChannelCodingEnum::BCC, 20, std::nullopt, "Equivalent to \"--format nonht --coding bcc --cbw 20\". Optimal tx-rate and tx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::RX_CBW_320 = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("RX_CBW_320", PacketFormatEnum::PacketFormat_Unknown, ChannelCodingEnum::BCC, std::nullopt, 320, "Equivalent to \"--rx-cbw 320\". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::RX_CBW_160 = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("RX_CBW_160", PacketFormatEnum::PacketFormat_Unknown, ChannelCodingEnum::BCC, std::nullopt, 160, "Equivalent to \"--rx-cbw 160\". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::RX_CBW_80 = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("RX_CBW_80", PacketFormatEnum::PacketFormat_Unknown, ChannelCodingEnum::BCC, std::nullopt, 80, "Equivalent to \"--rx-cbw 80\". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::RX_CBW_40 = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("RX_CBW_40", PacketFormatEnum::PacketFormat_Unknown, ChannelCodingEnum::BCC, std::nullopt, 40, "Equivalent to \"--rx-cbw 40\". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::RX_CBW_20 = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("RX_CBW_20", PacketFormatEnum::PacketFormat_Unknown, ChannelCodingEnum::BCC, std::nullopt, 20, "Equivalent to \"--rx-cbw 20\". Optimal rx-rate and rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_320_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_320_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 320, 320, "Equivalent to \"--format ehtsu --coding ldpc --cbw 320 --rx-cbw 320\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_160_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_160_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 160, 160, "Equivalent to \"--format ehtsu --coding ldpc --cbw 160 --rx-cbw 160\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_160_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_160_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 160, 160, "Equivalent to \"--format hesu --coding ldpc --cbw 160 --rx-cbw 160\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_160_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_160_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 160, 160, "Equivalent to \"--format vht --coding bcc --cbw 160 --rx-cbw 160\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_160_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_160_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 160, 160, "Equivalent to \"--format vht --coding ldpc --cbw 160 --rx-cbw 160\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_80_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_80_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 80, 80, "Equivalent to \"--format ehtsu --coding ldpc --cbw 80 --rx-cbw 80\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_80_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_80_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 80, 80, "Equivalent to \"--format hesu --coding ldpc --cbw 80 --rx-cbw 80\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_80_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_80_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 80, 80, "Equivalent to \"--format vht --coding bcc --cbw 80 --rx-cbw 80\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_80_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_80_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 80, 80, "Equivalent to \"--format vht --coding ldpc --cbw 80 --rx-cbw 80\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_40_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_40_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 40, 40, "Equivalent to \"--format ehtsu --coding ldpc --cbw 40 --rx-cbw 40\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_40_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_40_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 40, 40, "Equivalent to \"--format hesu --coding ldpc --cbw 40 --rx-cbw 40\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_40_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_40_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 40, 40, "Equivalent to \"--format vht --coding bcc --cbw 40 --rx-cbw 40\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_40_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_40_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 40, 40, "Equivalent to \"--format vht --coding ldpc --cbw 40 --rx-cbw 40\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_40_HT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_40_HT", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::BCC, 40, 40, "Equivalent to \"--format ht --coding bcc --cbw 40 --rx-cbw 40\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_40_HT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_40_HT_LDPC", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::LDPC, 40, 40, "Equivalent to \"--format ht --coding ldpc --cbw 40 --rx-cbw 40\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_EHTSU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_EHTSU", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::BCC, 20, 20, "Equivalent to \"--format ehtsu --coding ldpc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_EHTSU_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_EHTSU_LDPC", PacketFormatEnum::PacketFormat_EHTSU, ChannelCodingEnum::LDPC, 20, 20, "Equivalent to \"--format ehtsu --coding ldpc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_HESU = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_HESU", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::BCC, 20, 20, "Equivalent to \"--format hesu --coding ldpc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_HESU_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_HESU_LDPC", PacketFormatEnum::PacketFormat_HESU, ChannelCodingEnum::LDPC, 20, 20, "Equivalent to \"--format hesu --coding ldpc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_VHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_VHT", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::BCC, 20, 20, "Equivalent to \"--format vht --coding bcc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_VHT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_VHT_LDPC", PacketFormatEnum::PacketFormat_VHT, ChannelCodingEnum::LDPC, 20, 20, "Equivalent to \"--format vht --coding ldpc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_HT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_HT", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::BCC, 20, 20, "Equivalent to \"--format ht --coding bcc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_HT_LDPC = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_HT_LDPC", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::LDPC, 20, 20, "Equivalent to \"--format ht --coding ldpc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::TR_CBW_20_NonHT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("TR_CBW_20_NONHT", PacketFormatEnum::PacketFormat_NonHT, ChannelCodingEnum::BCC, 20, 20, "Equivalent to \"--format nonht --coding bcc --cbw 20 --rx-cbw 20\". Optimal tx/rx-rate and tx/rx-resample-ratio values will be set automatically for SDR devices."));
const std::shared_ptr<FrontEndModePreset> FrontEndModePreset::DEFAULT = std::shared_ptr<FrontEndModePreset>(new FrontEndModePreset("DEFAULT", PacketFormatEnum::PacketFormat_HT, ChannelCodingEnum::BCC, 20, 20, "Equivalent to TR_CBW_20_HT."));

const std::vector<std::shared_ptr<FrontEndModePreset>> &FrontEndModePreset::getPresetList() {
    static std::vector<std::shared_ptr<FrontEndModePreset>> list{
            TX_CBW_320_EHTSU,
            TX_CBW_160_EHTSU,
            TX_CBW_160_HESU,
            TX_CBW_160_VHT,
            TX_CBW_160_VHT_LDPC,
            TX_CBW_80_EHTSU,
            TX_CBW_80_HESU,
            TX_CBW_80_VHT,
            TX_CBW_80_VHT_LDPC,
            TX_CBW_40_EHTSU,
            TX_CBW_40_HESU,
            TX_CBW_40_VHT,
            TX_CBW_40_VHT_LDPC,
            TX_CBW_40_HT,
            TX_CBW_40_HT_LDPC,
            TX_CBW_20_EHTSU,
            TX_CBW_20_EHTSU_LDPC,
            TX_CBW_20_HESU,
            TX_CBW_20_HESU_LDPC,
            TX_CBW_20_VHT,
            TX_CBW_20_VHT_LDPC,
            TX_CBW_20_HT,
            TX_CBW_20_HT_LDPC,
            TX_CBW_20_NonHT,
            RX_CBW_320,
            RX_CBW_160,
            RX_CBW_80,
            RX_CBW_40,
            RX_CBW_20,
            TR_CBW_320_EHTSU,
            TR_CBW_160_EHTSU,
            TR_CBW_160_HESU,
            TR_CBW_160_VHT,
            TR_CBW_160_VHT_LDPC,
            TR_CBW_80_EHTSU,
            TR_CBW_80_HESU,
            TR_CBW_80_VHT,
            TR_CBW_80_VHT_LDPC,
            TR_CBW_40_EHTSU,
            TR_CBW_40_HESU,
            TR_CBW_40_VHT,
            TR_CBW_40_VHT_LDPC,
            TR_CBW_40_HT,
            TR_CBW_40_HT_LDPC,
            TR_CBW_20_EHTSU,
            TR_CBW_20_EHTSU_LDPC,
            TR_CBW_20_HESU,
            TR_CBW_20_HESU_LDPC,
            TR_CBW_20_VHT,
            TR_CBW_20_VHT_LDPC,
            TR_CBW_20_HT,
            TR_CBW_20_HT_LDPC,
            TR_CBW_20_NonHT,
            DEFAULT,
    };

    return list;
}

const std::map<std::string, std::shared_ptr<FrontEndModePreset>> &FrontEndModePreset::getPresetMap() {
    static std::map<std::string, std::shared_ptr<FrontEndModePreset>> presetMap;
    static std::once_flag flag;
    std::call_once(flag, [&] {
        for (const auto &preset: getPresetList())
            presetMap.emplace(preset->presetName, preset);
    });

    return presetMap;
}

FrontEndModePreset::FrontEndModePreset(std::string presetName, PacketFormatEnum format, ChannelCodingEnum coding,
                                       const std::optional<double> &txCbw, const std::optional<double> &rxCbw,
                                       std::string description) :
        presetName(std::move(presetName)), format(format), coding(coding),
        txCBW(txCbw), rxCBW(rxCbw),
        description(std::move(description)) {}

std::string FrontEndModePreset::printHelpContentForFrontEndModePreset() {
    std::stringstream ss;
    ss << "\n\nPicoScenes supports the following COTS NIC/SDR frontend mode presets:\n";
    ss << "-------------------------------------------------------------------------------------\n";
    for (const auto &preset: getPresetList())
        ss << std::setw(20) << preset->presetName << ": " << preset->description << "\n";
    ss << "-------------------------------------------------------------------------------------\n\n";

    return ss.str();
}

bool FrontEndModePreset::operator==(const FrontEndModePreset &rhs) const {
    return std::tie(presetName, format, coding, txCBW, rxCBW) == std::tie(rhs.presetName, rhs.format, rhs.coding, rhs.txCBW, rhs.rxCBW);
}

bool FrontEndModePreset::operator<(const FrontEndModePreset &rhs) const {
    return std::tie(presetName, format, coding, txCBW, rxCBW) < std::tie(rhs.presetName, rhs.format, rhs.coding, rhs.txCBW, rhs.rxCBW);
}

std::ostream &operator<<(std::ostream &os, const FrontEndModePreset &preset) {
    os << "presetName: " << preset.presetName << " description: " << preset.description;
    return os;
}
