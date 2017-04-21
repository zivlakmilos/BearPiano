#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "capacitive_touch.h"
#include "uart.h"

#define BAUD        9600
#define CAN_COUNT   3

uint8_t checkTouch(GPIO *emitPin, GPIO *recivePin)
{
    uint64_t touch = TOUCH_read(emitPin, recivePin);
    return touch > 10000 ? 1 : 0;
}

int main(void)
{
    TOUCH_init();
    UART_init(BAUD);

    GPIO emitPin[CAN_COUNT];
    GPIO recivePin[CAN_COUNT];
    char message[CAN_COUNT] = { '<', 'P', '>' };
    uint8_t i;

    for(i = 0; i < CAN_COUNT; i++)
    {
        emitPin[i].port = &PORTB;
        emitPin[i].pin = CAN_COUNT - i - 1;
        emitPin[i].lastState = 0;
        *((volatile uint8_t*)(emitPin[i].port - 1)) |= (1 << emitPin[i].pin);

        recivePin[i].port = &PORTC;
        recivePin[i].pin = i;
        recivePin[i].lastState = 0;
        *((volatile uint8_t*)(recivePin[i].port - 1)) &= ~(1 << recivePin[i].pin);
    }


    while(1)
    {
        for(i = 0; i < CAN_COUNT; i++)
        {
            uint8_t touch = checkTouch(&emitPin[i], &recivePin[i]);
            if(touch != recivePin[i].lastState)
            {
                recivePin[i].lastState = touch;
                if(touch > 0)
                {
                    UART_putchar(message[i]);
                }
            }
        }

        _delay_ms(25);
    }

    return 0;
}
