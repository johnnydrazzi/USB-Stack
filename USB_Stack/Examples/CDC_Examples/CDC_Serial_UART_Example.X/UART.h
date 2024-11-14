/*
 * File:   uart.h
 * Author: John Izzard
 *
 * Copyright (C) 2017-2024 John Izzard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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