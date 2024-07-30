/**
 * @file rpm_measurement.c
 * @brief RPM measurement implementation.
 */

#include "rpm_measurement.h"
#include <avr/interrupt.h>

#define PULSE_PER_ROTATION 600 /**< Number of pulses per rotation */
#define MIN_CONST 60 /**< Constant for minute conversion */
#define DIVIDEND (MIN_CONST * F_CPU / PULSE_PER_ROTATION) /**< Dividend for RPM calculation */
#define FILTER_POWER 2 /**< Filter power for averaging */

volatile uint16_t last_time; /**< Last capture time */
volatile uint16_t period; /**< Current period */

uint16_t period_buffer[1 << FILTER_POWER]; /**< Buffer for period values */
uint8_t period_buffer_index = 0; /**< Index for period buffer */
uint32_t period_sum = 0; /**< Sum of periods */

uint8_t timer1OverflowCounter = 0; /**< Timer1 overflow counter */

/**
 * @brief Timer1 capture interrupt service routine.
 */
ISR(TIMER1_CAPT_vect){
    timer1OverflowCounter = 0;
    uint16_t current_time = ICR1; // Get current capture time
    uint16_t delta = (int16_t)(current_time - last_time);
    last_time = current_time;
    period_sum -= period_buffer[period_buffer_index];
    period_sum += delta;
    period_buffer[period_buffer_index] = delta;
    period = period_sum >> FILTER_POWER;
    period_buffer_index = (period_buffer_index + 1) % (1 << FILTER_POWER);
}

/**
 * @brief Timer1 overflow interrupt service routine.
 */
ISR(TIMER1_OVF_vect){
    if (timer1OverflowCounter < 2){
        timer1OverflowCounter++;
    }
}

/**
 * @brief Initializes RPM measurement.
 */
void rpm_measurement_init() {
    TCCR1B = (1 << ICES1) | (1 << CS10); // Input capture on rising edge, no prescaler
    TIMSK1 |= (1 << ICIE1) | (1 << TOIE1); // Enable input capture interrupt and overflow interrupt
    PORTB |= (1 << PB0);
    DDRB |= (1 << PB1);
    sei(); // Enable global interrupts
}

/**
 * @brief Gets the current RPM.
 * 
 * @return uint16_t Current RPM value
 */
uint16_t rpm_measurement_get() {
    return timer1OverflowCounter < 2 ? DIVIDEND / period : 0;
}