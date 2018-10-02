/*
 * UART to a queue of transactions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libusb.h>

#include "driver.h"
#include "driver_internals.h"
#include "uart2trans.h"
#include "util.h"


#define HANDLE_RESPONSE(dev, struct_type, cmd) { \
  struct struct_type* response = (struct struct_type*)buf; \
  if ((response->command != cmd) || (response->sot != AFE4400EVM_SOT) || (response->eot != AFE4400EVM_EOT) || (response->splitter != AFE4400EVM_CR)) { \
    return -EBADMSG; \
  } \
  trans = new_trans(dev, sizeof(*response)); \
  if (trans == NULL) { \
    return -errno; \
  } \
  trans->type = cmd; \
  memcpy(trans->record, buf, sizeof(*response)); \
  return EXIT_SUCCESS; \
}

#define TRY_RETURN_RES_LIBUSB(x) { \
  register int res; \
  if ((res = x) != LIBUSB_SUCCESS) { \
    return -lusb2errno(res); \
  } \
}

static int lusb2errno(int lusberr) {
  switch (lusberr) {
    case LIBUSB_SUCCESS: {
      return EXIT_SUCCESS;
    }

    case LIBUSB_ERROR_IO: {
      return EIO;
    }

    case LIBUSB_ERROR_INVALID_PARAM: {
      return EINVAL;
    }

    case LIBUSB_ERROR_ACCESS: {
      return EPERM;
    }

    case LIBUSB_ERROR_NO_DEVICE: {
      return ENXIO;
    }

    case LIBUSB_ERROR_NOT_FOUND: {
      return ENOENT;
    }

    case LIBUSB_ERROR_BUSY: {
      return EBUSY;
    }

    case LIBUSB_ERROR_TIMEOUT: {
      return ETIMEDOUT;
    }

    case LIBUSB_ERROR_OVERFLOW: {
      return ENOSPC;
    }

    case LIBUSB_ERROR_PIPE: {
      return EPIPE;
    }

    case LIBUSB_ERROR_INTERRUPTED: {
      return EINTR;
    }

    case LIBUSB_ERROR_NO_MEM: {
      return ENOMEM;
    }

    case LIBUSB_ERROR_NOT_SUPPORTED: {
      return ENOTSUP;
    }

    default: {
      return EFAULT;
    }
  }
}

int afe4400evm_uart_cleanup(afe4400_t* dev) {
  if (dev == NULL) {
    return -EFAULT;
  }

  if (dev->devh != NULL) {
    TRY_RETURN_RES_LIBUSB(libusb_release_interface(dev->devh, 0));
    TRY_RETURN_RES_LIBUSB(libusb_release_interface(dev->devh, 1));
    libusb_close(dev->devh);
  }
  libusb_exit(NULL);

  return EXIT_SUCCESS;
}

/* All transactions can be packed in one single bulk transfer. */
int afe4400evm_uart_write(afe4400_t* dev, void* buf, size_t len) {
  //int res;
  int actual_length;

  /* To prevent MCU's buffer from exploding */
  //res = afe4400evm_poll_incoming_transactions(dev);
  //if ((res != EXIT_SUCCESS) && (res != -ETIMEDOUT)) {
  //  return res;
  //}

  DUMP_ARRAY_WS(buf, len);
  TRY_RETURN_RES_LIBUSB(libusb_bulk_transfer(dev->devh, AFE4400EVM_EP_OUT_ADDR, buf, len, &actual_length, AFE4400EVM_TIMEOUT));
  if (len != actual_length) {
    return -ECONNRESET;
  }

  return EXIT_SUCCESS;
}

int afe4400evm_uart_setup(afe4400_t* dev) {
  int if_num;

  if (dev == NULL) {
    return -EFAULT;
  }

  TRY_RETURN_RES_LIBUSB(libusb_init(NULL));
  libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_INFO);
  dev->devh = libusb_open_device_with_vid_pid(NULL, AFE4400EVM_VID, AFE4400EVM_PID);
  if (dev->devh == NULL) {
    return -ENXIO;
  }

  for (if_num = 0; if_num < 2; if_num ++) {
    if (libusb_kernel_driver_active(dev->devh, if_num)) {
      TRY_RETURN_RES_LIBUSB(libusb_detach_kernel_driver(dev->devh, if_num));
    }
    TRY_RETURN_RES_LIBUSB(libusb_claim_interface(dev->devh, if_num));
  }

  return EXIT_SUCCESS;
}

