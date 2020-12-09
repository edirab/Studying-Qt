/*
 * ex_codegen_dsp_types.h
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

#ifndef RTW_HEADER_ex_codegen_dsp_types_h_
#define RTW_HEADER_ex_codegen_dsp_types_h_
#include "rtwtypes.h"
#include "multiword_types.h"

/* Model Code Variants */
#ifndef struct_tag_DWn1lEK8ZsuktxpSoHCVBG
#define struct_tag_DWn1lEK8ZsuktxpSoHCVBG

struct tag_DWn1lEK8ZsuktxpSoHCVBG
{
  int32_T S0_isInitialized;
  real_T W0_states[38];
  real_T P0_InitialStates;
  real_T P1_Coefficients[39];
};

#endif                                 /*struct_tag_DWn1lEK8ZsuktxpSoHCVBG*/

#ifndef typedef_b_dsp_FIRFilter_0_ex_codegen__T
#define typedef_b_dsp_FIRFilter_0_ex_codegen__T

typedef tag_DWn1lEK8ZsuktxpSoHCVBG b_dsp_FIRFilter_0_ex_codegen__T;

#endif                               /*typedef_b_dsp_FIRFilter_0_ex_codegen__T*/

#ifndef struct_tag_orJbgB5yTZegguTijmD3LF
#define struct_tag_orJbgB5yTZegguTijmD3LF

struct tag_orJbgB5yTZegguTijmD3LF
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  b_dsp_FIRFilter_0_ex_codegen__T cSFunObject;
};

#endif                                 /*struct_tag_orJbgB5yTZegguTijmD3LF*/

#ifndef typedef_b_dspcodegen_FIRFilter_ex_cod_T
#define typedef_b_dspcodegen_FIRFilter_ex_cod_T

typedef tag_orJbgB5yTZegguTijmD3LF b_dspcodegen_FIRFilter_ex_cod_T;

#endif                               /*typedef_b_dspcodegen_FIRFilter_ex_cod_T*/

#ifndef struct_tag_PMfBDzoakfdM9QAdfx2o6D
#define struct_tag_PMfBDzoakfdM9QAdfx2o6D

struct tag_PMfBDzoakfdM9QAdfx2o6D
{
  uint32_T f1[8];
};

#endif                                 /*struct_tag_PMfBDzoakfdM9QAdfx2o6D*/

#ifndef typedef_cell_wrap_ex_codegen_dsp_T
#define typedef_cell_wrap_ex_codegen_dsp_T

typedef tag_PMfBDzoakfdM9QAdfx2o6D cell_wrap_ex_codegen_dsp_T;

#endif                                 /*typedef_cell_wrap_ex_codegen_dsp_T*/

#ifndef struct_tag_Rq6idjXGJJMHmlkXzj1uh
#define struct_tag_Rq6idjXGJJMHmlkXzj1uh

struct tag_Rq6idjXGJJMHmlkXzj1uh
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  cell_wrap_ex_codegen_dsp_T inputVarSize;
  int32_T NumChannels;
  b_dspcodegen_FIRFilter_ex_cod_T *FilterObj;
  b_dspcodegen_FIRFilter_ex_cod_T _pobj0;
};

#endif                                 /*struct_tag_Rq6idjXGJJMHmlkXzj1uh*/

#ifndef typedef_dsp_LowpassFilter_ex_codegen__T
#define typedef_dsp_LowpassFilter_ex_codegen__T

typedef tag_Rq6idjXGJJMHmlkXzj1uh dsp_LowpassFilter_ex_codegen__T;

#endif                               /*typedef_dsp_LowpassFilter_ex_codegen__T*/

/* Parameters (default storage) */
typedef struct P_ex_codegen_dsp_T_ P_ex_codegen_dsp_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_ex_codegen_dsp_T RT_MODEL_ex_codegen_dsp_T;

#endif                                 /* RTW_HEADER_ex_codegen_dsp_types_h_ */
