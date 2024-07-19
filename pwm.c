#include "pwm.h"

void pwm_set(uint8_t duty_cycle) {

    if (duty_cycle) {
        TCCR0A |= (1 << COM0A1);
    }
    else {
        TCCR0A &= ~(1 << COM0A1);
    }
    OCR0A = duty_cycle;
}

void pwm_init() {
	// Set PB3/OC2 as output
	DDRD |= (1<<DDD6);
	// Set Fast PWM mode with non-inverting output
    // WGM21:0 = 11 for Fast PWM
    // COM21:0 = 10 for non-inverting mode
    TCCR0A |= (1 << WGM00) | (1 << WGM01) ;
    // Set prescaler to 64 and start the timer
    // CS22:0 = 011 for prescaler 64
    TCCR0B |= (1 << CS01)|(1 << CS00);
    TIMSK0 |= (1<<TOIE0);

    // Set the duty cycle to 50% (128 out of 255)
    OCR0A = 0;

}