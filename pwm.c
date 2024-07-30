/**
 * @file pwm.c
 * @brief PWM implementation.
 */

#include "pwm.h"

/**
 * @brief Sets the PWM duty cycle.
 * 
 * @param duty_cycle Duty cycle value (0-255)
 */
void pwm_set(uint8_t duty_cycle) {
    if (duty_cycle) {
        TCCR0A |= (1 << COM0A1);
    } else {
        TCCR0A &= ~(1 << COM0A1);
    }
    OCR0A = duty_cycle;
}

/**
 * @brief Initializes PWM.
 */
void pwm_init() {
    DDRD |= (1 << DDD6); // Set PD6/OC0A as output
    TCCR0A |= (1 << WGM00) | (1 << WGM01); // Set Fast PWM mode
    TCCR0B |= (1 << CS01) | (1 << CS00); // Set prescaler to 64 and start the timer
    TIMSK0 |= (1 << TOIE0); // Enable overflow interrupt
    OCR0A = 0; // Set the duty cycle to 0%
}