#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "capacitive_touch.h"
#include "uart.h"

#define BAUD        9600
#define CAN_COUNT   8

static uint8_t checkTouch(GPIO *emitPin, GPIO *recivePin)
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
    /*
    char message[CAN_COUNT][3] = {
        "c1", "c1s", "d1", "d1s", "e1", "f1", "f1s", "g1", "g1s", "a1", "a1s", "b1"
    };
    */
    char message[CAN_COUNT][3] = {
        "c1", "d1", "e1", "f1", "g1", "a1", "h1", "c2"
    };
    uint8_t i, j;
    uint8_t canCount;

    for(i = 0, j = 0; i < 2; i++, j++)
    {
        emitPin[j].port = &PORTD;
        emitPin[j].pin = i + 2;
        emitPin[j].lastState = 0;
        *((volatile uint8_t*)(emitPin[j].port - 1)) |= (1 << emitPin[j].pin);

        recivePin[j].port = &PORTB;
        recivePin[j].pin = i + 2;
        recivePin[j].lastState = 0;
        *((volatile uint8_t*)(recivePin[j].port - 1)) &= ~(1 << recivePin[j].pin);
    }
    for(i = 0; i < 5; i++, j++)
    {
        emitPin[j].port = &PORTD;
        emitPin[j].pin = i + 4;
        emitPin[j].lastState = 0;
        *((volatile uint8_t*)(emitPin[j].port - 1)) |= (1 << emitPin[j].pin);

        recivePin[j].port = &PORTC;
        recivePin[j].pin = 5 - i;
        recivePin[j].lastState = 0;
        *((volatile uint8_t*)(recivePin[j].port - 1)) &= ~(1 << recivePin[j].pin);
    }
    for(i = 0; i < 2; i++, j++)
    {
        break;
        emitPin[j].port = &PORTB;
        emitPin[j].pin = i;
        emitPin[j].lastState = 0;
        *((volatile uint8_t*)(emitPin[j].port - 1)) |= (1 << emitPin[j].pin);

        recivePin[j].port = &PORTC;
        recivePin[j].pin = 1 - i;
        recivePin[j].lastState = 0;
        *((volatile uint8_t*)(recivePin[j].port - 1)) &= ~(1 << recivePin[j].pin);
    }

    j = 6;
    canCount = j;


    while(1)
    {
        for(i = 0; i < canCount; i++)
        {
            uint8_t touch = checkTouch(&emitPin[i], &recivePin[i]);
            if(touch != recivePin[i].lastState)
            {
                recivePin[i].lastState = touch;
                if(touch > 0)
                {
                    UART_putstr(message[i]);
                    UART_putchar('\n');
                }
            }
        }

        //_delay_ms(25);
    }

    return 0;
}
