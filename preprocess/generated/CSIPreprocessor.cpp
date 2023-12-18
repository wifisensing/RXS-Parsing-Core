// Include Files
#include "CSIPreprocessor.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "rt_defines.h"
#include <cfloat>
#include <cmath>
#include <cstring>

// Function Declarations
static void binary_expand_op(coder::array<boolean_T, 1U> &in1, const coder::array<float, 1U> &in2,
                             const coder::array<boolean_T, 1U> &in3);

namespace coder {
static void polyfit(const array<double, 1U> &x, const array<float, 1U> &y, float p[2]);

}
static float rt_atan2f_snf(float u0, float u1);

static double rt_hypotd_snf(double u0, double u1);

static float rt_hypotf_snf(float u0, float u1);

static float rt_remf_snf(float u0, float u1);

// Function Definitions
//
// Arguments    : coder::array<boolean_T, 1U> &in1
//                const coder::array<float, 1U> &in2
//                const coder::array<boolean_T, 1U> &in3
// Return Type  : void
//
static void binary_expand_op(coder::array<boolean_T, 1U> &in1, const coder::array<float, 1U> &in2,
                             const coder::array<boolean_T, 1U> &in3)
{
    int loop_ub;
    int stride_0_0;
    int stride_1_0;
    if (in3.size(0) == 1) {
        loop_ub = in2.size(0);
    } else {
        loop_ub = in3.size(0);
    }
    in1.set_size(loop_ub);
    stride_0_0 = (in2.size(0) != 1);
    stride_1_0 = (in3.size(0) != 1);
    for (int i{0}; i < loop_ub; i++) {
        in1[i] = ((in2[i * stride_0_0] == 0.0F) && in3[i * stride_1_0]);
    }
}

