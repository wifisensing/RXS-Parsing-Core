
#ifndef INTERPOLATECSIANDREMOVECSDANDAUTOUNPERMUTATION_TYPES_H
#define INTERPOLATECSIANDREMOVECSDANDAUTOUNPERMUTATION_TYPES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"

// Type Definitions
struct InterpolateCSIAndRemoveCSDAndAutoUnpermutationPersistentData {
    coder::array<double, 2U> lastMag;
    boolean_T lastMag_not_empty;
};

struct InterpolateCSIAndRemoveCSDAndAutoUnpermutationStackData {
    InterpolateCSIAndRemoveCSDAndAutoUnpermutationPersistentData *pd;
};

#endif
//
// File trailer for InterpolateCSIAndRemoveCSDAndAutoUnpermutation_types.h
//
// [EOF]
//
