#ifndef _CAPACITIVE_COUTH_H_
#define _CAPACITIVE_COUTH_H_

#include <stdint.h>

#include "gpio.h"

void TOUCH_init(void);
uint64_t TOUCH_read(GPIO *emitPin, GPIO *recivePin);

#endif // _CAPACITIVE_COUTH_H_
