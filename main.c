#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "capacitive_touch.h"

int main(void)
{
    //TOUCH_init();

    GPIO emitPin1;
    emitPin1.port = &PORTC;
    emitPin1.pin = 0;
    *((volatile uint8_t*)(emitPin1.port - 1)) |= (1 << emitPin1.pin);

    GPIO recivePin1;
    recivePin1.port = &PORTB;
    recivePin1.pin = 0;
    *((volatile uint8_t*)(recivePin1.port - 1)) &= ~(1 << recivePin1.pin);
    *((volatile uint8_t*)recivePin1.port) |= (1 << recivePin1.pin);

    DDRB |= (1 << DDB5);

    GPIO test;
    test.port = 0x25; //PORTB;
    test.pin = 5;

    while(1)
    {
        /*
        while(1)
        {
            //if((*((volatile uint8_t*)(recivePin1.port - 2)) >> recivePin1.pin) & 0x01)
            //if(*((volatile uint8_t*)(recivePin1.port - 2)) & (1 << recivePin1.pin))
            //if(*((volatile uint8_t*)(recivePin1.port - 2)) == 0x01)
            if(PINB & (1 << PB0))
            {
                PORTB |= (1 << PB5);
            } else
            {
                PORTB &= ~(1 << PB5);
            }
        }
        */
        //*((volatile uint32_t*)test.port) |= (1 << test.pin);
        if(TOUCH_read(&emitPin1, &recivePin1) > 200)
        {
            PORTB |= (1 << PB5);
            //_delay_ms(1000);
        } else
        {
            PORTB &= ~(1 << PB5);
        }

        _delay_ms(25);
    }

    return 0;
}
