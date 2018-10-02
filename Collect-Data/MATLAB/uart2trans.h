#ifndef __AFE4400EVM_UART2TRANS_H__
#define __AFE4400EVM_UART2TRANS_H__

#include "driver.h"

int afe4400evm_uart_cleanup(afe4400_t* dev);
int afe4400evm_uart_setup(afe4400_t* dev);
int afe4400evm_uart_write(afe4400_t* dev, void* buf, size_t len);
int afe4400evm_poll_incoming_transactions(afe4400_t* dev);
void* afe4400evm_get_transaction(afe4400_t* dev);
void* afe4400evm_get_transaction_by_type(afe4400_t* dev, uint8_t type);
void* afe4400evm_peek_transaction(afe4400_t* dev);
void* afe4400evm_peek_transaction_by_type(afe4400_t* dev, uint8_t type);
int afe4400evm_count_transactions(afe4400_t* dev);
int afe4400evm_destory_transactions(afe4400_t* dev);

#endif /* __AFE4400EVM_UART2TRANS_H__ */
