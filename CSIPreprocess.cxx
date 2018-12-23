//
// Created by 蒋志平 on 2018/11/10.
//

#include "CSIPreprocess.h"


static constexpr auto M_2PI = 2 * M_PI;

static int CSI_GROUPING_HT20[] = {-28, -26, -24, -22, -20, -18, -16, -14, -12, -10,  -8,  -6,  -4, -2, -1, 0, 1, 3,  5,  7,  9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 28};
static int CSI_GROUPING_HT40[] = {-58, -54, -50, -46, -42, -38, -34, -30, -26, -22, -18, -14, -10, -6, -2, 0, 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58};

static std::deque<double> linspace(double start, double end, double num_lin_spaced) {
    std::deque<double> interpValues;
    num_lin_spaced = std::floor(num_lin_spaced);
    double step = (end - start) / num_lin_spaced;

    for(auto i = 0 ; i <= num_lin_spaced ; i++) {
        interpValues.emplace_back(start + step * i);
    }

    return interpValues;
}

void groupingCSIInterpolation(std::deque<double> & origin, bool HT40) {
    std::deque<double> interp(117);
    int *groupingVector = HT40 ? CSI_GROUPING_HT40 : CSI_GROUPING_HT20;
    for (size_t i = 0 ; i < origin.size() ; i ++) {
        auto pos = groupingVector[i] - groupingVector[0];
        interp[pos] = origin[i];

        if (i > 0) {
            auto lastPos = groupingVector[i-1] - groupingVector[0];
            auto n_spaced = groupingVector[i] - groupingVector[i -1];
            auto linspacedValues = linspace(interp[lastPos], interp[pos], n_spaced);
            std::copy(linspacedValues.begin(), linspacedValues.end(), interp.begin() + lastPos);
        }
    }

    origin.clear();
    origin.resize(groupingVector[30] - groupingVector[0] + 1);
    std::copy(interp.begin(), interp.begin() + (groupingVector[30] - groupingVector[0] + 1), origin.begin());
}

static auto phase_unwrap_two = [](double cur, double next, double circularEqualValue) -> double
{
    auto diff = fmod(next - cur, circularEqualValue);
    auto diff_abs = fabs(diff);
    if (circularEqualValue/2.0 >= diff_abs)
        return cur + diff;
    else {
        diff = diff > 0 ? -(circularEqualValue - diff_abs) : (circularEqualValue - diff_abs);
        return cur + diff;
    }
};

void unwrapPhase_part(std::deque<double> & phases, int unwrapHead, int unwrapEnd, bool forward, double circularEqualValue) {
    auto step = forward ? 1 : -1;
    for(auto i = unwrapHead ; i != unwrapEnd ; i+=step) {
        phases[i+step] = phase_unwrap_two(phases[i], phases[i + step], circularEqualValue);
    }

}

void unwrapPhase(std::deque<double> & originalPhase) {
    auto dcStart = originalPhase.size() / 2; // 28 for ar9300, 15 for 5300
    auto dcEnd = dcStart + 1;

    // We ignore the 5300 @ 40MHz case, becuase the unwraped phase still needs to be interpolated.
    // Thus, we don't need to interpolate the exact -1-th, 0-th, 1-th subcarrier.
    if (originalPhase.size() == 114) { // ath9k 40MHz case
        dcStart = 57;
        dcEnd = 60;
    }

    unwrapPhase_part(originalPhase, dcStart - 1, 0, false, M_PI);
    unwrapPhase_part(originalPhase, dcStart - 1, dcStart, true, M_PI);

    if (fabs(originalPhase[dcStart] - originalPhase[dcStart-1]) > M_PI_4) { // Ath9k 40MHz case
        auto unwrapped_dcStart = phase_unwrap_two(originalPhase[dcStart-1], originalPhase[dcStart], M_PI_2);
        auto diff_unwrapped = originalPhase[dcStart] - unwrapped_dcStart;
        for(auto i = dcStart; i < originalPhase.size() ; i++) {
            originalPhase[i] = originalPhase[i] - diff_unwrapped;
        }
    }

    unwrapPhase_part(originalPhase, dcStart, originalPhase.size() - 1, true, M_PI);

    auto lower = originalPhase[dcStart - 1];
    auto higher = originalPhase[dcStart];
    auto gap = higher - lower;
    for(auto i = dcStart ; i < dcEnd ; i++) {
        auto newValue = lower + gap * (double)(i - dcStart + 1) / (double)(dcEnd - dcStart + 1);
        originalPhase.emplace(originalPhase.begin() + i, newValue);
    }

}

void interpMag(std::deque<double> & originalMag) {
    auto dcStart = originalMag.size() / 2; // 28 for ar9300, 15 for 5300
    auto dcEnd = dcStart + 1;

    // We ignore the 5300 @ 40MHz case, becuase the unwraped phase still needs to be interpolated.
    // Thus, we don't need to interpolate the exact -1-th, 0-th, 1-th subcarrier.
    if (originalMag.size() == 114) { // ath9k 40MHz case
        dcStart = 57;
        dcEnd = 60;
    }

    auto lower = originalMag[dcStart - 1];
    auto higher = originalMag[dcStart];
    auto gap = higher - lower;
    for(auto i = dcStart ; i < dcEnd ; i++) {
        auto newValue = lower + gap * (double)(i - dcStart + 1) / (double)(dcEnd - dcStart + 1);
        originalMag.emplace(originalMag.begin() + i, newValue);
    }
}

