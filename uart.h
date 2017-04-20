#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

void UART_init(int baud);
void UART_putchar(char ch);
void UART_putstr(const char *str);
void UART_putuint(uint64_t number);

#endif // _UART_H_
