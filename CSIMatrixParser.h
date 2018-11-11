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

void ar_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix);
void iwl_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix);

#endif //CSIMATRIXPARSER_H
