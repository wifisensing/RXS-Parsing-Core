//
// Created by 蒋志平 on 2020/4/6.
//

#include "CSIMatrixParser.h"

void usrp_parse_csi_matrix(const uint8_t *csi_addr, int nltf, int num_tones, std::complex<double> *csi_matrix) {
    for (auto i = 0; i < nltf * num_tones; i++) {
        const auto *floatValue = ((std::complex<float> *) csi_addr + i);
        csi_matrix[i] = std::complex<double>(floatValue->real(), floatValue->imag());
    }
}