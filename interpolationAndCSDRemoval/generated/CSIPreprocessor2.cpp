// Include Files
#include "CSIPreprocessor2.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "rt_defines.h"
#include <cfloat>
#include <cmath>

// Function Declarations
namespace coder {
static void angle(const ::coder::array<creal_T, 4U> &x, ::coder::array<real_T, 4U> &y);

static void b_abs(const ::coder::array<creal_T, 4U> &x, ::coder::array<real_T, 4U> &y);

static real_T b_abs(real_T x);

static void b_abs(const ::coder::array<real_T, 1U> &x, ::coder::array<real_T, 1U> &y);

static void b_exp(::coder::array<creal_T, 4U> &x);

static void b_exp(::coder::array<creal_T, 2U> &x);

static void b_fix(real_T *x);

static void b_floor(real_T *x);

static boolean_T b_isfinite(real_T x);

static boolean_T b_isinf(real_T x);

static boolean_T b_isnan(real_T x);

static real_T b_rem(real_T x);

static void b_round(real_T *x);

static void eml_find(const ::coder::array<boolean_T, 1U> &x, int32_T i_data[], int32_T *i_size);

static void eml_float_colon(real_T a, real_T b, ::coder::array<real_T, 2U> &y);

static int32_T eml_ndims_varsized(const int32_T sz[4]);

static void flip(::coder::array<real_T, 1U> &x);

static void flip(::coder::array<real_T, 4U> &x);

static void float_colon_length(real_T a, real_T b, int32_T *n, real_T *anew, real_T *bnew, boolean_T *n_too_large);

namespace internal {
static int32_T b_bsearch(const ::coder::array<real_T, 1U> &x, real_T xi);

static real_T minimum(const ::coder::array<real_T, 1U> &x);

static int32_T nonSingletonDim(const ::coder::array<real_T, 4U> &x);

namespace scalar {
static real_T b_angle(const creal_T x);

static real_T b_atan2(real_T y, real_T x);

static real_T c_abs(const creal_T x);

static real_T c_abs(real_T x);

static void c_exp(creal_T *x);

static void c_fix(real_T *x);

static void c_floor(real_T *x);

static real_T c_rem(real_T x);

static void c_round(real_T *x);

} // namespace scalar
static real_T xdlapy2(real_T x1, real_T x2);

} // namespace internal
static void interp1(const ::coder::array<real_T, 1U> &varargin_1, const ::coder::array<real_T, 4U> &varargin_2,
                    const ::coder::array<real_T, 1U> &varargin_3, ::coder::array<real_T, 4U> &Vq);

static void interp1(const ::coder::array<real_T, 1U> &varargin_1, const ::coder::array<real_T, 1U> &varargin_2,
                    const ::coder::array<real_T, 2U> &varargin_3, ::coder::array<real_T, 2U> &Vq);

static void interp1_work(::coder::array<real_T, 4U> &y, const ::coder::array<real_T, 1U> &xi,
                         ::coder::array<real_T, 1U> &x, ::coder::array<real_T, 4U> &yi);

static void interp1_work(::coder::array<real_T, 1U> &y, const ::coder::array<real_T, 2U> &xi,
                         ::coder::array<real_T, 1U> &x, ::coder::array<real_T, 2U> &yi);

static real_T maxabs(real_T a, real_T b);

static void repmat(const ::coder::array<real_T, 4U> &a, real_T varargin_1, ::coder::array<real_T, 4U> &b);

static void repmat(const ::coder::array<creal_T, 2U> &a, real_T varargin_3, real_T varargin_4,
                   ::coder::array<creal_T, 4U> &b);

static void sameSizeBinaryOp(const ::coder::array<creal_T, 4U> &x, const ::coder::array<creal_T, 4U> &y,
                             ::coder::array<creal_T, 4U> &output);

static void sameSizeBinaryOp(const ::coder::array<real_T, 4U> &x, const ::coder::array<real_T, 4U> &y,
                             ::coder::array<real_T, 4U> &output);

static void sameSizeBinaryOp(const ::coder::array<real_T, 4U> &x, const ::coder::array<creal_T, 4U> &y,
                             ::coder::array<creal_T, 4U> &output);

static void unwrap(::coder::array<real_T, 4U> &p);

static void unwrap_vector(::coder::array<real_T, 1U> &p);

} // namespace coder
static void getCSDShift(real_T numSTS, real_T CBW, real_T csdShift_data[], int32_T csdShift_size[2]);

static real_T getNumFFTTones(real_T packetFormat, real_T CBW);

static void magPhase2CSI(const coder::array<real_T, 4U> &mag, const coder::array<real_T, 4U> &phase,
                         coder::array<creal_T, 4U> &csi);

static real_T rt_atan2d_snf(real_T u0, real_T u1);

static real_T rt_hypotd_snf(real_T u0, real_T u1);

static real_T rt_remd_snf(real_T u0, real_T u1);

