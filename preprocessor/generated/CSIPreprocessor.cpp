// Include Files
#include "CSIPreprocessor.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "rt_defines.h"
#include <cfloat>
#include <cmath>
#include <cstring>

// Function Declarations
namespace coder
{
    static void eml_find(const ::coder::array<boolean_T, 1U> &x, int i_data[], int i_size[1]);
    namespace internal
    {
        static int b_bsearch(const ::coder::array<double, 1U> &x, double xi);
        static void merge(::coder::array<int, 1U> &idx, ::coder::array<double, 1U> &x, int offset, int np, int nq, ::
                          coder::array<int, 1U> &iwork, ::coder::array<double, 1U> &xwork);
        static void merge_block(::coder::array<int, 1U> &idx, ::coder::array<double, 1U> &x, int offset, int n, int
                                preSortLevel, ::coder::array<int, 1U> &iwork, ::coder::array<double, 1U> &xwork);
        static void sort(::coder::array<double, 1U> &x);
    }

    static void interp1(const ::coder::array<double, 1U> &varargin_1, const ::coder::array<double, 1U> &varargin_2,
                        const ::coder::array<double, 2U> &varargin_3, ::coder::array<double, 2U> &Vq);
    static void interp1(const ::coder::array<double, 1U> &varargin_1, const ::coder::array<double, 2U> &varargin_2,
                        const ::coder::array<double, 1U> &varargin_3, ::coder::array<double, 2U> &Vq);
}

static int div_s32(int numerator, int denominator);
static double rt_atan2d_snf(double u0, double u1);
static double rt_hypotd_snf(double u0, double u1);
static double rt_remd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : const ::coder::array<boolean_T, 1U> &x
//                int i_data[]
//                int i_size[1]
// Return Type  : void
//
namespace coder
{
    static void eml_find(const ::coder::array<boolean_T, 1U> &x, int i_data[], int i_size[1])
    {
        int idx;
        int ii;
        int k;
        boolean_T exitg1;
        k = (1 <= x.size(0));
        idx = 0;
        i_size[0] = k;
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

        if (k == 1) {
            if (idx == 0) {
                i_size[0] = 0;
            }
        } else {
            i_size[0] = (1 <= idx);
        }
    }

    //
    // Arguments    : const ::coder::array<double, 1U> &x
    //                double xi
    // Return Type  : int
    //
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
        // Arguments    : ::coder::array<int, 1U> &idx
        //                ::coder::array<double, 1U> &x
        //                int offset
        //                int np
        //                int nq
        //                ::coder::array<int, 1U> &iwork
        //                ::coder::array<double, 1U> &xwork
        // Return Type  : void
        //
        static void merge(::coder::array<int, 1U> &idx, ::coder::array<double, 1U> &x, int offset, int np, int nq, ::
                          coder::array<int, 1U> &iwork, ::coder::array<double, 1U> &xwork)
        {
            if (nq != 0) {
                int iout;
                int j;
                int n_tmp;
                int p;
                int q;
                n_tmp = np + nq;
                for (j = 0; j < n_tmp; j++) {
                    iout = offset + j;
                    iwork[j] = idx[iout];
                    xwork[j] = x[iout];
                }

                p = 0;
                q = np;
                iout = offset - 1;
                int exitg1;
                do {
                    exitg1 = 0;
                    iout++;
                    if (xwork[p] <= xwork[q]) {
                        idx[iout] = iwork[p];
                        x[iout] = xwork[p];
                        if (p + 1 < np) {
                            p++;
                        } else {
                            exitg1 = 1;
                        }
                    } else {
                        idx[iout] = iwork[q];
                        x[iout] = xwork[q];
                        if (q + 1 < n_tmp) {
                            q++;
                        } else {
                            q = iout - p;
                            for (j = p + 1; j <= np; j++) {
                                iout = q + j;
                                idx[iout] = iwork[j - 1];
                                x[iout] = xwork[j - 1];
                            }

                            exitg1 = 1;
                        }
                    }
                } while (exitg1 == 0);
            }
        }

        //
        // Arguments    : ::coder::array<int, 1U> &idx
        //                ::coder::array<double, 1U> &x
        //                int offset
        //                int n
        //                int preSortLevel
        //                ::coder::array<int, 1U> &iwork
        //                ::coder::array<double, 1U> &xwork
        // Return Type  : void
        //
        static void merge_block(::coder::array<int, 1U> &idx, ::coder::array<double, 1U> &x, int offset, int n, int
                                preSortLevel, ::coder::array<int, 1U> &iwork, ::coder::array<double, 1U> &xwork)
        {
            int bLen;
            int nPairs;
            nPairs = n >> preSortLevel;
            bLen = 1 << preSortLevel;
            while (nPairs > 1) {
                int nTail;
                int tailOffset;
                if ((nPairs & 1) != 0) {
                    nPairs--;
                    tailOffset = bLen * nPairs;
                    nTail = n - tailOffset;
                    if (nTail > bLen) {
                        merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
                    }
                }

                tailOffset = bLen << 1;
                nPairs >>= 1;
                for (nTail = 0; nTail < nPairs; nTail++) {
                    merge(idx, x, offset + nTail * tailOffset, bLen, bLen, iwork, xwork);
                }

                bLen = tailOffset;
            }

            if (n > bLen) {
                merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
            }
        }

