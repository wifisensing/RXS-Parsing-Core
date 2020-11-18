//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: CSIPreprocessor.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 18-Nov-2020 19:05:56
//
#ifndef CSIPREPROCESSOR_H
#define CSIPREPROCESSOR_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void CSIPreprocessor(const coder::array<creal_T, 2U> &CSI, const coder::
  array<short, 1U> &subcarrierIndex_int16, coder::array<creal_T, 2U> &newCSI,
  coder::array<short, 1U> &interpedIndex_int16);
extern void CSIPreprocessor_initialize();
extern void CSIPreprocessor_terminate();

#endif

//
// File trailer for CSIPreprocessor.h
//
// [EOF]
//
