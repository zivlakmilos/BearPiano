#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

typedef struct GPIO_S
{
    uint8_t port;
    uint8_t pin;
    uint8_t lastState;
} GPIO;

#endif // _GPIO_H_
