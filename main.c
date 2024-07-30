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
#include "rpm_controller.h"
#include "shell.h"
#include "terminal.h"

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

/**
 * @brief Monitor command function.
 *
 * @param args Command arguments
 * @param len Length of arguments
 * @return int 0 to proceed, EOF to exit
 */
int Monitor(char *args, size_t len)
{
    static int counter = 0; /**< Counter for monitor command */
    int key = getchar();
    switch (key)
    {
    case '+':
        rpm_controller_inc();
        break;

    case '-':
        rpm_controller_dec();
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
    terminal_reset_cursor();
    uint16_t rpm = rpm_measurement_get();
    ControllerState rpmState = rpm_controller_get_state();
    printf("rpm: %d\033[K\n", rpm);
    printf("soll: %d\033[K\n", rpm_controller_get_target_rpm());
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

    pwm_init();
    rpm_measurement_init();
    uart_init();
    rpm_controller_init(64, 8);
    sei();

    terminal_clear_screen();
    terminal_reset_cursor();
    terminal_enable_cursor();
    puts("ATMega328p RPM Controller");
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
