// Include Files
#include "CSIPreprocessor.h"
#include "InterpolateCSIAndRemoveCSDAndAutoUnpermutation_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "crl_permlogic.h"
#include "rt_defines.h"
#include <cfloat>
#include <cmath>
#include <math.h>

// Function Declarations
static void InterpolateCSIAndRemoveCSDAndAutoUnpermutation_init(CSIPreprocessor *aInstancePtr);

static void binary_expand_op(coder::array<boolean_T, 1U> &in1, const coder::array<double, 1U> &in2);

static int div_s32(int numerator, int denominator);

static double rt_atan2d_snf(double u0, double u1);

static double rt_hypotd_snf(double u0, double u1);

static double rt_remd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : void
// Return Type  : InterpolateCSIAndRemoveCSDAndAutoUnpermutationStackData *
//
InterpolateCSIAndRemoveCSDAndAutoUnpermutationStackData *CSIPreprocessor::getStackData()
{
    return &SD_;
}

//
// Arguments    : CSIPreprocessor *aInstancePtr
// Return Type  : void
//
static void InterpolateCSIAndRemoveCSDAndAutoUnpermutation_init(CSIPreprocessor *aInstancePtr)
{
    InterpolateCSIAndRemoveCSDAndAutoUnpermutationStackData *localSD;
    localSD = aInstancePtr->getStackData();
    localSD->pd->lastMag_not_empty = false;
}

//
// Arguments    : coder::array<boolean_T, 1U> &in1
//                const coder::array<double, 1U> &in2
// Return Type  : void
//
static void binary_expand_op(coder::array<boolean_T, 1U> &in1, const coder::array<double, 1U> &in2)
{
    coder::array<boolean_T, 1U> b_in2;
    int i;
    int loop_ub;
    int stride_0_0;
    int stride_1_0;
    if (in1.size(0) == 1) {
        i = in2.size(0);
    } else {
        i = in1.size(0);
    }
    b_in2.set_size(i);
    stride_0_0 = (in2.size(0) != 1);
    stride_1_0 = (in1.size(0) != 1);
    if (in1.size(0) == 1) {
        loop_ub = in2.size(0);
    } else {
        loop_ub = in1.size(0);
    }
    for (i = 0; i < loop_ub; i++) {
        b_in2[i] = ((in2[i * stride_0_0] == 0.0) && in1[i * stride_1_0]);
    }
    in1.set_size(b_in2.size(0));
    loop_ub = b_in2.size(0);
    for (i = 0; i < loop_ub; i++) {
        in1[i] = b_in2[i];
    }
}

