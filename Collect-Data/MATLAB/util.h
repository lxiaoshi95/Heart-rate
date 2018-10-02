#ifndef __AFE4400EVM_UTIL_H__
#define __AFE4400EVM_UTIL_H__

#include <time.h>

/* Common code structures (cannot use functions since type of parameter varies or needs return.) */
#define TRY_RETURN_RES(x) { \
  register int res; \
  if ((res = x) != EXIT_SUCCESS) { \
    return res; \
  } \
}

#define TRY_RETURN_ERRNO(x) { \
  if ((x) < 0) { \
    return -errno; \
  } \
}

#define DUMP_ARRAY(a) { \
  register size_t s; \
  for (s = 0; s < sizeof(a); s ++) { \
    printf(" %02x", ((uint8_t *)&a)[s]); \
  } \
  printf("\n"); \
}

#define DUMP_ARRAY_WS(a, l) { \
  register size_t s; \
  printf("%s(): ", __func__); \
  for (s = 0; s < l; s ++) { \
    printf(" %02x", ((uint8_t *)a)[s]); \
  } \
  printf("\n"); \
}

#endif /* __AFE4400EVM_UTIL_H__ */
