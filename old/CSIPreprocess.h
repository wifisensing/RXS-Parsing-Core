//
// Created by 蒋志平 on 2018/11/10.
//

#ifndef PICOSCENES_PLATFROM_CSI_PREPROCESS_HXX
#define PICOSCENES_PLATFROM_CSI_PREPROCESS_HXX

#include <cstring>
#include <cerrno>
#include <vector>
#include <array>
#include <cassert>
#include <thread>
#include <future>
#include <complex>
#include <deque>

int phaseUnwrapAroundDC(std::complex<double> csi_matrix[], double mag[], double phase[], int nrx, int ntx, int num_tones, int bw);

#endif //PICOSCENES_PLATFROM_CSI_PREPROCESS_HXX