static afe4400evm_transcation_t* new_trans(afe4400_t* dev, size_t payload_size) {
  uint8_t* payload;
  afe4400evm_transcation_t* trans;

  if (dev == NULL) {
    errno = EFAULT;
    return NULL;
  }

  if (payload_size <= 0) {
    errno = EINVAL;
    return NULL;
  }

  payload = malloc(payload_size);
  if (payload == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  trans = malloc(sizeof(afe4400evm_transcation_t));
  if (trans == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  /* Set appropraite fields */
  trans->len = payload_size;
  trans->record = payload;
  trans->next = NULL;

  /* Enqueue */
  if (dev->rxq == NULL) {
    dev->rxq = trans;
    dev->rxq_tail = trans;
  } else {
    dev->rxq_tail->next = trans;
    dev->rxq_tail = trans;
  }

  return trans;
}

/* Record and transaction (queue nodes) may be deleted separately, so this is cannot be used. */
/*
void afe4400evm_delete_transaction(afe4400evm_transcation_t* trans) {
  if (trans != NULL) {
    if (trans->record != NULL) {
      free(trans->record);
    }
    free(trans);
  }
}
*/

int afe4400evm_poll_incoming_transactions(afe4400_t* dev) {
  uint8_t buf[AFE4400EVM_BULK_SIZE];
  afe4400evm_transcation_t* trans;
  int actual_length;

  if (dev == NULL) {
    return -EFAULT;
  }

  memset(buf, 0, AFE4400EVM_BULK_SIZE);
  //printf("libusb_bulk_transfer read\n");
  TRY_RETURN_RES_LIBUSB(libusb_bulk_transfer(dev->devh, AFE4400EVM_EP_IN_ADDR, buf, AFE4400EVM_BULK_SIZE, &actual_length, AFE4400EVM_TIMEOUT));

  //printf("Got msg type %d, size %d\n", buf[0], actual_length);
  switch (buf[0]) {
    case AFE4400EVM_CMD_REG_READ: {
      HANDLE_RESPONSE(dev, afe4400evm_resp_reg_read, AFE4400EVM_CMD_REG_READ);
    }

    case AFE4400EVM_CMD_ADC_START: {
      HANDLE_RESPONSE(dev, afe4400evm_resp_adc, AFE4400EVM_CMD_ADC_START);
    }

    case AFE4400EVM_CMD_FW_VER: {
      HANDLE_RESPONSE(dev, afe4400evm_resp_fw_version, AFE4400EVM_CMD_FW_VER);
    }

    case AFE4400EVM_CMD_RDID: {
      HANDLE_RESPONSE(dev, afe4400evm_resp_rdid, AFE4400EVM_CMD_RDID);
    }

    default: {
      return -EBADMSG;
    }
  }
}

// FIXME: should dequeue from head (already enqueuing at tail)
void* afe4400evm_get_transaction(afe4400_t* dev) {
  afe4400evm_transcation_t* p;

  if (dev == NULL) {
    errno = -EFAULT;
    return NULL;
  }

  p = dev->rxq;
  if ((p == dev->rxq_tail) && (p != NULL)) {
    void* r = p->record;
    free(p);
    dev->rxq = NULL;
    dev->rxq_tail = NULL;
    return r;
  }
  while (p != NULL) {
    /* Should be equivalent to p->next->next == NULL */
    if (p->next == dev->rxq_tail) {
      void* r = p->next->record;

      free(p->next);
      p->next = NULL; /* Should be equivalent to p->next = p->next->next (before freeing.) */
      dev->rxq_tail = p;
      return r;
    }
    p = p->next;
  }

  errno = ENOMEDIUM;
  return NULL;
}

void* afe4400evm_get_transaction_by_type(afe4400_t* dev, uint8_t type) {
  afe4400evm_transcation_t* p;
  afe4400evm_transcation_t* found_prev = NULL;
  afe4400evm_transcation_t* found = NULL;

  if (dev == NULL) {
    errno = -EFAULT;
    return NULL;
  }
  //printf("%s searching for msg type %d, head / tail = %p / %p\n", __func__, type, dev->rxq, dev->rxq_tail);

  p = dev->rxq;
  if ((p != NULL) && (p->type == type)) {
    found = p;
  }
  while (p != NULL) {
    if ((p->next != NULL) && (p->next->type == type)) {
      found = p->next;
      found_prev = p;
    }
    p = p->next;
  }

  if (found != NULL) {
    void* r = found->record;

    if (found_prev == NULL) {
      /* Found node is the head */
      dev->rxq = found->next;
      if (found->next == NULL) {
        dev->rxq_tail = NULL;
      }
    } else {
      found_prev->next = found->next;
      if (found->next == NULL) {
        dev->rxq_tail = found_prev;
      }
    }

    free(found);
    //printf("%s returning msg type %d, new head / tail = %p / %p\n", __func__, type, dev->rxq, dev->rxq_tail);
    return r;
  } else {
    errno = ENOMEDIUM;
    return NULL;
  }
}

void* afe4400evm_peek_transaction(afe4400_t* dev) {
  if (dev == NULL) {
    errno = -EFAULT;
    return NULL;
  }

  return dev->rxq_tail->record;
}

void* afe4400evm_peek_transaction_by_type(afe4400_t* dev, uint8_t type) {
  afe4400evm_transcation_t* p;

  if (dev == NULL) {
    errno = -EFAULT;
    return NULL;
  }

  p = dev->rxq;
  while (p != NULL) {
    if (p->type == type) {
      return p->record;
    }
    p = p->next;
  }

  errno = ENOMEDIUM;
  return NULL;
}

/* For debugging, returns -1 on error */
int afe4400evm_count_transactions(afe4400_t* dev) {
  int count = 0;
  afe4400evm_transcation_t* p;

  if (dev == NULL) {
    errno = -EFAULT;
    return -1;
  }

  p = dev->rxq;
  while (p != NULL) {
    count ++;
    p = p->next;
  }

  return count;
}

/* Frees all transactions */
int afe4400evm_destory_transactions(afe4400_t* dev) {
  afe4400evm_transcation_t* next;
  afe4400evm_transcation_t* curr;

  if (dev == NULL) {
    return -EFAULT;
  }

  next = dev->rxq;
  while (next != NULL) {
    curr = next;
    next = curr->next;
    if (curr->record != NULL) {
      free(curr->record);
    }
    free(curr);
  }

  dev->rxq = NULL;
  dev->rxq_tail = NULL;

  return EXIT_SUCCESS;
}
