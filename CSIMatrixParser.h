//
// Created by Zhiping Jiang on 17-10-4.
//

#ifndef CSIMATRIXPARSER_H
#define CSIMATRIXPARSER_H

#include <cstring>
#include <cerrno>
#include <vector>
#include <array>
#include <cassert>
#include <thread>
#include <future>
#include <complex>
#include <deque>
void parse_csi_matrix_old(const uint8_t* csi_addr, int nrx, int ntx, int num_tones, std::complex<double> csi_matrix[]);
void parse_csi_matrix(const uint8_t* csi_addr, int nrx, int ntx, int num_tones, std::complex<double> csi_matrix[]);

int phaseUnwrapAroundDC(const std::complex<double> csi_matrix[], double mag[], double phase[], int nrx, int ntx, int num_tones);
#endif //CSIMATRIXPARSER_H
