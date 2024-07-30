// uart.c - UART implementation for the ATmega328P
// This source file contains the implementation of functions for initializing UART, transmitting and receiving data.

#include "uart.h"
#include <avr/interrupt.h> // Include interrupt handling
#include <stdio.h> // Include standard I/O functions

// Calculate the UBRR (USART Baud Rate Register) value
#define MYUBRR F_CPU/16/BAUD-1

// Function prototypes for standard I/O stream handling
int uart_putc(char var, FILE *stream);
int uart_getc(FILE *stream);

// Define a static file stream for UART I/O
static FILE uart_stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);

#define TX_BUFFER_SIZE 64 // Define the size of the transmission buffer

// Transmission buffer and head/tail indices
volatile char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;

#define RX_BUFFER_SIZE 64 // Define the size of the reception buffer

// Reception buffer and head/tail indices
volatile char rx_buffer[RX_BUFFER_SIZE]; 
volatile uint8_t rx_buffer_head = 0;
volatile uint8_t rx_buffer_tail = 0;

// Initialize UART
void uart_init()
{
    // Set the baud rate
    UBRR0L = MYUBRR;

    // Enable UART receiver and transmitter, and their interrupts
    UCSR0B = (1 << TXEN0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << RXCIE0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    // Redirect standard I/O to UART
    stdout = &uart_stdio;
    stdin = &uart_stdio;
}

// Transmit a single character via UART
void uart_putchar(char data)
{
    uint8_t next_head = (tx_buffer_head + 1) % TX_BUFFER_SIZE;

    // Wait until there is space in the buffer
    while (next_head == tx_buffer_tail)
        ;

    // Add data to the transmission buffer
    tx_buffer[tx_buffer_head] = data;
    tx_buffer_head = next_head;

    // Enable UDRE interrupt to start transmission
    UCSR0B |= (1 << UDRIE0);
}

// UART Data Register Empty Interrupt Service Routine
ISR(USART_UDRE_vect)
{
    if (tx_buffer_head != tx_buffer_tail)
    {
        // Transmit the next byte from the buffer
        UDR0 = tx_buffer[tx_buffer_tail];
        tx_buffer_tail = (tx_buffer_tail + 1) % TX_BUFFER_SIZE;
    }
    else
    {
        // Disable UDRE interrupt if the buffer is empty
        UCSR0B &= ~(1 << UDRIE0);
    }
}

// Function to output a character to the UART stream
int uart_putc(char var, FILE *stream) {
    // Translate newline to carriage return + newline
    if (var == '\n') uart_putchar('\r');
    uart_putchar(var);
    return 0;
}

// Function to get a character from the UART stream
int uart_getc(FILE* stream) {
    int next = -1;
    if (rx_buffer_head != rx_buffer_tail)
    {
        // Get the next character from the reception buffer
        next = rx_buffer[rx_buffer_tail];
        rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_SIZE;
    }
    return next;
}

// Add a character to the reception buffer
void add_to_rx_buffer(char data){
    uint8_t next_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
    if (next_head != rx_buffer_tail) {
        // Add data to the reception buffer if there is space
        rx_buffer[rx_buffer_head] = data;
        rx_buffer_head = next_head;
    }
}

// UART Receive Complete Interrupt Service Routine
ISR(USART_RX_vect) {
    uint8_t data = UDR0; // Read the received data
    add_to_rx_buffer(data == '\r' ? '\n' : data); // Convert carriage return to newline
}