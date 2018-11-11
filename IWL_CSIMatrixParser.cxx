//
// Created by 蒋志平 on 2018/11/10.
//

#include "CSIMatrixParser.h"
#ifdef BUILD_WITH_MEX
    #include "mex.h"
#endif

void iwl_parse_csi_matrix(const uint8_t *payload, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix) {
    uint32_t index = 0, remainder;

    for (auto subcarrierIdx = 0; subcarrierIdx < 30; subcarrierIdx ++ ) {
        index += 3;
        remainder = index % 8;

        for (auto nrxIdx = 0 ; nrxIdx < nrx ; nrxIdx ++ )
            for(auto ntxIdx = 0 ; ntxIdx < ntx ; ntxIdx ++ ) {
                auto position = ntxIdx * (nrx * 30) + nrxIdx * 30 + subcarrierIdx;
                char tmp = (payload[index / 8] >> remainder) |
                      (payload[index/8+1] << (8-remainder));
                csi_matrix[position].real((double)tmp);
                tmp = (payload[index / 8+1] >> remainder) |
                      (payload[index/8+2] << (8-remainder));
                csi_matrix[position].imag((double)tmp);
                index += 16;
            }
    }
}