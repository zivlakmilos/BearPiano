#include "uart.h"

#include <avr/io.h>

void UART_init(int baud)
{
    int baudRate = F_CPU / 16 / baud - 1;
    UBRR0H = (uint8_t)(baudRate >> 8);
    UBRR0L = (uint8_t)baudRate;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static void UART_transmit(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void UART_putchar(char ch)
{
    UART_transmit(ch);
}

void UART_putstr(const char *str)
{
    while(*str != '\0')
        UART_putchar(*(str++));
}

void UART_putuint(uint64_t number)
{
    uint8_t i = 0;
    char decimals[sizeof(number) + 2];

    if(number == 0)
        decimals[i++] = '0';

    while(number > 0)
    {
        decimals[i++] = (number % 10) + '0';
        number /= 10;
    }
    decimals[i++] = '\n';
    decimals[i] = '\0';
    UART_putstr(decimals);
}
