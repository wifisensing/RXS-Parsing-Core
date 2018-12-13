//
// Created by 蒋志平 on 2018/11/10.
//

#include "CSIMatrixParser.h"
#ifdef BUILD_WITH_MEX
    #include "mex.h"
#endif

static int positionComputationWRTPermutation(int nrx, int nltf, int num_tones, int ntxIdx, int nrxIdx, int nltfIdx, int subcarrierIdx, const struct ExtraInfo & rxExtraInfo) {
    auto new_nrxIdx = nrxIdx;
    if (rxExtraInfo.hasAntennaSelection == true && nrx > 1) {
        std::vector<int> ant_sel_values(std::begin(rxExtraInfo.ant_sel), std::begin(rxExtraInfo.ant_sel) + nrx);
        auto sorted_indexes = sort_indexes(ant_sel_values);
        sorted_indexes = sort_indexes(sorted_indexes); // double sort, shit but works !
        new_nrxIdx = sorted_indexes[nrxIdx];
    }

    return ntxIdx * (nltf * nrx * num_tones) + nltfIdx * (nrx * num_tones) + new_nrxIdx * num_tones + subcarrierIdx;
}

void iwl_parse_csi_matrix(const uint8_t *payload, int ntx, int nrx, int nltf, int num_tones, const struct ExtraInfo & rxExtraInfo, std::complex<double> *csi_matrix) {
    uint32_t index = 0, remainder;

    for (auto subcarrierIdx = 0; subcarrierIdx < num_tones; subcarrierIdx ++ ) {
        index += 3;
        remainder = index % 8;

        for (auto nrxIdx = 0 ; nrxIdx < nrx ; nrxIdx ++ )
            for (auto nltfIdx = 0; nltfIdx < nltf; nltfIdx ++)
                for(auto ntxIdx = 0 ; ntxIdx < ntx ; ntxIdx ++ ) {
                    auto position = positionComputationWRTPermutation(nrx, nltf, num_tones, ntxIdx, nrxIdx, nltfIdx, subcarrierIdx, rxExtraInfo);
                    char tmp = (payload[index / 8] >> remainder) |
                          (payload[index/8+1] << (8-remainder));
                    csi_matrix[position].real((double)tmp);
                    tmp = (payload[index / 8+1] >> remainder) |
                          (payload[index/8+2] << (8-remainder));
                    csi_matrix[position].imag((double)tmp);
                    index += 16;
    //                printf("pos=%3d, subcarrierIdx=%2d, nrxIdx=%d, ntxIdx=%d, real=%f, imag=%f\n", position, subcarrierIdx, nrxIdx, ntxIdx, csi_matrix[position].real(), csi_matrix[position].imag());
                }
    }
}