//
// Arguments    : const array<double, 1U> &x
//                const array<float, 1U> &y
//                float p[2]
// Return Type  : void
//
namespace coder {
static void polyfit(const array<double, 1U> &x, const array<float, 1U> &y, float p[2])
{
    array<double, 2U> V;
    array<float, 1U> B;
    double tau_data[2];
    int i;
    int ix;
    int kend;
    int lastv;
    int m;
    int pvt;
    int u0;
    signed char jpvt[2];
    V.set_size(x.size(0), 2);
    if (x.size(0) != 0) {
        i = x.size(0);
        for (lastv = 0; lastv < i; lastv++) {
            V[lastv + V.size(0)] = 1.0;
            V[lastv] = x[lastv];
        }
    }
    m = V.size(0);
    u0 = V.size(0);
    if (u0 > 2) {
        u0 = 2;
    }
    if (u0 - 1 >= 0) {
        std::memset(&tau_data[0], 0, static_cast<unsigned int>(u0) * sizeof(double));
    }
    if (V.size(0) == 0) {
        jpvt[0] = 1;
        jpvt[1] = 2;
    } else {
        double vn1[2];
        double vn2[2];
        double work[2];
        double absxk;
        double scale;
        double t;
        double temp;
        int ix0;
        int ma;
        ma = V.size(0);
        for (lastv = 0; lastv < 2; lastv++) {
            jpvt[lastv] = static_cast<signed char>(lastv + 1);
            work[lastv] = 0.0;
            ix0 = lastv * ma;
            temp = 0.0;
            if (m >= 1) {
                if (m == 1) {
                    temp = std::abs(V[ix0]);
                } else {
                    scale = 3.3121686421112381E-170;
                    kend = ix0 + m;
                    for (ix = ix0 + 1; ix <= kend; ix++) {
                        absxk = std::abs(V[ix - 1]);
                        if (absxk > scale) {
                            t = scale / absxk;
                            temp = temp * t * t + 1.0;
                            scale = absxk;
                        } else {
                            t = absxk / scale;
                            temp += t * t;
                        }
                    }
                    temp = scale * std::sqrt(temp);
                }
            }
            vn1[lastv] = temp;
            vn2[lastv] = temp;
        }
        for (int b_i{0}; b_i < u0; b_i++) {
            double atmp;
            int ii;
            int ip1;
            int mmi;
            ip1 = b_i + 2;
            ix0 = b_i * ma;
            ii = ix0 + b_i;
            mmi = (m - b_i) - 1;
            ix = 0;
            if ((2 - b_i > 1) && (vn1[1] > vn1[b_i])) {
                ix = 1;
            }
            pvt = b_i + ix;
            if (pvt != b_i) {
                ix = pvt * ma;
                for (lastv = 0; lastv < m; lastv++) {
                    kend = ix + lastv;
                    temp = V[kend];
                    i = ix0 + lastv;
                    V[kend] = V[i];
                    V[i] = temp;
                }
                ix = jpvt[pvt];
                jpvt[pvt] = jpvt[b_i];
                jpvt[b_i] = static_cast<signed char>(ix);
                vn1[pvt] = vn1[b_i];
                vn2[pvt] = vn2[b_i];
            }
            if (b_i + 1 < m) {
                atmp = V[ii];
                ix0 = ii + 2;
                tau_data[b_i] = 0.0;
                if (mmi + 1 > 0) {
                    temp = 0.0;
                    if (mmi >= 1) {
                        if (mmi == 1) {
                            temp = std::abs(V[ii + 1]);
                        } else {
                            scale = 3.3121686421112381E-170;
                            kend = (ii + mmi) + 1;
                            for (lastv = ix0; lastv <= kend; lastv++) {
                                absxk = std::abs(V[lastv - 1]);
                                if (absxk > scale) {
                                    t = scale / absxk;
                                    temp = temp * t * t + 1.0;
                                    scale = absxk;
                                } else {
                                    t = absxk / scale;
                                    temp += t * t;
                                }
                            }
                            temp = scale * std::sqrt(temp);
                        }
                    }
                    if (temp != 0.0) {
                        scale = rt_hypotd_snf(V[ii], temp);
                        if (V[ii] >= 0.0) {
                            scale = -scale;
                        }
                        if (std::abs(scale) < 1.0020841800044864E-292) {
                            ix = 0;
                            i = (ii + mmi) + 1;
                            do {
                                ix++;
                                for (lastv = ix0; lastv <= i; lastv++) {
                                    V[lastv - 1] = 9.9792015476736E+291 * V[lastv - 1];
                                }
                                scale *= 9.9792015476736E+291;
                                atmp *= 9.9792015476736E+291;
                            } while ((std::abs(scale) < 1.0020841800044864E-292) && (ix < 20));
                            temp = 0.0;
                            if (mmi >= 1) {
                                if (mmi == 1) {
                                    temp = std::abs(V[ii + 1]);
                                } else {
                                    scale = 3.3121686421112381E-170;
                                    for (lastv = ix0; lastv <= i; lastv++) {
                                        absxk = std::abs(V[lastv - 1]);
                                        if (absxk > scale) {
                                            t = scale / absxk;
                                            temp = temp * t * t + 1.0;
                                            scale = absxk;
                                        } else {
                                            t = absxk / scale;
                                            temp += t * t;
                                        }
                                    }
                                    temp = scale * std::sqrt(temp);
                                }
                            }
                            scale = rt_hypotd_snf(atmp, temp);
                            if (atmp >= 0.0) {
                                scale = -scale;
                            }
                            tau_data[b_i] = (scale - atmp) / scale;
                            temp = 1.0 / (atmp - scale);
                            for (lastv = ix0; lastv <= i; lastv++) {
                                V[lastv - 1] = temp * V[lastv - 1];
                            }
                            for (lastv = 0; lastv < ix; lastv++) {
                                scale *= 1.0020841800044864E-292;
                            }
                            atmp = scale;
                        } else {
                            tau_data[b_i] = (scale - V[ii]) / scale;
                            temp = 1.0 / (V[ii] - scale);
                            i = (ii + mmi) + 1;
                            for (lastv = ix0; lastv <= i; lastv++) {
                                V[lastv - 1] = temp * V[lastv - 1];
                            }
                            atmp = scale;
                        }
                    }
                }
                V[ii] = atmp;
            } else {
                tau_data[b_i] = 0.0;
            }
            if (b_i + 1 < 2) {
                int jA;
                atmp = V[ii];
                V[ii] = 1.0;
                jA = (ii + ma) + 1;
                if (tau_data[0] != 0.0) {
                    lastv = mmi;
                    ix = ii + mmi;
                    while ((lastv + 1 > 0) && (V[ix] == 0.0)) {
                        lastv--;
                        ix--;
                    }
                    ix = 1;
                    pvt = jA;
                    int exitg1;
                    do {
                        exitg1 = 0;
                        if (pvt <= jA + lastv) {
                            if (V[pvt - 1] != 0.0) {
                                exitg1 = 1;
                            } else {
                                pvt++;
                            }
                        } else {
                            ix = 0;
                            exitg1 = 1;
                        }
                    } while (exitg1 == 0);
                } else {
                    lastv = -1;
                    ix = 0;
                }
                if (lastv + 1 > 0) {
                    if (ix != 0) {
                        work[0] = 0.0;
                        kend = 0;
                        for (ix0 = jA; ma < 0 ? ix0 >= jA : ix0 <= jA; ix0 += ma) {
                            temp = 0.0;
                            i = ix0 + lastv;
                            for (pvt = ix0; pvt <= i; pvt++) {
                                temp += V[pvt - 1] * V[(ii + pvt) - ix0];
                            }
                            work[kend] += temp;
                            kend++;
                        }
                    }
                    if (!(-tau_data[0] == 0.0)) {
                        for (pvt = 0; pvt < ix; pvt++) {
                            if (work[0] != 0.0) {
                                temp = work[0] * -tau_data[0];
                                i = lastv + jA;
                                for (kend = jA; kend <= i; kend++) {
                                    V[kend - 1] = V[kend - 1] + V[(ii + kend) - jA] * temp;
                                }
                            }
                            jA += ma;
                        }
                    }
                }
                V[ii] = atmp;
            }
            for (pvt = ip1; pvt < 3; pvt++) {
                ix = (b_i + ma) + 1;
                if (vn1[1] != 0.0) {
                    temp = std::abs(V[ix - 1]) / vn1[1];
                    temp = 1.0 - temp * temp;
                    if (temp < 0.0) {
                        temp = 0.0;
                    }
                    scale = vn1[1] / vn2[1];
                    scale = temp * (scale * scale);
                    if (scale <= 1.4901161193847656E-8) {
                        if (b_i + 1 < m) {
                            ix0 = ix + 1;
                            temp = 0.0;
                            if (mmi >= 1) {
                                if (mmi == 1) {
                                    temp = std::abs(V[ix]);
                                } else {
                                    scale = 3.3121686421112381E-170;
                                    kend = ix + mmi;
                                    for (lastv = ix0; lastv <= kend; lastv++) {
                                        absxk = std::abs(V[lastv - 1]);
                                        if (absxk > scale) {
                                            t = scale / absxk;
                                            temp = temp * t * t + 1.0;
                                            scale = absxk;
                                        } else {
                                            t = absxk / scale;
                                            temp += t * t;
                                        }
                                    }
                                    temp = scale * std::sqrt(temp);
                                }
                            }
                            vn1[1] = temp;
                            vn2[1] = temp;
                        } else {
                            vn1[1] = 0.0;
                            vn2[1] = 0.0;
                        }
                    } else {
                        vn1[1] *= std::sqrt(temp);
                    }
                }
            }
        }
    }
    kend = 0;
    if (u0 > 0) {
        for (lastv = 0; lastv < u0; lastv++) {
            if (V[lastv + V.size(0) * lastv] != 0.0) {
                kend++;
            }
        }
    }
    B.set_size(y.size(0));
    ix = y.size(0);
    for (i = 0; i < ix; i++) {
        B[i] = y[i];
    }
    p[0] = 0.0F;
    p[1] = 0.0F;
    m = V.size(0);
    for (pvt = 0; pvt < u0; pvt++) {
        if (tau_data[pvt] != 0.0) {
            float wj;
            wj = B[pvt];
            i = pvt + 2;
            for (int b_i{i}; b_i <= m; b_i++) {
                wj += static_cast<float>(V[(b_i + V.size(0) * pvt) - 1]) * B[b_i - 1];
            }
            wj *= static_cast<float>(tau_data[pvt]);
            if (wj != 0.0F) {
                B[pvt] = B[pvt] - wj;
                for (int b_i{i}; b_i <= m; b_i++) {
                    B[b_i - 1] = B[b_i - 1] - static_cast<float>(V[(b_i + V.size(0) * pvt) - 1]) * wj;
                }
            }
        }
    }
    for (int b_i{0}; b_i < kend; b_i++) {
        p[jpvt[b_i] - 1] = B[b_i];
    }
    for (pvt = kend; pvt >= 1; pvt--) {
        ix = jpvt[pvt - 1] - 1;
        p[ix] /= static_cast<float>(V[(pvt + V.size(0) * (pvt - 1)) - 1]);
        for (int b_i{0}; b_i <= pvt - 2; b_i++) {
            p[jpvt[0] - 1] -= p[ix] * static_cast<float>(V[V.size(0) * (pvt - 1)]);
        }
    }
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
} // namespace coder
static float rt_atan2f_snf(float u0, float u1)
{
    float y;
    if (std::isnan(u0) || std::isnan(u1)) {
        y = rtNaNF;
    } else if (std::isinf(u0) && std::isinf(u1)) {
        int i;
        int i1;
        if (u0 > 0.0F) {
            i = 1;
        } else {
            i = -1;
        }
        if (u1 > 0.0F) {
            i1 = 1;
        } else {
            i1 = -1;
        }
        y = std::atan2(static_cast<float>(i), static_cast<float>(i1));
    } else if (u1 == 0.0F) {
        if (u0 > 0.0F) {
            y = RT_PIF / 2.0F;
        } else if (u0 < 0.0F) {
            y = -(RT_PIF / 2.0F);
        } else {
            y = 0.0F;
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
    double b;
    double y;
    a = std::abs(u0);
    b = std::abs(u1);
    if (a < b) {
        a /= b;
        y = b * std::sqrt(a * a + 1.0);
    } else if (a > b) {
        b /= a;
        y = a * std::sqrt(b * b + 1.0);
    } else if (std::isnan(b)) {
        y = rtNaN;
    } else {
        y = a * 1.4142135623730951;
    }
    return y;
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
static float rt_hypotf_snf(float u0, float u1)
{
    float a;
    float b;
    float y;
    a = std::abs(u0);
    b = std::abs(u1);
    if (a < b) {
        a /= b;
        y = b * std::sqrt(a * a + 1.0F);
    } else if (a > b) {
        b /= a;
        y = a * std::sqrt(b * b + 1.0F);
    } else if (std::isnan(b)) {
        y = rtNaNF;
    } else {
        y = a * 1.41421354F;
    }
    return y;
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
static float rt_remf_snf(float u0, float u1)
{
    float y;
    if (std::isnan(u0) || std::isnan(u1) || std::isinf(u0)) {
        y = rtNaNF;
    } else if (std::isinf(u1)) {
        y = u0;
    } else if ((u1 != 0.0F) && (u1 != std::trunc(u1))) {
        float q;
        q = std::abs(u0 / u1);
        if (!(std::abs(q - std::floor(q + 0.5F)) > FLT_EPSILON * q)) {
            y = 0.0F * u0;
        } else {
            y = std::fmod(u0, u1);
        }
    } else {
        y = std::fmod(u0, u1);
    }
    return y;
}

//
// Arguments    : void
// Return Type  : void
//
CSIPreprocessor::CSIPreprocessor() = default;

//
// Arguments    : void
// Return Type  : void
//
CSIPreprocessor::~CSIPreprocessor() = default;

//
// Arguments    : coder::array<creal32_T, 4U> &CSI
//                const coder::array<short, 1U> &subcarrierIndex_int16
//                double numSTS
//                double numRx
//                double numCSI
//                double format
//                double CBW
//                boolean_T removeCSD
//                coder::array<creal32_T, 4U> &resultCSI
//                coder::array<float, 4U> &resultMag
//                coder::array<float, 4U> &resultPhase
//                coder::array<short, 1U> &interpedIndex_int16
//                coder::array<float, 3U> &phaseSlope
//                coder::array<float, 3U> &phaseIntercept
// Return Type  : void
//
void CSIPreprocessor::InterpolateCSIAndRemoveCSD(
    coder::array<creal32_T, 4U> &CSI, const coder::array<short, 1U> &subcarrierIndex_int16, double numSTS, double numRx,
    double numCSI, double format, double CBW, boolean_T removeCSD, coder::array<creal32_T, 4U> &resultCSI,
    coder::array<float, 4U> &resultMag, coder::array<float, 4U> &resultPhase,
    coder::array<short, 1U> &interpedIndex_int16, coder::array<float, 3U> &phaseSlope,
    coder::array<float, 3U> &phaseIntercept)
{
    coder::array<creal_T, 4U> phaseShiftAll;
    coder::array<creal_T, 2U> phaseShiftPerRx;
    coder::array<double, 2U> Vq;
    coder::array<double, 2U> y;
    coder::array<double, 1U> interpedIndex;
    coder::array<float, 4U> b;
    coder::array<float, 4U> b_x;
    coder::array<float, 4U> pivotPhase;
    coder::array<float, 4U> rawMag;
    coder::array<float, 4U> rawPhase;
    coder::array<float, 4U> wrappedDCPhase;
    coder::array<float, 1U> b_resultPhase;
    coder::array<float, 1U> vwork;
    coder::array<int, 1U> b_y;
    coder::array<int, 1U> r;
    coder::array<int, 1U> x;
    coder::array<short, 1U> subcarrierIndex;
    coder::array<boolean_T, 1U> positiveInput;
    coder::array<boolean_T, 1U> r1;
    coder::array<boolean_T, 1U> x_tmp;
    double im;
    double nfft;
    float c_re_tmp;
    float dp;
    float dp_corr;
    float pkm1;
    int ibmat;
    int ibtile;
    int ii_data;
    int jcol;
    int k;
    int k_tmp;
    int loop_ub_tmp;
    int ncols;
    int npages;
    int nrows;
    int ntilecols;
    int nx_tmp;
    int nycols_tmp;
    unsigned int pivotIndex_data;
    int vstride;
    boolean_T b_b;
    boolean_T exitg1;
    boolean_T rEQ0;
    subcarrierIndex.set_size(subcarrierIndex_int16.size(0));
    ibtile = subcarrierIndex_int16.size(0);
    for (jcol = 0; jcol < ibtile; jcol++) {
        subcarrierIndex[jcol] = subcarrierIndex_int16[jcol];
    }
    if (removeCSD) {
        creal_T b_csdInSamples_data[8];
        double csdInSamples_data[8];
        switch (static_cast<int>(numSTS)) {
        case 1:
            nrows = 1;
            csdInSamples_data[0] = 0.0;
            break;
        case 2:
            nrows = 2;
            csdInSamples_data[0] = -0.0;
            csdInSamples_data[1] = -400.0;
            break;
        case 3:
            nrows = 3;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            break;
        case 4:
            nrows = 4;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            break;
        case 5:
            nrows = 5;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            break;
        case 6:
            nrows = 6;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            csdInSamples_data[5] = -650.0;
            break;
        case 7:
            nrows = 7;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            csdInSamples_data[5] = -650.0;
            csdInSamples_data[6] = -100.0;
            break;
        case 8:
            nrows = 8;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            csdInSamples_data[5] = -650.0;
            csdInSamples_data[6] = -100.0;
            csdInSamples_data[7] = -750.0;
            break;
        default:
            nrows = 1;
            csdInSamples_data[0] = 0.0;
            break;
        }
        ibtile = nrows - 1;
        for (jcol = 0; jcol <= ibtile; jcol++) {
            csdInSamples_data[jcol] = csdInSamples_data[jcol] * CBW * 0.001;
        }
        nfft = 64.0 * (CBW / 20.0);
        if (format >= 3.0) {
            nfft *= 4.0;
        }
        for (jcol = 0; jcol < nrows; jcol++) {
            b_csdInSamples_data[jcol].re = csdInSamples_data[jcol];
            b_csdInSamples_data[jcol].im = 0.0;
        }
        phaseShiftPerRx.set_size(subcarrierIndex.size(0), nrows);
        ibtile = subcarrierIndex.size(0);
        for (jcol = 0; jcol < nrows; jcol++) {
            for (npages = 0; npages < ibtile; npages++) {
                double b_re_tmp;
                double re;
                double re_tmp;
                ncols = subcarrierIndex[npages];
                im = static_cast<double>(ncols) * 6.2831853071795862;
                re_tmp = b_csdInSamples_data[jcol].im;
                b_re_tmp = b_csdInSamples_data[jcol].re;
                re = 0.0 * b_re_tmp - im * re_tmp;
                im = 0.0 * re_tmp + im * b_re_tmp;
                if (im == 0.0) {
                    phaseShiftPerRx[npages + phaseShiftPerRx.size(0) * jcol].re = re / nfft;
                    phaseShiftPerRx[npages + phaseShiftPerRx.size(0) * jcol].im = 0.0;
                } else if (re == 0.0) {
                    phaseShiftPerRx[npages + phaseShiftPerRx.size(0) * jcol].re = 0.0;
                    phaseShiftPerRx[npages + phaseShiftPerRx.size(0) * jcol].im = im / nfft;
                } else {
                    phaseShiftPerRx[npages + phaseShiftPerRx.size(0) * jcol].re = re / nfft;
                    phaseShiftPerRx[npages + phaseShiftPerRx.size(0) * jcol].im = im / nfft;
                }
            }
        }
        nx_tmp = phaseShiftPerRx.size(0) * phaseShiftPerRx.size(1);
        for (k = 0; k < nx_tmp; k++) {
            if (phaseShiftPerRx[k].re == 0.0) {
                im = phaseShiftPerRx[k].im;
                phaseShiftPerRx[k].re = std::cos(im);
                phaseShiftPerRx[k].im = std::sin(im);
            } else if (phaseShiftPerRx[k].im == 0.0) {
                phaseShiftPerRx[k].re = std::exp(phaseShiftPerRx[k].re);
                phaseShiftPerRx[k].im = 0.0;
            } else if (std::isinf(phaseShiftPerRx[k].im) && std::isinf(phaseShiftPerRx[k].re) &&
                       (phaseShiftPerRx[k].re < 0.0)) {
                phaseShiftPerRx[k].re = 0.0;
                phaseShiftPerRx[k].im = 0.0;
            } else {
                nfft = std::exp(phaseShiftPerRx[k].re / 2.0);
                im = phaseShiftPerRx[k].im;
                phaseShiftPerRx[k].re = nfft * (nfft * std::cos(im));
                phaseShiftPerRx[k].im = nfft * (nfft * std::sin(im));
            }
        }
        phaseShiftAll.set_size(phaseShiftPerRx.size(0), phaseShiftPerRx.size(1), static_cast<int>(numRx),
                               static_cast<int>(numCSI));
        nrows = phaseShiftPerRx.size(0);
        ncols = phaseShiftPerRx.size(1);
        ntilecols = static_cast<int>(numRx) * static_cast<int>(numCSI);
        for (npages = 0; npages < ntilecols; npages++) {
            ibtile = npages * nx_tmp - 1;
            for (jcol = 0; jcol < ncols; jcol++) {
                vstride = jcol * nrows;
                ibmat = ibtile + vstride;
                for (k = 0; k < nrows; k++) {
                    phaseShiftAll[(ibmat + k) + 1] = phaseShiftPerRx[vstride + k];
                }
            }
        }
        ibtile = CSI.size(0) * CSI.size(1) * CSI.size(2) * CSI.size(3);
        for (jcol = 0; jcol < ibtile; jcol++) {
            pkm1 = static_cast<float>(phaseShiftAll[jcol].re);
            dp = static_cast<float>(phaseShiftAll[jcol].im);
            dp_corr = CSI[jcol].re;
            c_re_tmp = CSI[jcol].im;
            CSI[jcol].re = dp_corr * pkm1 - c_re_tmp * dp;
            CSI[jcol].im = dp_corr * dp + c_re_tmp * pkm1;
        }
    }
    jcol = subcarrierIndex[subcarrierIndex.size(0) - 1];
    if (jcol < subcarrierIndex[0]) {
        y.set_size(1, 0);
    } else {
        ibtile = jcol - subcarrierIndex[0];
        y.set_size(1, ibtile + 1);
        for (jcol = 0; jcol <= ibtile; jcol++) {
            y[jcol] = static_cast<short>(subcarrierIndex[0] + static_cast<short>(jcol));
        }
    }
    b_y.set_size(subcarrierIndex.size(0));
    ibtile = subcarrierIndex.size(0);
    x.set_size(subcarrierIndex.size(0));
    for (jcol = 0; jcol < ibtile; jcol++) {
        b_y[jcol] = subcarrierIndex[jcol];
        x[jcol] = subcarrierIndex[jcol];
    }
    nx_tmp = subcarrierIndex.size(0) - 1;
    nrows = y.size(1);
    Vq.set_size(1, y.size(1));
    for (jcol = 0; jcol < nrows; jcol++) {
        Vq[jcol] = rtNaN;
    }
    if (y.size(1) != 0) {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
            jcol = subcarrierIndex.size(0) >> 1;
            for (ntilecols = 0; ntilecols < jcol; ntilecols++) {
                ncols = x[ntilecols];
                nrows = nx_tmp - ntilecols;
                x[ntilecols] = x[nrows];
                x[nrows] = ncols;
            }
            if (subcarrierIndex.size(0) > 1) {
                for (k = 0; k < jcol; k++) {
                    nrows = b_y[k];
                    ncols = nx_tmp - k;
                    b_y[k] = b_y[ncols];
                    b_y[ncols] = nrows;
                }
            }
        }
        ibtile = y.size(1);
        for (k = 0; k < ibtile; k++) {
            im = y[k];
            if ((!(im > x[x.size(0) - 1])) && (!(im < x[0]))) {
                nrows = x.size(0);
                ibmat = 1;
                ncols = 2;
                while (nrows > ncols) {
                    ntilecols = (ibmat >> 1) + (nrows >> 1);
                    if (((ibmat & 1) == 1) && ((nrows & 1) == 1)) {
                        ntilecols++;
                    }
                    if (y[k] >= x[ntilecols - 1]) {
                        ibmat = ntilecols;
                        ncols = ntilecols + 1;
                    } else {
                        nrows = ntilecols;
                    }
                }
                nrows = x[ibmat - 1];
                nfft = (y[k] - static_cast<double>(nrows)) / static_cast<double>(x[ibmat] - nrows);
                if (nfft == 0.0) {
                    Vq[k] = b_y[ibmat - 1];
                } else if (nfft == 1.0) {
                    Vq[k] = b_y[ibmat];
                } else {
                    nrows = b_y[ibmat - 1];
                    if (nrows == b_y[ibmat]) {
                        Vq[k] = nrows;
                    } else {
                        Vq[k] = (1.0 - nfft) * static_cast<double>(nrows) + nfft * static_cast<double>(b_y[ibmat]);
                    }
                }
            }
        }
    }
    interpedIndex.set_size(Vq.size(1));
    ibtile = Vq.size(1);
    for (jcol = 0; jcol < ibtile; jcol++) {
        interpedIndex[jcol] = Vq[jcol];
    }
    nrows = CSI.size(0) * CSI.size(1) * CSI.size(2) * CSI.size(3);
    rawPhase.set_size(CSI.size(0), CSI.size(1), CSI.size(2), CSI.size(3));
    for (k = 0; k < nrows; k++) {
        rawPhase[k] = rt_atan2f_snf(CSI[k].im, CSI[k].re);
    }
    rawMag.set_size(CSI.size(0), CSI.size(1), CSI.size(2), CSI.size(3));
    for (k = 0; k < nrows; k++) {
        rawMag[k] = rt_hypotf_snf(CSI[k].re, CSI[k].im);
    }
    nrows = subcarrierIndex.size(0);
    b_y.set_size(subcarrierIndex.size(0));
    x.set_size(subcarrierIndex.size(0));
    for (k = 0; k < nrows; k++) {
        ncols = static_cast<int>(std::abs(static_cast<double>(subcarrierIndex[k])));
        b_y[k] = ncols;
        x[k] = ncols;
    }
    nrows = x.size(0);
    if (x.size(0) <= 2) {
        if (x.size(0) == 1) {
            ncols = x[0];
        } else {
            nrows = x[x.size(0) - 1];
            if (x[0] > nrows) {
                ncols = nrows;
            } else {
                ncols = x[0];
            }
        }
    } else {
        ncols = x[0];
        for (k = 2; k <= nrows; k++) {
            jcol = x[k - 1];
            if (ncols > jcol) {
                ncols = jcol;
            }
        }
    }
    positiveInput.set_size(b_y.size(0));
    ibtile = b_y.size(0);
    for (jcol = 0; jcol < ibtile; jcol++) {
        positiveInput[jcol] = (b_y[jcol] == ncols);
    }
    k = (positiveInput.size(0) >= 1);
    nrows = 0;
    ncols = 0;
    exitg1 = false;
    while ((!exitg1) && (ncols <= positiveInput.size(0) - 1)) {
        if (positiveInput[ncols]) {
            nrows = 1;
            ii_data = ncols + 1;
            exitg1 = true;
        } else {
            ncols++;
        }
    }
    if (k == 1) {
        if (nrows == 0) {
            k = 0;
        }
    } else {
        k = (nrows >= 1);
    }
    for (jcol = 0; jcol < k; jcol++) {
        pivotIndex_data = static_cast<unsigned int>(ii_data);
    }
    positiveInput.set_size(interpedIndex.size(0));
    ibtile = interpedIndex.size(0);
    for (jcol = 0; jcol < ibtile; jcol++) {
        positiveInput[jcol] = (interpedIndex[jcol] == subcarrierIndex[static_cast<int>(pivotIndex_data) - 1]);
    }
    k_tmp = (positiveInput.size(0) >= 1);
    nrows = 0;
    k = k_tmp;
    ncols = 0;
    exitg1 = false;
    while ((!exitg1) && (ncols <= positiveInput.size(0) - 1)) {
        if (positiveInput[ncols]) {
            nrows = 1;
            ii_data = ncols + 1;
            exitg1 = true;
        } else {
            ncols++;
        }
    }
    if (k_tmp == 1) {
        if (nrows == 0) {
            k = 0;
        }
    } else {
        k = (nrows >= 1);
    }
    pivotPhase.set_size(1, rawPhase.size(1), rawPhase.size(2), rawPhase.size(3));
    ibtile = rawPhase.size(3);
    for (jcol = 0; jcol < ibtile; jcol++) {
        ncols = rawPhase.size(2);
        for (npages = 0; npages < ncols; npages++) {
            ntilecols = rawPhase.size(1);
            for (vstride = 0; vstride < ntilecols; vstride++) {
                pivotPhase[(vstride + pivotPhase.size(1) * npages) + pivotPhase.size(1) * pivotPhase.size(2) * jcol] =
                    rawPhase[(((static_cast<int>(pivotIndex_data) + rawPhase.size(0) * vstride) +
                               rawPhase.size(0) * rawPhase.size(1) * npages) +
                              rawPhase.size(0) * rawPhase.size(1) * rawPhase.size(2) * jcol) -
                             1];
            }
        }
    }
    nrows = 1;
    if (rawPhase.size(0) != 1) {
        nrows = 0;
    } else if (rawPhase.size(1) == 1) {
        if (rawPhase.size(2) != 1) {
            nrows = 2;
        } else if (rawPhase.size(3) != 1) {
            nrows = 3;
        }
    }
    ibtile = rawPhase.size(nrows) - 1;
    vwork.set_size(rawPhase.size(nrows));
    vstride = 1;
    for (int b_k{0}; b_k < nrows; b_k++) {
        vstride *= rawPhase.size(b_k);
    }
    ibmat = (rawPhase.size(nrows) - 1) * vstride;
    npages = 1;
    nrows += 2;
    for (int b_k{nrows}; b_k < 5; b_k++) {
        npages *= rawPhase.size(b_k - 1);
    }
    ncols = 0;
    for (jcol = 0; jcol < npages; jcol++) {
        ntilecols = ncols - 1;
        ncols += ibmat;
        for (int j{0}; j < vstride; j++) {
            float cumsum_dp_corr;
            ntilecols++;
            ncols++;
            for (int b_k{0}; b_k <= ibtile; b_k++) {
                vwork[b_k] = rawPhase[ntilecols + b_k * vstride];
            }
            nrows = vwork.size(0);
            cumsum_dp_corr = 0.0F;
            pivotIndex_data = 1U;
            exitg1 = false;
            while ((!exitg1) && (static_cast<int>(pivotIndex_data) < nrows)) {
                dp = vwork[static_cast<int>(pivotIndex_data) - 1];
                if (std::isinf(dp) || std::isnan(dp)) {
                    pivotIndex_data = static_cast<unsigned int>(static_cast<int>(pivotIndex_data) + 1);
                } else {
                    exitg1 = true;
                }
            }
            if (static_cast<int>(pivotIndex_data) < vwork.size(0)) {
                pkm1 = vwork[static_cast<int>(pivotIndex_data) - 1];
                int exitg2;
                do {
                    exitg2 = 0;
                    pivotIndex_data++;
                    exitg1 = false;
                    while ((!exitg1) && (pivotIndex_data <= static_cast<unsigned int>(nrows))) {
                        dp = vwork[static_cast<int>(pivotIndex_data) - 1];
                        if (std::isinf(dp) || std::isnan(dp)) {
                            pivotIndex_data++;
                        } else {
                            exitg1 = true;
                        }
                    }
                    if (pivotIndex_data > static_cast<unsigned int>(nrows)) {
                        exitg2 = 1;
                    } else {
                        c_re_tmp = vwork[static_cast<int>(pivotIndex_data) - 1];
                        dp = c_re_tmp - pkm1;
                        dp_corr = dp / 6.28318548F;
                        if (std::abs(rt_remf_snf(dp_corr, 1.0F)) <= 0.5F) {
                            dp_corr = std::trunc(dp_corr);
                        } else {
                            dp_corr = std::round(dp_corr);
                        }
                        if (std::abs(dp) >= 3.14159274F) {
                            cumsum_dp_corr += dp_corr;
                        }
                        pkm1 = c_re_tmp;
                        vwork[static_cast<int>(pivotIndex_data) - 1] = c_re_tmp - 6.28318548F * cumsum_dp_corr;
                    }
                } while (exitg2 == 0);
            }
            for (int b_k{0}; b_k <= ibtile; b_k++) {
                rawPhase[ntilecols + b_k * vstride] = vwork[b_k];
            }
        }
    }
    x.set_size(subcarrierIndex.size(0));
    ibtile = subcarrierIndex.size(0);
    for (jcol = 0; jcol < ibtile; jcol++) {
        x[jcol] = subcarrierIndex[jcol];
    }
    vstride = rawPhase.size(0);
    nrows = rawPhase.size(1) * rawPhase.size(2) * rawPhase.size(3);
    nycols_tmp = nrows - 1;
    resultPhase.set_size(interpedIndex.size(0), rawPhase.size(1), rawPhase.size(2), rawPhase.size(3));
    loop_ub_tmp = interpedIndex.size(0) * rawPhase.size(1) * rawPhase.size(2) * rawPhase.size(3);
    for (jcol = 0; jcol < loop_ub_tmp; jcol++) {
        resultPhase[jcol] = rtNaNF;
    }
    if (interpedIndex.size(0) != 0) {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
            jcol = subcarrierIndex.size(0) >> 1;
            for (ntilecols = 0; ntilecols < jcol; ntilecols++) {
                ncols = x[ntilecols];
                nrows = nx_tmp - ntilecols;
                x[ntilecols] = x[nrows];
                x[nrows] = ncols;
            }
            if ((rawPhase.size(0) != 0) && (rawPhase.size(1) != 0) && (rawPhase.size(2) != 0) &&
                (rawPhase.size(3) != 0) && (rawPhase.size(0) > 1)) {
                nrows = rawPhase.size(0) - 1;
                ncols = rawPhase.size(0) >> 1;
                for (int j{0}; j <= nycols_tmp; j++) {
                    ntilecols = j * rawPhase.size(0);
                    for (int b_k{0}; b_k < ncols; b_k++) {
                        ibtile = ntilecols + b_k;
                        pkm1 = rawPhase[ibtile];
                        jcol = (ntilecols + nrows) - b_k;
                        rawPhase[ibtile] = rawPhase[jcol];
                        rawPhase[jcol] = pkm1;
                    }
                }
            }
        }
        ibtile = interpedIndex.size(0);
        for (int b_k{0}; b_k < ibtile; b_k++) {
            if (std::isnan(interpedIndex[b_k])) {
                for (int j{0}; j <= nycols_tmp; j++) {
                    resultPhase[b_k + j * ibtile] = rtNaNF;
                }
            } else if ((!(interpedIndex[b_k] > x[x.size(0) - 1])) && (!(interpedIndex[b_k] < x[0]))) {
                nrows = x.size(0);
                ibmat = 1;
                ncols = 2;
                while (nrows > ncols) {
                    ntilecols = (ibmat >> 1) + (nrows >> 1);
                    if (((ibmat & 1) == 1) && ((nrows & 1) == 1)) {
                        ntilecols++;
                    }
                    if (interpedIndex[b_k] >= x[ntilecols - 1]) {
                        ibmat = ntilecols;
                        ncols = ntilecols + 1;
                    } else {
                        nrows = ntilecols;
                    }
                }
                nrows = x[ibmat - 1];
                nfft = (interpedIndex[b_k] - static_cast<double>(nrows)) / static_cast<double>(x[ibmat] - nrows);
                if (nfft == 0.0) {
                    for (int j{0}; j <= nycols_tmp; j++) {
                        resultPhase[b_k + j * ibtile] = rawPhase[(ibmat + j * vstride) - 1];
                    }
                } else if (nfft == 1.0) {
                    for (int j{0}; j <= nycols_tmp; j++) {
                        resultPhase[b_k + j * ibtile] = rawPhase[ibmat + j * vstride];
                    }
                } else {
                    for (int j{0}; j <= nycols_tmp; j++) {
                        nrows = ibmat + j * vstride;
                        pkm1 = rawPhase[nrows - 1];
                        dp = rawPhase[nrows];
                        if (pkm1 == dp) {
                            resultPhase[b_k + j * ibtile] = pkm1;
                        } else {
                            resultPhase[b_k + j * ibtile] =
                                static_cast<float>(1.0 - nfft) * pkm1 + static_cast<float>(nfft) * dp;
                        }
                    }
                }
            }
        }
    }
    b_x.set_size(k, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
    ibtile = resultPhase.size(3);
    for (jcol = 0; jcol < ibtile; jcol++) {
        ncols = resultPhase.size(2);
        for (npages = 0; npages < ncols; npages++) {
            ntilecols = resultPhase.size(1);
            for (vstride = 0; vstride < ntilecols; vstride++) {
                for (ibmat = 0; ibmat < k; ibmat++) {
                    b_x[(b_x.size(0) * vstride + b_x.size(0) * b_x.size(1) * npages) +
                        b_x.size(0) * b_x.size(1) * b_x.size(2) * jcol] =
                        resultPhase[(((ii_data + resultPhase.size(0) * vstride) +
                                      resultPhase.size(0) * resultPhase.size(1) * npages) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * jcol) -
                                    1];
                }
            }
        }
    }
    ibtile = b_x.size(0) * b_x.size(1) * b_x.size(2) * b_x.size(3);
    pivotPhase.set_size(b_x.size(0), b_x.size(1), b_x.size(2), b_x.size(3));
    for (jcol = 0; jcol < ibtile; jcol++) {
        pivotPhase[jcol] = b_x[jcol] - pivotPhase[jcol];
    }
    b.set_size(interpedIndex.size(0), pivotPhase.size(1), pivotPhase.size(2), pivotPhase.size(3));
    b_b = ((interpedIndex.size(0) != 0) && (pivotPhase.size(1) != 0) && (pivotPhase.size(2) != 0) &&
           (pivotPhase.size(3) != 0));
    if (b_b) {
        nrows = pivotPhase.size(3);
        ncols = pivotPhase.size(2);
        ntilecols = pivotPhase.size(1);
        jcol = interpedIndex.size(0) - 1;
        for (k = 0; k < nrows; k++) {
            for (int b_k{0}; b_k < ncols; b_k++) {
                for (ibtile = 0; ibtile < ntilecols; ibtile++) {
                    for (ibmat = 0; ibmat <= jcol; ibmat++) {
                        b[((ibmat + b.size(0) * ibtile) + b.size(0) * b.size(1) * b_k) +
                          b.size(0) * b.size(1) * b.size(2) * k] =
                            pivotPhase[(ibtile + pivotPhase.size(1) * b_k) +
                                       pivotPhase.size(1) * pivotPhase.size(2) * k];
                    }
                }
            }
        }
    }
    for (jcol = 0; jcol < loop_ub_tmp; jcol++) {
        resultPhase[jcol] = resultPhase[jcol] - b[jcol];
    }
    x_tmp.set_size(interpedIndex.size(0));
    ibtile = interpedIndex.size(0);
    for (jcol = 0; jcol < ibtile; jcol++) {
        x_tmp[jcol] = (interpedIndex[jcol] == 0.0);
    }
    rEQ0 = false;
    nrows = 1;
    exitg1 = false;
    while ((!exitg1) && (nrows <= x_tmp.size(0))) {
        if (x_tmp[nrows - 1]) {
            rEQ0 = true;
            exitg1 = true;
        } else {
            nrows++;
        }
    }
    if (rEQ0) {
        nrows = 0;
        k = k_tmp;
        ncols = 0;
        exitg1 = false;
        while ((!exitg1) && (ncols <= x_tmp.size(0) - 1)) {
            if (x_tmp[ncols]) {
                nrows = 1;
                ii_data = ncols + 1;
                exitg1 = true;
            } else {
                ncols++;
            }
        }
        if (k_tmp == 1) {
            if (nrows == 0) {
                k = 0;
            }
        } else {
            k = (nrows >= 1);
        }
        for (jcol = 0; jcol < k; jcol++) {
            pivotIndex_data = static_cast<unsigned int>(ii_data);
        }
        pivotPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        ibtile = resultPhase.size(3);
        wrappedDCPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        for (jcol = 0; jcol < ibtile; jcol++) {
            ncols = resultPhase.size(2);
            for (npages = 0; npages < ncols; npages++) {
                ntilecols = resultPhase.size(1);
                for (vstride = 0; vstride < ntilecols; vstride++) {
                    pkm1 = resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * vstride) +
                                         resultPhase.size(0) * resultPhase.size(1) * npages) +
                                        resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * jcol) -
                                       1];
                    pivotPhase[(vstride + pivotPhase.size(1) * npages) +
                               pivotPhase.size(1) * pivotPhase.size(2) * jcol] = pkm1;
                    wrappedDCPhase[(vstride + wrappedDCPhase.size(1) * npages) +
                                   wrappedDCPhase.size(1) * wrappedDCPhase.size(2) * jcol] = pkm1;
                }
            }
        }
        ncols = pivotPhase.size(1) * (pivotPhase.size(2) * pivotPhase.size(3)) - 1;
        nrows = 0;
        for (jcol = 0; jcol <= ncols; jcol++) {
            if ((pivotPhase[jcol] < -3.1415926535897931) || (pivotPhase[jcol] > 3.1415926535897931)) {
                nrows++;
            }
        }
        r.set_size(nrows);
        nrows = 0;
        for (jcol = 0; jcol <= ncols; jcol++) {
            if ((pivotPhase[jcol] < -3.1415926535897931) || (pivotPhase[jcol] > 3.1415926535897931)) {
                r[nrows] = jcol;
                nrows++;
            }
        }
        pivotPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        ibtile = resultPhase.size(3);
        for (jcol = 0; jcol < ibtile; jcol++) {
            ncols = resultPhase.size(2);
            for (npages = 0; npages < ncols; npages++) {
                ntilecols = resultPhase.size(1);
                for (vstride = 0; vstride < ntilecols; vstride++) {
                    pivotPhase[(vstride + pivotPhase.size(1) * npages) +
                               pivotPhase.size(1) * pivotPhase.size(2) * jcol] =
                        resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * vstride) +
                                      resultPhase.size(0) * resultPhase.size(1) * npages) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * jcol) -
                                    1];
                }
            }
        }
        nrows = resultPhase.size(1) * resultPhase.size(2) * resultPhase.size(3);
        vwork.set_size(r.size(0));
        ibtile = r.size(0);
        for (jcol = 0; jcol < ibtile; jcol++) {
            vwork[jcol] = pivotPhase[r[jcol]] + 3.14159274F;
        }
        positiveInput.set_size(vwork.size(0));
        ibtile = vwork.size(0);
        for (jcol = 0; jcol < ibtile; jcol++) {
            positiveInput[jcol] = (vwork[jcol] > 0.0F);
            pkm1 = vwork[jcol];
            if (std::isnan(pkm1) || std::isinf(pkm1)) {
                dp = rtNaNF;
            } else if (pkm1 == 0.0F) {
                dp = 0.0F;
            } else {
                dp = std::fmod(pkm1, 6.28318548F);
                rEQ0 = (dp == 0.0F);
                if (!rEQ0) {
                    dp_corr = std::abs(pkm1 / 6.28318548F);
                    rEQ0 = !(std::abs(dp_corr - std::floor(dp_corr + 0.5F)) > 1.1920929E-7F * dp_corr);
                }
                if (rEQ0) {
                    dp = 0.0F;
                } else if (pkm1 < 0.0F) {
                    dp += 6.28318548F;
                }
            }
            vwork[jcol] = dp;
        }
        if (vwork.size(0) == positiveInput.size(0)) {
            r1.set_size(vwork.size(0));
            ibtile = vwork.size(0);
            for (jcol = 0; jcol < ibtile; jcol++) {
                r1[jcol] = ((vwork[jcol] == 0.0F) && positiveInput[jcol]);
            }
        } else {
            binary_expand_op(r1, vwork, positiveInput);
        }
        ncols = r1.size(0) - 1;
        for (jcol = 0; jcol <= ncols; jcol++) {
            if (r1[jcol]) {
                vwork[jcol] = 6.28318548F;
            }
        }
        ibtile = vwork.size(0);
        for (jcol = 0; jcol < ibtile; jcol++) {
            wrappedDCPhase[r[jcol]] = vwork[jcol] - 3.14159274F;
        }
        wrappedDCPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        ibtile = resultPhase.size(3);
        for (jcol = 0; jcol < ibtile; jcol++) {
            ncols = resultPhase.size(2);
            for (npages = 0; npages < ncols; npages++) {
                ntilecols = resultPhase.size(1);
                for (vstride = 0; vstride < ntilecols; vstride++) {
                    wrappedDCPhase[(vstride + wrappedDCPhase.size(1) * npages) +
                                   wrappedDCPhase.size(1) * wrappedDCPhase.size(2) * jcol] =
                        resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * vstride) +
                                      resultPhase.size(0) * resultPhase.size(1) * npages) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * jcol) -
                                    1] -
                        wrappedDCPhase[(vstride + wrappedDCPhase.size(1) * npages) +
                                       wrappedDCPhase.size(1) * wrappedDCPhase.size(2) * jcol];
                }
            }
        }
        positiveInput.set_size(nrows);
        for (jcol = 0; jcol < nrows; jcol++) {
            positiveInput[jcol] = (wrappedDCPhase[jcol] == 0.0F);
        }
        rEQ0 = true;
        nrows = 1;
        exitg1 = false;
        while ((!exitg1) && (nrows <= positiveInput.size(0))) {
            if (!positiveInput[nrows - 1]) {
                rEQ0 = false;
                exitg1 = true;
            } else {
                nrows++;
            }
        }
        if (!rEQ0) {
            b.set_size(interpedIndex.size(0), wrappedDCPhase.size(1), wrappedDCPhase.size(2), wrappedDCPhase.size(3));
            if (b_b) {
                nrows = wrappedDCPhase.size(3);
                ncols = wrappedDCPhase.size(2);
                ntilecols = wrappedDCPhase.size(1);
                jcol = interpedIndex.size(0) - 1;
                for (k = 0; k < nrows; k++) {
                    for (int b_k{0}; b_k < ncols; b_k++) {
                        for (ibtile = 0; ibtile < ntilecols; ibtile++) {
                            for (ibmat = 0; ibmat <= jcol; ibmat++) {
                                b[((ibmat + b.size(0) * ibtile) + b.size(0) * b.size(1) * b_k) +
                                  b.size(0) * b.size(1) * b.size(2) * k] =
                                    wrappedDCPhase[(ibtile + wrappedDCPhase.size(1) * b_k) +
                                                   wrappedDCPhase.size(1) * wrappedDCPhase.size(2) * k];
                            }
                        }
                    }
                }
            }
            for (jcol = 0; jcol < loop_ub_tmp; jcol++) {
                resultPhase[jcol] = resultPhase[jcol] - b[jcol];
            }
        }
    }
    jcol = static_cast<int>(numSTS);
    npages = static_cast<int>(numRx);
    vstride = static_cast<int>(numCSI);
    phaseSlope.set_size(jcol, static_cast<int>(numRx), static_cast<int>(numCSI));
    phaseIntercept.set_size(jcol, static_cast<int>(numRx), static_cast<int>(numCSI));
    for (nrows = 0; nrows < jcol; nrows++) {
        for (ncols = 0; ncols < npages; ncols++) {
            for (ntilecols = 0; ntilecols < vstride; ntilecols++) {
                ibtile = resultPhase.size(0);
                b_resultPhase.set_size(resultPhase.size(0));
                for (ibmat = 0; ibmat < ibtile; ibmat++) {
                    b_resultPhase[ibmat] =
                        resultPhase[((ibmat + resultPhase.size(0) * nrows) +
                                     resultPhase.size(0) * resultPhase.size(1) * ncols) +
                                    resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * ntilecols];
                }
                float fitResult[2];
                coder::polyfit(interpedIndex, b_resultPhase, fitResult);
                phaseSlope[(nrows + phaseSlope.size(0) * ncols) + phaseSlope.size(0) * phaseSlope.size(1) * ntilecols] =
                    fitResult[0];
                phaseIntercept[(nrows + phaseIntercept.size(0) * ncols) +
                               phaseIntercept.size(0) * phaseIntercept.size(1) * ntilecols] = fitResult[1];
            }
        }
    }
    vstride = rawMag.size(0);
    resultMag.set_size(interpedIndex.size(0), rawMag.size(1), rawMag.size(2), rawMag.size(3));
    for (jcol = 0; jcol < loop_ub_tmp; jcol++) {
        resultMag[jcol] = rtNaNF;
    }
    if (interpedIndex.size(0) != 0) {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
            jcol = subcarrierIndex.size(0) >> 1;
            for (ntilecols = 0; ntilecols < jcol; ntilecols++) {
                ncols = subcarrierIndex[ntilecols];
                nrows = nx_tmp - ntilecols;
                subcarrierIndex[ntilecols] = subcarrierIndex[nrows];
                subcarrierIndex[nrows] = static_cast<short>(ncols);
            }
            if ((rawMag.size(0) != 0) && (rawMag.size(1) != 0) && (rawMag.size(2) != 0) && (rawMag.size(3) != 0) &&
                (rawMag.size(0) > 1)) {
                nrows = rawMag.size(0) - 1;
                ncols = rawMag.size(0) >> 1;
                for (int j{0}; j <= nycols_tmp; j++) {
                    ntilecols = j * rawMag.size(0);
                    for (k = 0; k < ncols; k++) {
                        ibtile = ntilecols + k;
                        pkm1 = rawMag[ibtile];
                        jcol = (ntilecols + nrows) - k;
                        rawMag[ibtile] = rawMag[jcol];
                        rawMag[jcol] = pkm1;
                    }
                }
            }
        }
        ibtile = interpedIndex.size(0);
        for (k = 0; k < ibtile; k++) {
            if (std::isnan(interpedIndex[k])) {
                for (int j{0}; j <= nycols_tmp; j++) {
                    resultMag[k + j * ibtile] = rtNaNF;
                }
            } else if ((!(interpedIndex[k] > subcarrierIndex[subcarrierIndex.size(0) - 1])) &&
                       (!(interpedIndex[k] < subcarrierIndex[0]))) {
                nrows = subcarrierIndex.size(0);
                ibmat = 1;
                ncols = 2;
                while (nrows > ncols) {
                    ntilecols = (ibmat >> 1) + (nrows >> 1);
                    if (((ibmat & 1) == 1) && ((nrows & 1) == 1)) {
                        ntilecols++;
                    }
                    if (interpedIndex[k] >= subcarrierIndex[ntilecols - 1]) {
                        ibmat = ntilecols;
                        ncols = ntilecols + 1;
                    } else {
                        nrows = ntilecols;
                    }
                }
                short r_tmp;
                r_tmp = subcarrierIndex[ibmat - 1];
                nfft = (interpedIndex[k] - static_cast<double>(r_tmp)) /
                       static_cast<double>(subcarrierIndex[ibmat] - r_tmp);
                if (nfft == 0.0) {
                    for (int j{0}; j <= nycols_tmp; j++) {
                        resultMag[k + j * ibtile] = rawMag[(ibmat + j * vstride) - 1];
                    }
                } else if (nfft == 1.0) {
                    for (int j{0}; j <= nycols_tmp; j++) {
                        resultMag[k + j * ibtile] = rawMag[ibmat + j * vstride];
                    }
                } else {
                    for (int j{0}; j <= nycols_tmp; j++) {
                        nrows = ibmat + j * vstride;
                        pkm1 = rawMag[nrows - 1];
                        dp = rawMag[nrows];
                        if (pkm1 == dp) {
                            resultMag[k + j * ibtile] = pkm1;
                        } else {
                            resultMag[k + j * ibtile] =
                                static_cast<float>(1.0 - nfft) * pkm1 + static_cast<float>(nfft) * dp;
                        }
                    }
                }
            }
        }
    }
    resultCSI.set_size(resultPhase.size(0), resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
    for (jcol = 0; jcol < loop_ub_tmp; jcol++) {
        resultCSI[jcol].re = resultPhase[jcol] * 0.0F;
        resultCSI[jcol].im = resultPhase[jcol];
    }
    for (k = 0; k < loop_ub_tmp; k++) {
        if (resultCSI[k].re == 0.0F) {
            dp = resultCSI[k].im;
            resultCSI[k].re = std::cos(dp);
            resultCSI[k].im = std::sin(dp);
        } else if (resultCSI[k].im == 0.0F) {
            resultCSI[k].re = std::exp(resultCSI[k].re);
            resultCSI[k].im = 0.0F;
        } else if (std::isinf(resultCSI[k].im) && std::isinf(resultCSI[k].re) && (resultCSI[k].re < 0.0F)) {
            resultCSI[k].re = 0.0F;
            resultCSI[k].im = 0.0F;
        } else {
            pkm1 = std::exp(resultCSI[k].re / 2.0F);
            dp = resultCSI[k].im;
            resultCSI[k].re = pkm1 * (pkm1 * std::cos(dp));
            resultCSI[k].im = pkm1 * (pkm1 * std::sin(dp));
        }
    }
    resultCSI.set_size(resultMag.size(0), resultMag.size(1), resultMag.size(2), resultMag.size(3));
    for (jcol = 0; jcol < loop_ub_tmp; jcol++) {
        resultCSI[jcol].re = resultMag[jcol] * resultCSI[jcol].re;
        resultCSI[jcol].im = resultMag[jcol] * resultCSI[jcol].im;
    }
    interpedIndex_int16.set_size(interpedIndex.size(0));
    ibtile = interpedIndex.size(0);
    for (jcol = 0; jcol < ibtile; jcol++) {
        interpedIndex_int16[jcol] = static_cast<short>(std::round(interpedIndex[jcol]));
    }
}

//
// File trailer for CSIPreprocessor.cpp
//
// [EOF]
//
