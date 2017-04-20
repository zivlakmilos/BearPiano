#include "capacitive_touch.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint64_t time;

ISR(TIMER0_OVF_vect)
{
    time += TCNT0;
}

void TOUCH_init(void)
{
    SREG &= ~0x80;

    TCCR0A |= (1 << COM0A1);        // Clear OC0A on Compare Match
    TIMSK0 |= (1 << TOIE0);         // Timer overflow interrupt

    SREG |= 0x80;
}

uint64_t TOUCH_read(GPIO *emitPin, GPIO *recivePin)
{
    TCNT0 = 0;
    time = 0;

    SREG &= ~0x80;
    TCCR0B |= (1 << CS00);      // Start timer
    SREG |= 0x80;

    *((volatile uint8_t*)emitPin->port) &= ~(1 << emitPin->pin);

    while((*((volatile uint8_t*)(recivePin->port - 2)) >> recivePin->pin) & 0x01);
    time += TCNT0;

    SREG &= ~0x80;
    TCCR0B &= ~(1 << CS00);     // Stop timer
    SREG |= 0x80;

    *((volatile uint8_t*)emitPin->port) |= (1 << emitPin->pin);

    return time;
}