        //
        // Arguments    : ::coder::array<double, 1U> &x
        // Return Type  : void
        //
        static void sort(::coder::array<double, 1U> &x)
        {
            array<double, 1U> b_xwork;
            array<double, 1U> vwork;
            array<double, 1U> xwork;
            array<int, 1U> b_iwork;
            array<int, 1U> iidx;
            array<int, 1U> iwork;
            double c_xwork[256];
            double x4[4];
            int c_iwork[256];
            int idx4[4];
            int dim;
            int i;
            int k;
            int vlen;
            int vstride;
            signed char perm[4];
            dim = 0;
            if (x.size(0) != 1) {
                dim = -1;
            }

            if (dim + 2 <= 1) {
                i = x.size(0);
            } else {
                i = 1;
            }

            vlen = i - 1;
            vwork.set_size(i);
            vstride = 1;
            for (k = 0; k <= dim; k++) {
                vstride *= x.size(0);
            }

            for (int j = 0; j < vstride; j++) {
                for (k = 0; k <= vlen; k++) {
                    vwork[k] = x[j + k * vstride];
                }

                iidx.set_size(vwork.size(0));
                dim = vwork.size(0);
                for (i = 0; i < dim; i++) {
                    iidx[i] = 0;
                }

                i = vwork.size(0);
                if (vwork.size(0) != 0) {
                    int i2;
                    int i3;
                    int i4;
                    x4[0] = 0.0;
                    idx4[0] = 0;
                    x4[1] = 0.0;
                    idx4[1] = 0;
                    x4[2] = 0.0;
                    idx4[2] = 0;
                    x4[3] = 0.0;
                    idx4[3] = 0;
                    dim = vwork.size(0);
                    iwork.set_size(vwork.size(0));
                    for (i3 = 0; i3 < dim; i3++) {
                        iwork[i3] = 0;
                    }

                    dim = vwork.size(0);
                    xwork.set_size(vwork.size(0));
                    for (i3 = 0; i3 < dim; i3++) {
                        xwork[i3] = 0.0;
                    }

                    dim = 0;
                    for (k = 0; k < i; k++) {
                        dim++;
                        idx4[dim - 1] = k + 1;
                        x4[dim - 1] = vwork[k];
                        if (dim == 4) {
                            double d;
                            double d1;
                            if (x4[0] <= x4[1]) {
                                dim = 1;
                                i2 = 2;
                            } else {
                                dim = 2;
                                i2 = 1;
                            }

                            if (x4[2] <= x4[3]) {
                                i3 = 3;
                                i4 = 4;
                            } else {
                                i3 = 4;
                                i4 = 3;
                            }

                            d = x4[dim - 1];
                            d1 = x4[i3 - 1];
                            if (d <= d1) {
                                d = x4[i2 - 1];
                                if (d <= d1) {
                                    perm[0] = static_cast<signed char>(dim);
                                    perm[1] = static_cast<signed char>(i2);
                                    perm[2] = static_cast<signed char>(i3);
                                    perm[3] = static_cast<signed char>(i4);
                                } else if (d <= x4[i4 - 1]) {
                                    perm[0] = static_cast<signed char>(dim);
                                    perm[1] = static_cast<signed char>(i3);
                                    perm[2] = static_cast<signed char>(i2);
                                    perm[3] = static_cast<signed char>(i4);
                                } else {
                                    perm[0] = static_cast<signed char>(dim);
                                    perm[1] = static_cast<signed char>(i3);
                                    perm[2] = static_cast<signed char>(i4);
                                    perm[3] = static_cast<signed char>(i2);
                                }
                            } else {
                                d1 = x4[i4 - 1];
                                if (d <= d1) {
                                    if (x4[i2 - 1] <= d1) {
                                        perm[0] = static_cast<signed char>(i3);
                                        perm[1] = static_cast<signed char>(dim);
                                        perm[2] = static_cast<signed char>(i2);
                                        perm[3] = static_cast<signed char>(i4);
                                    } else {
                                        perm[0] = static_cast<signed char>(i3);
                                        perm[1] = static_cast<signed char>(dim);
                                        perm[2] = static_cast<signed char>(i4);
                                        perm[3] = static_cast<signed char>(i2);
                                    }
                                } else {
                                    perm[0] = static_cast<signed char>(i3);
                                    perm[1] = static_cast<signed char>(i4);
                                    perm[2] = static_cast<signed char>(dim);
                                    perm[3] = static_cast<signed char>(i2);
                                }
                            }

                            iidx[k - 3] = idx4[perm[0] - 1];
                            iidx[k - 2] = idx4[perm[1] - 1];
                            iidx[k - 1] = idx4[perm[2] - 1];
                            iidx[k] = idx4[perm[3] - 1];
                            vwork[k - 3] = x4[perm[0] - 1];
                            vwork[k - 2] = x4[perm[1] - 1];
                            vwork[k - 1] = x4[perm[2] - 1];
                            vwork[k] = x4[perm[3] - 1];
                            dim = 0;
                        }
                    }

                    if (dim > 0) {
                        perm[1] = 0;
                        perm[2] = 0;
                        perm[3] = 0;
                        if (dim == 1) {
                            perm[0] = 1;
                        } else if (dim == 2) {
                            if (x4[0] <= x4[1]) {
                                perm[0] = 1;
                                perm[1] = 2;
                            } else {
                                perm[0] = 2;
                                perm[1] = 1;
                            }
                        } else if (x4[0] <= x4[1]) {
                            if (x4[1] <= x4[2]) {
                                perm[0] = 1;
                                perm[1] = 2;
                                perm[2] = 3;
                            } else if (x4[0] <= x4[2]) {
                                perm[0] = 1;
                                perm[1] = 3;
                                perm[2] = 2;
                            } else {
                                perm[0] = 3;
                                perm[1] = 1;
                                perm[2] = 2;
                            }
                        } else if (x4[0] <= x4[2]) {
                            perm[0] = 2;
                            perm[1] = 1;
                            perm[2] = 3;
                        } else if (x4[1] <= x4[2]) {
                            perm[0] = 2;
                            perm[1] = 3;
                            perm[2] = 1;
                        } else {
                            perm[0] = 3;
                            perm[1] = 2;
                            perm[2] = 1;
                        }

                        for (k = 0; k < dim; k++) {
                            i3 = perm[k] - 1;
                            i2 = (i - dim) + k;
                            iidx[i2] = idx4[i3];
                            vwork[i2] = x4[i3];
                        }
                    }

                    i2 = 2;
                    if (i > 1) {
                        if (i >= 256) {
                            int nBlocks;
                            nBlocks = i >> 8;
                            for (int b = 0; b < nBlocks; b++) {
                                int offset;
                                offset = (b << 8) - 1;
                                for (int b_b = 0; b_b < 6; b_b++) {
                                    int bLen;
                                    int bLen2;
                                    int nPairs;
                                    bLen = 1 << (b_b + 2);
                                    bLen2 = bLen << 1;
                                    nPairs = 256 >> (b_b + 3);
                                    for (k = 0; k < nPairs; k++) {
                                        i3 = (offset + k * bLen2) + 1;
                                        for (i2 = 0; i2 < bLen2; i2++) {
                                            dim = i3 + i2;
                                            c_iwork[i2] = iidx[dim];
                                            c_xwork[i2] = vwork[dim];
                                        }

                                        i4 = 0;
                                        i2 = bLen;
                                        dim = i3 - 1;
                                        int exitg1;
                                        do {
                                            exitg1 = 0;
                                            dim++;
                                            if (c_xwork[i4] <= c_xwork[i2]) {
                                                iidx[dim] = c_iwork[i4];
                                                vwork[dim] = c_xwork[i4];
                                                if (i4 + 1 < bLen) {
                                                    i4++;
                                                } else {
                                                    exitg1 = 1;
                                                }
                                            } else {
                                                iidx[dim] = c_iwork[i2];
                                                vwork[dim] = c_xwork[i2];
                                                if (i2 + 1 < bLen2) {
                                                    i2++;
                                                } else {
                                                    dim -= i4;
                                                    for (i2 = i4 + 1; i2 <= bLen; i2++) {
                                                        i3 = dim + i2;
                                                        iidx[i3] = c_iwork[i2 - 1];
                                                        vwork[i3] = c_xwork[i2 - 1];
                                                    }

                                                    exitg1 = 1;
                                                }
                                            }
                                        } while (exitg1 == 0);
                                    }
                                }
                            }

                            dim = nBlocks << 8;
                            i2 = i - dim;
                            if (i2 > 0) {
                                merge_block(iidx, vwork, dim, i2, 2, iwork, xwork);
                            }

                            i2 = 8;
                        }

                        dim = iwork.size(0);
                        b_iwork.set_size(iwork.size(0));
                        for (i3 = 0; i3 < dim; i3++) {
                            b_iwork[i3] = iwork[i3];
                        }

                        dim = xwork.size(0);
                        b_xwork.set_size(xwork.size(0));
                        for (i3 = 0; i3 < dim; i3++) {
                            b_xwork[i3] = xwork[i3];
                        }

                        merge_block(iidx, vwork, 0, i, i2, b_iwork, b_xwork);
                    }
                }

                for (k = 0; k <= vlen; k++) {
                    x[j + k * vstride] = vwork[k];
                }
            }
        }

