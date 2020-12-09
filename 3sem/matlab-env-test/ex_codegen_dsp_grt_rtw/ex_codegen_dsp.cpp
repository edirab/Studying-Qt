/*
 * ex_codegen_dsp.cpp
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

#define NUMST 100

void RandSrcInitState_GZ(const uint32_T seed[], uint32_T state[], int32_T nChans)
{
  int32_T i;
  int32_T tmp;

  /* InitializeConditions for S-Function (sdsprandsrc2): '<Root>/Random Source' */
  /* RandSrcInitState_GZ */
  for (i = 0; i < nChans; i++) {
    tmp = i << 1;
    state[tmp] = 362436069U;
    state[tmp + 1] = seed[i] == 0U ? 521288629U : seed[i];
  }

  /* End of InitializeConditions for S-Function (sdsprandsrc2): '<Root>/Random Source' */
}

void RandSrc_GZ_D(real_T y[], const real_T mean[], int32_T meanLen, const real_T
                  xstd[], int32_T xstdLen, uint32_T state[], int32_T nChans,
                  int32_T nSamps)
{
  static const real_T vt[65] = { 0.340945, 0.4573146, 0.5397793, 0.6062427,
    0.6631691, 0.7136975, 0.7596125, 0.8020356, 0.8417227, 0.8792102, 0.9148948,
    0.9490791, 0.9820005, 1.0138492, 1.044781, 1.0749254, 1.1043917, 1.1332738,
    1.161653, 1.189601, 1.2171815, 1.2444516, 1.2714635, 1.298265, 1.3249008,
    1.3514125, 1.3778399, 1.4042211, 1.4305929, 1.4569915, 1.4834527, 1.5100122,
    1.5367061, 1.5635712, 1.5906454, 1.617968, 1.6455802, 1.6735255, 1.7018503,
    1.7306045, 1.7598422, 1.7896223, 1.8200099, 1.851077, 1.8829044, 1.9155831,
    1.9492166, 1.9839239, 2.0198431, 2.0571356, 2.095993, 2.136645, 2.1793713,
    2.2245175, 2.2725186, 2.3239338, 2.3795008, 2.4402218, 2.5075117, 2.5834658,
    2.6713916, 2.7769942, 2.7769942, 2.7769942, 2.7769942 };

  real_T r;
  real_T s;
  real_T std;
  real_T x;
  real_T y_0;
  int32_T chan;
  int32_T i;
  int32_T icng_tmp;
  int32_T j;
  int32_T samp;
  uint32_T icng;
  uint32_T jsr;

  /* S-Function (sdsprandsrc2): '<Root>/Random Source' */
  /* RandSrc_GZ_D */
  for (chan = 0; chan < nChans; chan++) {
    std = xstd[xstdLen > 1 ? chan : 0];
    icng_tmp = chan << 1;
    icng = state[icng_tmp];
    jsr = state[icng_tmp + 1];
    for (samp = 0; samp < nSamps; samp++) {
      icng = 69069U * icng + 1234567U;
      jsr ^= jsr << 13;
      jsr ^= jsr >> 17;
      jsr ^= jsr << 5;
      i = static_cast<int32_T>(icng + jsr);
      j = (i & 63) + 1;
      r = static_cast<real_T>(i) * 4.6566128730773926E-10 * vt[j];
      x = std::abs(r);
      y_0 = vt[j - 1];
      if (!(x <= y_0)) {
        x = (x - y_0) / (vt[j] - y_0);
        icng = 69069U * icng + 1234567U;
        jsr ^= jsr << 13;
        jsr ^= jsr >> 17;
        jsr ^= jsr << 5;
        y_0 = static_cast<real_T>(static_cast<int32_T>(icng + jsr)) *
          2.328306436538696E-10 + 0.5;
        s = x + y_0;
        if (s > 1.301198) {
          r = r < 0.0 ? 0.4878992 * x - 0.4878992 : 0.4878992 - 0.4878992 * x;
        } else {
          if (!(s <= 0.9689279)) {
            x = 0.4878992 - 0.4878992 * x;
            if (y_0 > 12.67706 - std::exp(-0.5 * x * x) * 12.37586) {
              r = r < 0.0 ? -x : x;
            } else {
              if (!(std::exp(-0.5 * vt[j] * vt[j]) + y_0 * 0.01958303 / vt[j] <=
                    std::exp(-0.5 * r * r))) {
                do {
                  icng = 69069U * icng + 1234567U;
                  jsr ^= jsr << 13;
                  jsr ^= jsr >> 17;
                  jsr ^= jsr << 5;
                  x = std::log(static_cast<real_T>(static_cast<int32_T>(icng +
                    jsr)) * 2.328306436538696E-10 + 0.5) / 2.776994;
                  icng = 69069U * icng + 1234567U;
                  jsr ^= jsr << 13;
                  jsr ^= jsr >> 17;
                  jsr ^= jsr << 5;
                } while (std::log(static_cast<real_T>(static_cast<int32_T>(icng
                            + jsr)) * 2.328306436538696E-10 + 0.5) * -2.0 <= x *
                         x);

                r = r < 0.0 ? x - 2.776994 : 2.776994 - x;
              }
            }
          }
        }
      }

      y[chan * nSamps + samp] = mean[meanLen > 1 ? chan : 0] + std * r;
    }

    state[icng_tmp] = icng;
    state[icng_tmp + 1] = jsr;
  }

  /* End of S-Function (sdsprandsrc2): '<Root>/Random Source' */
}

