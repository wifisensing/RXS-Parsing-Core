
#ifndef CSIPREPROCESSOR_H
#define CSIPREPROCESSOR_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class CSIPreprocessor {
  public:
    CSIPreprocessor();
    ~CSIPreprocessor();
    void InterpolateCSIAndRemoveCSD(coder::array<creal32_T, 4U> &CSI,
                                    const coder::array<short, 1U> &subcarrierIndex_int16, double numSTS, double numRx,
                                    double numCSI, double format, double CBW, boolean_T removeCSD,
                                    coder::array<creal32_T, 4U> &resultCSI, coder::array<float, 4U> &resultMag,
                                    coder::array<float, 4U> &resultPhase, coder::array<short, 1U> &interpedIndex_int16,
                                    coder::array<float, 3U> &phaseSlope, coder::array<float, 3U> &phaseIntercept);
};

#endif
//
// File trailer for CSIPreprocessor.h
//
// [EOF]
//