inline double mirrorPhase(double phase) {
    if (phase >= 0)
        return std::fmod(phase - M_2PI, M_2PI);
    else return std::fmod(phase + M_2PI, M_2PI);
}

inline bool isMonotonic(const double & p1, const double & p2, const double & p3) {
    if(p1 * p2 * p3 != 0 && (p1 - p2) * (p2 - p3) > 0)
        return true;

    if (p1 >= p2 && p2 >= p3)
        return true;

    if (p1 <= p2 && p2 <= p3)
        return true;

    return false;
}

inline double matlabMod(double value , double base) {
    auto result = std::fmod(value + base * 2.0, base);
    return result;
}

inline void unwrapMonotonic2(double phases[3]) {
    auto pd21 = phases[1] - phases[0];
    auto pd31 = phases[2] - phases[0];
    auto newpd21 = phases[0] + matlabMod(pd21, M_PI);
    auto newpd31 = phases[0] + matlabMod(pd31, M_PI);
    phases[1] = newpd21;
    phases[2] = newpd31;
}

void unwrapMonotonic(double phases[3]) {
    if (isMonotonic(phases[0], phases[1], phases[2]))
        return;

    double bestp1 = 0, bestp2 = 0, bestp3 = 0, score = 999;
    auto p1o = phases[0];
    auto p2o = phases[1];
    auto p3o = phases[2];
    auto p2m = mirrorPhase(p2o);
    auto p3m = mirrorPhase(p3o);

    if (isMonotonic(p1o, p2o, p3m)) {
        auto curScore = std::abs(p1o - p3m);
        if (score > curScore) {
            bestp1 = p1o;
            bestp2 = p2o;
            bestp3 = p3m;
            score = curScore;
        }
    }

    if (isMonotonic(p1o, p2m, p3o)) {
        auto curScore = std::abs(p1o - p3o);
        if (score > curScore) {
            bestp1 = p1o;
            bestp2 = p2m;
            bestp3 = p3o;
            score = curScore;
        }
    }

    if (isMonotonic(p1o, p2m, p3m)) {
        auto curScore = std::abs(p1o - p3m);
        if (score > std::abs(p1o - p3m)) {
            bestp1 = p1o;
            bestp2 = p2m;
            bestp3 = p3m;
            score = curScore;
        }
    }

    phases[0] = bestp1;
    phases[1] = bestp2;
    phases[2] = bestp3;
}

void phaseUnwrapBetweenAntennas(std::deque<std::deque<double>> & phases, int ntx, int nrx) {
    if (nrx < 3) // strange phase combination happends only when nrx is larger than 3
        return;

    for(auto i = 0 ; i < ntx; i++) {
        auto segHead = nrx * i;
        auto segEnd =  nrx * (i+1);
        double dc_phases_origin[3];
        std::deque<double> dc_phases_unwraped;
        // do unwrap between antennas.
        auto dcPos = phases[segHead].size() / 2;
        for(auto j = segHead ; j < segEnd; j ++) {
            dc_phases_origin[j - segHead] = (phases[j][dcPos]);
            dc_phases_unwraped.emplace_back(dc_phases_origin[j - segHead]);
        }
        unwrapPhase_part(dc_phases_unwraped, 0, dc_phases_unwraped.size() - 1, true, M_2PI);

        auto gap2 = dc_phases_unwraped[1] - dc_phases_origin[1];
        for(auto & phase: phases[segEnd-2])
            phase += gap2;

        auto gap3 = dc_phases_unwraped[2] - dc_phases_origin[2];
        for(auto & phase: phases[segEnd-1])
            phase += gap3;
    }
}

int phaseUnwrapAroundDC(std::complex<double> csi_matrix[], double magArray[], double phaseArray[], int ntx, int nrx, int num_tones, int bw) {
    std::deque<std::deque<double>> mags(nrx * ntx);
    std::deque<std::deque<double>> phases(nrx * ntx);

    auto currentPos = 0;
    for (auto i = 0 ; i < nrx * ntx ; i ++) {
        for(auto j = 0 ; j < num_tones; j++, currentPos++) {
            mags[i].emplace_back(std::abs(csi_matrix[currentPos]));
            phases[i].emplace_back(std::arg(csi_matrix[currentPos]));
        }
        interpMag(mags[i]);
        unwrapPhase(phases[i]);

        if (mags[i].size() == 31) {
            groupingCSIInterpolation(mags[i], bw);
            groupingCSIInterpolation(phases[i], bw);
        }
    }

    phaseUnwrapBetweenAntennas(phases, ntx, nrx);

    currentPos = 0;
    for (auto i = 0 ; i < nrx * ntx ; i ++) {
        assert(mags[i].size() == phases[i].size());
        for (auto j = 0; j < (int) mags[i].size(); j++, currentPos++) {
            phaseArray[currentPos] = phases[i][j];
            magArray[currentPos]   = mags[i][j];
            
            auto x = std::cos(phases[i][j]) * mags[i][j];
            auto y = std::sin(phases[i][j]) * mags[i][j];
            csi_matrix[currentPos] = std::complex<double>(x, y);
        }
    }

    return mags[0].size();
}
