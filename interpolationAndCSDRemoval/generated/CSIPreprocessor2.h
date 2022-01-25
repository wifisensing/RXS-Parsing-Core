
#ifndef CSIPREPROCESSOR2_H
#define CSIPREPROCESSOR2_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class CSIPreprocessor2 {
  public:
    CSIPreprocessor2();
    ~CSIPreprocessor2();
    void InterpolateCSIAndRemoveCSD(coder::array<creal_T, 4U> &CSI,
                                    const coder::array<int16_T, 1U> &subcarrierIndex_int16, real_T numSTS,
                                    real_T numESS, real_T numRx, real_T numCSI, real_T packetFormat, real_T CBW,
                                    boolean_T performCSDRemoval, coder::array<creal_T, 4U> &resultCSI,
                                    coder::array<real_T, 4U> &resultMag, coder::array<real_T, 4U> &resultPhase,
                                    coder::array<int16_T, 1U> &interpedIndex_int16);
};

#endif
//
// File trailer for CSIPreprocessor2.h
//
// [EOF]
//
