/*
 * File:   uart.h
 * Author: John Izzard
 *
 * Year:   2016
 */

#ifndef UART_H
#define UART_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#include "uart_settings.h"
#include <stdint.h>
#include <stdbool.h>

void    uart__init(uint8_t uart);
void    uart__set_baud(uint8_t uart, uint16_t baud);
bool    uart__data_ready(uint8_t uart);
bool    uart__tx_idle(uint8_t uart);
uint8_t uart__read(uint8_t uart);
void    uart__read_string(uint8_t uart, uint8_t* output, uint8_t* delimiter, uint8_t attempts);
void    uart__write(uint8_t uart, uint8_t byte);
void    uart__write_string(uint8_t uart, uint8_t* string);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* UART_H */