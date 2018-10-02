#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "driver.h"

int main(int argc, char* argv[]) {
  int res;
  afe4400_t* dev;
  afe4400_result_t result;
  unsigned count = -1;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s [number_of_samples]\ne.g.: %s 2000\n", argv[0], argv[0]);
    return -EINVAL;
  }
  sscanf(argv[1], "%u", &count);
  if (count <= 0) {
    fprintf(stderr, "Usage: %s [number_of_samples]\ne.g.: %s 2000\n", argv[0], argv[0]);
    return -EINVAL;
  }

  dev = afe4400evm_init();
  if (dev == NULL) {
    fprintf(stderr, "Cannot open device: %s.\n", strerror(errno));
    return -errno;
  }

  res = afe4400evm_reset(dev);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot reset board: %s.\n", strerror(-res));
    return res;
  }


  res = afe4400evm_start_adc(dev, false, count);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot start ADC: %s.\n", strerror(-res));
    return res;
  }
  res = afe4400evm_stop_adc(dev); //wtf

  fprintf(stdout, "Red\tRed_Ambient\tIR\tIR_Ambient\tRed_Net\tIR_Net\n============================================================\n");
  fflush(stdout);
  while (count) {
    res = afe4400evm_poll_adc(dev, &result);
    if (res != EXIT_SUCCESS) {
      fprintf(stderr, "Cannot read ADC: %s.\n", strerror(-res));
      return res;
    }

    fprintf(stdout, "%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n",
            afe4400evm_adc_to_mv(result.red),
            afe4400evm_adc_to_mv(result.red_ambient),
            afe4400evm_adc_to_mv(result.ir),
            afe4400evm_adc_to_mv(result.ir_ambient),
            afe4400evm_adc_to_mv(result.red_net),
            afe4400evm_adc_to_mv(result.ir_net)
           );
    fflush(stdout);
    count --;
  }

  res = afe4400evm_cleanup(dev);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot close device: %s.\n", strerror(-res));
    return res;
  }

  return EXIT_SUCCESS;
}
