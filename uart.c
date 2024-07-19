#include "uart.h"
#include <avr/interrupt.h>
#include <stdio.h>
int uart_putchar_printf(char var, FILE *stream);
static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
#define TX_BUFFER_SIZE 64

volatile char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;

#define RX_BUFFER_SIZE = 128;
CommandHandler cmd_handler_callback;
char rx_buffer[30]; // muss groß genug sein das alle Zeichen der befehle rein passen.
uint8_t buffer_index = 0;

void uart_init(CommandHandler command_handler)
{
    cmd_handler_callback = command_handler;
    // UBRR0H = UBRRH_VALUE;
    UBRR0 = MYUBRR;

    // Enable receiver and transmitter
    UCSR0B = (1 << TXEN0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << RXCIE0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    // use uart as stdout
    stdout = &uart_stdout;
}

void uart_transmit(unsigned char data)
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
void uart_write(char *buffer)
{
    while (*buffer)
    {
        uart_transmit(*buffer);
        buffer++;
    }
}
void uart_write_line(char *buffer)
{
    uart_write(buffer);
    uart_write("\r\n");
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

int uart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r\n 
    if (var == '\n') uart_transmit('\r');
    uart_transmit(var);
    return 0;
}

ISR(USART_RX_vect)
{
    char next = UDR0;
    uart_transmit(next); // echo
    if ((next == '\n' || next == '\r') && buffer_index > 0)
    {
        rx_buffer[buffer_index] = '\0';
        PORTB ^= (1 << PB2);
        cmd_handler_callback(rx_buffer[0], rx_buffer + 1, buffer_index - 1);
        buffer_index = 0; // index zurück setzen um nächsten befehl zu empfangen
        uart_write("\n->");
    }
    else
    {
        rx_buffer[buffer_index++] = next;
    }
}