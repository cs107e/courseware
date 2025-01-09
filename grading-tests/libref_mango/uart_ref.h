#pragma once
#include "uart.h"
void ref_uart_init(void);
int ref_uart_getchar(void);
int ref_uart_putchar(int ch);
void ref_uart_flush(void);
bool ref_uart_haschar(void);
int ref_uart_putstring(const char *str);
void ref_uart_send(char byte);
unsigned char ref_uart_recv(void);
void ref_uart_use_interrupts(handlerfn_t handler, void *client_data);
void ref_uart_start_error(void);
void ref_uart_end_error(void);
