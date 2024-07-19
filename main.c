#include "constants.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/setbaud.h>
#include "rpm_measurement.h"
#include "pwm.h"
#include "uart.h"
#include "utils.h"
#include "pi_controller.h"

volatile uint16_t target_rpm = 0;
ControllerState rpmState;

#define MAX_OUTPUT 255
#define MIN_OUTPUT 0
uint8_t rpm_controller(uint16_t target_rpm, uint16_t current_rpm)
{
    rpmState = pi_controller_run(rpmState, current_rpm, target_rpm);
    return (uint8_t)constrain_int16_t(rpmState.output, MIN_OUTPUT, MAX_OUTPUT);
}


ISR(TIMER0_OVF_vect)
{
    uint16_t rpm = rpm_measurement_get();
    uint8_t duty_cycle = rpm_controller(target_rpm, rpm);
    pwm_set(duty_cycle);
}

uint8_t logging = 0;

void process_command(char command, char *buffer, uint8_t length)
{
    switch (command)
    {
    case 'r':
        target_rpm = constrain_uint16_t(atoi(buffer), 0, 400); // stark vereinfacht, atoi steht für ascii to integer
        break;
    case 'p':
        rpmState.kp = constrain_uint8_t(atoi(buffer), 0, 255); // stark vereinfacht, atoi steht für ascii to integer
        break;
    case 'i':
        rpmState.ki = constrain_uint8_t(atoi(buffer), 0, 255); // stark vereinfacht, atoi steht für ascii to integer
        break;
    case 'l':
        logging ^= 1; // enable logging
        break;
    default:
        uart_write(">>> unknown command: ");
        uart_write_line(buffer);
    }
}

int main()
{

    rpmState = createNew(64, 4);
    pwm_init();
    rpm_measurement_init();
    uart_init(&process_command);


    sei();
    uart_transmit('A');
    puts("hello world\0");
    printf("->");
    DDRB |= (1 << PB5);
    while (1)
    {

        PORTB ^= (1 << PB5);
        _delay_ms(500);
        if (logging)
        {

            uint16_t rpm = rpm_measurement_get();
            // uart_transmit(period >> 8);
            // uart_transmit(period & 0xFF);
            uart_write("\033[2J");
            uart_write("\033[f");
            printf("rpm: %d\n", rpm);
            printf("soll: %d\n", target_rpm);
            printf("e: %d\n", rpmState.e);
            printf("esum: %d\n", rpmState.esum);
            printf("p: %ld\n", rpmState.p);
            printf("i: %ld\n", rpmState.i);
            printf("output: %d\n", rpmState.output);
            printf("OCR0A: %d\n", OCR0A);
            uart_write("->");
            // strncpy(buffer, rx_buffer, buffer_index);
            // buffer[buffer_index] = '\0';
            // uart_write(buffer);
        }
    }
}