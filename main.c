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
        printf(">>> unknown command: ");
        puts(buffer);
    }
}
int HelloWorld(char *args, int len)
{
    puts("hello world!");
    return EOF;
}
int counter = 0;
int Monitor(char *args, int len)
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
        printf("\033[2J");
        printf("\033[f");
        printf("\033[?25h");
        return EOF;

    default:
        break;
    }
    if (counter == 0)
    {
        printf("\033[2J");
        printf("\033[?25l");
    }
    if (counter++ % 20)
        return NULL;
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
    return NULL;
}
int main()
{

    rpmState = createNew(64, 4);
    pwm_init();
    rpm_measurement_init();
    uart_init(&process_command);

    sei();

    printf("\033[2J");
    printf("\033[f");
    printf("\033[?25h");
    puts("hello world");
    printf("->");
    DDRB |= (1 << PB5);
    char buffer[30];
    uint16_t counter = 0;
    AddCommand("hello", HelloWorld);
    AddCommand("mon", Monitor);
    while (1)
    {
        if (counter % 500 == 0)
        {
            PORTB ^= (1 << PB5);
        }

        counter++;
        _delay_ms(1);
        loop();
        //        if (get_line(buffer, 30) != 0) {
        //
        //            // Process the received line
        //            // For example, echo it back
        //            printf("Received: %s\n", buffer);
        //        } else {
        //            //puts("0");
        //        }
        //        //printf("==> %d", res);
        //        if (logging)
        //        {
        //
        //            uint16_t rpm = rpm_measurement_get();
        //            // uart_transmit(period >> 8);
        //            // uart_transmit(period & 0xFF);
        //            printf("\033[2J");
        //            printf("\033[f");
        //            printf("rpm: %d\n", rpm);
        //            printf("soll: %d\n", target_rpm);
        //            printf("e: %d\n", rpmState.e);
        //            printf("esum: %d\n", rpmState.esum);
        //            printf("p: %ld\n", rpmState.p);
        //            printf("i: %ld\n", rpmState.i);
        //            printf("output: %d\n", rpmState.output);
        //            printf("OCR0A: %d\n", OCR0A);
        //            printf("->");
        //            // strncpy(buffer, rx_buffer, buffer_index);
        //            // buffer[buffer_index] = '\0';
        //            // uart_write(buffer);
        //        }
    }
}