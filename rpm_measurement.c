#include "rpm_measurement.h"
#include <avr/interrupt.h>

#define PULSE_PER_ROTATION 600
#define MIN_CONST 60
#define DIVIDEND (MIN_CONST*F_CPU/PULSE_PER_ROTATION)
#define FILTER_POWER 2

volatile uint16_t last_time;
volatile uint16_t period;

uint16_t period_buffer[1<<FILTER_POWER];
uint8_t period_buffer_index = 0;
uint32_t period_sum = 0;

uint8_t timer1OverflowCounter = 0;


ISR(TIMER1_CAPT_vect){
    timer1OverflowCounter = 0;
	uint16_t current_time = ICR1; // Get current capture time
	uint16_t delta = (int16_t)(current_time - last_time);
	last_time = current_time;
	period_sum -= period_buffer[period_buffer_index];
	period_sum += delta;
	period_buffer[period_buffer_index] = delta;
	period = period_sum >> FILTER_POWER;
	period_buffer_index = (period_buffer_index + 1) % (1<<FILTER_POWER);
}

ISR(TIMER1_OVF_vect){
    if (timer1OverflowCounter < 2){
        timer1OverflowCounter++;
    }
}

void rpm_measurement_init() {
    // Set the Timer/Counter Control Registers for CTC mode
	TCCR1B = (1 << ICES1) | (1 << CS10); // Input capture on rising edge, no prescaler
    TIMSK1 |= (1 << ICIE1)|(1<<TOIE1); // Enable input capture interrupt

    PORTB |= (1<<PB0);
    DDRB |= (1<<PB1);
    // Enable global interrupts
    sei();
}
uint16_t rpm_measurement_get(){
    return timer1OverflowCounter<2 
            ? DIVIDEND/period
            : 0;
}