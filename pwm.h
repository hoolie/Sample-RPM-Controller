/**
 * @file pwm.h
 * @brief Header file for PWM functions.
 */

#ifndef _PWM_
#define _PWM_

#include "constants.h"
#include <avr/io.h>

/**
 * @brief Initializes PWM.
 */
void pwm_init(void);

/**
 * @brief Sets the PWM duty cycle.
 * 
 * @param duty_cycle Duty cycle value (0-255)
 */
void pwm_set(uint8_t duty_cycle);

#endif // _PWM_