// Function Definitions
//
// Arguments    : const ::coder::array<creal_T, 4U> &x
//                ::coder::array<real_T, 4U> &y
// Return Type  : void
//
namespace coder {
static void angle(const ::coder::array<creal_T, 4U> &x, ::coder::array<real_T, 4U> &y)
{
    int32_T nx;
    nx = x.size(0) * x.size(1) * x.size(2) * x.size(3);
    y.set_size(x.size(0), x.size(1), x.size(2), x.size(3));
    for (int32_T k{0}; k < nx; k++) {
        y[k] = internal::scalar::b_angle(x[k]);
    }
}

//
// Arguments    : real_T x
// Return Type  : real_T
//
static real_T b_abs(real_T x)
{
    return internal::scalar::c_abs(x);
}

//
// Arguments    : const ::coder::array<creal_T, 4U> &x
//                ::coder::array<real_T, 4U> &y
// Return Type  : void
//
static void b_abs(const ::coder::array<creal_T, 4U> &x, ::coder::array<real_T, 4U> &y)
{
    int32_T nx;
    nx = x.size(0) * x.size(1) * x.size(2) * x.size(3);
    y.set_size(x.size(0), x.size(1), x.size(2), x.size(3));
    for (int32_T k{0}; k < nx; k++) {
        y[k] = internal::scalar::c_abs(x[k]);
    }
}

//
// Arguments    : const ::coder::array<real_T, 1U> &x
//                ::coder::array<real_T, 1U> &y
// Return Type  : void
//
static void b_abs(const ::coder::array<real_T, 1U> &x, ::coder::array<real_T, 1U> &y)
{
    int32_T nx;
    nx = x.size(0);
    y.set_size(x.size(0));
    for (int32_T k{0}; k < nx; k++) {
        y[k] = internal::scalar::c_abs(x[k]);
    }
}

//
// Arguments    : ::coder::array<creal_T, 2U> &x
// Return Type  : void
//
static void b_exp(::coder::array<creal_T, 2U> &x)
{
    int32_T nx;
    nx = x.size(0) * x.size(1);
    for (int32_T k{0}; k < nx; k++) {
        internal::scalar::c_exp(&x[k]);
    }
}

//
// Arguments    : ::coder::array<creal_T, 4U> &x
// Return Type  : void
//
static void b_exp(::coder::array<creal_T, 4U> &x)
{
    int32_T nx;
    nx = x.size(0) * x.size(1) * x.size(2) * x.size(3);
    for (int32_T k{0}; k < nx; k++) {
        internal::scalar::c_exp(&x[k]);
    }
}

//
// Arguments    : real_T *x
// Return Type  : void
//
static void b_fix(real_T *x)
{
    internal::scalar::c_fix(x);
}

//
// Arguments    : real_T *x
// Return Type  : void
//
static void b_floor(real_T *x)
{
    internal::scalar::c_floor(x);
}

//
// Arguments    : real_T x
// Return Type  : boolean_T
//
static boolean_T b_isfinite(real_T x)
{
    return (!b_isinf(x)) && (!b_isnan(x));
}

//
// Arguments    : real_T x
// Return Type  : boolean_T
//
static boolean_T b_isinf(real_T x)
{
    return std::isinf(x);
}

//
// Arguments    : real_T x
// Return Type  : boolean_T
//
static boolean_T b_isnan(real_T x)
{
    return std::isnan(x);
}

//
// Arguments    : real_T x
// Return Type  : real_T
//
static real_T b_rem(real_T x)
{
    return internal::scalar::c_rem(x);
}

//
// Arguments    : real_T *x
// Return Type  : void
//
static void b_round(real_T *x)
{
    internal::scalar::c_round(x);
}

//
// Arguments    : const ::coder::array<boolean_T, 1U> &x
//                int32_T i_data[]
//                int32_T *i_size
// Return Type  : void
//
static void eml_find(const ::coder::array<boolean_T, 1U> &x, int32_T i_data[], int32_T *i_size)
{
    int32_T idx;
    int32_T ii;
    boolean_T exitg1;
    *i_size = (1 <= x.size(0));
    idx = 0;
    ii = 0;
    exitg1 = false;
    while ((!exitg1) && (ii <= x.size(0) - 1)) {
        if (x[ii]) {
            idx = 1;
            i_data[0] = ii + 1;
            exitg1 = true;
        } else {
            ii++;
        }
    }
    if (*i_size == 1) {
        if (idx == 0) {
            *i_size = 0;
        }
    } else {
        *i_size = (1 <= idx);
    }
}

//
// Arguments    : real_T a
//                real_T b
//                ::coder::array<real_T, 2U> &y
// Return Type  : void
//
static void eml_float_colon(real_T a, real_T b, ::coder::array<real_T, 2U> &y)
{
    real_T a1;
    real_T b1;
    int32_T n;
    boolean_T n_too_large;
    float_colon_length(a, b, &n, &a1, &b1, &n_too_large);
    y.set_size(1, n);
    if (n > 0) {
        y[0] = a1;
        if (n > 1) {
            int32_T nm1d2;
            y[n - 1] = b1;
            nm1d2 = (n - 1) / 2;
            for (int32_T k{0}; k <= nm1d2 - 2; k++) {
                y[k + 1] = a1 + (static_cast<real_T>(k) + 1.0);
                y[(n - k) - 2] = b1 - (static_cast<real_T>(k) + 1.0);
            }
            if (nm1d2 << 1 == n - 1) {
                y[nm1d2] = (a1 + b1) / 2.0;
            } else {
                y[nm1d2] = a1 + static_cast<real_T>(nm1d2);
                y[nm1d2 + 1] = b1 - static_cast<real_T>(nm1d2);
            }
        }
    }
}

//
// Arguments    : const int32_T sz[4]
// Return Type  : int32_T
//
static int32_T eml_ndims_varsized(const int32_T sz[4])
{
    int32_T n;
    n = 4;
    while ((n > 2) && (sz[n - 1] == 1)) {
        n--;
    }
    return n;
}

//
// Arguments    : ::coder::array<real_T, 1U> &x
// Return Type  : void
//
static void flip(::coder::array<real_T, 1U> &x)
{
    if ((x.size(0) != 0) && (x.size(0) > 1)) {
        int32_T n;
        int32_T nd2;
        n = x.size(0) - 1;
        nd2 = x.size(0) >> 1;
        for (int32_T k{0}; k < nd2; k++) {
            real_T tmp;
            tmp = x[k];
            x[k] = x[n - k];
            x[n - k] = tmp;
        }
    }
}

//
// Arguments    : ::coder::array<real_T, 4U> &x
// Return Type  : void
//
static void flip(::coder::array<real_T, 4U> &x)
{
    int32_T iv[4];
    if ((x.size(0) != 0) && (x.size(1) != 0) && (x.size(2) != 0) && (x.size(3) != 0) && (x.size(0) > 1)) {
        int32_T i;
        int32_T k;
        int32_T n;
        int32_T nd2;
        int32_T npages;
        int32_T vlen;
        vlen = x.size(0);
        npages = 1;
        iv[0] = (*(int32_T(*)[4])x.size())[0];
        iv[1] = (*(int32_T(*)[4])x.size())[1];
        iv[2] = (*(int32_T(*)[4])x.size())[2];
        iv[3] = (*(int32_T(*)[4])x.size())[3];
        i = eml_ndims_varsized(iv);
        for (k = 2; k <= i; k++) {
            npages *= x.size(k - 1);
        }
        n = x.size(0) - 1;
        nd2 = x.size(0) >> 1;
        i = npages - 1;
        for (int32_T j{0}; j <= i; j++) {
            npages = j * vlen;
            for (k = 0; k < nd2; k++) {
                real_T tmp;
                int32_T i1;
                int32_T tmp_tmp;
                tmp_tmp = npages + k;
                tmp = x[tmp_tmp];
                i1 = (npages + n) - k;
                x[tmp_tmp] = x[i1];
                x[i1] = tmp;
            }
        }
    }
}

//
// Arguments    : real_T a
//                real_T b
//                int32_T *n
//                real_T *anew
//                real_T *bnew
//                boolean_T *n_too_large
// Return Type  : void
//
static void float_colon_length(real_T a, real_T b, int32_T *n, real_T *anew, real_T *bnew, boolean_T *n_too_large)
{
    real_T cdiff;
    real_T ndbl;
    *anew = a;
    ndbl = (b - a) + 0.5;
    b_floor(&ndbl);
    *bnew = a + ndbl;
    cdiff = *bnew - b;
    if (b_abs(cdiff) < 4.4408920985006262E-16 * maxabs(a, b)) {
        ndbl++;
        *bnew = b;
    } else if (cdiff > 0.0) {
        *bnew = a + (ndbl - 1.0);
    } else {
        ndbl++;
    }
    *n_too_large = false;
    if (ndbl >= 0.0) {
        *n = static_cast<int32_T>(std::floor(ndbl));
    } else {
        *n = 0;
    }
}

//
// Arguments    : const ::coder::array<real_T, 1U> &x
//                real_T xi
// Return Type  : int32_T
//
namespace internal {
static int32_T b_bsearch(const ::coder::array<real_T, 1U> &x, real_T xi)
{
    int32_T high_i;
    int32_T low_ip1;
    int32_T n;
    high_i = x.size(0);
    n = 1;
    low_ip1 = 2;
    while (high_i > low_ip1) {
        int32_T mid_i;
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
// Arguments    : const ::coder::array<real_T, 1U> &x
// Return Type  : real_T
//
static real_T minimum(const ::coder::array<real_T, 1U> &x)
{
    real_T ex;
    int32_T last;
    last = x.size(0);
    if (x.size(0) <= 2) {
        if (x.size(0) == 1) {
            ex = x[0];
        } else if ((x[0] > x[x.size(0) - 1]) || (b_isnan(x[0]) && (!b_isnan(x[x.size(0) - 1])))) {
            ex = x[x.size(0) - 1];
        } else {
            ex = x[0];
        }
    } else {
        int32_T idx;
        int32_T k;
        if (!b_isnan(x[0])) {
            idx = 1;
        } else {
            boolean_T exitg1;
            idx = 0;
            k = 2;
            exitg1 = false;
            while ((!exitg1) && (k <= last)) {
                if (!b_isnan(x[k - 1])) {
                    idx = k;
                    exitg1 = true;
                } else {
                    k++;
                }
            }
        }
        if (idx == 0) {
            ex = x[0];
        } else {
            ex = x[idx - 1];
            idx++;
            for (k = idx; k <= last; k++) {
                real_T d;
                d = x[k - 1];
                if (ex > d) {
                    ex = d;
                }
            }
        }
    }
    return ex;
}

//
// Arguments    : const ::coder::array<real_T, 4U> &x
// Return Type  : int32_T
//
static int32_T nonSingletonDim(const ::coder::array<real_T, 4U> &x)
{
    int32_T dim;
    dim = 2;
    if (x.size(0) != 1) {
        dim = 1;
    } else if (x.size(1) == 1) {
        if (x.size(2) != 1) {
            dim = 3;
        } else if (x.size(3) != 1) {
            dim = 4;
        }
    }
    return dim;
}

//
// Arguments    : const creal_T x
// Return Type  : real_T
//
namespace scalar {
static real_T b_angle(const creal_T x)
{
    return b_atan2(x.im, x.re);
}

//
// Arguments    : real_T y
//                real_T x
// Return Type  : real_T
//
static real_T b_atan2(real_T y, real_T x)
{
    return rt_atan2d_snf(y, x);
}

//
// Arguments    : const creal_T x
// Return Type  : real_T
//
static real_T c_abs(const creal_T x)
{
    return xdlapy2(x.re, x.im);
}

//
// Arguments    : real_T x
// Return Type  : real_T
//
static real_T c_abs(real_T x)
{
    return std::abs(x);
}

//
// Arguments    : creal_T *x
// Return Type  : void
//
static void c_exp(creal_T *x)
{
    if (x->im == 0.0) {
        x->re = std::exp(x->re);
        x->im = 0.0;
    } else if (b_isinf(x->im) && b_isinf(x->re) && (x->re < 0.0)) {
        x->re = 0.0;
        x->im = 0.0;
    } else {
        real_T d;
        real_T r;
        r = std::exp(x->re / 2.0);
        d = x->im;
        x->re = r * (r * std::cos(x->im));
        x->im = r * (r * std::sin(d));
    }
}

//
// Arguments    : real_T *x
// Return Type  : void
//
static void c_fix(real_T *x)
{
    *x = std::trunc(*x);
}

//
// Arguments    : real_T *x
// Return Type  : void
//
static void c_floor(real_T *x)
{
    *x = std::floor(*x);
}

//
// Arguments    : real_T x
// Return Type  : real_T
//
static real_T c_rem(real_T x)
{
    return rt_remd_snf(x, 1.0);
}

//
// Arguments    : real_T *x
// Return Type  : void
//
static void c_round(real_T *x)
{
    *x = std::round(*x);
}

//
// Arguments    : real_T x1
//                real_T x2
// Return Type  : real_T
//
} // namespace scalar
static real_T xdlapy2(real_T x1, real_T x2)
{
    return rt_hypotd_snf(x1, x2);
}

//
// Arguments    : const ::coder::array<real_T, 1U> &varargin_1
//                const ::coder::array<real_T, 1U> &varargin_2
//                const ::coder::array<real_T, 2U> &varargin_3
//                ::coder::array<real_T, 2U> &Vq
// Return Type  : void
//
} // namespace internal
static void interp1(const ::coder::array<real_T, 1U> &varargin_1, const ::coder::array<real_T, 1U> &varargin_2,
                    const ::coder::array<real_T, 2U> &varargin_3, ::coder::array<real_T, 2U> &Vq)
{
    array<real_T, 1U> b_varargin_1;
    array<real_T, 1U> b_varargin_2;
    int32_T i;
    int32_T loop_ub;
    b_varargin_2.set_size(varargin_2.size(0));
    loop_ub = varargin_2.size(0) - 1;
    for (i = 0; i <= loop_ub; i++) {
        b_varargin_2[i] = varargin_2[i];
    }
    b_varargin_1.set_size(varargin_1.size(0));
    loop_ub = varargin_1.size(0) - 1;
    for (i = 0; i <= loop_ub; i++) {
        b_varargin_1[i] = varargin_1[i];
    }
    interp1_work(b_varargin_2, varargin_3, b_varargin_1, Vq);
}

//
// Arguments    : const ::coder::array<real_T, 1U> &varargin_1
//                const ::coder::array<real_T, 4U> &varargin_2
//                const ::coder::array<real_T, 1U> &varargin_3
//                ::coder::array<real_T, 4U> &Vq
// Return Type  : void
//
static void interp1(const ::coder::array<real_T, 1U> &varargin_1, const ::coder::array<real_T, 4U> &varargin_2,
                    const ::coder::array<real_T, 1U> &varargin_3, ::coder::array<real_T, 4U> &Vq)
{
    array<real_T, 4U> b_varargin_2;
    array<real_T, 1U> b_varargin_1;
    int32_T i;
    int32_T loop_ub;
    b_varargin_2.set_size(varargin_2.size(0), varargin_2.size(1), varargin_2.size(2), varargin_2.size(3));
    loop_ub = varargin_2.size(0) * varargin_2.size(1) * varargin_2.size(2) * varargin_2.size(3) - 1;
    for (i = 0; i <= loop_ub; i++) {
        b_varargin_2[i] = varargin_2[i];
    }
    b_varargin_1.set_size(varargin_1.size(0));
    loop_ub = varargin_1.size(0) - 1;
    for (i = 0; i <= loop_ub; i++) {
        b_varargin_1[i] = varargin_1[i];
    }
    interp1_work(b_varargin_2, varargin_3, b_varargin_1, Vq);
}

//
// Arguments    : ::coder::array<real_T, 1U> &y
//                const ::coder::array<real_T, 2U> &xi
//                ::coder::array<real_T, 1U> &x
//                ::coder::array<real_T, 2U> &yi
// Return Type  : void
//
static void interp1_work(::coder::array<real_T, 1U> &y, const ::coder::array<real_T, 2U> &xi,
                         ::coder::array<real_T, 1U> &x, ::coder::array<real_T, 2U> &yi)
{
    int32_T k;
    int32_T nx;
    int32_T outsize_idx_1;
    boolean_T b;
    nx = x.size(0) - 1;
    outsize_idx_1 = xi.size(1);
    yi.set_size(1, xi.size(1));
    for (k = 0; k < outsize_idx_1; k++) {
        yi[k] = rtNaN;
    }
    b = (xi.size(1) == 0);
    if (!b) {
        k = 0;
        int32_T exitg1;
        do {
            exitg1 = 0;
            if (k <= nx) {
                if (b_isnan(x[k])) {
                    exitg1 = 1;
                } else {
                    k++;
                }
            } else {
                real_T xtmp;
                if (x[1] < x[0]) {
                    k = (nx + 1) >> 1;
                    for (outsize_idx_1 = 0; outsize_idx_1 < k; outsize_idx_1++) {
                        xtmp = x[outsize_idx_1];
                        x[outsize_idx_1] = x[nx - outsize_idx_1];
                        x[nx - outsize_idx_1] = xtmp;
                    }
                    flip(y);
                }
                outsize_idx_1 = xi.size(1);
                for (k = 0; k < outsize_idx_1; k++) {
                    xtmp = xi[k];
                    if (b_isnan(xtmp)) {
                        yi[k] = rtNaN;
                    } else if ((!(xtmp > x[x.size(0) - 1])) && (!(xtmp < x[0]))) {
                        nx = internal::b_bsearch(x, xtmp) - 1;
                        xtmp = (xtmp - x[nx]) / (x[nx + 1] - x[nx]);
                        if (xtmp == 0.0) {
                            yi[k] = y[nx];
                        } else if (xtmp == 1.0) {
                            yi[k] = y[nx + 1];
                        } else if (y[nx] == y[nx + 1]) {
                            yi[k] = y[nx];
                        } else {
                            yi[k] = (1.0 - xtmp) * y[nx] + xtmp * y[nx + 1];
                        }
                    }
                }
                exitg1 = 1;
            }
        } while (exitg1 == 0);
    }
}

//
// Arguments    : ::coder::array<real_T, 4U> &y
//                const ::coder::array<real_T, 1U> &xi
//                ::coder::array<real_T, 1U> &x
//                ::coder::array<real_T, 4U> &yi
// Return Type  : void
//
static void interp1_work(::coder::array<real_T, 4U> &y, const ::coder::array<real_T, 1U> &xi,
                         ::coder::array<real_T, 1U> &x, ::coder::array<real_T, 4U> &yi)
{
    int32_T iv[4];
    int32_T i;
    int32_T k;
    int32_T nx;
    int32_T nxi;
    int32_T nycols;
    int32_T nyrows;
    nyrows = y.size(0);
    nycols = 0;
    iv[0] = (*(int32_T(*)[4])y.size())[0];
    iv[1] = (*(int32_T(*)[4])y.size())[1];
    iv[2] = (*(int32_T(*)[4])y.size())[2];
    iv[3] = (*(int32_T(*)[4])y.size())[3];
    i = eml_ndims_varsized(iv);
    for (k = 2; k <= i; k++) {
        nycols = (nycols + 1) * y.size(k - 1) - 1;
    }
    nx = x.size(0) - 1;
    yi.set_size(xi.size(0), y.size(1), y.size(2), y.size(3));
    nxi = xi.size(0) * y.size(1) * y.size(2) * y.size(3);
    for (i = 0; i < nxi; i++) {
        yi[i] = rtNaN;
    }
    if (xi.size(0) != 0) {
        k = 0;
        int32_T exitg1;
        do {
            exitg1 = 0;
            if (k <= nx) {
                if (b_isnan(x[k])) {
                    exitg1 = 1;
                } else {
                    k++;
                }
            } else {
                real_T xtmp;
                int32_T b_j1;
                if (x[1] < x[0]) {
                    i = (nx + 1) >> 1;
                    for (b_j1 = 0; b_j1 < i; b_j1++) {
                        xtmp = x[b_j1];
                        nxi = nx - b_j1;
                        x[b_j1] = x[nxi];
                        x[nxi] = xtmp;
                    }
                    flip(y);
                }
                nxi = xi.size(0);
                for (k = 0; k < nxi; k++) {
                    if (b_isnan(xi[k])) {
                        for (b_j1 = 0; b_j1 <= nycols; b_j1++) {
                            yi[k + b_j1 * nxi] = rtNaN;
                        }
                    } else if ((!(xi[k] > x[x.size(0) - 1])) && (!(xi[k] < x[0]))) {
                        nx = internal::b_bsearch(x, xi[k]);
                        xtmp = x[nx - 1];
                        xtmp = (xi[k] - xtmp) / (x[nx] - xtmp);
                        if (xtmp == 0.0) {
                            for (b_j1 = 0; b_j1 <= nycols; b_j1++) {
                                yi[k + b_j1 * nxi] = y[(nx + b_j1 * nyrows) - 1];
                            }
                        } else if (xtmp == 1.0) {
                            for (b_j1 = 0; b_j1 <= nycols; b_j1++) {
                                yi[k + b_j1 * nxi] = y[nx + b_j1 * nyrows];
                            }
                        } else {
                            for (b_j1 = 0; b_j1 <= nycols; b_j1++) {
                                real_T b_y1;
                                real_T y2;
                                b_y1 = y[(nx + b_j1 * nyrows) - 1];
                                y2 = y[nx + b_j1 * nyrows];
                                if (b_y1 == y2) {
                                    yi[k + b_j1 * nxi] = b_y1;
                                } else {
                                    yi[k + b_j1 * nxi] = (1.0 - xtmp) * b_y1 + xtmp * y2;
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
// Arguments    : real_T a
//                real_T b
// Return Type  : real_T
//
static real_T maxabs(real_T a, real_T b)
{
    return std::fmax(b_abs(a), b_abs(b));
}

//
// Arguments    : const ::coder::array<creal_T, 2U> &a
//                real_T varargin_3
//                real_T varargin_4
//                ::coder::array<creal_T, 4U> &b
// Return Type  : void
//
static void repmat(const ::coder::array<creal_T, 2U> &a, real_T varargin_3, real_T varargin_4,
                   ::coder::array<creal_T, 4U> &b)
{
    int32_T tile_size[4];
    int32_T ibtile;
    int32_T k;
    int32_T ncols;
    int32_T nrows;
    int32_T ntilecols;
    int32_T tile_size_tmp;
    tile_size[0] = 1;
    tile_size[1] = 1;
    ibtile = static_cast<int32_T>(varargin_3);
    tile_size[2] = static_cast<int32_T>(varargin_3);
    tile_size_tmp = static_cast<int32_T>(varargin_4);
    tile_size[3] = static_cast<int32_T>(varargin_4);
    b.set_size(a.size(0), a.size(1), ibtile, tile_size_tmp);
    nrows = a.size(0);
    ncols = a.size(1);
    ntilecols = 1;
    ibtile = eml_ndims_varsized(tile_size);
    for (k = 2; k <= ibtile; k++) {
        ntilecols *= tile_size[k - 1];
    }
    for (int32_T jtilecol{0}; jtilecol < ntilecols; jtilecol++) {
        ibtile = jtilecol * (nrows * ncols) - 1;
        for (int32_T jcol{0}; jcol < ncols; jcol++) {
            int32_T ibmat;
            tile_size_tmp = jcol * nrows;
            ibmat = ibtile + tile_size_tmp;
            for (k = 0; k < nrows; k++) {
                b[(ibmat + k) + 1] = a[tile_size_tmp + k];
            }
        }
    }
}

//
// Arguments    : const ::coder::array<real_T, 4U> &a
//                real_T varargin_1
//                ::coder::array<real_T, 4U> &b
// Return Type  : void
//
static void repmat(const ::coder::array<real_T, 4U> &a, real_T varargin_1, ::coder::array<real_T, 4U> &b)
{
    b.set_size(static_cast<int32_T>(varargin_1), a.size(1), a.size(2), a.size(3));
    if ((static_cast<int32_T>(varargin_1) != 0) && (a.size(1) != 0) && (a.size(2) != 0) && (a.size(3) != 0)) {
        int32_T na;
        na = a.size(3);
        for (int32_T k{0}; k < na; k++) {
            int32_T b_na;
            b_na = a.size(2);
            for (int32_T b_k{0}; b_k < b_na; b_k++) {
                int32_T c_na;
                c_na = a.size(1);
                for (int32_T c_k{0}; c_k < c_na; c_k++) {
                    int32_T i;
                    i = static_cast<int32_T>(varargin_1) - 1;
                    for (int32_T t{0}; t <= i; t++) {
                        b[((t + b.size(0) * c_k) + b.size(0) * b.size(1) * b_k) +
                          b.size(0) * b.size(1) * b.size(2) * k] =
                            a[(c_k + a.size(1) * b_k) + a.size(1) * a.size(2) * k];
                    }
                }
            }
        }
    }
}

//
// Arguments    : const ::coder::array<real_T, 4U> &x
//                const ::coder::array<creal_T, 4U> &y
//                ::coder::array<creal_T, 4U> &output
// Return Type  : void
//
static void sameSizeBinaryOp(const ::coder::array<real_T, 4U> &x, const ::coder::array<creal_T, 4U> &y,
                             ::coder::array<creal_T, 4U> &output)
{
    int32_T loop_ub;
    output.set_size(x.size(0), x.size(1), x.size(2), x.size(3));
    loop_ub = x.size(0) * x.size(1) * x.size(2) * x.size(3);
    for (int32_T i{0}; i < loop_ub; i++) {
        output[i].re = x[i] * y[i].re;
        output[i].im = x[i] * y[i].im;
    }
}

//
// Arguments    : const ::coder::array<creal_T, 4U> &x
//                const ::coder::array<creal_T, 4U> &y
//                ::coder::array<creal_T, 4U> &output
// Return Type  : void
//
static void sameSizeBinaryOp(const ::coder::array<creal_T, 4U> &x, const ::coder::array<creal_T, 4U> &y,
                             ::coder::array<creal_T, 4U> &output)
{
    int32_T loop_ub;
    output.set_size(x.size(0), x.size(1), x.size(2), x.size(3));
    loop_ub = x.size(0) * x.size(1) * x.size(2) * x.size(3);
    for (int32_T i{0}; i < loop_ub; i++) {
        output[i].re = x[i].re * y[i].re - x[i].im * y[i].im;
        output[i].im = x[i].re * y[i].im + x[i].im * y[i].re;
    }
}

//
// Arguments    : const ::coder::array<real_T, 4U> &x
//                const ::coder::array<real_T, 4U> &y
//                ::coder::array<real_T, 4U> &output
// Return Type  : void
//
static void sameSizeBinaryOp(const ::coder::array<real_T, 4U> &x, const ::coder::array<real_T, 4U> &y,
                             ::coder::array<real_T, 4U> &output)
{
    int32_T loop_ub;
    output.set_size(x.size(0), x.size(1), x.size(2), x.size(3));
    loop_ub = x.size(0) * x.size(1) * x.size(2) * x.size(3);
    for (int32_T i{0}; i < loop_ub; i++) {
        output[i] = x[i] - y[i];
    }
}

//
// Arguments    : ::coder::array<real_T, 4U> &p
// Return Type  : void
//
static void unwrap(::coder::array<real_T, 4U> &p)
{
    array<real_T, 1U> vwork;
    int32_T iv[4];
    int32_T dim;
    int32_T i1;
    int32_T k;
    int32_T npages;
    int32_T vlen;
    int32_T vspread;
    int32_T vstride;
    dim = internal::nonSingletonDim(p) - 1;
    vlen = p.size(dim) - 1;
    vwork.set_size(p.size(dim));
    iv[0] = (*(int32_T(*)[4])p.size())[0];
    iv[1] = (*(int32_T(*)[4])p.size())[1];
    iv[2] = (*(int32_T(*)[4])p.size())[2];
    iv[3] = (*(int32_T(*)[4])p.size())[3];
    if (dim + 1 > eml_ndims_varsized(iv)) {
        vstride = p.size(0) * p.size(1) * p.size(2) * p.size(3);
    } else {
        vstride = 1;
        for (k = 0; k < dim; k++) {
            vstride *= p.size(k);
        }
    }
    vspread = (p.size(dim) - 1) * vstride;
    iv[0] = (*(int32_T(*)[4])p.size())[0];
    iv[1] = (*(int32_T(*)[4])p.size())[1];
    iv[2] = (*(int32_T(*)[4])p.size())[2];
    iv[3] = (*(int32_T(*)[4])p.size())[3];
    if (dim + 1 > eml_ndims_varsized(iv)) {
        npages = 1;
    } else {
        npages = 1;
        dim += 2;
        iv[0] = (*(int32_T(*)[4])p.size())[0];
        iv[1] = (*(int32_T(*)[4])p.size())[1];
        iv[2] = (*(int32_T(*)[4])p.size())[2];
        iv[3] = (*(int32_T(*)[4])p.size())[3];
        i1 = eml_ndims_varsized(iv);
        for (k = dim; k <= i1; k++) {
            npages *= p.size(k - 1);
        }
    }
    dim = 0;
    for (int32_T i{0}; i < npages; i++) {
        i1 = dim - 1;
        dim += vspread;
        for (int32_T j{0}; j < vstride; j++) {
            i1++;
            dim++;
            for (k = 0; k <= vlen; k++) {
                vwork[k] = p[i1 + k * vstride];
            }
            unwrap_vector(vwork);
            for (k = 0; k <= vlen; k++) {
                p[i1 + k * vstride] = vwork[k];
            }
        }
    }
}

//
// Arguments    : ::coder::array<real_T, 1U> &p
// Return Type  : void
//
static void unwrap_vector(::coder::array<real_T, 1U> &p)
{
    real_T cumsum_dp_corr;
    real_T dp_corr;
    int32_T m;
    uint32_T k;
    m = p.size(0);
    cumsum_dp_corr = 0.0;
    k = 1U;
    while ((static_cast<int32_T>(k) < m) && (!b_isfinite(p[static_cast<int32_T>(k) - 1]))) {
        k = static_cast<uint32_T>(static_cast<int32_T>(k) + 1);
    }
    if (static_cast<int32_T>(k) < p.size(0)) {
        real_T pkm1;
        pkm1 = p[static_cast<int32_T>(k) - 1];
        int32_T exitg1;
        do {
            exitg1 = 0;
            k++;
            while ((static_cast<real_T>(k) <= m) && (!b_isfinite(p[static_cast<int32_T>(k) - 1]))) {
                k++;
            }
            if (static_cast<real_T>(k) > m) {
                exitg1 = 1;
            } else {
                real_T dp_tmp;
                dp_tmp = p[static_cast<int32_T>(k) - 1];
                pkm1 = dp_tmp - pkm1;
                dp_corr = pkm1 / 6.2831853071795862;
                if (b_abs(b_rem(dp_corr)) <= 0.5) {
                    b_fix(&dp_corr);
                } else {
                    b_round(&dp_corr);
                }
                if (b_abs(pkm1) >= 3.1415926535897931) {
                    cumsum_dp_corr += dp_corr;
                }
                pkm1 = dp_tmp;
                p[static_cast<int32_T>(k) - 1] = dp_tmp - 6.2831853071795862 * cumsum_dp_corr;
            }
        } while (exitg1 == 0);
    }
}

//
// Arguments    : real_T numSTS
//                real_T CBW
//                real_T csdShift_data[]
//                int32_T csdShift_size[2]
// Return Type  : void
//
} // namespace coder
static void getCSDShift(real_T numSTS, real_T CBW, real_T csdShift_data[], int32_T csdShift_size[2])
{
    int32_T loop_ub;
    switch (static_cast<int32_T>(numSTS)) {
    case 1:
        csdShift_size[1] = 1;
        csdShift_data[0] = 0.0;
        break;
    case 2:
        csdShift_size[1] = 2;
        csdShift_data[0] = -0.0;
        csdShift_data[1] = -400.0;
        break;
    case 3:
        csdShift_size[1] = 3;
        csdShift_data[0] = 0.0;
        csdShift_data[1] = -400.0;
        csdShift_data[2] = -200.0;
        break;
    case 4:
        csdShift_size[1] = 4;
        csdShift_data[0] = 0.0;
        csdShift_data[1] = -400.0;
        csdShift_data[2] = -200.0;
        csdShift_data[3] = -600.0;
        break;
    case 5:
        csdShift_size[1] = 5;
        csdShift_data[0] = 0.0;
        csdShift_data[1] = -400.0;
        csdShift_data[2] = -200.0;
        csdShift_data[3] = -600.0;
        csdShift_data[4] = -350.0;
        break;
    case 6:
        csdShift_size[1] = 6;
        csdShift_data[0] = 0.0;
        csdShift_data[1] = -400.0;
        csdShift_data[2] = -200.0;
        csdShift_data[3] = -600.0;
        csdShift_data[4] = -350.0;
        csdShift_data[5] = -650.0;
        break;
    case 7:
        csdShift_size[1] = 7;
        csdShift_data[0] = 0.0;
        csdShift_data[1] = -400.0;
        csdShift_data[2] = -200.0;
        csdShift_data[3] = -600.0;
        csdShift_data[4] = -350.0;
        csdShift_data[5] = -650.0;
        csdShift_data[6] = -100.0;
        break;
    case 8:
        csdShift_size[1] = 8;
        csdShift_data[0] = 0.0;
        csdShift_data[1] = -400.0;
        csdShift_data[2] = -200.0;
        csdShift_data[3] = -600.0;
        csdShift_data[4] = -350.0;
        csdShift_data[5] = -650.0;
        csdShift_data[6] = -100.0;
        csdShift_data[7] = -750.0;
        break;
    default:
        csdShift_size[1] = 1;
        csdShift_data[0] = 0.0;
        break;
    }
    csdShift_size[0] = 1;
    loop_ub = csdShift_size[1] - 1;
    for (int32_T i{0}; i <= loop_ub; i++) {
        csdShift_data[i] = csdShift_data[i] * CBW * 0.001;
    }
}

//
// Arguments    : real_T packetFormat
//                real_T CBW
// Return Type  : real_T
//
static real_T getNumFFTTones(real_T packetFormat, real_T CBW)
{
    real_T nfft;
    nfft = 64.0 * (CBW / 20.0);
    //  default for NonHT, HT and VHT
    if (packetFormat >= 3.0) {
        //  HE-SU or HE-MU or EHT
        nfft *= 4.0;
    }
    return nfft;
}

//
// Arguments    : const coder::array<real_T, 4U> &mag
//                const coder::array<real_T, 4U> &phase
//                coder::array<creal_T, 4U> &csi
// Return Type  : void
//
static void magPhase2CSI(const coder::array<real_T, 4U> &mag, const coder::array<real_T, 4U> &phase,
                         coder::array<creal_T, 4U> &csi)
{
    coder::array<creal_T, 4U> r;
    int32_T loop_ub;
    r.set_size(phase.size(0), phase.size(1), phase.size(2), phase.size(3));
    loop_ub = phase.size(0) * phase.size(1) * phase.size(2) * phase.size(3);
    for (int32_T i{0}; i < loop_ub; i++) {
        r[i].re = phase[i] * 0.0;
        r[i].im = phase[i];
    }
    coder::b_exp(r);
    coder::sameSizeBinaryOp(mag, r, csi);
}

//
// Arguments    : real_T u0
//                real_T u1
// Return Type  : real_T
//
static real_T rt_atan2d_snf(real_T u0, real_T u1)
{
    real_T y;
    if (std::isnan(u0) || std::isnan(u1)) {
        y = rtNaN;
    } else if (std::isinf(u0) && std::isinf(u1)) {
        int32_T b_u0;
        int32_T b_u1;
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
        y = std::atan2(static_cast<real_T>(b_u0), static_cast<real_T>(b_u1));
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
// Arguments    : real_T u0
//                real_T u1
// Return Type  : real_T
//
static real_T rt_hypotd_snf(real_T u0, real_T u1)
{
    real_T a;
    real_T y;
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
// Arguments    : real_T u0
//                real_T u1
// Return Type  : real_T
//
static real_T rt_remd_snf(real_T u0, real_T u1)
{
    real_T y;
    if (std::isnan(u0) || std::isnan(u1) || std::isinf(u0)) {
        y = rtNaN;
    } else if (std::isinf(u1)) {
        y = u0;
    } else if ((u1 != 0.0) && (u1 != std::trunc(u1))) {
        real_T q;
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
CSIPreprocessor2::CSIPreprocessor2()
{
}

//
// Arguments    : void
// Return Type  : void
//
CSIPreprocessor2::~CSIPreprocessor2()
{
    // (no terminate code required)
}

//
// Don't use assertion for in-MATLAB use
//
// Arguments    : coder::array<creal_T, 4U> &CSI
//                const coder::array<int16_T, 1U> &subcarrierIndex_int16
//                real_T numSTS
//                real_T numESS
//                real_T numRx
//                real_T numCSI
//                real_T packetFormat
//                real_T CBW
//                boolean_T performCSDRemoval
//                coder::array<creal_T, 4U> &resultCSI
//                coder::array<real_T, 4U> &resultMag
//                coder::array<real_T, 4U> &resultPhase
//                coder::array<int16_T, 1U> &interpedIndex_int16
// Return Type  : void
//
void CSIPreprocessor2::InterpolateCSIAndRemoveCSD(coder::array<creal_T, 4U> &CSI,
                                                  const coder::array<int16_T, 1U> &subcarrierIndex_int16, real_T numSTS,
                                                  real_T, real_T numRx, real_T numCSI, real_T packetFormat, real_T CBW,
                                                  boolean_T performCSDRemoval, coder::array<creal_T, 4U> &resultCSI,
                                                  coder::array<real_T, 4U> &resultMag,
                                                  coder::array<real_T, 4U> &resultPhase,
                                                  coder::array<int16_T, 1U> &interpedIndex_int16)
{
    coder::array<creal_T, 4U> r2;
    coder::array<creal_T, 4U> r3;
    coder::array<creal_T, 2U> r1;
    coder::array<real_T, 4U> all_unwrap_interp;
    coder::array<real_T, 4U> b_all_unwrap_interp;
    coder::array<real_T, 4U> b_rawPhase;
    coder::array<real_T, 4U> r4;
    coder::array<real_T, 4U> r5;
    coder::array<real_T, 4U> r6;
    coder::array<real_T, 4U> rawPhase;
    coder::array<real_T, 2U> b_y;
    coder::array<real_T, 2U> r;
    coder::array<real_T, 1U> interpedIndex;
    coder::array<real_T, 1U> subcarrierIndex;
    coder::array<real_T, 1U> varargin_1_tmp;
    coder::array<boolean_T, 1U> b_interpedIndex;
    coder::array<boolean_T, 1U> b_varargin_1_tmp;
    creal_T b_csdInSamples_data[8];
    real_T csdInSamples_data[8];
    real_T y;
    int32_T csdInSamples_size[2];
    int32_T b_loop_ub;
    int32_T c_loop_ub;
    int32_T csdInSamples_size_idx_1;
    int32_T i;
    int32_T i1;
    int32_T i2;
    int32_T loop_ub;
    int32_T pivotIndex_data;
    int32_T tmp_size;
    //      assert(isa(CSI, 'double') && ~isreal(CSI) && all(size(CSI) >= [0 0 0 0]));
    //      assert(isa(subcarrierIndex_int16,'int16') && size(subcarrierIndex_int16, 1) > 0 &&
    //      size(subcarrierIndex_int16, 2) == 1); assert(isa(numSTS, 'double') && isscalar(numSTS)); assert(isa(numESS,
    //      'double') && isscalar(numESS)); assert(isa(numRx, 'double') && isscalar(numRx)); assert(isa(numCSI,
    //      'double') && isscalar(numCSI)); assert(isa(packetFormat, 'double') && isscalar(packetFormat)); assert(
    //      isa(CBW, 'double')&& isscalar(CBW)); assert(isa(performCSDRemoval, 'logical') &&
    //      isscalar(performCSDRemoval));
    //  main part
    subcarrierIndex.set_size(subcarrierIndex_int16.size(0));
    loop_ub = subcarrierIndex_int16.size(0);
    for (i = 0; i < loop_ub; i++) {
        subcarrierIndex[i] = subcarrierIndex_int16[i];
    }
    if (performCSDRemoval) {
        getCSDShift(numSTS, CBW, csdInSamples_data, csdInSamples_size);
        y = getNumFFTTones(packetFormat, CBW);
        csdInSamples_size_idx_1 = csdInSamples_size[1];
        loop_ub = csdInSamples_size[1];
        for (i = 0; i < loop_ub; i++) {
            b_csdInSamples_data[i].re = csdInSamples_data[i];
            b_csdInSamples_data[i].im = 0.0;
        }
        r1.set_size(subcarrierIndex.size(0), csdInSamples_size[1]);
        for (i = 0; i < csdInSamples_size_idx_1; i++) {
            loop_ub = subcarrierIndex.size(0);
            for (i1 = 0; i1 < loop_ub; i1++) {
                real_T b_re;
                real_T b_re_tmp;
                real_T im;
                real_T re;
                real_T re_tmp;
                re = subcarrierIndex[i1] * -0.0;
                im = subcarrierIndex[i1] * -6.2831853071795862;
                re_tmp = b_csdInSamples_data[i].im;
                b_re_tmp = b_csdInSamples_data[i].re;
                b_re = re * b_re_tmp - im * re_tmp;
                im = re * re_tmp + im * b_re_tmp;
                if (im == 0.0) {
                    r1[i1 + r1.size(0) * i].re = b_re / y;
                    r1[i1 + r1.size(0) * i].im = 0.0;
                } else if (b_re == 0.0) {
                    r1[i1 + r1.size(0) * i].re = 0.0;
                    r1[i1 + r1.size(0) * i].im = im / y;
                } else {
                    r1[i1 + r1.size(0) * i].re = b_re / y;
                    r1[i1 + r1.size(0) * i].im = im / y;
                }
            }
        }
        coder::b_exp(r1);
        coder::repmat(r1, numRx, numCSI, r2);
        coder::sameSizeBinaryOp(CSI, r2, r3);
        CSI.set_size(r3.size(0), r3.size(1), r3.size(2), r3.size(3));
        loop_ub = r3.size(0) * r3.size(1) * r3.size(2) * r3.size(3);
        for (i = 0; i < loop_ub; i++) {
            CSI[i] = r3[i];
        }
    }
    if (subcarrierIndex[subcarrierIndex.size(0) - 1] < subcarrierIndex[0]) {
        b_y.set_size(1, 0);
    } else if ((coder::b_isinf(subcarrierIndex[0]) || coder::b_isinf(subcarrierIndex[subcarrierIndex.size(0) - 1])) &&
               (subcarrierIndex[0] == subcarrierIndex[subcarrierIndex.size(0) - 1])) {
        b_y.set_size(1, 1);
        b_y[0] = rtNaN;
    } else {
        y = subcarrierIndex[0];
        coder::b_floor(&y);
        if (y == subcarrierIndex[0]) {
            b_y.set_size(1,
                         static_cast<int32_T>(subcarrierIndex[subcarrierIndex.size(0) - 1] - subcarrierIndex[0]) + 1);
            loop_ub = static_cast<int32_T>(subcarrierIndex[subcarrierIndex.size(0) - 1] - subcarrierIndex[0]);
            for (i = 0; i <= loop_ub; i++) {
                b_y[i] = subcarrierIndex[0] + static_cast<real_T>(i);
            }
        } else {
            coder::eml_float_colon(subcarrierIndex[0], subcarrierIndex[subcarrierIndex.size(0) - 1], b_y);
        }
    }
    coder::interp1(subcarrierIndex, subcarrierIndex, b_y, r);
    interpedIndex.set_size(r.size(1));
    loop_ub = r.size(1);
    for (i = 0; i < loop_ub; i++) {
        interpedIndex[i] = r[i];
    }
    coder::angle(CSI, rawPhase);
    //  find the closest index to DC
    coder::b_abs(subcarrierIndex, varargin_1_tmp);
    y = coder::internal::minimum(varargin_1_tmp);
    b_varargin_1_tmp.set_size(varargin_1_tmp.size(0));
    loop_ub = varargin_1_tmp.size(0);
    for (i = 0; i < loop_ub; i++) {
        b_varargin_1_tmp[i] = (varargin_1_tmp[i] == y);
    }
    coder::eml_find(b_varargin_1_tmp, (int32_T *)&csdInSamples_size_idx_1, &tmp_size);
    for (i = 0; i < tmp_size; i++) {
        pivotIndex_data = csdInSamples_size_idx_1;
    }
    //  the index of pivotIndex in interpedIndex
    r4.set_size(rawPhase.size(0), rawPhase.size(1), rawPhase.size(2), rawPhase.size(3));
    loop_ub = rawPhase.size(0) * rawPhase.size(1) * rawPhase.size(2) * rawPhase.size(3);
    for (i = 0; i < loop_ub; i++) {
        r4[i] = rawPhase[i];
    }
    coder::unwrap(r4);
    coder::interp1(subcarrierIndex, r4, interpedIndex, all_unwrap_interp);
    b_interpedIndex.set_size(interpedIndex.size(0));
    loop_ub = interpedIndex.size(0);
    for (i = 0; i < loop_ub; i++) {
        b_interpedIndex[i] = (interpedIndex[i] == subcarrierIndex[pivotIndex_data - 1]);
    }
    coder::eml_find(b_interpedIndex, (int32_T *)&csdInSamples_size_idx_1, &tmp_size);
    loop_ub = all_unwrap_interp.size(1);
    b_loop_ub = all_unwrap_interp.size(2);
    c_loop_ub = all_unwrap_interp.size(3);
    b_all_unwrap_interp.set_size(tmp_size, all_unwrap_interp.size(1), all_unwrap_interp.size(2),
                                 all_unwrap_interp.size(3));
    for (i = 0; i < c_loop_ub; i++) {
        for (i1 = 0; i1 < b_loop_ub; i1++) {
            for (i2 = 0; i2 < loop_ub; i2++) {
                for (int32_T i3{0}; i3 < tmp_size; i3++) {
                    b_all_unwrap_interp[(b_all_unwrap_interp.size(0) * i2 +
                                         b_all_unwrap_interp.size(0) * b_all_unwrap_interp.size(1) * i1) +
                                        b_all_unwrap_interp.size(0) * b_all_unwrap_interp.size(1) *
                                            b_all_unwrap_interp.size(2) * i] =
                        all_unwrap_interp[(((csdInSamples_size_idx_1 + all_unwrap_interp.size(0) * i2) +
                                            all_unwrap_interp.size(0) * all_unwrap_interp.size(1) * i1) +
                                           all_unwrap_interp.size(0) * all_unwrap_interp.size(1) *
                                               all_unwrap_interp.size(2) * i) -
                                          1];
                }
            }
        }
    }
    loop_ub = rawPhase.size(1);
    b_loop_ub = rawPhase.size(2);
    c_loop_ub = rawPhase.size(3);
    b_rawPhase.set_size(1, rawPhase.size(1), rawPhase.size(2), rawPhase.size(3));
    for (i = 0; i < c_loop_ub; i++) {
        for (i1 = 0; i1 < b_loop_ub; i1++) {
            for (i2 = 0; i2 < loop_ub; i2++) {
                b_rawPhase[(i2 + b_rawPhase.size(1) * i1) + b_rawPhase.size(1) * b_rawPhase.size(2) * i] =
                    rawPhase[(((pivotIndex_data + rawPhase.size(0) * i2) + rawPhase.size(0) * rawPhase.size(1) * i1) +
                              rawPhase.size(0) * rawPhase.size(1) * rawPhase.size(2) * i) -
                             1];
            }
        }
    }
    coder::sameSizeBinaryOp(b_all_unwrap_interp, b_rawPhase, r5);
    coder::repmat(r5, static_cast<real_T>(interpedIndex.size(0)), r6);
    coder::sameSizeBinaryOp(all_unwrap_interp, r6, resultPhase);
    coder::b_abs(CSI, r4);
    coder::interp1(subcarrierIndex, r4, interpedIndex, resultMag);
    magPhase2CSI(resultMag, resultPhase, resultCSI);
    interpedIndex_int16.set_size(interpedIndex.size(0));
    loop_ub = interpedIndex.size(0);
    for (i = 0; i < loop_ub; i++) {
        interpedIndex_int16[i] = static_cast<int16_T>(std::round(interpedIndex[i]));
    }
}

//
// File trailer for CSIPreprocessor2.cpp
//
// [EOF]
//
