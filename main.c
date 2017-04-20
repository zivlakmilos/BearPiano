#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "capacitive_touch.h"
#include "uart.h"

#define BAUD        9600

uint8_t binaryTouch(GPIO *emitPin, GPIO *recivePin)
{
    uint64_t touch = TOUCH_read(emitPin, recivePin);
    return touch > 10000 ? 1 : 0;
}

int main(void)
{
    TOUCH_init();
    UART_init(BAUD);

    GPIO emitPin1;
    emitPin1.port = &PORTC;
    emitPin1.pin = 0;
    emitPin1.lastState = 0;
    *((volatile uint8_t*)(emitPin1.port - 1)) |= (1 << emitPin1.pin);

    GPIO recivePin1;
    recivePin1.port = &PORTB;
    recivePin1.pin = 0;
    recivePin1.lastState = 0;
    *((volatile uint8_t*)(recivePin1.port - 1)) &= ~(1 << recivePin1.pin);

    DDRB |= (1 << DDB5);

    while(1)
    {
        uint8_t touch = binaryTouch(&emitPin1, &recivePin1);

        if(touch != recivePin1.lastState)
        {
            recivePin1.lastState = touch;
            if(touch > 0)
            {
                UART_putchar('t');
            }
        }

        _delay_ms(25);
    }

    return 0;
}
