//
// Created by 蒋志平 on 2020/4/6.
//

#include "CSIMatrixParser.h"

void usrp_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix, std::complex<double> *nonht_csi) {
    for (auto i = 0; i < ntx * nrx * num_tones; i++) {
        const auto *floatValue = ((std::complex<float> *) csi_addr + i);
        csi_matrix[i] = std::complex<double>(floatValue->real(), floatValue->imag());
    }

    auto HTCSILength = sizeof(std::complex<float>) * num_tones;
    for (auto i = 0; i < 52; i++) {
        const auto *floatValue = ((std::complex<float> *) csi_addr + HTCSILength);
        nonht_csi[i] = std::complex<double>(floatValue->real(), floatValue->imag());
    }

}