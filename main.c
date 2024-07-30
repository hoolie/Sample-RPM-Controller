/**
 * @file main.c
 * @brief Main file for the ATmega328P project.
 */

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
#include "shell.h"
#include "terminal.h"

volatile uint16_t target_rpm = 0; /**< Target RPM value */
ControllerState rpmState; /**< PI controller state */

#define MAX_OUTPUT 255 /**< Maximum controller output */
#define MIN_OUTPUT 0   /**< Minimum controller output */

/**
 * @brief Runs the RPM controller.
 * 
 * @param target_rpm Target RPM value
 * @param current_rpm Current RPM value
 * @return uint8_t Controller output
 */
uint8_t rpm_controller(uint16_t target_rpm, uint16_t current_rpm)
{
    rpmState = pi_controller_run(rpmState, current_rpm, target_rpm);
    return (uint8_t)constrain_int16_t(rpmState.output, MIN_OUTPUT, MAX_OUTPUT);
}

/**
 * @brief Timer0 overflow interrupt service routine.
 */
ISR(TIMER0_OVF_vect)
{
    uint16_t rpm = rpm_measurement_get();
    uint8_t duty_cycle = rpm_controller(target_rpm, rpm);
    pwm_set(duty_cycle);
}

uint8_t logging = 0; /**< Logging flag */

/**
 * @brief Example command function that prints "hello world".
 * 
 * @param args Command arguments
 * @param len Length of arguments
 * @return int EOF on completion
 */
int HelloWorld(char *args, size_t len)
{
    puts("hello world!");
    return EOF;
}

int counter = 0; /**< Counter for monitor command */

/**
 * @brief Monitor command function.
 * 
 * @param args Command arguments
 * @param len Length of arguments
 * @return int 0 on success, EOF to exit
 */
int Monitor(char *args, size_t len)
{
    int key = getchar();
    switch (key)
    {
    case '+':
        target_rpm++;
        break;

    case '-':
        target_rpm--;
        break;
    case 'x':
        counter = 0;
        terminal_clear_screen();
        terminal_reset_cursor();
        terminal_enable_cursor();
        return EOF;

    default:
        break;
    }
    if (counter == 0)
    {
        terminal_clear_screen();
        terminal_disable_cursor();
    }
    if (counter++ % 20)
        return 0;
    printf("\033[f");
    uint16_t rpm = rpm_measurement_get();
    printf("rpm: %d\033[K\n", rpm);
    printf("soll: %d\033[K\n", target_rpm);
    printf("e: %d\033[K\n", rpmState.e);
    printf("esum: %d\033[K\n", rpmState.esum);
    printf("p: %ld\033[K\n", rpmState.p);
    printf("i: %ld\033[K\n", rpmState.i);
    printf("output: %d\033[K\n", rpmState.output);
    printf("OCR0A: %d\033[K\n", OCR0A);
    return 0;
}

/**
 * @brief Main function.
 * 
 * @return int Program exit status
 */
int main()
{
    rpmState = createNew(64, 4);
    pwm_init();
    rpm_measurement_init();
    uart_init();
    sei();

    terminal_clear_screen();
    terminal_reset_cursor();
    terminal_enable_cursor();
    puts("hello world");
    printf("->");
    DDRB |= (1 << PB5);
    uint16_t counter = 0;
    shell_add_command("hello", HelloWorld);
    shell_add_command("mon", Monitor);
    while (1)
    {
        if (counter % 500 == 0)
        {
            PORTB ^= (1 << PB5);
        }

        counter++;
        _delay_ms(1);
        shell_loop();
    }
}

