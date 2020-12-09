/*
 * ex_codegen_dsp_data.cpp
 *
 * Code generation for model "ex_codegen_dsp".
 *
 * Model version              : 7.1
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C++ source code generated on : Sun Dec  6 17:56:13 2020
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ex_codegen_dsp.h"
#include "ex_codegen_dsp_private.h"

/* Block parameters (default storage) */
P_ex_codegen_dsp_T ex_codegen_dspModelClass::ex_codegen_dsp_P = {
  /* Mask Parameter: RandomSource_MeanVal
   * Referenced by: '<Root>/Random Source'
   */
  0.0,

  /* Mask Parameter: LMSFilter_leakage
   * Referenced by: '<Root>/LMS Filter'
   */
  1.0,

  /* Mask Parameter: LMSFilter_mu
   * Referenced by: '<Root>/LMS Filter'
   */
  0.1,

  /* Mask Parameter: RandomSource_rawSeed
   * Referenced by: '<Root>/Random Source'
   */
  23341U,

  /* Computed Parameter: RandomSource_VarianceRTP
   * Referenced by: '<Root>/Random Source'
   */
  1.0
};
