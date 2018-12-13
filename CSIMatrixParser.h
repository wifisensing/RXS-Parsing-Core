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
#include <numeric>
#include <algorithm>
#include "rxs_enhanced.h"

template <typename T> std::vector<size_t> sort_indexes(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}

void ar_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int num_tones, std::complex<double> *csi_matrix);
void iwl_parse_csi_matrix(const uint8_t *csi_addr, int ntx, int nrx, int nltf, int num_tones, const struct ExtraInfo & rxExtraInfo, std::complex<double> *csi_matrix);

#endif //CSIMATRIXPARSER_H
