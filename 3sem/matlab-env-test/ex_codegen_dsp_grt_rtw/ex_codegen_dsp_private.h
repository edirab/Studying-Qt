/*
 * ex_codegen_dsp_private.h
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

#ifndef RTW_HEADER_ex_codegen_dsp_private_h_
#define RTW_HEADER_ex_codegen_dsp_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"

extern void RandSrcInitState_GZ(const uint32_T seed[], uint32_T state[], int32_T
  nChans);
extern void RandSrc_GZ_D(real_T y[], const real_T mean[], int32_T meanLen, const
  real_T xstd[], int32_T xstdLen, uint32_T state[], int32_T nChans, int32_T
  nSamps);
extern void MWSPCGlmsnw_D(const real_T x[], const real_T d[], real_T mu,
  uint32_T *startIdxPtr, real_T xBuf[], real_T wBuf[], int32_T wLen, real_T
  leakFac, int32_T xLen, real_T y[], real_T eY[], real_T wY[]);

#endif                                /* RTW_HEADER_ex_codegen_dsp_private_h_ */
