#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "driver.h"

int main(int argc, char* argv[]) {
  int res;
  afe4400_t* dev;

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

  res = afe4400evm_cleanup(dev);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot close device: %s.\n", strerror(-res));
    return res;
  }

  return EXIT_SUCCESS;
}