void MWSPCGlmsnw_D(const real_T x[], const real_T d[], real_T mu, uint32_T
                   *startIdxPtr, real_T xBuf[], real_T wBuf[], int32_T wLen,
                   real_T leakFac, int32_T xLen, real_T y[], real_T eY[], real_T
                   wY[])
{
  real_T bufEnergy;
  int32_T i;
  int32_T j;
  int32_T startIdx;
  int32_T tmp;

  /* S-Function (sdsplms): '<Root>/LMS Filter' */
  startIdx = static_cast<int32_T>(*startIdxPtr);
  for (i = 0; i < xLen; i++) {
    y[i] = 0.0;
    bufEnergy = 0.0;

    /* Copy the current sample at the END of the circular buffer and update BuffStartIdx
     */
    xBuf[startIdx] = x[i];
    startIdx++;
    if (startIdx == wLen) {
      startIdx = 0;
    }

    /* Multiply wgtBuff_vector (not yet updated) and inBuff_vector
     */
    /* Get the energy of the signal in updated buffer
     */
    for (j = startIdx; j < wLen; j++) {
      y[i] += wBuf[j - startIdx] * xBuf[j];
      bufEnergy += xBuf[j] * xBuf[j];
    }

    for (j = 0; j < startIdx; j++) {
      y[i] += wBuf[(j + wLen) - startIdx] * xBuf[j];
      bufEnergy += xBuf[j] * xBuf[j];
    }

    /* Ger error for the current sample
     */
    eY[i] = d[i] - y[i];

    /* Update weight-vector for next input sample
     */
    for (j = startIdx; j < wLen; j++) {
      tmp = j - startIdx;
      wBuf[tmp] = xBuf[j] / (bufEnergy + 2.2204460492503131E-16) * eY[i] * mu +
        wBuf[tmp] * leakFac;
    }

    for (j = 0; j < startIdx; j++) {
      tmp = (j + wLen) - startIdx;
      wBuf[tmp] = xBuf[j] / (bufEnergy + 2.2204460492503131E-16) * eY[i] * mu +
        wBuf[tmp] * leakFac;
    }
  }

  for (j = 0; j < wLen; j++) {
    wY[j] = wBuf[(wLen - j) - 1];
  }

  *startIdxPtr = static_cast<uint32_T>(startIdx);

  /* End of S-Function (sdsplms): '<Root>/LMS Filter' */
}

