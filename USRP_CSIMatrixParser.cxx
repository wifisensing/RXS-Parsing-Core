//
// Created by 蒋志平 on 2020/4/6.
//

#include "CSIMatrixParser.h"

void usrp_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix) {
    for (auto i = 0; i < ntx * nrx * num_tones; i++)
        csi_matrix[i] = std::complex<double>(((std::complex<float> *) csi_addr + i)->real(), ((std::complex<float> *) csi_addr + i)->imag());
}