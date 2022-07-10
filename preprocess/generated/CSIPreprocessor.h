
#ifndef CSIPREPROCESSOR_H
#define CSIPREPROCESSOR_H

// Include Files
#include "InterpolateCSIAndRemoveCSDAndAutoUnpermutation_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class CSIPreprocessor {
  public:
    CSIPreprocessor();
    ~CSIPreprocessor();
    void InterpolateCSIAndRemoveCSDAndAutoUnpermutation(
        coder::array<creal_T, 4U> &CSI, const coder::array<short, 1U> &subcarrierIndex_int16, double numSTS,
        double numESS, double numRx, double numCSI, double packetFormat, double CBW, boolean_T performCSDRemoval,
        boolean_T unpermuteRxChainRatio, coder::array<creal_T, 4U> &resultCSI, coder::array<double, 4U> &resultMag,
        coder::array<double, 4U> &resultPhase, coder::array<short, 1U> &interpedIndex_int16, unsigned char *perm,
        double *permCoef);
    InterpolateCSIAndRemoveCSDAndAutoUnpermutationStackData *getStackData();

  private:
    InterpolateCSIAndRemoveCSDAndAutoUnpermutationPersistentData pd_;
    InterpolateCSIAndRemoveCSDAndAutoUnpermutationStackData SD_;
};

#endif
//
// File trailer for CSIPreprocessor.h
//
// [EOF]
//
