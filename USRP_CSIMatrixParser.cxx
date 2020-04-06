//
// Created by 蒋志平 on 2020/4/6.
//

#include "CSIMatrixParser.h"

void usrp_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix) {
    memcpy(csi_matrix, csi_addr, ntx * nrx * num_tones * sizeof(std::complex<double>));
}