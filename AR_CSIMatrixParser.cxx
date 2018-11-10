//
// Created by Zhiping Jiang on 17-10-4.
//

#include "CSIMatrixParser.h"
#ifdef BUILD_WITH_MEX
    #include "mex.h"
#endif

static inline int signbit_convert(int data, int maxbit) {
    if (data & (1 << (maxbit - 1)))
    { /*  negative */
        data -= (1 << maxbit);    
    }
    return data;
}

static inline void parse10bitsValues(const uint8_t *rawByte, int * v0, int * v1, int * v2, int *v3) {
    static auto negativeSignBit = (1 << (10 - 1));
    static auto minNegativeValue = (1 << 10);
    int array[4];
    array[0] = ((rawByte[0] & 0xff) >> 0) + ((rawByte[1] & 0x03) << 8);
    array[1] = ((rawByte[1] & 0xfc) >> 2) + ((rawByte[2] & 0x0f) << 6);
    array[2] = ((rawByte[2] & 0xf0) >> 4) + ((rawByte[3] & 0x3f) << 4);
    array[3] = ((rawByte[3] & 0xc0) >> 6) + ((rawByte[4] & 0xff) << 2);
    for (int &i : array) {
        if (i & negativeSignBit)
            i -= minNegativeValue;
    }
    *v0 = array[0];
    *v1 = array[1];
    *v2 = array[2];
    *v3 = array[3];
}

static void parser_job(const uint8_t* csi_addr, int nrx, int ntx, int num_tones, int segmentStart, int segmentEnd, std::complex<double> csi_matrix[]) {
    auto value_pos = 0;
    auto subcarrierIdx = 0;
    auto ntxIdx = 0;
    auto nrxIdx = 0;
    auto position = 0;
    int v0, v1, v2, v3;
    for(auto i = segmentStart ; i < segmentEnd; i++) {
        parse10bitsValues(csi_addr + i * 5, &v0, &v1, &v2, &v3);

        value_pos = i * 2;
        nrxIdx = value_pos % nrx;
        ntxIdx = (value_pos / nrx) % ntx;
        subcarrierIdx = value_pos / (nrx * ntx);
        position = ntxIdx * (nrx * num_tones) + nrxIdx * num_tones + subcarrierIdx;
        csi_matrix[position].real(v1);
        csi_matrix[position].imag(v0);
//        printf("csi[%d][%d][%d].r = %f, .i = %f\n", nrxIdx, ntxIdx, subcarrierIdx ,csi_matrix[position].real(), csi_matrix[position].imag());

        value_pos = i * 2 + 1;
        nrxIdx = value_pos % nrx;
        ntxIdx = (value_pos / nrx) % ntx;
        subcarrierIdx = value_pos / (nrx * ntx);
        position = ntxIdx * (nrx * num_tones) + nrxIdx * num_tones + subcarrierIdx;
        csi_matrix[position].real(v3);
        csi_matrix[position].imag(v2);
//        printf("csi[%d][%d][%d].r = %f, .i = %f\n", nrxIdx, ntxIdx, subcarrierIdx ,csi_matrix[position].real(), csi_matrix[position].imag());
    }
}

void ar_parse_csi_matrix(const uint8_t *csi_addr, int nrx, int ntx, int num_tones, std::complex<double> *csi_matrix) {
    auto totalSegments = nrx * ntx * num_tones / 2;
    parser_job(csi_addr, nrx, ntx, num_tones, 0, totalSegments, csi_matrix);
//    NO situation is found that parallel_for is faster than single-thread...sigh...
//#include <tbb/tbb.h>
//    tbb::parallel_for(tbb::blocked_range<size_t>(0, totalSegments), [=](const tbb::blocked_range<size_t> & r) {
//        parser_job(csi_addr, nrx, ntx, num_tones, r.begin(), r.end(), csi_matrix);
//    });
}

void ar_parse_csi_matrix_old(const uint8_t* csi_addr, int nrx, int ntx, int num_tones, std::complex<double> csi_matrix[]){
#define BITS_PER_SYMBOL         10
    uint8_t k;
    uint8_t bits_left, nrx_idx, ntx_idx, position;
    uint32_t bitmask, idx, current_data, h_data, h_idx;
    int real,imag;
    /* init bits_left
     * we process 16 bits at a time*/
    bits_left = 16;

    /* according to the h/w, we have 10 bit resolution 
     * for each real and imag value of the csi matrix H 
     */
    bitmask = (1 << 10) - 1;
    idx = h_idx = 0;
    /* get 16 bits for processing */
    h_data = csi_addr[idx++];
    h_data += (csi_addr[idx++] << 8);
    current_data = h_data & ((1 << 16) -1);
    
    /* loop for every subcarrier */
    for(k = 0;k < num_tones;k++){
        /* loop for each tx antenna */
        for(ntx_idx = 0;ntx_idx < ntx;ntx_idx++){
            /* loop for each rx antenna */
            for(nrx_idx = 0;nrx_idx < nrx;nrx_idx++){
                position = ntx_idx * (nrx * num_tones) + nrx_idx * num_tones + k;
                /* bits number less than 10, get next 16 bits */
                if((bits_left - 10) < 0){
                    h_data = csi_addr[idx++];
                    h_data += (csi_addr[idx++] << 8);
                    current_data += h_data << bits_left;
                    bits_left += 16;
                }
                
                imag = current_data & bitmask;
                imag = signbit_convert(imag, BITS_PER_SYMBOL);
                csi_matrix[position].imag(imag);

                bits_left -= 10;
                current_data = current_data >> 10;
                
                /* bits number less than 10, get next 16 bits */
                if((bits_left - 10) < 0){
                    h_data = csi_addr[idx++];
                    h_data += (csi_addr[idx++] << 8);
                    current_data += h_data << bits_left;
                    bits_left += 16;
                }

                real = current_data & bitmask;
                real = signbit_convert(real, BITS_PER_SYMBOL);
                csi_matrix[position].real(real);

                bits_left -= 10;
                current_data = current_data >> 10;
//                printf("csi_old[%d][%d][%d].r = %f, .i = %f\n", nrx_idx, ntx_idx, k ,csi_matrix[position].real(), csi_matrix[position].imag());
            }
        }
    }
#undef BITS_PER_SYMBOL
}
