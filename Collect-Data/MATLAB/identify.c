#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "driver.h"

#define MAX_BOARD_NAME_LEN 128

int main(int argc, char* argv[]) {
  int res;
  afe4400_t* dev;
  char name[MAX_BOARD_NAME_LEN];

  dev = afe4400evm_init();
  if (dev == NULL) {
    fprintf(stderr, "Cannot open device: %s.\n", strerror(errno));
    return -errno;
  }

  res = afe4400evm_board_name(dev, name, MAX_BOARD_NAME_LEN);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot read board information: %s.\n", strerror(-res));
    return res;
  }

  fprintf(stdout, "%s\n", name);

  res = afe4400evm_cleanup(dev);
  if (res != EXIT_SUCCESS) {
    fprintf(stderr, "Cannot close device: %s.\n", strerror(-res));
    return res;
  }

  return EXIT_SUCCESS;
}
