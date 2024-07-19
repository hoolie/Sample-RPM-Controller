#ifndef _UART_
#define _UART_
#include "constants.h"
#include <avr/io.h>
typedef void (*CommandHandler)(char command, char* buffer, uint8_t length);

void uart_init(CommandHandler command_handler);

void uart_transmit(unsigned char data);
void uart_write(char* data);
void uart_write_line(char* data);
#endif