/* Model step function */
void ex_codegen_dspModelClass::step()
{
  /* local block i/o variables */
  real_T rtb_LMSFilter_o3[32];
  b_dsp_FIRFilter_0_ex_codegen__T *obj_0;
  b_dspcodegen_FIRFilter_ex_cod_T *obj;
  real_T acc2;
  real_T rtb_LMSFilter_o1;
  real_T rtb_LMSFilter_o2;
  real_T rtb_LowpassFilter;
  real_T rtb_RandomSource;
  int32_T j;

  /* S-Function (sdsprandsrc2): '<Root>/Random Source' */
  RandSrc_GZ_D(&rtb_RandomSource, &ex_codegen_dsp_P.RandomSource_MeanVal, 1,
               &ex_codegen_dsp_P.RandomSource_VarianceRTP, 1,
               ex_codegen_dsp_DW.RandomSource_STATE_DWORK, 1, 1);

  /* MATLABSystem: '<Root>/Lowpass Filter' */
  obj = ex_codegen_dsp_DW.obj.FilterObj;
  if (obj->isInitialized != 1) {
    obj->isSetupComplete = false;
    obj->isInitialized = 1;
    obj->isSetupComplete = true;

    /* System object Initialization function: dsp.FIRFilter */
    for (j = 0; j < 38; j++) {
      obj->cSFunObject.W0_states[j] = obj->cSFunObject.P0_InitialStates;
    }
  }

  obj_0 = &obj->cSFunObject;

  /* System object Outputs function: dsp.FIRFilter */
  /* Consume delay line and beginning of input samples */
  acc2 = rtb_RandomSource * obj->cSFunObject.P1_Coefficients[0];
  rtb_LowpassFilter = acc2;
  for (j = 0; j < 38; j++) {
    acc2 = obj_0->P1_Coefficients[j + 1] * obj_0->W0_states[j];
    rtb_LowpassFilter += acc2;
  }

  /* Update delay line for next frame */
  for (j = 36; j >= 0; j--) {
    obj_0->W0_states[j + 1] = obj_0->W0_states[j];
  }

  obj->cSFunObject.W0_states[0] = rtb_RandomSource;

  /* End of MATLABSystem: '<Root>/Lowpass Filter' */

  /* S-Function (sdsplms): '<Root>/LMS Filter' */
  MWSPCGlmsnw_D(&rtb_RandomSource, &rtb_LowpassFilter,
                ex_codegen_dsp_P.LMSFilter_mu,
                &ex_codegen_dsp_DW.LMSFilter_BUFF_IDX_DWORK,
                &ex_codegen_dsp_DW.LMSFilter_IN_BUFFER_DWORK[0U],
                &ex_codegen_dsp_DW.LMSFilter_WGT_IC_DWORK[0U], 32,
                ex_codegen_dsp_P.LMSFilter_leakage, 1, &rtb_LMSFilter_o1,
                &rtb_LMSFilter_o2, &rtb_LMSFilter_o3[0U]);
}

