//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: CSIPreprocessor.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 18-Nov-2020 19:05:56
//

// Include Files
#include "CSIPreprocessor.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "rt_defines.h"
#include <cfloat>
#include <cmath>

// Function Declarations
namespace coder
{
  namespace internal
  {
    static int b_bsearch(const ::coder::array<double, 1U> &x, double xi);
  }

  static void interp1(const ::coder::array<double, 1U> &varargin_1, const ::
                      coder::array<double, 1U> &varargin_2, const ::coder::array<
                      double, 1U> &varargin_3, ::coder::array<double, 1U> &Vq);
}

static int div_s32(int numerator, int denominator);
static double rt_atan2d_snf(double u0, double u1);
static double rt_hypotd_snf(double u0, double u1);
static double rt_remd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : const ::coder::array<double, 1U> &x
//                double xi
// Return Type  : int
//
namespace coder
{
  namespace internal
  {
    static int b_bsearch(const ::coder::array<double, 1U> &x, double xi)
    {
      int high_i;
      int low_ip1;
      int n;
      high_i = x.size(0);
      n = 1;
      low_ip1 = 2;
      while (high_i > low_ip1) {
        int mid_i;
        mid_i = (n >> 1) + (high_i >> 1);
        if (((n & 1) == 1) && ((high_i & 1) == 1)) {
          mid_i++;
        }

        if (xi >= x[mid_i - 1]) {
          n = mid_i;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }

      return n;
    }

    //
    // Arguments    : const ::coder::array<double, 1U> &varargin_1
    //                const ::coder::array<double, 1U> &varargin_2
    //                const ::coder::array<double, 1U> &varargin_3
    //                ::coder::array<double, 1U> &Vq
    // Return Type  : void
    //
  }