        //
        // Arguments    : const ::coder::array<double, 1U> &varargin_1
        //                const ::coder::array<double, 1U> &varargin_2
        //                const ::coder::array<double, 2U> &varargin_3
        //                ::coder::array<double, 2U> &Vq
        // Return Type  : void
        //
    }

    static void interp1(const ::coder::array<double, 1U> &varargin_1, const ::coder::array<double, 1U> &varargin_2,
                        const ::coder::array<double, 2U> &varargin_3, ::coder::array<double, 2U> &Vq)
    {
        array<double, 1U> x;
        array<double, 1U> y;
        int k;
        int nx;
        int outsize_idx_1;
        y.set_size(varargin_2.size(0));
        outsize_idx_1 = varargin_2.size(0);
        for (k = 0; k < outsize_idx_1; k++) {
            y[k] = varargin_2[k];
        }

        x.set_size(varargin_1.size(0));
        outsize_idx_1 = varargin_1.size(0);
        for (k = 0; k < outsize_idx_1; k++) {
            x[k] = varargin_1[k];
        }

        nx = varargin_1.size(0) - 1;
        outsize_idx_1 = varargin_3.size(1);
        Vq.set_size(1, varargin_3.size(1));
        for (k = 0; k < outsize_idx_1; k++) {
            Vq[k] = rtNaN;
        }

        if (varargin_3.size(1) != 0) {
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
                            outsize_idx_1 = nx - n;
                            x[n] = x[outsize_idx_1];
                            x[outsize_idx_1] = xtmp;
                        }

                        if ((varargin_2.size(0) != 0) && (varargin_2.size(0) > 1)) {
                            n = varargin_2.size(0) - 1;
                            outsize_idx_1 = varargin_2.size(0) >> 1;
                            for (k = 0; k < outsize_idx_1; k++) {
                                xtmp = y[k];
                                nx = n - k;
                                y[k] = y[nx];
                                y[nx] = xtmp;
                            }
                        }
                    }

                    outsize_idx_1 = varargin_3.size(1);
                    for (k = 0; k < outsize_idx_1; k++) {
                        xtmp = varargin_3[k];
                        if (std::isnan(xtmp)) {
                            Vq[k] = rtNaN;
                        } else {
                            if ((!(xtmp > x[x.size(0) - 1])) && (!(xtmp < x[0]))) {
                                double r;
                                n = internal::b_bsearch(x, xtmp) - 1;
                                r = (xtmp - x[n]) / (x[n + 1] - x[n]);
                                if (r == 0.0) {
                                    Vq[k] = y[n];
                                } else if (r == 1.0) {
                                    Vq[k] = y[n + 1];
                                } else {
                                    xtmp = y[n + 1];
                                    if (y[n] == xtmp) {
                                        Vq[k] = y[n];
                                    } else {
                                        Vq[k] = (1.0 - r) * y[n] + r * xtmp;
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
    // Arguments    : const ::coder::array<double, 1U> &varargin_1
    //                const ::coder::array<double, 2U> &varargin_2
    //                const ::coder::array<double, 1U> &varargin_3
    //                ::coder::array<double, 2U> &Vq
    // Return Type  : void
    //
    static void interp1(const ::coder::array<double, 1U> &varargin_1, const ::coder::array<double, 2U> &varargin_2,
                        const ::coder::array<double, 1U> &varargin_3, ::coder::array<double, 2U> &Vq)
    {
        array<double, 2U> y;
        array<double, 1U> x;
        int i;
        int nd2;
        int nx;
        int nycols;
        int nyrows;
        y.set_size(varargin_2.size(0), varargin_2.size(1));
        nd2 = varargin_2.size(0) * varargin_2.size(1);
        for (i = 0; i < nd2; i++) {
            y[i] = varargin_2[i];
        }

        x.set_size(varargin_1.size(0));
        nd2 = varargin_1.size(0);
        for (i = 0; i < nd2; i++) {
            x[i] = varargin_1[i];
        }

        nyrows = varargin_2.size(0);
        nycols = varargin_2.size(1) - 1;
        nx = varargin_1.size(0) - 1;
        Vq.set_size(varargin_3.size(0), varargin_2.size(1));
        nd2 = varargin_3.size(0) * varargin_2.size(1);
        for (i = 0; i < nd2; i++) {
            Vq[i] = rtNaN;
        }

        if (varargin_3.size(0) != 0) {
            int k;
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
                    int j;
                    int n;
                    if (varargin_1[1] < varargin_1[0]) {
                        int b_j1;
                        i = (nx + 1) >> 1;
                        for (b_j1 = 0; b_j1 < i; b_j1++) {
                            xtmp = x[b_j1];
                            nd2 = nx - b_j1;
                            x[b_j1] = x[nd2];
                            x[nd2] = xtmp;
                        }

                        if ((varargin_2.size(0) != 0) && (varargin_2.size(1) != 0) && (varargin_2.size(0) > 1)) {
                            n = varargin_2.size(0) - 1;
                            nd2 = varargin_2.size(0) >> 1;
                            i = varargin_2.size(1) - 1;
                            for (j = 0; j <= i; j++) {
                                nx = j * varargin_2.size(0);
                                for (k = 0; k < nd2; k++) {
                                    int i1;
                                    b_j1 = nx + k;
                                    xtmp = y[b_j1];
                                    i1 = (nx + n) - k;
                                    y[b_j1] = y[i1];
                                    y[i1] = xtmp;
                                }
                            }
                        }
                    }

                    nd2 = varargin_3.size(0);
                    for (k = 0; k < nd2; k++) {
                        if (std::isnan(varargin_3[k])) {
                            for (j = 0; j <= nycols; j++) {
                                Vq[k + j * nd2] = rtNaN;
                            }
                        } else {
                            if ((!(varargin_3[k] > x[x.size(0) - 1])) && (!(varargin_3[k] < x[0]))) {
                                n = internal::b_bsearch(x, varargin_3[k]);
                                xtmp = x[n - 1];
                                xtmp = (varargin_3[k] - xtmp) / (x[n] - xtmp);
                                if (xtmp == 0.0) {
                                    for (j = 0; j <= nycols; j++) {
                                        Vq[k + j * nd2] = y[(n + j * nyrows) - 1];
                                    }
                                } else if (xtmp == 1.0) {
                                    for (j = 0; j <= nycols; j++) {
                                        Vq[k + j * nd2] = y[n + j * nyrows];
                                    }
                                } else {
                                    for (j = 0; j <= nycols; j++) {
                                        double b_y1;
                                        double y2;
                                        nx = n + j * nyrows;
                                        b_y1 = y[nx - 1];
                                        y2 = y[nx];
                                        if (b_y1 == y2) {
                                            Vq[k + j * nd2] = b_y1;
                                        } else {
                                            Vq[k + j * nd2] = (1.0 - xtmp) * b_y1 + xtmp * y2;
                                        }
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
// function [resultCSI, resultMag, resultPhase, interpedIndex_int16] = CSIPreprocessor(CSI, subcarrierIndex_int16)
// Arguments    : const coder::array<creal_T, 2U> &CSI
//                const coder::array<short, 1U> &subcarrierIndex_int16
//                coder::array<creal_T, 2U> &resultCSI
//                coder::array<double, 2U> &resultMag
//                coder::array<double, 2U> &resultPhase
//                coder::array<short, 1U> &interpedIndex_int16
// Return Type  : void
//
void CSIPreprocessor(const coder::array<creal_T, 2U> &CSI, const coder::array<short, 1U> &subcarrierIndex_int16, coder::
                     array<creal_T, 2U> &resultCSI, coder::array<double, 2U> &resultMag, coder::array<double, 2U>
                     &resultPhase, coder::array<short, 1U> &interpedIndex_int16)
{
    coder::array<creal_T, 2U> newCSI;
    coder::array<double, 2U> all_unwrap_interp;
    coder::array<double, 2U> newPhase;
    coder::array<double, 2U> rawPhase;
    coder::array<double, 1U> interpedIndex;
    coder::array<double, 1U> subcarrierIndex;
    coder::array<double, 1U> vwork;
    coder::array<boolean_T, 1U> b_vwork;
    coder::array<boolean_T, 1U> interpedIndex_data;
    double apnd;
    double cdiff;
    double cumsum_dp_corr;
    double ndbl;
    int pivotIndex_data[1];
    int tmp_data[1];
    int tmp_size[1];
    unsigned int b_k;
    unsigned int dataSize_idx_1;
    int i;
    int i1;
    int i2;
    int k;
    int nm1d2;
    int npages;
    int sz_idx_1;
    int vlen;
    int vspread;
    int vstride;
    boolean_T b_interpedIndex_data[1];
    boolean_T exitg1;

    // 'CSIPreprocessor:3' subcarrierIndex = sort(double(subcarrierIndex_int16));
    subcarrierIndex.set_size(subcarrierIndex_int16.size(0));
    i2 = subcarrierIndex_int16.size(0);
    for (i = 0; i < i2; i++) {
        subcarrierIndex[i] = subcarrierIndex_int16[i];
    }

    coder::internal::sort(subcarrierIndex);

    // 'CSIPreprocessor:4' interpedIndex = interp1(subcarrierIndex, subcarrierIndex, subcarrierIndex(1) : subcarrierIndex(end))'; 
    if (subcarrierIndex[subcarrierIndex.size(0) - 1] < subcarrierIndex[0]) {
        newPhase.set_size(1, 0);
    } else if (std::floor(subcarrierIndex[0]) == subcarrierIndex[0]) {
        newPhase.set_size(1, (static_cast<int>(std::floor(subcarrierIndex[subcarrierIndex.size(0) - 1] -
                             subcarrierIndex[0])) + 1));
        i2 = static_cast<int>(std::floor(subcarrierIndex[subcarrierIndex.size(0) - 1] - subcarrierIndex[0]));
        for (i = 0; i <= i2; i++) {
            newPhase[i] = subcarrierIndex[0] + static_cast<double>(i);
        }
    } else {
        ndbl = std::floor((subcarrierIndex[subcarrierIndex.size(0) - 1] - subcarrierIndex[0]) + 0.5);
        apnd = subcarrierIndex[0] + ndbl;
        cdiff = apnd - subcarrierIndex[subcarrierIndex.size(0) - 1];
        if (std::abs(cdiff) < 4.4408920985006262E-16 * std::fmax(std::abs(subcarrierIndex[0]), std::abs
                (subcarrierIndex[subcarrierIndex.size(0) - 1]))) {
            ndbl++;
            apnd = subcarrierIndex[subcarrierIndex.size(0) - 1];
        } else if (cdiff > 0.0) {
            apnd = subcarrierIndex[0] + (ndbl - 1.0);
        } else {
            ndbl++;
        }

        if (ndbl >= 0.0) {
            i2 = static_cast<int>(ndbl);
        } else {
            i2 = 0;
        }

        newPhase.set_size(1, i2);
        if (i2 > 0) {
            newPhase[0] = subcarrierIndex[0];
            if (i2 > 1) {
                newPhase[i2 - 1] = apnd;
                nm1d2 = (i2 - 1) / 2;
                for (k = 0; k <= nm1d2 - 2; k++) {
                    newPhase[k + 1] = subcarrierIndex[0] + (static_cast<double>(k) + 1.0);
                    newPhase[(i2 - k) - 2] = apnd - (static_cast<double>(k) + 1.0);
                }

                if (nm1d2 << 1 == i2 - 1) {
                    newPhase[nm1d2] = (subcarrierIndex[0] + apnd) / 2.0;
                } else {
                    newPhase[nm1d2] = subcarrierIndex[0] + static_cast<double>(nm1d2);
                    newPhase[nm1d2 + 1] = apnd - static_cast<double>(nm1d2);
                }
            }
        }
    }

    coder::interp1(subcarrierIndex, subcarrierIndex, newPhase, rawPhase);
    interpedIndex.set_size(rawPhase.size(1));
    i2 = rawPhase.size(1);
    for (i = 0; i < i2; i++) {
        interpedIndex[i] = rawPhase[i];
    }

    // 'CSIPreprocessor:5' reshapedCSI = reshape(CSI, numel(subcarrierIndex), []);
    if (subcarrierIndex.size(0) > 0) {
        sz_idx_1 = div_s32(CSI.size(0) * CSI.size(1), subcarrierIndex.size(0));
    } else {
        sz_idx_1 = 0;
    }

    // 'CSIPreprocessor:7' rawPhase = angle(reshapedCSI);
    nm1d2 = subcarrierIndex.size(0) * sz_idx_1;
    rawPhase.set_size(subcarrierIndex.size(0), sz_idx_1);
    for (k = 0; k < nm1d2; k++) {
        rawPhase[k] = rt_atan2d_snf(CSI[k].im, CSI[k].re);
    }

    //  find the closest index to DC
    // 'CSIPreprocessor:10' pivotIndex = find(abs(subcarrierIndex) == min(abs(subcarrierIndex - 0)), 1, 'first');
    nm1d2 = subcarrierIndex.size(0);
    vwork.set_size(subcarrierIndex.size(0));
    for (k = 0; k < nm1d2; k++) {
        vwork[k] = std::abs(subcarrierIndex[k]);
    }

    i2 = vwork.size(0);
    if (vwork.size(0) <= 2) {
        if (vwork.size(0) == 1) {
            ndbl = vwork[0];
        } else if ((vwork[0] > vwork[1]) || (std::isnan(vwork[0]) && (!std::isnan(vwork[1])))) {
            ndbl = vwork[1];
        } else {
            ndbl = vwork[0];
        }
    } else {
        if (!std::isnan(vwork[0])) {
            nm1d2 = 1;
        } else {
            nm1d2 = 0;
            k = 2;
            exitg1 = false;
            while ((!exitg1) && (k <= vwork.size(0))) {
                if (!std::isnan(vwork[k - 1])) {
                    nm1d2 = k;
                    exitg1 = true;
                } else {
                    k++;
                }
            }
        }

        if (nm1d2 == 0) {
            ndbl = vwork[0];
        } else {
            ndbl = vwork[nm1d2 - 1];
            i = nm1d2 + 1;
            for (k = i; k <= i2; k++) {
                apnd = vwork[k - 1];
                if (ndbl > apnd) {
                    ndbl = apnd;
                }
            }
        }
    }

    nm1d2 = subcarrierIndex.size(0);
    vwork.set_size(subcarrierIndex.size(0));
    for (k = 0; k < nm1d2; k++) {
        vwork[k] = std::abs(subcarrierIndex[k]);
    }

    b_vwork.set_size(vwork.size(0));
    i2 = vwork.size(0);
    for (i = 0; i < i2; i++) {
        b_vwork[i] = (vwork[i] == ndbl);
    }

    coder::eml_find(b_vwork, tmp_data, tmp_size);
    i2 = tmp_size[0];
    if (0 <= i2 - 1) {
        std::memcpy(&pivotIndex_data[0], &tmp_data[0], i2 * sizeof(int));
    }

    //  the index of pivotIndex in interpedIndex
    // 'CSIPreprocessor:12' pivotInterpedIndex = find(interpedIndex == subcarrierIndex(pivotIndex), 1, 'first');
    // 'CSIPreprocessor:13' pivotPhase = rawPhase(pivotIndex, :);
    // 'CSIPreprocessor:15' all_unwrap_interp = interp1(subcarrierIndex, unwrap(rawPhase), interpedIndex);
    newPhase.set_size(rawPhase.size(0), rawPhase.size(1));
    i2 = rawPhase.size(0) * rawPhase.size(1);
    for (i = 0; i < i2; i++) {
        newPhase[i] = rawPhase[i];
    }

    nm1d2 = 1;
    if (rawPhase.size(0) != 1) {
        nm1d2 = 0;
    }

    vlen = rawPhase.size(nm1d2) - 1;
    vwork.set_size(rawPhase.size(nm1d2));
    vstride = 1;
    for (k = 0; k < nm1d2; k++) {
        vstride *= rawPhase.size(0);
    }

    vspread = (rawPhase.size(nm1d2) - 1) * vstride;
    npages = 1;
    i = nm1d2 + 2;
    for (k = i; k < 3; k++) {
        npages *= rawPhase.size(1);
    }

    i2 = 0;
    for (i = 0; i < npages; i++) {
        i1 = i2 - 1;
        i2 += vspread;
        for (int j = 0; j < vstride; j++) {
            i1++;
            i2++;
            nm1d2 = i1;
            for (k = 0; k <= vlen; k++) {
                vwork[k] = newPhase[nm1d2];
                nm1d2 += vstride;
            }

            nm1d2 = vwork.size(0);
            cumsum_dp_corr = 0.0;
            b_k = 1U;
            exitg1 = false;
            while ((!exitg1) && (static_cast<int>(b_k) < nm1d2)) {
                apnd = vwork[static_cast<int>(b_k) - 1];
                if (std::isinf(apnd) || std::isnan(apnd)) {
                    b_k = static_cast<unsigned int>(static_cast<int>(b_k) + 1);
                } else {
                    exitg1 = true;
                }
            }

            if (static_cast<int>(b_k) < vwork.size(0)) {
                ndbl = vwork[static_cast<int>(b_k) - 1];
                int exitg2;
                do {
                    exitg2 = 0;
                    b_k++;
                    exitg1 = false;
                    while ((!exitg1) && (b_k <= static_cast<unsigned int>(nm1d2))) {
                        apnd = vwork[static_cast<int>(b_k) - 1];
                        if (std::isinf(apnd) || std::isnan(apnd)) {
                            b_k++;
                        } else {
                            exitg1 = true;
                        }
                    }

                    if (b_k > static_cast<unsigned int>(nm1d2)) {
                        exitg2 = 1;
                    } else {
                        cdiff = vwork[static_cast<int>(b_k) - 1];
                        ndbl = cdiff - ndbl;
                        apnd = ndbl / 6.2831853071795862;
                        if (std::abs(rt_remd_snf(apnd, 1.0)) <= 0.5) {
                            apnd = std::trunc(apnd);
                        } else {
                            apnd = std::round(apnd);
                        }

                        if (std::abs(ndbl) >= 3.1415926535897931) {
                            cumsum_dp_corr += apnd;
                        }

                        ndbl = cdiff;
                        vwork[static_cast<int>(b_k) - 1] = cdiff - 6.2831853071795862 * cumsum_dp_corr;
                    }
                } while (exitg2 == 0);
            }

            nm1d2 = i1;
            for (k = 0; k <= vlen; k++) {
                newPhase[nm1d2] = vwork[k];
                nm1d2 += vstride;
            }
        }
    }

    coder::interp1(subcarrierIndex, newPhase, interpedIndex, all_unwrap_interp);

    // 'CSIPreprocessor:16' newPhase = all_unwrap_interp - (all_unwrap_interp(pivotInterpedIndex,:) - pivotPhase);
    nm1d2 = interpedIndex.size(0);
    i2 = interpedIndex.size(0);
    for (i = 0; i < i2; i++) {
        b_interpedIndex_data[i] = (interpedIndex[i] == subcarrierIndex[pivotIndex_data[i] - 1]);
    }

    interpedIndex_data.set((&b_interpedIndex_data[0]), nm1d2);
    coder::eml_find(interpedIndex_data, tmp_data, tmp_size);
    newPhase.set_size(all_unwrap_interp.size(0), all_unwrap_interp.size(1));
    i2 = all_unwrap_interp.size(1);
    for (i = 0; i < i2; i++) {
        nm1d2 = all_unwrap_interp.size(0);
        for (i1 = 0; i1 < nm1d2; i1++) {
            newPhase[i1 + newPhase.size(0) * i] = all_unwrap_interp[i1 + all_unwrap_interp.size(0) * i] -
                (all_unwrap_interp[(tmp_data[i1] + all_unwrap_interp.size(0) * i) - 1] - rawPhase[(pivotIndex_data[i1] +
                  rawPhase.size(0) * i) - 1]);
        }
    }

    // 'CSIPreprocessor:17' newMag = interp1(subcarrierIndex, abs(reshapedCSI), interpedIndex);
    nm1d2 = subcarrierIndex.size(0) * sz_idx_1;
    rawPhase.set_size(subcarrierIndex.size(0), sz_idx_1);
    for (k = 0; k < nm1d2; k++) {
        rawPhase[k] = rt_hypotd_snf(CSI[k].re, CSI[k].im);
    }

    coder::interp1(subcarrierIndex, rawPhase, interpedIndex, all_unwrap_interp);

    // 'CSIPreprocessor:18' newCSI = magPhase2CSI(newMag, newPhase);
    // 'magPhase2CSI:3' csi = mag .* exp(1i * phaseArray);
    newCSI.set_size(newPhase.size(0), newPhase.size(1));
    i2 = newPhase.size(0) * newPhase.size(1);
    for (i = 0; i < i2; i++) {
        newCSI[i].re = newPhase[i] * 0.0;
        newCSI[i].im = newPhase[i];
    }

    nm1d2 = newCSI.size(0) * newCSI.size(1);
    for (k = 0; k < nm1d2; k++) {
        if (newCSI[k].im == 0.0) {
            newCSI[k].re = std::exp(newCSI[k].re);
            newCSI[k].im = 0.0;
        } else if (std::isinf(newCSI[k].im) && std::isinf(newCSI[k].re) && (newCSI[k].re < 0.0)) {
            newCSI[k].re = 0.0;
            newCSI[k].im = 0.0;
        } else {
            ndbl = std::exp(newCSI[k].re / 2.0);
            apnd = newCSI[k].im;
            newCSI[k].re = ndbl * (ndbl * std::cos(newCSI[k].im));
            newCSI[k].im = ndbl * (ndbl * std::sin(apnd));
        }
    }

    i2 = all_unwrap_interp.size(0) * all_unwrap_interp.size(1);
    newCSI.set_size(all_unwrap_interp.size(0), all_unwrap_interp.size(1));
    for (i = 0; i < i2; i++) {
        newCSI[i].re = all_unwrap_interp[i] * newCSI[i].re;
        newCSI[i].im = all_unwrap_interp[i] * newCSI[i].im;
    }

    // 'CSIPreprocessor:20' dataSize = size(CSI);
    b_k = static_cast<unsigned int>(CSI.size(0));
    dataSize_idx_1 = static_cast<unsigned int>(CSI.size(1));

    // 'CSIPreprocessor:21' if numel(dataSize) > 1 && dataSize(1) ~= numel(CSI) && dataSize(2) ~= numel(CSI)
    if ((CSI.size(0) != CSI.size(0) * CSI.size(1)) && (CSI.size(1) != CSI.size(0) * CSI.size(1))) {
        // 'CSIPreprocessor:22' dataSize(1) = size(newCSI, 1);
        b_k = static_cast<unsigned int>(newCSI.size(0));
    } else {
        // 'CSIPreprocessor:23' else
        // 'CSIPreprocessor:24' if dataSize(1) == numel(CSI)
        if (CSI.size(0) == CSI.size(0) * CSI.size(1)) {
            // 'CSIPreprocessor:25' dataSize(1) = numel(newCSI);
            b_k = static_cast<unsigned int>(newCSI.size(0) * newCSI.size(1));
        } else {
            if (CSI.size(1) == CSI.size(0) * CSI.size(1)) {
                // 'CSIPreprocessor:26' elseif dataSize(2) == numel(CSI)
                // 'CSIPreprocessor:27' dataSize(2) = numel(newCSI);
                dataSize_idx_1 = static_cast<unsigned int>(newCSI.size(0) * newCSI.size(1));
            }
        }
    }

    // 'CSIPreprocessor:31' resultCSI = reshape(newCSI, dataSize);
    resultCSI.set_size((static_cast<int>(b_k)), (static_cast<int>(dataSize_idx_1)));
    nm1d2 = static_cast<int>(b_k) * static_cast<int>(dataSize_idx_1);

    // 'CSIPreprocessor:32' resultMag = reshape(newMag, dataSize);
    resultMag.set_size((static_cast<int>(b_k)), (static_cast<int>(dataSize_idx_1)));

    // 'CSIPreprocessor:33' resultPhase = reshape(newPhase, dataSize);
    resultPhase.set_size((static_cast<int>(b_k)), (static_cast<int>(dataSize_idx_1)));
    for (i = 0; i < nm1d2; i++) {
        resultCSI[i] = newCSI[i];
        resultMag[i] = all_unwrap_interp[i];
        resultPhase[i] = newPhase[i];
    }

    // 'CSIPreprocessor:34' interpedIndex_int16 = int16(interpedIndex);
    interpedIndex_int16.set_size(interpedIndex.size(0));
    i2 = interpedIndex.size(0);
    for (i = 0; i < i2; i++) {
        short b_i;
        apnd = std::round(interpedIndex[i]);
        if (apnd < 32768.0) {
            if (apnd >= -32768.0) {
                b_i = static_cast<short>(apnd);
            } else {
                b_i = MIN_int16_T;
            }
        } else if (apnd >= 32768.0) {
            b_i = MAX_int16_T;
        } else {
            b_i = 0;
        }

        interpedIndex_int16[i] = b_i;
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