//
// Arguments    : int numerator
//                int denominator
// Return Type  : int
//
static int div_s32(int numerator, int denominator)
{
    int quotient;
    if (denominator == 0) {
        if (numerator >= 0) {
            quotient = MAX_int32_T;
        } else {
            quotient = MIN_int32_T;
        }
    } else {
        unsigned int b_denominator;
        unsigned int b_numerator;
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
    } else if (!std::isnan(y)) {
        y = a * 1.4142135623730951;
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
// Arguments    : void
// Return Type  : void
//
CSIPreprocessor::CSIPreprocessor()
{
    SD_.pd = &pd_;
    InterpolateCSIAndRemoveCSDAndAutoUnpermutation_init(this);
}

//
// Arguments    : void
// Return Type  : void
//
CSIPreprocessor::~CSIPreprocessor() = default;

//
// Arguments    : coder::array<creal_T, 4U> &CSI
//                const coder::array<short, 1U> &subcarrierIndex_int16
//                double numSTS
//                double numESS
//                double numRx
//                double numCSI
//                double packetFormat
//                double CBW
//                boolean_T performCSDRemoval
//                boolean_T unpermuteRxChainRatio
//                coder::array<creal_T, 4U> &resultCSI
//                coder::array<double, 4U> &resultMag
//                coder::array<double, 4U> &resultPhase
//                coder::array<short, 1U> &interpedIndex_int16
//                unsigned char *perm
//                double *permCoef
// Return Type  : void
//
void CSIPreprocessor::InterpolateCSIAndRemoveCSDAndAutoUnpermutation(
    coder::array<creal_T, 4U> &CSI, const coder::array<short, 1U> &subcarrierIndex_int16, double numSTS, double,
    double numRx, double numCSI, double packetFormat, double CBW, boolean_T performCSDRemoval,
    boolean_T unpermuteRxChainRatio, coder::array<creal_T, 4U> &resultCSI, coder::array<double, 4U> &resultMag,
    coder::array<double, 4U> &resultPhase, coder::array<short, 1U> &interpedIndex_int16, unsigned char *perm,
    double *permCoef)
{
    coder::array<creal_T, 4U> phaseShiftAll;
    coder::array<creal_T, 2U> phaseShiftPerRx;
    coder::array<double, 4U> b;
    coder::array<double, 4U> b_resultPhase;
    coder::array<double, 4U> b_x;
    coder::array<double, 4U> pivotPhase;
    coder::array<double, 4U> rawMag;
    coder::array<double, 4U> rawPhase;
    coder::array<double, 3U> b_rawPhase;
    coder::array<double, 2U> Vq;
    coder::array<double, 2U> b_magDb;
    coder::array<double, 2U> magDb;
    coder::array<double, 2U> now1;
    coder::array<double, 2U> now2;
    coder::array<double, 2U> y;
    coder::array<double, 1U> X;
    coder::array<double, 1U> Y;
    coder::array<double, 1U> b_X;
    coder::array<double, 1U> b_Y;
    coder::array<double, 1U> c_X;
    coder::array<double, 1U> c_Y;
    coder::array<double, 1U> interpedIndex;
    coder::array<int, 1U> r1;
    coder::array<int, 1U> r4;
    coder::array<int, 1U> r5;
    coder::array<short, 1U> subcarrierIndex;
    coder::array<boolean_T, 4U> r2;
    coder::array<boolean_T, 4U> r3;
    coder::array<boolean_T, 2U> b_r;
    coder::array<boolean_T, 1U> c_x;
    coder::array<boolean_T, 1U> positiveInput;
    double cumsum_dp_corr;
    double dp;
    double meanpast2;
    double nfft;
    double r;
    int b_exponent;
    int c_exponent;
    int d_exponent;
    int e_exponent;
    int exponent;
    int f_exponent;
    int high_i;
    int i;
    int i1;
    int ibmat;
    int ibtile;
    int ii_data;
    int jtilecol;
    int k;
    int ncols;
    int nrows;
    int nx;
    int nycols;
    int nyrows;
    unsigned int pivotIndex_data;
    boolean_T exitg1;
    boolean_T nanflag;
    if (!pd_.lastMag_not_empty) {
        pd_.lastMag.set_size(0, 2);
    }
    subcarrierIndex.set_size(subcarrierIndex_int16.size(0));
    ibtile = subcarrierIndex_int16.size(0);
    for (i = 0; i < ibtile; i++) {
        subcarrierIndex[i] = subcarrierIndex_int16[i];
    }
    if (performCSDRemoval) {
        creal_T b_csdInSamples_data[8];
        double csdInSamples_data[8];
        switch (static_cast<int>(numSTS)) {
        case 1:
            ncols = 1;
            csdInSamples_data[0] = 0.0;
            break;
        case 2:
            ncols = 2;
            csdInSamples_data[0] = -0.0;
            csdInSamples_data[1] = -400.0;
            break;
        case 3:
            ncols = 3;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            break;
        case 4:
            ncols = 4;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            break;
        case 5:
            ncols = 5;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            break;
        case 6:
            ncols = 6;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            csdInSamples_data[5] = -650.0;
            break;
        case 7:
            ncols = 7;
            csdInSamples_data[0] = 0.0;
            csdInSamples_data[1] = -400.0;
            csdInSamples_data[2] = -200.0;
            csdInSamples_data[3] = -600.0;
            csdInSamples_data[4] = -350.0;
            csdInSamples_data[5] = -650.0;
            csdInSamples_data[6] = -100.0;
            break;
        case 8:
            ncols = 8;
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
            ncols = 1;
            csdInSamples_data[0] = 0.0;
            break;
        }
        ibtile = ncols - 1;
        for (i = 0; i <= ibtile; i++) {
            csdInSamples_data[i] = csdInSamples_data[i] * CBW * 0.001;
        }
        nfft = 64.0 * (CBW / 20.0);
        if (packetFormat >= 3.0) {
            nfft *= 4.0;
        }
        for (i = 0; i < ncols; i++) {
            b_csdInSamples_data[i].re = csdInSamples_data[i];
            b_csdInSamples_data[i].im = 0.0;
        }
        phaseShiftPerRx.set_size(subcarrierIndex.size(0), ncols);
        ibtile = subcarrierIndex.size(0);
        for (i = 0; i < ncols; i++) {
            for (i1 = 0; i1 < ibtile; i1++) {
                nrows = subcarrierIndex[i1];
                dp = static_cast<double>(nrows) * 6.2831853071795862;
                cumsum_dp_corr = b_csdInSamples_data[i].im;
                meanpast2 = b_csdInSamples_data[i].re;
                r = 0.0 * meanpast2 - dp * cumsum_dp_corr;
                dp = 0.0 * cumsum_dp_corr + dp * meanpast2;
                if (dp == 0.0) {
                    phaseShiftPerRx[i1 + phaseShiftPerRx.size(0) * i].re = r / nfft;
                    phaseShiftPerRx[i1 + phaseShiftPerRx.size(0) * i].im = 0.0;
                } else if (r == 0.0) {
                    phaseShiftPerRx[i1 + phaseShiftPerRx.size(0) * i].re = 0.0;
                    phaseShiftPerRx[i1 + phaseShiftPerRx.size(0) * i].im = dp / nfft;
                } else {
                    phaseShiftPerRx[i1 + phaseShiftPerRx.size(0) * i].re = r / nfft;
                    phaseShiftPerRx[i1 + phaseShiftPerRx.size(0) * i].im = dp / nfft;
                }
            }
        }
        nx = phaseShiftPerRx.size(0) * phaseShiftPerRx.size(1);
        for (k = 0; k < nx; k++) {
            if (phaseShiftPerRx[k].im == 0.0) {
                phaseShiftPerRx[k].re = std::exp(phaseShiftPerRx[k].re);
                phaseShiftPerRx[k].im = 0.0;
            } else if (std::isinf(phaseShiftPerRx[k].im) && std::isinf(phaseShiftPerRx[k].re) &&
                       (phaseShiftPerRx[k].re < 0.0)) {
                phaseShiftPerRx[k].re = 0.0;
                phaseShiftPerRx[k].im = 0.0;
            } else {
                r = std::exp(phaseShiftPerRx[k].re / 2.0);
                dp = phaseShiftPerRx[k].im;
                phaseShiftPerRx[k].re = r * (r * std::cos(phaseShiftPerRx[k].im));
                phaseShiftPerRx[k].im = r * (r * std::sin(dp));
            }
        }
        phaseShiftAll.set_size(phaseShiftPerRx.size(0), phaseShiftPerRx.size(1), static_cast<int>(numRx),
                               static_cast<int>(numCSI));
        nrows = phaseShiftPerRx.size(0);
        ncols = phaseShiftPerRx.size(1);
        nx = static_cast<int>(numRx) * static_cast<int>(numCSI);
        for (jtilecol = 0; jtilecol < nx; jtilecol++) {
            ibtile = jtilecol * (nrows * ncols) - 1;
            for (nyrows = 0; nyrows < ncols; nyrows++) {
                high_i = nyrows * nrows;
                ibmat = ibtile + high_i;
                for (k = 0; k < nrows; k++) {
                    phaseShiftAll[(ibmat + k) + 1] = phaseShiftPerRx[high_i + k];
                }
            }
        }
        ibtile = CSI.size(0) * CSI.size(1) * CSI.size(2) * CSI.size(3);
        for (i = 0; i < ibtile; i++) {
            cumsum_dp_corr = CSI[i].re;
            meanpast2 = phaseShiftAll[i].im;
            nfft = CSI[i].im;
            dp = phaseShiftAll[i].re;
            CSI[i].re = cumsum_dp_corr * dp - nfft * meanpast2;
            CSI[i].im = cumsum_dp_corr * meanpast2 + nfft * dp;
        }
    }
    if (subcarrierIndex[subcarrierIndex.size(0) - 1] < subcarrierIndex[0]) {
        y.set_size(1, 0);
    } else {
        y.set_size(1, (subcarrierIndex[subcarrierIndex.size(0) - 1] - subcarrierIndex[0]) + 1);
        ibtile = subcarrierIndex[subcarrierIndex.size(0) - 1] - subcarrierIndex[0];
        for (i = 0; i <= ibtile; i++) {
            y[i] = static_cast<short>(subcarrierIndex[0] + static_cast<short>(i));
        }
    }
    Y.set_size(subcarrierIndex.size(0));
    ibtile = subcarrierIndex.size(0);
    for (i = 0; i < ibtile; i++) {
        Y[i] = subcarrierIndex[i];
    }
    X.set_size(subcarrierIndex.size(0));
    ibtile = subcarrierIndex.size(0);
    for (i = 0; i < ibtile; i++) {
        X[i] = subcarrierIndex[i];
    }
    nx = subcarrierIndex.size(0) - 1;
    nrows = y.size(1);
    Vq.set_size(1, y.size(1));
    for (i = 0; i < nrows; i++) {
        Vq[i] = rtNaN;
    }
    if (y.size(1) != 0) {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
            i = subcarrierIndex.size(0) >> 1;
            for (ibtile = 0; ibtile < i; ibtile++) {
                nrows = static_cast<int>(X[ibtile]);
                high_i = nx - ibtile;
                X[ibtile] = X[high_i];
                X[high_i] = nrows;
            }
            if (subcarrierIndex.size(0) > 1) {
                ibtile = subcarrierIndex.size(0) - 1;
                ibmat = subcarrierIndex.size(0) >> 1;
                for (k = 0; k < ibmat; k++) {
                    ncols = static_cast<int>(Y[k]);
                    nrows = ibtile - k;
                    Y[k] = Y[nrows];
                    Y[nrows] = ncols;
                }
            }
        }
        nx = y.size(1);
        for (k = 0; k < nx; k++) {
            dp = y[k];
            if ((!(dp > X[X.size(0) - 1])) && (!(dp < X[0]))) {
                high_i = X.size(0);
                ibtile = 1;
                nrows = 2;
                while (high_i > nrows) {
                    ncols = (ibtile >> 1) + (high_i >> 1);
                    if (((ibtile & 1) == 1) && ((high_i & 1) == 1)) {
                        ncols++;
                    }
                    if (y[k] >= X[ncols - 1]) {
                        ibtile = ncols;
                        nrows = ncols + 1;
                    } else {
                        high_i = ncols;
                    }
                }
                nfft = X[ibtile - 1];
                r = (y[k] - nfft) / (X[ibtile] - nfft);
                if (r == 0.0) {
                    Vq[k] = Y[ibtile - 1];
                } else if (r == 1.0) {
                    Vq[k] = Y[ibtile];
                } else if (Y[ibtile - 1] == Y[ibtile]) {
                    Vq[k] = Y[ibtile - 1];
                } else {
                    Vq[k] = (1.0 - r) * Y[ibtile - 1] + r * Y[ibtile];
                }
            }
        }
    }
    interpedIndex.set_size(Vq.size(1));
    ibtile = Vq.size(1);
    for (i = 0; i < ibtile; i++) {
        interpedIndex[i] = Vq[i];
    }
    nx = CSI.size(0) * CSI.size(1) * CSI.size(2) * CSI.size(3);
    rawPhase.set_size(CSI.size(0), CSI.size(1), CSI.size(2), CSI.size(3));
    for (k = 0; k < nx; k++) {
        rawPhase[k] = rt_atan2d_snf(CSI[k].im, CSI[k].re);
    }
    nx = CSI.size(0) * CSI.size(1) * CSI.size(2) * CSI.size(3);
    rawMag.set_size(CSI.size(0), CSI.size(1), CSI.size(2), CSI.size(3));
    for (k = 0; k < nx; k++) {
        rawMag[k] = rt_hypotd_snf(CSI[k].re, CSI[k].im);
    }
    *perm = 0U;
    *permCoef = 0.0;
    if (unpermuteRxChainRatio && (rawMag.size(2) >= 2)) {
        boolean_T x[2];
        ncols = static_cast<int>(std::round(static_cast<double>(rawMag.size(0)) * 0.92));
        if (static_cast<double>(ncols) / 3.0 < 150.0) {
            nrows = 3;
        } else {
            nrows = static_cast<int>(std::floor(static_cast<double>(ncols) / 150.0));
        }
        if ((nrows == 0) || ((nrows > 0) && (ncols < 1))) {
            nrows = 1;
            ncols = 0;
        }
        ncols = ((div_s32(ncols - 1, nrows) + 1) << 1) / 2;
        magDb.set_size(ncols, 2);
        b_r.set_size(ncols, 2);
        for (i = 0; i < 2; i++) {
            for (i1 = 0; i1 < ncols; i1++) {
                magDb[i1 + magDb.size(0) * i] = rawMag[nrows * i1 + rawMag.size(0) * rawMag.size(1) * i];
                b_r[i1 + b_r.size(0) * i] = std::isnan(rawMag[nrows * i1 + rawMag.size(0) * rawMag.size(1) * i]);
            }
        }
        ncols = (b_r.size(0) << 1) - 1;
        high_i = 0;
        for (nyrows = 0; nyrows <= ncols; nyrows++) {
            if (b_r[nyrows]) {
                high_i++;
            }
        }
        r1.set_size(high_i);
        high_i = 0;
        for (nyrows = 0; nyrows <= ncols; nyrows++) {
            if (b_r[nyrows]) {
                r1[high_i] = nyrows + 1;
                high_i++;
            }
        }
        ibtile = r1.size(0);
        for (i = 0; i < ibtile; i++) {
            magDb[r1[i] - 1] = 1.0E-9;
        }
        x[0] = (pd_.lastMag.size(0) == magDb.size(0));
        x[1] = true;
        nanflag = true;
        k = 0;
        exitg1 = false;
        while ((!exitg1) && (k < 2)) {
            if (!x[k]) {
                nanflag = false;
                exitg1 = true;
            } else {
                k++;
            }
        }
        if (nanflag) {
            double D;
            double b_D;
            double meannow1;
            double meannow2;
            double meanpast1;
            boolean_T b_nanflag;
            //  the same pattern
            if (magDb.size(0) == 0) {
                dp = 0.0;
            } else {
                if (magDb.size(0) <= 1024) {
                    ncols = magDb.size(0);
                    ibmat = 0;
                    high_i = 1;
                } else {
                    ncols = 1024;
                    high_i = magDb.size(0) / 1024;
                    ibmat = magDb.size(0) - (high_i << 10);
                    if (ibmat > 0) {
                        high_i++;
                    } else {
                        ibmat = 1024;
                    }
                }
                dp = magDb[0];
                for (k = 2; k <= ncols; k++) {
                    dp += magDb[k - 1];
                }
                for (ncols = 2; ncols <= high_i; ncols++) {
                    nrows = (ncols - 1) << 10;
                    nfft = magDb[nrows];
                    if (ncols == high_i) {
                        nx = ibmat;
                    } else {
                        nx = 1024;
                    }
                    for (k = 2; k <= nx; k++) {
                        nfft += magDb[(nrows + k) - 1];
                    }
                    dp += nfft;
                }
            }
            meannow1 = dp / static_cast<double>(magDb.size(0));
            if (magDb.size(0) == 0) {
                dp = 0.0;
            } else {
                if (magDb.size(0) <= 1024) {
                    ncols = magDb.size(0);
                    ibmat = 0;
                    high_i = 1;
                } else {
                    ncols = 1024;
                    high_i = magDb.size(0) / 1024;
                    ibmat = magDb.size(0) - (high_i << 10);
                    if (ibmat > 0) {
                        high_i++;
                    } else {
                        ibmat = 1024;
                    }
                }
                dp = magDb[magDb.size(0)];
                for (k = 2; k <= ncols; k++) {
                    dp += magDb[(k + magDb.size(0)) - 1];
                }
                for (ncols = 2; ncols <= high_i; ncols++) {
                    nrows = (ncols - 1) << 10;
                    nfft = magDb[nrows + magDb.size(0)];
                    if (ncols == high_i) {
                        nx = ibmat;
                    } else {
                        nx = 1024;
                    }
                    for (k = 2; k <= nx; k++) {
                        nfft += magDb[((nrows + k) + magDb.size(0)) - 1];
                    }
                    dp += nfft;
                }
            }
            meannow2 = dp / static_cast<double>(magDb.size(0));
            if (pd_.lastMag.size(0) == 0) {
                dp = 0.0;
            } else {
                if (pd_.lastMag.size(0) <= 1024) {
                    ncols = pd_.lastMag.size(0);
                    ibmat = 0;
                    high_i = 1;
                } else {
                    ncols = 1024;
                    high_i = pd_.lastMag.size(0) / 1024;
                    ibmat = pd_.lastMag.size(0) - (high_i << 10);
                    if (ibmat > 0) {
                        high_i++;
                    } else {
                        ibmat = 1024;
                    }
                }
                dp = pd_.lastMag[0];
                for (k = 2; k <= ncols; k++) {
                    dp += pd_.lastMag[k - 1];
                }
                for (ncols = 2; ncols <= high_i; ncols++) {
                    nrows = (ncols - 1) << 10;
                    nfft = pd_.lastMag[nrows];
                    if (ncols == high_i) {
                        nx = ibmat;
                    } else {
                        nx = 1024;
                    }
                    for (k = 2; k <= nx; k++) {
                        nfft += pd_.lastMag[(nrows + k) - 1];
                    }
                    dp += nfft;
                }
            }
            meanpast1 = dp / static_cast<double>(pd_.lastMag.size(0));
            if (pd_.lastMag.size(0) == 0) {
                dp = 0.0;
            } else {
                if (pd_.lastMag.size(0) <= 1024) {
                    ncols = pd_.lastMag.size(0);
                    ibmat = 0;
                    high_i = 1;
                } else {
                    ncols = 1024;
                    high_i = pd_.lastMag.size(0) / 1024;
                    ibmat = pd_.lastMag.size(0) - (high_i << 10);
                    if (ibmat > 0) {
                        high_i++;
                    } else {
                        ibmat = 1024;
                    }
                }
                dp = pd_.lastMag[pd_.lastMag.size(0)];
                for (k = 2; k <= ncols; k++) {
                    dp += pd_.lastMag[(k + pd_.lastMag.size(0)) - 1];
                }
                for (ncols = 2; ncols <= high_i; ncols++) {
                    nrows = (ncols - 1) << 10;
                    nfft = pd_.lastMag[nrows + pd_.lastMag.size(0)];
                    if (ncols == high_i) {
                        nx = ibmat;
                    } else {
                        nx = 1024;
                    }
                    for (k = 2; k <= nx; k++) {
                        nfft += pd_.lastMag[((nrows + k) + pd_.lastMag.size(0)) - 1];
                    }
                    dp += nfft;
                }
            }
            meanpast2 = dp / static_cast<double>(pd_.lastMag.size(0));
            ibtile = magDb.size(0);
            now1.set_size(1, magDb.size(0));
            for (i = 0; i < ibtile; i++) {
                now1[i] = magDb[i] - meannow1;
            }
            ibtile = magDb.size(0);
            now2.set_size(1, magDb.size(0));
            for (i = 0; i < ibtile; i++) {
                now2[i] = magDb[i + magDb.size(0)] - meannow2;
            }
            nrows = now1.size(1) - 1;
            cumsum_dp_corr = rtNaN;
            b_X.set_size(now1.size(1));
            ibtile = now1.size(1);
            for (i = 0; i < ibtile; i++) {
                b_X[i] = now1[i];
            }
            X.set_size(b_X.size(0));
            ibtile = b_X.size(0);
            for (i = 0; i < ibtile; i++) {
                X[i] = b_X[i];
            }
            ibtile = pd_.lastMag.size(0);
            b_Y.set_size(pd_.lastMag.size(0));
            for (i = 0; i < ibtile; i++) {
                b_Y[i] = pd_.lastMag[i] - meanpast1;
            }
            Y.set_size(b_Y.size(0));
            ibtile = b_Y.size(0);
            for (i = 0; i < ibtile; i++) {
                Y[i] = b_Y[i];
            }
            nx = b_X.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += b_X[ncols] * b_X[ncols];
            }
            nfft = std::sqrt(nfft);
            ibtile = b_X.size(0);
            for (i = 0; i < ibtile; i++) {
                X[i] = b_X[i] / nfft;
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &nycols);
            }
            nx = b_Y.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += b_Y[ncols] * b_Y[ncols];
            }
            nfft = std::sqrt(nfft);
            ibtile = b_Y.size(0);
            for (i = 0; i < ibtile; i++) {
                Y[i] = b_Y[i] / nfft;
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &b_exponent);
            }
            nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(X[high_i])) {
                    nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            b_nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(Y[high_i])) {
                    b_nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            if ((!b_nanflag) && (!nanflag)) {
                nfft = 0.0;
                i = now1.size(1);
                for (high_i = 0; high_i < i; high_i++) {
                    nfft += X[high_i] * Y[high_i];
                }
                cumsum_dp_corr = (1.0 - nfft) * static_cast<double>(nfft < 1.0);
            }
            nrows = now2.size(1) - 1;
            D = rtNaN;
            c_X.set_size(now2.size(1));
            ibtile = now2.size(1);
            for (i = 0; i < ibtile; i++) {
                c_X[i] = now2[i];
            }
            X.set_size(c_X.size(0));
            ibtile = c_X.size(0);
            for (i = 0; i < ibtile; i++) {
                X[i] = c_X[i];
            }
            ibtile = pd_.lastMag.size(0);
            c_Y.set_size(pd_.lastMag.size(0));
            for (i = 0; i < ibtile; i++) {
                c_Y[i] = pd_.lastMag[i + pd_.lastMag.size(0)] - meanpast2;
            }
            Y.set_size(c_Y.size(0));
            ibtile = c_Y.size(0);
            for (i = 0; i < ibtile; i++) {
                Y[i] = c_Y[i];
            }
            nx = c_X.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += c_X[ncols] * c_X[ncols];
            }
            nfft = std::sqrt(nfft);
            ibtile = c_X.size(0);
            for (i = 0; i < ibtile; i++) {
                X[i] = c_X[i] / nfft;
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &ii_data);
            }
            nx = c_Y.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += c_Y[ncols] * c_Y[ncols];
            }
            nfft = std::sqrt(nfft);
            ibtile = c_Y.size(0);
            for (i = 0; i < ibtile; i++) {
                Y[i] = c_Y[i] / nfft;
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &exponent);
            }
            nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(X[high_i])) {
                    nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            b_nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(Y[high_i])) {
                    b_nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            if ((!b_nanflag) && (!nanflag)) {
                nfft = 0.0;
                i = now2.size(1);
                for (high_i = 0; high_i < i; high_i++) {
                    nfft += X[high_i] * Y[high_i];
                }
                D = (1.0 - nfft) * static_cast<double>(nfft < 1.0);
            }
            nrows = now1.size(1) - 1;
            b_D = rtNaN;
            nx = b_X.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += b_X[ncols] * b_X[ncols];
            }
            nfft = std::sqrt(nfft);
            ncols = b_X.size(0) - 1;
            X.set_size(b_X.size(0));
            for (i = 0; i <= ncols; i++) {
                X[i] = b_X[i] / nfft;
            }
            ibtile = X.size(0);
            for (i = 0; i < ibtile; i++) {
                b_X[i] = X[i];
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &c_exponent);
            }
            nx = c_Y.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += c_Y[ncols] * c_Y[ncols];
            }
            nfft = std::sqrt(nfft);
            ncols = c_Y.size(0) - 1;
            Y.set_size(c_Y.size(0));
            for (i = 0; i <= ncols; i++) {
                Y[i] = c_Y[i] / nfft;
            }
            ibtile = Y.size(0);
            for (i = 0; i < ibtile; i++) {
                c_Y[i] = Y[i];
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &d_exponent);
            }
            nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(b_X[high_i])) {
                    nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            b_nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(c_Y[high_i])) {
                    b_nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            if ((!b_nanflag) && (!nanflag)) {
                nfft = 0.0;
                i = now1.size(1);
                for (high_i = 0; high_i < i; high_i++) {
                    nfft += b_X[high_i] * c_Y[high_i];
                }
                b_D = (1.0 - nfft) * static_cast<double>(nfft < 1.0);
            }
            nrows = now2.size(1) - 1;
            dp = rtNaN;
            nx = c_X.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += c_X[ncols] * c_X[ncols];
            }
            nfft = std::sqrt(nfft);
            ncols = c_X.size(0) - 1;
            X.set_size(c_X.size(0));
            for (i = 0; i <= ncols; i++) {
                X[i] = c_X[i] / nfft;
            }
            ibtile = X.size(0);
            for (i = 0; i < ibtile; i++) {
                c_X[i] = X[i];
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &e_exponent);
            }
            nx = b_Y.size(0);
            r = 0.0;
            nfft = 0.0;
            for (ncols = 0; ncols < nx; ncols++) {
                nfft += b_Y[ncols] * b_Y[ncols];
            }
            nfft = std::sqrt(nfft);
            ncols = b_Y.size(0) - 1;
            Y.set_size(b_Y.size(0));
            for (i = 0; i <= ncols; i++) {
                Y[i] = b_Y[i] / nfft;
            }
            ibtile = Y.size(0);
            for (i = 0; i < ibtile; i++) {
                b_Y[i] = Y[i];
            }
            if ((!(nfft < rtInf)) && (nfft > 0.0)) {
                r = rtInf;
            }
            if ((!std::isinf(r)) && (!(r <= 2.2250738585072014E-308))) {
                frexp(r, &f_exponent);
            }
            nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(c_X[high_i])) {
                    nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            b_nanflag = false;
            high_i = 0;
            exitg1 = false;
            while ((!exitg1) && (high_i <= nrows)) {
                if (std::isnan(b_Y[high_i])) {
                    b_nanflag = true;
                    exitg1 = true;
                } else {
                    high_i++;
                }
            }
            if ((!b_nanflag) && (!nanflag)) {
                nfft = 0.0;
                i = now2.size(1);
                for (high_i = 0; high_i < i; high_i++) {
                    nfft += c_X[high_i] * b_Y[high_i];
                }
                dp = (1.0 - nfft) * static_cast<double>(nfft < 1.0);
            }
            *permCoef = permLogic(cumsum_dp_corr + 1.0E-9, b_D + 1.0E-9, dp + 1.0E-9, D + 1.0E-9, meannow1, meannow2,
                                  meanpast1, meanpast2);
            if (*permCoef < 0.0) {
                i = rawPhase.size(0);
                i1 = rawPhase.size(1);
                high_i = rawPhase.size(0);
                nx = rawPhase.size(1);
                ncols = rawPhase.size(0) - 1;
                nrows = rawPhase.size(1) - 1;
                b_rawPhase.set_size(rawPhase.size(0), rawPhase.size(1), 2);
                for (ibmat = 0; ibmat < 2; ibmat++) {
                    for (jtilecol = 0; jtilecol <= nrows; jtilecol++) {
                        for (ibtile = 0; ibtile <= ncols; ibtile++) {
                            b_rawPhase[(ibtile + b_rawPhase.size(0) * jtilecol) +
                                       b_rawPhase.size(0) * b_rawPhase.size(1) * ibmat] =
                                rawPhase[(ibtile + high_i * jtilecol) + high_i * nx * (1 - ibmat)];
                        }
                    }
                }
                rawPhase.set_size(i, i1, 2, 1);
                ncols = (i * i1) << 1;
                for (i = 0; i < ncols; i++) {
                    rawPhase[i] = b_rawPhase[i];
                }
                i = rawMag.size(0);
                i1 = rawMag.size(1);
                high_i = rawMag.size(0);
                nx = rawMag.size(1);
                ncols = rawMag.size(0) - 1;
                nrows = rawMag.size(1) - 1;
                b_rawPhase.set_size(rawMag.size(0), rawMag.size(1), 2);
                for (ibmat = 0; ibmat < 2; ibmat++) {
                    for (jtilecol = 0; jtilecol <= nrows; jtilecol++) {
                        for (ibtile = 0; ibtile <= ncols; ibtile++) {
                            b_rawPhase[(ibtile + b_rawPhase.size(0) * jtilecol) +
                                       b_rawPhase.size(0) * b_rawPhase.size(1) * ibmat] =
                                rawMag[(ibtile + high_i * jtilecol) + high_i * nx * (1 - ibmat)];
                        }
                    }
                }
                rawMag.set_size(i, i1, 2, 1);
                ncols = (i * i1) << 1;
                for (i = 0; i < ncols; i++) {
                    rawMag[i] = b_rawPhase[i];
                }
                ncols = magDb.size(0) - 1;
                b_magDb.set_size(magDb.size(0), 2);
                for (i = 0; i < 2; i++) {
                    for (i1 = 0; i1 <= ncols; i1++) {
                        b_magDb[i1 + b_magDb.size(0) * i] = magDb[i1 + magDb.size(0) * (1 - i)];
                    }
                }
                magDb.set_size(b_magDb.size(0), 2);
                ibtile = b_magDb.size(0) * 2;
                for (i = 0; i < ibtile; i++) {
                    magDb[i] = b_magDb[i];
                }
                *perm = 1U;
            }
        }
        pd_.lastMag.set_size(magDb.size(0), 2);
        ibtile = magDb.size(0) * 2;
        for (i = 0; i < ibtile; i++) {
            pd_.lastMag[i] = magDb[i];
        }
        pd_.lastMag_not_empty = (pd_.lastMag.size(0) != 0);
    }
    nx = subcarrierIndex.size(0);
    Y.set_size(subcarrierIndex.size(0));
    for (k = 0; k < nx; k++) {
        Y[k] = std::abs(static_cast<double>(subcarrierIndex[k]));
    }
    nx = subcarrierIndex.size(0);
    X.set_size(subcarrierIndex.size(0));
    for (k = 0; k < nx; k++) {
        X[k] = std::abs(static_cast<double>(subcarrierIndex[k]));
    }
    ncols = X.size(0);
    if (X.size(0) <= 2) {
        if (X.size(0) == 1) {
            nrows = static_cast<int>(X[0]);
        } else if (X[0] > X[X.size(0) - 1]) {
            nrows = static_cast<int>(X[X.size(0) - 1]);
        } else {
            nrows = static_cast<int>(X[0]);
        }
    } else {
        nrows = static_cast<int>(X[0]);
        for (k = 2; k <= ncols; k++) {
            i = static_cast<int>(X[k - 1]);
            if (nrows > i) {
                nrows = i;
            }
        }
    }
    positiveInput.set_size(Y.size(0));
    ibtile = Y.size(0);
    for (i = 0; i < ibtile; i++) {
        positiveInput[i] = (static_cast<int>(Y[i]) == nrows);
    }
    exponent = (positiveInput.size(0) >= 1);
    ncols = 0;
    nrows = 0;
    exitg1 = false;
    while ((!exitg1) && (nrows <= positiveInput.size(0) - 1)) {
        if (positiveInput[nrows]) {
            ncols = 1;
            ii_data = nrows + 1;
            exitg1 = true;
        } else {
            nrows++;
        }
    }
    if (exponent == 1) {
        if (ncols == 0) {
            exponent = 0;
        }
    } else {
        exponent = (ncols >= 1);
    }
    for (i = 0; i < exponent; i++) {
        pivotIndex_data = static_cast<unsigned int>(ii_data);
    }
    positiveInput.set_size(interpedIndex.size(0));
    ibtile = interpedIndex.size(0);
    for (i = 0; i < ibtile; i++) {
        positiveInput[i] = (interpedIndex[i] == subcarrierIndex[static_cast<int>(pivotIndex_data) - 1]);
    }
    exponent = (positiveInput.size(0) >= 1);
    ncols = 0;
    nrows = 0;
    exitg1 = false;
    while ((!exitg1) && (nrows <= positiveInput.size(0) - 1)) {
        if (positiveInput[nrows]) {
            ncols = 1;
            ii_data = nrows + 1;
            exitg1 = true;
        } else {
            nrows++;
        }
    }
    if (exponent == 1) {
        if (ncols == 0) {
            exponent = 0;
        }
    } else {
        exponent = (ncols >= 1);
    }
    ibtile = rawPhase.size(1);
    ncols = rawPhase.size(2);
    nrows = rawPhase.size(3);
    pivotPhase.set_size(1, rawPhase.size(1), rawPhase.size(2), rawPhase.size(3));
    for (i = 0; i < nrows; i++) {
        for (i1 = 0; i1 < ncols; i1++) {
            for (ibmat = 0; ibmat < ibtile; ibmat++) {
                pivotPhase[(ibmat + pivotPhase.size(1) * i1) + pivotPhase.size(1) * pivotPhase.size(2) * i] =
                    rawPhase[(((static_cast<int>(pivotIndex_data) + rawPhase.size(0) * ibmat) +
                               rawPhase.size(0) * rawPhase.size(1) * i1) +
                              rawPhase.size(0) * rawPhase.size(1) * rawPhase.size(2) * i) -
                             1];
            }
        }
    }
    ncols = 1;
    if (rawPhase.size(0) != 1) {
        ncols = 0;
    } else if (rawPhase.size(1) == 1) {
        if (rawPhase.size(2) != 1) {
            ncols = 2;
        } else if (rawPhase.size(3) != 1) {
            ncols = 3;
        }
    }
    jtilecol = rawPhase.size(ncols) - 1;
    X.set_size(rawPhase.size(ncols));
    nx = 1;
    for (k = 0; k < ncols; k++) {
        nx *= rawPhase.size(k);
    }
    ibmat = (rawPhase.size(ncols) - 1) * nx;
    ibtile = 1;
    ncols += 2;
    for (k = ncols; k < 5; k++) {
        ibtile *= rawPhase.size(k - 1);
    }
    ncols = 0;
    for (nyrows = 0; nyrows < ibtile; nyrows++) {
        nrows = ncols - 1;
        ncols += ibmat;
        for (b_exponent = 0; b_exponent < nx; b_exponent++) {
            nrows++;
            ncols++;
            for (k = 0; k <= jtilecol; k++) {
                X[k] = rawPhase[nrows + k * nx];
            }
            high_i = X.size(0);
            cumsum_dp_corr = 0.0;
            pivotIndex_data = 1U;
            exitg1 = false;
            while ((!exitg1) && (static_cast<int>(pivotIndex_data) < high_i)) {
                dp = X[static_cast<int>(pivotIndex_data) - 1];
                if (std::isinf(dp) || std::isnan(dp)) {
                    pivotIndex_data = static_cast<unsigned int>(static_cast<int>(pivotIndex_data) + 1);
                } else {
                    exitg1 = true;
                }
            }
            if (static_cast<int>(pivotIndex_data) < X.size(0)) {
                nfft = X[static_cast<int>(pivotIndex_data) - 1];
                int exitg2;
                do {
                    exitg2 = 0;
                    pivotIndex_data++;
                    exitg1 = false;
                    while ((!exitg1) && (pivotIndex_data <= static_cast<unsigned int>(high_i))) {
                        dp = X[static_cast<int>(pivotIndex_data) - 1];
                        if (std::isinf(dp) || std::isnan(dp)) {
                            pivotIndex_data++;
                        } else {
                            exitg1 = true;
                        }
                    }
                    if (pivotIndex_data > static_cast<unsigned int>(high_i)) {
                        exitg2 = 1;
                    } else {
                        r = X[static_cast<int>(pivotIndex_data) - 1];
                        dp = r - nfft;
                        nfft = dp / 6.2831853071795862;
                        if (std::abs(rt_remd_snf(nfft, 1.0)) <= 0.5) {
                            nfft = std::trunc(nfft);
                        } else {
                            nfft = std::round(nfft);
                        }
                        if (std::abs(dp) >= 3.1415926535897931) {
                            cumsum_dp_corr += nfft;
                        }
                        nfft = r;
                        X[static_cast<int>(pivotIndex_data) - 1] = r - 6.2831853071795862 * cumsum_dp_corr;
                    }
                } while (exitg2 == 0);
            }
            for (k = 0; k <= jtilecol; k++) {
                rawPhase[nrows + k * nx] = X[k];
            }
        }
    }
    X.set_size(subcarrierIndex.size(0));
    ibtile = subcarrierIndex.size(0);
    for (i = 0; i < ibtile; i++) {
        X[i] = subcarrierIndex[i];
    }
    nyrows = rawPhase.size(0);
    nycols = rawPhase.size(1) * rawPhase.size(2) * rawPhase.size(3) - 1;
    nx = subcarrierIndex.size(0) - 1;
    resultPhase.set_size(interpedIndex.size(0), rawPhase.size(1), rawPhase.size(2), rawPhase.size(3));
    ibtile = interpedIndex.size(0) * rawPhase.size(1) * rawPhase.size(2) * rawPhase.size(3);
    for (i = 0; i < ibtile; i++) {
        resultPhase[i] = rtNaN;
    }
    if (interpedIndex.size(0) != 0) {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
            i = subcarrierIndex.size(0) >> 1;
            for (ibtile = 0; ibtile < i; ibtile++) {
                nrows = static_cast<int>(X[ibtile]);
                high_i = nx - ibtile;
                X[ibtile] = X[high_i];
                X[high_i] = nrows;
            }
            if ((rawPhase.size(0) != 0) && (rawPhase.size(1) != 0) && (rawPhase.size(2) != 0) &&
                (rawPhase.size(3) != 0) && (rawPhase.size(0) > 1)) {
                jtilecol = rawPhase.size(0);
                ibtile = rawPhase.size(0) - 1;
                ibmat = rawPhase.size(0) >> 1;
                i = rawPhase.size(1) * rawPhase.size(2) * rawPhase.size(3) - 1;
                for (b_exponent = 0; b_exponent <= i; b_exponent++) {
                    nrows = b_exponent * jtilecol;
                    for (k = 0; k < ibmat; k++) {
                        nx = nrows + k;
                        nfft = rawPhase[nx];
                        i1 = (nrows + ibtile) - k;
                        rawPhase[nx] = rawPhase[i1];
                        rawPhase[i1] = nfft;
                    }
                }
            }
        }
        nx = interpedIndex.size(0);
        for (k = 0; k < nx; k++) {
            if (std::isnan(interpedIndex[k])) {
                for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                    resultPhase[k + b_exponent * nx] = rtNaN;
                }
            } else if ((!(interpedIndex[k] > X[X.size(0) - 1])) && (!(interpedIndex[k] < X[0]))) {
                high_i = X.size(0);
                ibtile = 1;
                nrows = 2;
                while (high_i > nrows) {
                    ncols = (ibtile >> 1) + (high_i >> 1);
                    if (((ibtile & 1) == 1) && ((high_i & 1) == 1)) {
                        ncols++;
                    }
                    if (interpedIndex[k] >= X[ncols - 1]) {
                        ibtile = ncols;
                        nrows = ncols + 1;
                    } else {
                        high_i = ncols;
                    }
                }
                nfft = X[ibtile - 1];
                r = (interpedIndex[k] - nfft) / (X[ibtile] - nfft);
                if (r == 0.0) {
                    for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                        resultPhase[k + b_exponent * nx] = rawPhase[(ibtile + b_exponent * nyrows) - 1];
                    }
                } else if (r == 1.0) {
                    for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                        resultPhase[k + b_exponent * nx] = rawPhase[ibtile + b_exponent * nyrows];
                    }
                } else {
                    for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                        ncols = ibtile + b_exponent * nyrows;
                        nfft = rawPhase[ncols - 1];
                        dp = rawPhase[ncols];
                        if (nfft == dp) {
                            resultPhase[k + b_exponent * nx] = nfft;
                        } else {
                            resultPhase[k + b_exponent * nx] = (1.0 - r) * nfft + r * dp;
                        }
                    }
                }
            }
        }
    }
    ibtile = resultPhase.size(1);
    ncols = resultPhase.size(2);
    nrows = resultPhase.size(3);
    b_x.set_size(exponent, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
    for (i = 0; i < nrows; i++) {
        for (i1 = 0; i1 < ncols; i1++) {
            for (ibmat = 0; ibmat < ibtile; ibmat++) {
                for (jtilecol = 0; jtilecol < exponent; jtilecol++) {
                    b_x[(b_x.size(0) * ibmat + b_x.size(0) * b_x.size(1) * i1) +
                        b_x.size(0) * b_x.size(1) * b_x.size(2) * i] =
                        resultPhase[(((ii_data + resultPhase.size(0) * ibmat) +
                                      resultPhase.size(0) * resultPhase.size(1) * i1) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * i) -
                                    1];
                }
            }
        }
    }
    ibtile = b_x.size(0) * b_x.size(1) * b_x.size(2) * b_x.size(3);
    pivotPhase.set_size(b_x.size(0), b_x.size(1), b_x.size(2), b_x.size(3));
    for (i = 0; i < ibtile; i++) {
        pivotPhase[i] = b_x[i] - pivotPhase[i];
    }
    b.set_size(interpedIndex.size(0), pivotPhase.size(1), pivotPhase.size(2), pivotPhase.size(3));
    if ((interpedIndex.size(0) != 0) && (pivotPhase.size(1) != 0) && (pivotPhase.size(2) != 0) &&
        (pivotPhase.size(3) != 0)) {
        ncols = pivotPhase.size(3);
        nrows = pivotPhase.size(2);
        high_i = pivotPhase.size(1);
        i = interpedIndex.size(0) - 1;
        for (k = 0; k < ncols; k++) {
            for (exponent = 0; exponent < nrows; exponent++) {
                for (nx = 0; nx < high_i; nx++) {
                    for (ibtile = 0; ibtile <= i; ibtile++) {
                        b[((ibtile + b.size(0) * nx) + b.size(0) * b.size(1) * exponent) +
                          b.size(0) * b.size(1) * b.size(2) * k] =
                            pivotPhase[(nx + pivotPhase.size(1) * exponent) +
                                       pivotPhase.size(1) * pivotPhase.size(2) * k];
                    }
                }
            }
        }
    }
    ibtile = resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * resultPhase.size(3);
    for (i = 0; i < ibtile; i++) {
        resultPhase[i] = resultPhase[i] - b[i];
    }
    c_x.set_size(interpedIndex.size(0));
    ibtile = interpedIndex.size(0);
    for (i = 0; i < ibtile; i++) {
        c_x[i] = (interpedIndex[i] == 0.0);
    }
    nanflag = false;
    ncols = 1;
    exitg1 = false;
    while ((!exitg1) && (ncols <= c_x.size(0))) {
        if (c_x[ncols - 1]) {
            nanflag = true;
            exitg1 = true;
        } else {
            ncols++;
        }
    }
    if (nanflag) {
        positiveInput.set_size(interpedIndex.size(0));
        ibtile = interpedIndex.size(0);
        for (i = 0; i < ibtile; i++) {
            positiveInput[i] = (interpedIndex[i] == 0.0);
        }
        exponent = (positiveInput.size(0) >= 1);
        ncols = 0;
        nrows = 0;
        exitg1 = false;
        while ((!exitg1) && (nrows <= positiveInput.size(0) - 1)) {
            if (positiveInput[nrows]) {
                ncols = 1;
                ii_data = nrows + 1;
                exitg1 = true;
            } else {
                nrows++;
            }
        }
        if (exponent == 1) {
            if (ncols == 0) {
                exponent = 0;
            }
        } else {
            exponent = (ncols >= 1);
        }
        for (i = 0; i < exponent; i++) {
            pivotIndex_data = static_cast<unsigned int>(ii_data);
        }
        ibtile = resultPhase.size(1);
        ncols = resultPhase.size(2);
        nrows = resultPhase.size(3);
        pivotPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        for (i = 0; i < nrows; i++) {
            for (i1 = 0; i1 < ncols; i1++) {
                for (ibmat = 0; ibmat < ibtile; ibmat++) {
                    pivotPhase[(ibmat + pivotPhase.size(1) * i1) + pivotPhase.size(1) * pivotPhase.size(2) * i] =
                        resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * ibmat) +
                                      resultPhase.size(0) * resultPhase.size(1) * i1) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * i) -
                                    1];
                }
            }
        }
        ibtile = resultPhase.size(1);
        ncols = resultPhase.size(2);
        nrows = resultPhase.size(3);
        r2.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        for (i = 0; i < nrows; i++) {
            for (i1 = 0; i1 < ncols; i1++) {
                for (ibmat = 0; ibmat < ibtile; ibmat++) {
                    r2[(ibmat + r2.size(1) * i1) + r2.size(1) * r2.size(2) * i] =
                        (resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * ibmat) +
                                       resultPhase.size(0) * resultPhase.size(1) * i1) +
                                      resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * i) -
                                     1] < -3.1415926535897931);
                }
            }
        }
        ibtile = resultPhase.size(1);
        ncols = resultPhase.size(2);
        nrows = resultPhase.size(3);
        r3.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        for (i = 0; i < nrows; i++) {
            for (i1 = 0; i1 < ncols; i1++) {
                for (ibmat = 0; ibmat < ibtile; ibmat++) {
                    r3[(ibmat + r3.size(1) * i1) + r3.size(1) * r3.size(2) * i] =
                        (resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * ibmat) +
                                       resultPhase.size(0) * resultPhase.size(1) * i1) +
                                      resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * i) -
                                     1] > 3.1415926535897931);
                }
            }
        }
        ncols = r2.size(1) * (r2.size(2) * r2.size(3)) - 1;
        high_i = 0;
        for (nyrows = 0; nyrows <= ncols; nyrows++) {
            if (r2[nyrows] || r3[nyrows]) {
                high_i++;
            }
        }
        r4.set_size(high_i);
        high_i = 0;
        for (nyrows = 0; nyrows <= ncols; nyrows++) {
            if (r2[nyrows] || r3[nyrows]) {
                r4[high_i] = nyrows + 1;
                high_i++;
            }
        }
        ibtile = resultPhase.size(1);
        ncols = resultPhase.size(2);
        nrows = resultPhase.size(3);
        b_resultPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        for (i = 0; i < nrows; i++) {
            for (i1 = 0; i1 < ncols; i1++) {
                for (ibmat = 0; ibmat < ibtile; ibmat++) {
                    b_resultPhase[(ibmat + b_resultPhase.size(1) * i1) +
                                  b_resultPhase.size(1) * b_resultPhase.size(2) * i] =
                        resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * ibmat) +
                                      resultPhase.size(0) * resultPhase.size(1) * i1) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * i) -
                                    1];
                }
            }
        }
        X.set_size(r4.size(0));
        ibtile = r4.size(0);
        for (i = 0; i < ibtile; i++) {
            X[i] = b_resultPhase[r4[i] - 1] + 3.1415926535897931;
        }
        positiveInput.set_size(X.size(0));
        ibtile = X.size(0);
        for (i = 0; i < ibtile; i++) {
            positiveInput[i] = (X[i] > 0.0);
        }
        ibtile = X.size(0);
        for (i = 0; i < ibtile; i++) {
            nfft = X[i];
            if (std::isnan(nfft) || std::isinf(nfft)) {
                dp = rtNaN;
            } else if (nfft == 0.0) {
                dp = 0.0;
            } else {
                dp = std::fmod(nfft, 6.2831853071795862);
                nanflag = (dp == 0.0);
                if (!nanflag) {
                    r = std::abs(nfft / 6.2831853071795862);
                    nanflag = !(std::abs(r - std::floor(r + 0.5)) > 2.2204460492503131E-16 * r);
                }
                if (nanflag) {
                    dp = 0.0;
                } else if (nfft < 0.0) {
                    dp += 6.2831853071795862;
                }
            }
            X[i] = dp;
        }
        if (X.size(0) == positiveInput.size(0)) {
            positiveInput.set_size(X.size(0));
            ibtile = X.size(0);
            for (i = 0; i < ibtile; i++) {
                positiveInput[i] = ((X[i] == 0.0) && positiveInput[i]);
            }
        } else {
            binary_expand_op(positiveInput, X);
        }
        ncols = positiveInput.size(0);
        for (nyrows = 0; nyrows < ncols; nyrows++) {
            if (positiveInput[nyrows]) {
                X[nyrows] = 6.2831853071795862;
            }
        }
        ncols = r2.size(1) * (r2.size(2) * r2.size(3)) - 1;
        high_i = 0;
        for (nyrows = 0; nyrows <= ncols; nyrows++) {
            if (r2[nyrows] || r3[nyrows]) {
                high_i++;
            }
        }
        r5.set_size(high_i);
        high_i = 0;
        for (nyrows = 0; nyrows <= ncols; nyrows++) {
            if (r2[nyrows] || r3[nyrows]) {
                r5[high_i] = nyrows + 1;
                high_i++;
            }
        }
        ibtile = X.size(0);
        for (i = 0; i < ibtile; i++) {
            pivotPhase[r5[i] - 1] = X[i] - 3.1415926535897931;
        }
        ibtile = resultPhase.size(1);
        ncols = resultPhase.size(2);
        nrows = resultPhase.size(3);
        pivotPhase.set_size(1, resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
        for (i = 0; i < nrows; i++) {
            for (i1 = 0; i1 < ncols; i1++) {
                for (ibmat = 0; ibmat < ibtile; ibmat++) {
                    pivotPhase[(ibmat + pivotPhase.size(1) * i1) + pivotPhase.size(1) * pivotPhase.size(2) * i] =
                        resultPhase[(((static_cast<int>(pivotIndex_data) + resultPhase.size(0) * ibmat) +
                                      resultPhase.size(0) * resultPhase.size(1) * i1) +
                                     resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * i) -
                                    1] -
                        pivotPhase[(ibmat + pivotPhase.size(1) * i1) + pivotPhase.size(1) * pivotPhase.size(2) * i];
                }
            }
        }
        positiveInput.set_size(pivotPhase.size(1) * pivotPhase.size(2) * pivotPhase.size(3));
        ibtile = pivotPhase.size(1) * pivotPhase.size(2) * pivotPhase.size(3);
        for (i = 0; i < ibtile; i++) {
            positiveInput[i] = (pivotPhase[i] == 0.0);
        }
        nanflag = true;
        ncols = 1;
        exitg1 = false;
        while ((!exitg1) && (ncols <= positiveInput.size(0))) {
            if (!positiveInput[ncols - 1]) {
                nanflag = false;
                exitg1 = true;
            } else {
                ncols++;
            }
        }
        if (!nanflag) {
            b.set_size(interpedIndex.size(0), pivotPhase.size(1), pivotPhase.size(2), pivotPhase.size(3));
            if ((interpedIndex.size(0) != 0) && (pivotPhase.size(1) != 0) && (pivotPhase.size(2) != 0) &&
                (pivotPhase.size(3) != 0)) {
                ncols = pivotPhase.size(3);
                nrows = pivotPhase.size(2);
                high_i = pivotPhase.size(1);
                i = interpedIndex.size(0) - 1;
                for (k = 0; k < ncols; k++) {
                    for (exponent = 0; exponent < nrows; exponent++) {
                        for (nx = 0; nx < high_i; nx++) {
                            for (ibtile = 0; ibtile <= i; ibtile++) {
                                b[((ibtile + b.size(0) * nx) + b.size(0) * b.size(1) * exponent) +
                                  b.size(0) * b.size(1) * b.size(2) * k] =
                                    pivotPhase[(nx + pivotPhase.size(1) * exponent) +
                                               pivotPhase.size(1) * pivotPhase.size(2) * k];
                            }
                        }
                    }
                }
            }
            ibtile = resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * resultPhase.size(3);
            for (i = 0; i < ibtile; i++) {
                resultPhase[i] = resultPhase[i] - b[i];
            }
        }
    }
    nyrows = rawMag.size(0);
    nycols = rawMag.size(1) * rawMag.size(2) * rawMag.size(3) - 1;
    nx = subcarrierIndex.size(0) - 1;
    resultMag.set_size(interpedIndex.size(0), rawMag.size(1), rawMag.size(2), rawMag.size(3));
    ibtile = interpedIndex.size(0) * rawMag.size(1) * rawMag.size(2) * rawMag.size(3);
    for (i = 0; i < ibtile; i++) {
        resultMag[i] = rtNaN;
    }
    if (interpedIndex.size(0) != 0) {
        if (subcarrierIndex[1] < subcarrierIndex[0]) {
            i = subcarrierIndex.size(0) >> 1;
            for (ibtile = 0; ibtile < i; ibtile++) {
                nrows = subcarrierIndex[ibtile];
                ncols = nx - ibtile;
                subcarrierIndex[ibtile] = subcarrierIndex[ncols];
                subcarrierIndex[ncols] = static_cast<short>(nrows);
            }
            if ((rawMag.size(0) != 0) && (rawMag.size(1) != 0) && (rawMag.size(2) != 0) && (rawMag.size(3) != 0) &&
                (rawMag.size(0) > 1)) {
                jtilecol = rawMag.size(0);
                ibtile = rawMag.size(0) - 1;
                ibmat = rawMag.size(0) >> 1;
                i = rawMag.size(1) * rawMag.size(2) * rawMag.size(3) - 1;
                for (b_exponent = 0; b_exponent <= i; b_exponent++) {
                    nrows = b_exponent * jtilecol;
                    for (k = 0; k < ibmat; k++) {
                        nx = nrows + k;
                        nfft = rawMag[nx];
                        i1 = (nrows + ibtile) - k;
                        rawMag[nx] = rawMag[i1];
                        rawMag[i1] = nfft;
                    }
                }
            }
        }
        nx = interpedIndex.size(0);
        for (k = 0; k < nx; k++) {
            if (std::isnan(interpedIndex[k])) {
                for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                    resultMag[k + b_exponent * nx] = rtNaN;
                }
            } else if ((!(interpedIndex[k] > subcarrierIndex[subcarrierIndex.size(0) - 1])) &&
                       (!(interpedIndex[k] < subcarrierIndex[0]))) {
                high_i = subcarrierIndex.size(0);
                ibtile = 1;
                nrows = 2;
                while (high_i > nrows) {
                    ncols = (ibtile >> 1) + (high_i >> 1);
                    if (((ibtile & 1) == 1) && ((high_i & 1) == 1)) {
                        ncols++;
                    }
                    if (interpedIndex[k] >= subcarrierIndex[ncols - 1]) {
                        ibtile = ncols;
                        nrows = ncols + 1;
                    } else {
                        high_i = ncols;
                    }
                }
                r = (interpedIndex[k] - static_cast<double>(subcarrierIndex[ibtile - 1])) /
                    static_cast<double>(subcarrierIndex[ibtile] - subcarrierIndex[ibtile - 1]);
                if (r == 0.0) {
                    for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                        resultMag[k + b_exponent * nx] = rawMag[(ibtile + b_exponent * nyrows) - 1];
                    }
                } else if (r == 1.0) {
                    for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                        resultMag[k + b_exponent * nx] = rawMag[ibtile + b_exponent * nyrows];
                    }
                } else {
                    for (b_exponent = 0; b_exponent <= nycols; b_exponent++) {
                        ncols = ibtile + b_exponent * nyrows;
                        nfft = rawMag[ncols - 1];
                        dp = rawMag[ncols];
                        if (nfft == dp) {
                            resultMag[k + b_exponent * nx] = nfft;
                        } else {
                            resultMag[k + b_exponent * nx] = (1.0 - r) * nfft + r * dp;
                        }
                    }
                }
            }
        }
    }
    resultCSI.set_size(resultPhase.size(0), resultPhase.size(1), resultPhase.size(2), resultPhase.size(3));
    ibtile = resultPhase.size(0) * resultPhase.size(1) * resultPhase.size(2) * resultPhase.size(3);
    for (i = 0; i < ibtile; i++) {
        resultCSI[i].re = resultPhase[i] * 0.0;
        resultCSI[i].im = resultPhase[i];
    }
    nx = resultCSI.size(0) * resultCSI.size(1) * resultCSI.size(2) * resultCSI.size(3);
    for (k = 0; k < nx; k++) {
        if (resultCSI[k].im == 0.0) {
            resultCSI[k].re = std::exp(resultCSI[k].re);
            resultCSI[k].im = 0.0;
        } else if (std::isinf(resultCSI[k].im) && std::isinf(resultCSI[k].re) && (resultCSI[k].re < 0.0)) {
            resultCSI[k].re = 0.0;
            resultCSI[k].im = 0.0;
        } else {
            r = std::exp(resultCSI[k].re / 2.0);
            dp = resultCSI[k].im;
            resultCSI[k].re = r * (r * std::cos(resultCSI[k].im));
            resultCSI[k].im = r * (r * std::sin(dp));
        }
    }
    ibtile = resultMag.size(0) * resultMag.size(1) * resultMag.size(2) * resultMag.size(3);
    resultCSI.set_size(resultMag.size(0), resultMag.size(1), resultMag.size(2), resultMag.size(3));
    for (i = 0; i < ibtile; i++) {
        resultCSI[i].re = resultMag[i] * resultCSI[i].re;
        resultCSI[i].im = resultMag[i] * resultCSI[i].im;
    }
    interpedIndex_int16.set_size(interpedIndex.size(0));
    ibtile = interpedIndex.size(0);
    for (i = 0; i < ibtile; i++) {
        interpedIndex_int16[i] = static_cast<short>(std::round(interpedIndex[i]));
    }
}

//
// File trailer for CSIPreprocessor.cpp
//
// [EOF]
//
