#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "driver.h"

#define TIFORMAT

int main(int argc, char* argv[]) {
  int res;
  uint8_t i;
  uint32_t reg;
  afe4400_t* dev;

  dev = afe4400evm_init();
  if (dev == NULL) {
    fprintf(stderr, "Cannot open device: %s.\n", strerror(errno));
    return -errno;
  }

#ifndef TIFORMAT
  fprintf(stdout, "Address\t\tRegister\t\tValue\n=========================================\n");
#endif
  for (i = 0; i <= AFE4400EVM_REG_LAST; i ++) {
    res = afe4400evm_read_register(dev, i, &reg);
#ifdef TIFORMAT
    if (res == EXIT_SUCCESS) {
      fprintf(stdout, "AFE4403_%s\t%06X\n", register_names[i], reg);
    } else {
      fprintf(stdout, "AFE4403_%s\tError: %s\n", register_names[i], strerror(-res));
    }
#else
    if (res == EXIT_SUCCESS) {
      fprintf(stdout, "0x%02x\t\t%s\t\t0x%06x\n", i, register_names[i], reg);
    } else {
      fprintf(stdout, "0x%02x\t\t%s\t\tError: %s\n", i, register_names[i], strerror(-res));
    }
#endif /* TIFORMAT */
  }

  res = afe4400evm_cleanup(dev);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot close device: %s.\n", strerror(-res));
    return res;
  }

  return EXIT_SUCCESS;
}