  static void interp1(const ::coder::array<double, 1U> &varargin_1, const ::
                      coder::array<double, 1U> &varargin_2, const ::coder::array<
                      double, 1U> &varargin_3, ::coder::array<double, 1U> &Vq)
  {
    array<double, 1U> x;
    array<double, 1U> y;
    int k;
    int nd2;
    int nx;
    y.set_size(varargin_2.size(0));
    nd2 = varargin_2.size(0);
    for (k = 0; k < nd2; k++) {
      y[k] = varargin_2[k];
    }

    x.set_size(varargin_1.size(0));
    nd2 = varargin_1.size(0);
    for (k = 0; k < nd2; k++) {
      x[k] = varargin_1[k];
    }

    nx = varargin_1.size(0) - 1;
    Vq.set_size(varargin_3.size(0));
    nd2 = varargin_3.size(0);
    for (k = 0; k < nd2; k++) {
      Vq[k] = rtNaN;
    }

    if (varargin_3.size(0) != 0) {
      k = 0;
      int exitg1;
      do {
        exitg1 = 0;
        if (k <= nx) {
          if (std::isnan(varargin_1[k])) {
            exitg1 = 1;
          } else {
            k++;
          }
        } else {
          double xtmp;
          int n;
          if (varargin_1[1] < varargin_1[0]) {
            k = (nx + 1) >> 1;
            for (n = 0; n < k; n++) {
              xtmp = x[n];
              nd2 = nx - n;
              x[n] = x[nd2];
              x[nd2] = xtmp;
            }

            if ((varargin_2.size(0) != 0) && (varargin_2.size(0) > 1)) {
              n = varargin_2.size(0) - 1;
              nd2 = varargin_2.size(0) >> 1;
              for (k = 0; k < nd2; k++) {
                xtmp = y[k];
                nx = n - k;
                y[k] = y[nx];
                y[nx] = xtmp;
              }
            }
          }

          nd2 = varargin_3.size(0);
          for (k = 0; k < nd2; k++) {
            if (std::isnan(varargin_3[k])) {
              Vq[k] = rtNaN;
            } else {
              if ((!(varargin_3[k] > x[x.size(0) - 1])) && (!(varargin_3[k] < x
                    [0]))) {
                n = internal::b_bsearch(x, varargin_3[k]) - 1;
                xtmp = (varargin_3[k] - x[n]) / (x[n + 1] - x[n]);
                if (xtmp == 0.0) {
                  Vq[k] = y[n];
                } else if (xtmp == 1.0) {
                  Vq[k] = y[n + 1];
                } else {
                  double d;
                  d = y[n + 1];
                  if (y[n] == d) {
                    Vq[k] = y[n];
                  } else {
                    Vq[k] = (1.0 - xtmp) * y[n] + xtmp * d;
                  }
                }
              }
            }
          }

          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }
  }

  //
  // Arguments    : int numerator
  //                int denominator
  // Return Type  : int
  //
}

static int div_s32(int numerator, int denominator)
{
  unsigned int b_numerator;
  int quotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    unsigned int b_denominator;
    if (numerator < 0) {
      b_numerator = ~static_cast<unsigned int>(numerator) + 1U;
    } else {
      b_numerator = static_cast<unsigned int>(numerator);
    }

    if (denominator < 0) {
      b_denominator = ~static_cast<unsigned int>(denominator) + 1U;
    } else {
      b_denominator = static_cast<unsigned int>(denominator);
    }

    b_numerator /= b_denominator;
    if ((numerator < 0) != (denominator < 0)) {
      quotient = -static_cast<int>(b_numerator);
    } else {
      quotient = static_cast<int>(b_numerator);
    }
  }

  return quotient;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else if (std::isinf(u0) && std::isinf(u1)) {
    int b_u0;
    int b_u1;
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = std::atan2(static_cast<double>(b_u0), static_cast<double>(b_u1));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }

  return y;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = std::abs(u0);
  y = std::abs(u1);
  if (a < y) {
    a /= y;
    y *= std::sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * std::sqrt(y * y + 1.0);
  } else {
    if (!std::isnan(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_remd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1) || std::isinf(u0)) {
    y = rtNaN;
  } else if (std::isinf(u1)) {
    y = u0;
  } else if ((u1 != 0.0) && (u1 != std::trunc(u1))) {
    double q;
    q = std::abs(u0 / u1);
    if (!(std::abs(q - std::floor(q + 0.5)) > DBL_EPSILON * q)) {
      y = 0.0 * u0;
    } else {
      y = std::fmod(u0, u1);
    }
  } else {
    y = std::fmod(u0, u1);
  }

  return y;
}

//
// Arguments    : const coder::array<creal_T, 2U> &CSI
//                const coder::array<short, 1U> &subcarrierIndex_int16
//                coder::array<creal_T, 2U> &newCSI
//                coder::array<short, 1U> &interpedIndex_int16
// Return Type  : void
//
void CSIPreprocessor(const coder::array<creal_T, 2U> &CSI, const coder::array<
                     short, 1U> &subcarrierIndex_int16, coder::array<creal_T, 2U>
                     &newCSI, coder::array<short, 1U> &interpedIndex_int16)
{
  coder::array<creal_T, 1U> r2;
  coder::array<double, 2U> r;
  coder::array<double, 2U> y;
  coder::array<double, 1U> b_y;
  coder::array<double, 1U> c_y;
  coder::array<double, 1U> interpedIndex;
  coder::array<double, 1U> r1;
  coder::array<double, 1U> subcarrierIndex;
  coder::array<double, 1U> vwork;
  double cumsum_dp_corr;
  double dp_corr;
  double xtmp;
  int exitg1;
  int i1;
  int k;
  int loop_ub;
  int n;
  int outsize_idx_1;
  int sz_idx_1;
  int vstride;
  subcarrierIndex.set_size(subcarrierIndex_int16.size(0));
  loop_ub = subcarrierIndex_int16.size(0);
  for (vstride = 0; vstride < loop_ub; vstride++) {
    subcarrierIndex[vstride] = subcarrierIndex_int16[vstride];
  }

  n = subcarrierIndex.size(0);
  if (subcarrierIndex.size(0) <= 2) {
    if (subcarrierIndex.size(0) == 1) {
      i1 = static_cast<int>(subcarrierIndex[0]);
    } else if (subcarrierIndex[0] > subcarrierIndex[1]) {
      i1 = static_cast<int>(subcarrierIndex[1]);
    } else {
      i1 = static_cast<int>(subcarrierIndex[0]);
    }
  } else {
    i1 = static_cast<int>(subcarrierIndex[0]);
    for (k = 2; k <= n; k++) {
      vstride = static_cast<int>(subcarrierIndex[k - 1]);
      if (i1 > vstride) {
        i1 = vstride;
      }
    }
  }

  n = subcarrierIndex.size(0);
  if (subcarrierIndex.size(0) <= 2) {
    if (subcarrierIndex.size(0) == 1) {
      outsize_idx_1 = static_cast<int>(subcarrierIndex[0]);
    } else if (subcarrierIndex[0] < subcarrierIndex[1]) {
      outsize_idx_1 = static_cast<int>(subcarrierIndex[1]);
    } else {
      outsize_idx_1 = static_cast<int>(subcarrierIndex[0]);
    }
  } else {
    outsize_idx_1 = static_cast<int>(subcarrierIndex[0]);
    for (k = 2; k <= n; k++) {
      vstride = static_cast<int>(subcarrierIndex[k - 1]);
      if (outsize_idx_1 < vstride) {
        outsize_idx_1 = vstride;
      }
    }
  }

  if (outsize_idx_1 < i1) {
    y.set_size(1, 0);
  } else {
    loop_ub = static_cast<short>(outsize_idx_1) - static_cast<short>(i1);
    y.set_size(1, (loop_ub + 1));
    for (vstride = 0; vstride <= loop_ub; vstride++) {
      y[vstride] = static_cast<short>(static_cast<short>(i1) + static_cast<short>
        (vstride));
    }
  }

  b_y.set_size(subcarrierIndex.size(0));
  loop_ub = subcarrierIndex.size(0);
  for (vstride = 0; vstride < loop_ub; vstride++) {
    b_y[vstride] = subcarrierIndex[vstride];
  }

  vwork.set_size(subcarrierIndex.size(0));
  loop_ub = subcarrierIndex.size(0);
  for (vstride = 0; vstride < loop_ub; vstride++) {
    vwork[vstride] = subcarrierIndex[vstride];
  }

  n = subcarrierIndex.size(0) - 1;
  outsize_idx_1 = y.size(1);
  r.set_size(1, y.size(1));
  for (vstride = 0; vstride < outsize_idx_1; vstride++) {
    r[vstride] = rtNaN;
  }

  if (y.size(1) != 0) {
    k = 0;
    do {
      exitg1 = 0;
      if (k <= n) {
        if (std::isnan(subcarrierIndex[k])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
          vstride = (n + 1) >> 1;
          for (i1 = 0; i1 < vstride; i1++) {
            xtmp = vwork[i1];
            outsize_idx_1 = n - i1;
            vwork[i1] = vwork[outsize_idx_1];
            vwork[outsize_idx_1] = xtmp;
          }

          if ((subcarrierIndex.size(0) != 0) && (subcarrierIndex.size(0) > 1)) {
            n = subcarrierIndex.size(0) - 1;
            outsize_idx_1 = subcarrierIndex.size(0) >> 1;
            for (k = 0; k < outsize_idx_1; k++) {
              xtmp = b_y[k];
              i1 = n - k;
              b_y[k] = b_y[i1];
              b_y[i1] = xtmp;
            }
          }
        }

        outsize_idx_1 = y.size(1);
        for (k = 0; k < outsize_idx_1; k++) {
          dp_corr = y[k];
          if (std::isnan(dp_corr)) {
            r[k] = rtNaN;
          } else {
            if ((!(dp_corr > vwork[vwork.size(0) - 1])) && (!(dp_corr < vwork[0])))
            {
              n = coder::internal::b_bsearch(vwork, dp_corr) - 1;
              xtmp = (dp_corr - vwork[n]) / (vwork[n + 1] - vwork[n]);
              if (xtmp == 0.0) {
                r[k] = b_y[n];
              } else if (xtmp == 1.0) {
                r[k] = b_y[n + 1];
              } else {
                dp_corr = b_y[n + 1];
                if (b_y[n] == dp_corr) {
                  r[k] = b_y[n];
                } else {
                  r[k] = (1.0 - xtmp) * b_y[n] + xtmp * dp_corr;
                }
              }
            }
          }
        }

        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  interpedIndex.set_size(r.size(1));
  loop_ub = r.size(1);
  for (vstride = 0; vstride < loop_ub; vstride++) {
    interpedIndex[vstride] = r[vstride];
  }

  if (subcarrierIndex.size(0) > 0) {
    sz_idx_1 = div_s32(CSI.size(0) * CSI.size(1), subcarrierIndex.size(0));
  } else {
    sz_idx_1 = 0;
  }

  loop_ub = subcarrierIndex.size(0) - 1;
  newCSI.set_size(interpedIndex.size(0), sz_idx_1);
  for (int colIndex = 0; colIndex < sz_idx_1; colIndex++) {
    b_y.set_size((loop_ub + 1));
    for (k = 0; k <= loop_ub; k++) {
      b_y[k] = rt_hypotd_snf(CSI[k + (loop_ub + 1) * colIndex].re, CSI[k +
        (loop_ub + 1) * colIndex].im);
    }

    outsize_idx_1 = subcarrierIndex.size(0);
    i1 = subcarrierIndex.size(0);
    c_y.set_size(subcarrierIndex.size(0));
    for (k = 0; k < outsize_idx_1; k++) {
      c_y[k] = rt_atan2d_snf(CSI[k + i1 * colIndex].im, CSI[k + i1 * colIndex].
        re);
    }

    outsize_idx_1 = 0;
    if (c_y.size(0) != 1) {
      outsize_idx_1 = -1;
    }

    if (outsize_idx_1 + 2 <= 1) {
      n = c_y.size(0) - 1;
    } else {
      n = 0;
    }

    vwork.set_size((n + 1));
    vstride = 1;
    for (k = 0; k <= outsize_idx_1; k++) {
      vstride *= c_y.size(0);
    }

    i1 = -1;
    for (int j = 0; j < vstride; j++) {
      unsigned int b_k;
      boolean_T exitg2;
      i1++;
      outsize_idx_1 = i1;
      for (k = 0; k <= n; k++) {
        vwork[k] = c_y[outsize_idx_1];
        outsize_idx_1 += vstride;
      }

      outsize_idx_1 = vwork.size(0);
      cumsum_dp_corr = 0.0;
      b_k = 1U;
      exitg2 = false;
      while ((!exitg2) && (static_cast<int>(b_k) < outsize_idx_1)) {
        dp_corr = vwork[static_cast<int>(b_k) - 1];
        if (std::isinf(dp_corr) || std::isnan(dp_corr)) {
          b_k = static_cast<unsigned int>(static_cast<int>(b_k) + 1);
        } else {
          exitg2 = true;
        }
      }

      if (static_cast<int>(b_k) < vwork.size(0)) {
        xtmp = vwork[static_cast<int>(b_k) - 1];
        do {
          exitg1 = 0;
          b_k++;
          exitg2 = false;
          while ((!exitg2) && (b_k <= static_cast<unsigned int>(outsize_idx_1)))
          {
            dp_corr = vwork[static_cast<int>(b_k) - 1];
            if (std::isinf(dp_corr) || std::isnan(dp_corr)) {
              b_k++;
            } else {
              exitg2 = true;
            }
          }

          if (b_k > static_cast<unsigned int>(outsize_idx_1)) {
            exitg1 = 1;
          } else {
            double dp_tmp;
            dp_tmp = vwork[static_cast<int>(b_k) - 1];
            xtmp = dp_tmp - xtmp;
            dp_corr = xtmp / 6.2831853071795862;
            if (std::abs(rt_remd_snf(dp_corr, 1.0)) <= 0.5) {
              dp_corr = std::trunc(dp_corr);
            } else {
              dp_corr = std::round(dp_corr);
            }

            if (std::abs(xtmp) >= 3.1415926535897931) {
              cumsum_dp_corr += dp_corr;
            }

            xtmp = dp_tmp;
            vwork[static_cast<int>(b_k) - 1] = dp_tmp - 6.2831853071795862 *
              cumsum_dp_corr;
          }
        } while (exitg1 == 0);
      }

      outsize_idx_1 = i1;
      for (k = 0; k <= n; k++) {
        c_y[outsize_idx_1] = vwork[k];
        outsize_idx_1 += vstride;
      }
    }

    coder::interp1(subcarrierIndex, b_y, interpedIndex, r1);
    coder::interp1(subcarrierIndex, c_y, interpedIndex, vwork);
    r2.set_size(vwork.size(0));
    outsize_idx_1 = vwork.size(0);
    for (vstride = 0; vstride < outsize_idx_1; vstride++) {
      r2[vstride].re = vwork[vstride] * 0.0;
      r2[vstride].im = vwork[vstride];
    }

    n = r2.size(0);
    for (k = 0; k < n; k++) {
      if (r2[k].im == 0.0) {
        r2[k].re = std::exp(r2[k].re);
        r2[k].im = 0.0;
      } else if (std::isinf(r2[k].im) && std::isinf(r2[k].re) && (r2[k].re < 0.0))
      {
        r2[k].re = 0.0;
        r2[k].im = 0.0;
      } else {
        xtmp = std::exp(r2[k].re / 2.0);
        r2[k].re = xtmp * (xtmp * std::cos(r2[k].im));
        r2[k].im = xtmp * (xtmp * std::sin(r2[k].im));
      }
    }

    r2.set_size(r1.size(0));
    outsize_idx_1 = r1.size(0);
    for (vstride = 0; vstride < outsize_idx_1; vstride++) {
      r2[vstride].re = r1[vstride] * r2[vstride].re;
      r2[vstride].im = r1[vstride] * r2[vstride].im;
    }

    outsize_idx_1 = r2.size(0);
    for (vstride = 0; vstride < outsize_idx_1; vstride++) {
      newCSI[vstride + newCSI.size(0) * colIndex] = r2[vstride];
    }
  }

  outsize_idx_1 = interpedIndex.size(0);
  sz_idx_1 = CSI.size(1);
  i1 = interpedIndex.size(0);
  newCSI.set_size(interpedIndex.size(0), CSI.size(1));
  for (vstride = 0; vstride < sz_idx_1; vstride++) {
    for (n = 0; n < outsize_idx_1; n++) {
      newCSI[n + newCSI.size(0) * vstride] = newCSI[n + i1 * vstride];
    }
  }

  interpedIndex_int16.set_size(interpedIndex.size(0));
  loop_ub = interpedIndex.size(0);
  for (vstride = 0; vstride < loop_ub; vstride++) {
    short i;
    dp_corr = std::round(interpedIndex[vstride]);
    if (dp_corr < 32768.0) {
      if (dp_corr >= -32768.0) {
        i = static_cast<short>(dp_corr);
      } else {
        i = MIN_int16_T;
      }
    } else if (dp_corr >= 32768.0) {
      i = MAX_int16_T;
    } else {
      i = 0;
    }

    interpedIndex_int16[vstride] = i;
  }
}

//
// Arguments    : void
// Return Type  : void
//
void CSIPreprocessor_initialize()
{
}

//
// Arguments    : void
// Return Type  : void
//
void CSIPreprocessor_terminate()
{
  // (no terminate code required)
}

//
// File trailer for CSIPreprocessor.cpp
//
// [EOF]
//
