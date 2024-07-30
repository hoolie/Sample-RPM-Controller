/**
 * @file uart.h
 * @brief UART interface for the ATmega328P.
 *
 * This header file contains function declarations for initializing UART and transmitting data.
 */

#ifndef _UART_
#define _UART_

#include "constants.h"
#include <avr/io.h>

/**
 * @brief Initializes the UART.
 *
 * This function sets up the UART with the defined baud rate, enables the receiver
 * and transmitter, and sets the frame format.
 * It also initializes stdin and stdout to uart.
 */
void uart_init(void);

/**
 * @brief Transmits a single byte of data via UART.
 *
 * @param data The byte of data to transmit.
 */
void uart_transmit(unsigned char data);

#endif // _UART_
