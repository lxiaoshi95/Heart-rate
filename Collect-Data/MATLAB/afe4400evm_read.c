/*
 * Read a number of PPG samples.
 */

#include <mex.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#include <libusb-1.0/libusb.h>

#include "driver.h"

static char* usage = "Usage: [R, IR, R_net, IR_net, R_ambient, IR_ambient] = afe4400evm_read(length)\nlength must be a positive integer.\n\n";

static void readafe(size_t len, double buf_r[], double buf_ir[], double buf_r_net[], double buf_ir_net[], double buf_r_ambient[], double buf_ir_ambient[]) {
  int res;
  afe4400_t* dev;
  afe4400_result_t result;
  unsigned count = 0;

  dev = afe4400evm_init();
  if (dev == NULL) {
    mexErrMsgTxt("Cannot open device");
  }
  
  res = afe4400evm_reset(dev);
  if (res != EXIT_SUCCESS) {
    mexErrMsgTxt("Cannot reset device");
  }
  
  res = afe4400evm_start_adc(dev, false, count);
  if (res != EXIT_SUCCESS) {
    mexErrMsgTxt("Cannot start ADC");
  }
  res = afe4400evm_stop_adc(dev); //wtf
  
  while (count < len) {
    res = afe4400evm_poll_adc(dev, &result);
    if (res != EXIT_SUCCESS) {
      mexErrMsgTxt("Cannot read ADC");
    }

    buf_r[count] = afe4400evm_adc_to_mv(result.red);
    buf_ir[count] = afe4400evm_adc_to_mv(result.ir);
    buf_r_net[count] = afe4400evm_adc_to_mv(result.red_net);
    buf_ir_net[count] = afe4400evm_adc_to_mv(result.ir_net);
    buf_r_ambient[count] = afe4400evm_adc_to_mv(result.red_ambient);
    buf_ir_ambient[count] = afe4400evm_adc_to_mv(result.ir_ambient);
    count ++;
  }

  res = afe4400evm_cleanup(dev);
  if (res != EXIT_SUCCESS) {
    mexErrMsgTxt("Cannot close device");
  }
}


void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  double len;

  if ((nrhs != 1) || (nlhs != 6)) {
    mexPrintf(usage);
    mexErrMsgTxt("Inavlid number of input/output");
    return;
  }
  if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) || (mxGetN(prhs[0]) * mxGetM (prhs[0]) != 1)) {
    mexPrintf(usage);
    mexErrMsgTxt("Invalid length type");
    return;
  }
  len = mxGetScalar(prhs[0]);
  if ((floor(len) != len) || (len <= 0)) {
    mexPrintf(usage);
    mexErrMsgTxt("Invalid length");
    return;
  }

  plhs[0] = mxCreateDoubleMatrix(1, (int)len, mxREAL);
  plhs[1] = mxCreateDoubleMatrix(1, (int)len, mxREAL);
  plhs[2] = mxCreateDoubleMatrix(1, (int)len, mxREAL);
  plhs[3] = mxCreateDoubleMatrix(1, (int)len, mxREAL);
  plhs[4] = mxCreateDoubleMatrix(1, (int)len, mxREAL);
  plhs[5] = mxCreateDoubleMatrix(1, (int)len, mxREAL);
  readafe(len, mxGetPr(plhs[0]), mxGetPr(plhs[1]), mxGetPr(plhs[2]), mxGetPr(plhs[3]), mxGetPr(plhs[4]), mxGetPr(plhs[5]));
}