/* Model initialize function */
void ex_codegen_dspModelClass::initialize()
{
  {
    static const real_T tmp[39] = { -0.00059194361517663689,
      -0.0020231187609940074, -0.0021060229913033154, 0.0012560050342482782,
      0.0045836073548239592, 0.00081354750291903971, -0.0074945947911449614,
      -0.0059854409238031476, 0.0089175430449157777, 0.014763032770771055,
      -0.0061136921890367051, -0.026666288251129273, -0.00451612337795531,
      0.040069256336854729, 0.028624786731400381, -0.052515463472640085,
      -0.082387824369927212, 0.061352556551937866, 0.30991387818783289,
      0.43544828894513982, 0.30991387818783289, 0.061352556551937866,
      -0.082387824369927212, -0.052515463472640085, 0.028624786731400381,
      0.040069256336854729, -0.00451612337795531, -0.026666288251129273,
      -0.0061136921890367051, 0.014763032770771055, 0.0089175430449157777,
      -0.0059854409238031476, -0.0074945947911449614, 0.00081354750291903971,
      0.0045836073548239592, 0.0012560050342482782, -0.0021060229913033154,
      -0.0020231187609940074, -0.00059194361517663689 };

    b_dspcodegen_FIRFilter_ex_cod_T *iobj_0;
    int32_T i;

    /* Start for MATLABSystem: '<Root>/Lowpass Filter' */
    ex_codegen_dsp_DW.obj._pobj0.matlabCodegenIsDeleted = true;
    ex_codegen_dsp_DW.obj.matlabCodegenIsDeleted = true;
    ex_codegen_dsp_DW.obj.isInitialized = 0;
    ex_codegen_dsp_DW.obj.NumChannels = -1;
    ex_codegen_dsp_DW.obj.matlabCodegenIsDeleted = false;
    ex_codegen_dsp_DW.objisempty = true;
    ex_codegen_dsp_DW.obj.isSetupComplete = false;
    ex_codegen_dsp_DW.obj.isInitialized = 1;
    iobj_0 = &ex_codegen_dsp_DW.obj._pobj0;
    iobj_0->isInitialized = 0;

    /* System object Constructor function: dsp.FIRFilter */
    iobj_0->cSFunObject.P0_InitialStates = 0.0;
    for (i = 0; i < 39; i++) {
      iobj_0->cSFunObject.P1_Coefficients[i] = tmp[i];
    }

    iobj_0->matlabCodegenIsDeleted = false;
    ex_codegen_dsp_DW.obj.FilterObj = iobj_0;
    ex_codegen_dsp_DW.obj.NumChannels = 1;
    ex_codegen_dsp_DW.obj.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<Root>/Lowpass Filter' */
  }

  {
    b_dspcodegen_FIRFilter_ex_cod_T *obj;
    int32_T i;

    /* InitializeConditions for S-Function (sdsprandsrc2): '<Root>/Random Source' */
    ex_codegen_dsp_DW.RandomSource_SEED_DWORK =
      ex_codegen_dsp_P.RandomSource_rawSeed;
    RandSrcInitState_GZ(&ex_codegen_dsp_DW.RandomSource_SEED_DWORK,
                        &ex_codegen_dsp_DW.RandomSource_STATE_DWORK[0], 1);

    /* InitializeConditions for MATLABSystem: '<Root>/Lowpass Filter' */
    obj = ex_codegen_dsp_DW.obj.FilterObj;
    if (obj->isInitialized == 1) {
      /* System object Initialization function: dsp.FIRFilter */
      for (i = 0; i < 38; i++) {
        obj->cSFunObject.W0_states[i] = obj->cSFunObject.P0_InitialStates;
      }
    }

    /* End of InitializeConditions for MATLABSystem: '<Root>/Lowpass Filter' */
  }
}

/* Model terminate function */
void ex_codegen_dspModelClass::terminate()
{
  b_dspcodegen_FIRFilter_ex_cod_T *obj;

  /* Terminate for MATLABSystem: '<Root>/Lowpass Filter' */
  if (!ex_codegen_dsp_DW.obj.matlabCodegenIsDeleted) {
    ex_codegen_dsp_DW.obj.matlabCodegenIsDeleted = true;
    if ((ex_codegen_dsp_DW.obj.isInitialized == 1) &&
        ex_codegen_dsp_DW.obj.isSetupComplete) {
      obj = ex_codegen_dsp_DW.obj.FilterObj;
      if (obj->isInitialized == 1) {
        obj->isInitialized = 2;
      }

      ex_codegen_dsp_DW.obj.NumChannels = -1;
    }
  }

  obj = &ex_codegen_dsp_DW.obj._pobj0;
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    if (obj->isInitialized == 1) {
      obj->isInitialized = 2;
    }
  }

  /* End of Terminate for MATLABSystem: '<Root>/Lowpass Filter' */
}

/* Constructor */
ex_codegen_dspModelClass::ex_codegen_dspModelClass() :
  ex_codegen_dsp_DW(),
  ex_codegen_dsp_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
ex_codegen_dspModelClass::~ex_codegen_dspModelClass()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_ex_codegen_dsp_T * ex_codegen_dspModelClass::getRTM()
{
  return (&ex_codegen_dsp_M);
}
