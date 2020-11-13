//
// Created by 蒋志平 on 2020/11/6.
//

#include "PicoScenesFrameTxParameters.hxx"


std::string PicoScenesFrameTxParameters::toString() const {
    std::stringstream ss;
//    ss << "tx_param[mcs=" << int(mcs) << ", bonding=" << channelBonding << ", sgi=" << useShortGI << ", sounding =" << forceSounding << ", NESS=" << int(numExtraSounding) << ", LDPC=" << useLDPC << "]";
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const PicoScenesFrameTxParameters &parameters) {
    os << parameters.toString();
    return os;
}