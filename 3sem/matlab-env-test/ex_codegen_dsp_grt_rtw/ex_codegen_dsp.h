/*
 * ex_codegen_dsp.h
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

#ifndef RTW_HEADER_ex_codegen_dsp_h_
#define RTW_HEADER_ex_codegen_dsp_h_
#include <cmath>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "ex_codegen_dsp_types.h"

/* Shared type includes */
#include "multiword_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  dsp_LowpassFilter_ex_codegen__T obj; /* '<Root>/Lowpass Filter' */
  real_T LMSFilter_IN_BUFFER_DWORK[32];/* '<Root>/LMS Filter' */
  real_T LMSFilter_WGT_IC_DWORK[32];   /* '<Root>/LMS Filter' */
  uint32_T RandomSource_SEED_DWORK;    /* '<Root>/Random Source' */
  uint32_T RandomSource_STATE_DWORK[2];/* '<Root>/Random Source' */
  uint32_T LMSFilter_BUFF_IDX_DWORK;   /* '<Root>/LMS Filter' */
  boolean_T objisempty;                /* '<Root>/Lowpass Filter' */
  boolean_T isInitialized;             /* '<Root>/Lowpass Filter' */
} DW_ex_codegen_dsp_T;

/* Parameters (default storage) */
struct P_ex_codegen_dsp_T_ {
  real_T RandomSource_MeanVal;         /* Mask Parameter: RandomSource_MeanVal
                                        * Referenced by: '<Root>/Random Source'
                                        */
  real_T LMSFilter_leakage;            /* Mask Parameter: LMSFilter_leakage
                                        * Referenced by: '<Root>/LMS Filter'
                                        */
  real_T LMSFilter_mu;                 /* Mask Parameter: LMSFilter_mu
                                        * Referenced by: '<Root>/LMS Filter'
                                        */
  uint32_T RandomSource_rawSeed;       /* Mask Parameter: RandomSource_rawSeed
                                        * Referenced by: '<Root>/Random Source'
                                        */
  real_T RandomSource_VarianceRTP;
                                 /* Computed Parameter: RandomSource_VarianceRTP
                                  * Referenced by: '<Root>/Random Source'
                                  */
};

/* Real-time Model Data Structure */
struct tag_RTM_ex_codegen_dsp_T {
  const char_T *errorStatus;
};

/* Class declaration for model ex_codegen_dsp */
class ex_codegen_dspModelClass {
  /* public data and function members */
 public:
  /* model initialize function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  void terminate();

  /* Constructor */
  ex_codegen_dspModelClass();

  /* Destructor */
  ~ex_codegen_dspModelClass();

  /* Real-Time Model get method */
  RT_MODEL_ex_codegen_dsp_T * getRTM();

  /* private data and function members */
 private:
  /* Tunable parameters */
  static P_ex_codegen_dsp_T ex_codegen_dsp_P;

  /* Block states */
  DW_ex_codegen_dsp_T ex_codegen_dsp_DW;

  /* Real-Time Model */
  RT_MODEL_ex_codegen_dsp_T ex_codegen_dsp_M;
};

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Signal To Workspace' : Unused code path elimination
 * Block '<Root>/Sine Wave' : Unused code path elimination
 * Block '<Root>/Sum' : Unused code path elimination
 * Block '<Root>/Sum1' : Unused code path elimination
 * Block '<Root>/Time Scope' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ex_codegen_dsp'
 */
#endif                                 /* RTW_HEADER_ex_codegen_dsp_h_ */
