#include "uart.h"
#include <avr/interrupt.h>
#include <stdio.h>
int uart_putc(char var, FILE *stream);
int uart_getc(FILE *stream);
static FILE uart_stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);
#define TX_BUFFER_SIZE 64

volatile char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;

#define RX_BUFFER_SIZE 64
volatile char rx_buffer[RX_BUFFER_SIZE]; 
volatile uint8_t rx_buffer_head = 0;
volatile uint8_t rx_buffer_tail = 0;
uint8_t buffer_index = 0;
//CommandHandler cmd_handler_callback;

void uart_init()
{
//    cmd_handler_callback = command_handler;
    // UBRR0H = UBRRH_VALUE;
    UBRR0L = 0;

    // Enable receiver and transmitter
    UCSR0B = (1 << TXEN0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << RXCIE0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    // use uart as stdout
    stdout = &uart_stdio;
    stdin = &uart_stdio;
}

void uart_putchar(char data)
{
    uint8_t next_head = (tx_buffer_head + 1) % TX_BUFFER_SIZE;

    // Wait for space in buffer
    while (next_head == tx_buffer_tail)
        ;

    tx_buffer[tx_buffer_head] = data;
    tx_buffer_head = next_head;

    // Enable UDRE interrupt to start transmission
    UCSR0B |= (1 << UDRIE0);
}

ISR(USART_UDRE_vect)
{
    if (tx_buffer_head != tx_buffer_tail)
    {
        UDR0 = tx_buffer[tx_buffer_tail];
        tx_buffer_tail = (tx_buffer_tail + 1) % TX_BUFFER_SIZE;
    }
    else
    {
        // Disable UDRE interrupt if no more data to send
        UCSR0B &= ~(1 << UDRIE0);
    }
}

int uart_putc(char var, FILE *stream) {
    // translate \n to \r\n 
    if (var == '\n') uart_putchar('\r');
    uart_putchar(var);
    return 0;
}

int uart_getc(FILE* stream) {
    int next = -1;
    if (rx_buffer_head != rx_buffer_tail)
    {
        next = rx_buffer[rx_buffer_tail];
        rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_SIZE;
    }
    return next;
}

void add_to_rx_buffer(char data){

    uint8_t next_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
    if (next_head != rx_buffer_tail) {
        rx_buffer[rx_buffer_head] = data;
        rx_buffer_head = next_head;
    }
}
ISR(USART_RX_vect) {
    uint8_t data = UDR0; // Read the received data
    add_to_rx_buffer(data == '\r' ? '\n':data);
}
//    char next = UDR0;
//
//    uint8_t next_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
//
//    // Wait for space in buffer
//    while (next_head == rx_buffer_tail)
//        ;
//
//    rx_buffer[rx_buffer_head] = next;
//    rx_buffer_head = next_head;
//
    // Enable UDRE interrupt to start transmission
//    putchar(next); // echo
//    if ((next == '\n' || next == '\r') && buffer_index > 0)
//    {
//        rx_buffer[buffer_index] = '\0';
//        PORTB ^= (1 << PB2);
//        cmd_handler_callback(rx_buffer[0], rx_buffer + 1, buffer_index - 1);
//        buffer_index = 0; // index zurück setzen um nächsten befehl zu empfangen
//        printf("\n->");
//    }
//    else
//    {
//        rx_buffer[buffer_index++] = next;
//    }
//}