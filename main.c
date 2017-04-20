#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "capacitive_touch.h"
#include "uart.h"

#define BAUD        9600

int main(void)
{
    TOUCH_init();
    UART_init(BAUD);

    GPIO emitPin1;
    emitPin1.port = &PORTC;
    emitPin1.pin = 0;
    *((volatile uint8_t*)(emitPin1.port - 1)) |= (1 << emitPin1.pin);

    GPIO recivePin1;
    recivePin1.port = &PORTB;
    recivePin1.pin = 0;
    *((volatile uint8_t*)(recivePin1.port - 1)) &= ~(1 << recivePin1.pin);
    //*((volatile uint8_t*)recivePin1.port) |= (1 << recivePin1.pin);

    DDRB |= (1 << DDB5);

    while(1)
    {
        uint64_t touch = TOUCH_read(&emitPin1, &recivePin1);
        UART_putuint(touch);
        _delay_ms(25);
        continue;

        if(touch > 10000)
        {
            PORTB |= (1 << PB5);
        } else
        {
            PORTB &= ~(1 << PB5);
        }

        _delay_ms(25);
    }

    return 0;
}
