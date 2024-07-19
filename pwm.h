#ifndef _PWM_
#define _PWM_
#include "constants.h"
#include <avr/io.h>

void pwm_init(void);
void pwm_set(uint8_t duty_cycle);
#endif