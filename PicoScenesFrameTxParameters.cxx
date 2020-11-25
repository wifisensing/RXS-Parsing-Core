//
// Created by 蒋志平 on 2020/11/6.
//

#include "PicoScenesFrameTxParameters.hxx"


std::string PicoScenesFrameTxParameters::toString() const {
    std::stringstream ss;
    ss << "tx_param[mcs=" << std::to_string(mcs[0]) << ", cbw=" << ChannelBandwidth2String(cbw) << ", gi=" << GuardInterval2String(guardInterval) << ", sounding =" << forceSounding << ", NESS=" << int(numExtraSounding) << ", coding=" << ChannelCoding2String(coding[0]) << "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameTxParameters &parameters) {
    os << parameters.toString();
    return os;
}
