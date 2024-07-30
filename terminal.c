/**
 * @file terminal.c
 * @brief Terminal interface implementation.
 */

#include <stdio.h>
#include <string.h>
#include "terminal.h"

char command_buffer[COMMAND_BUFFER_SIZE]; /**< Buffer for terminal commands */
uint8_t command_buffer_index = 0; /**< Index for the command buffer */

/**
 * @brief Reads a line from the terminal with echo.
 * 
 * @param buffer Buffer to store the line
 * @param max_len Maximum length of the buffer
 * @param echo_stream Stream to echo the input
 * @return int Length of the read line
 */
int terminal_get_line_echo(char *buffer, size_t max_len, FILE *echo_stream) {
    int next = getchar();

    while (next != EOF) {
        putc(next, echo_stream);
        if (next == '\n') {
            command_buffer[command_buffer_index] = '\0';
            strncpy(buffer, command_buffer, max_len);
            command_buffer_index = 0;
            return strlen(buffer);
        } else {
            command_buffer[command_buffer_index] = (char)next;
            command_buffer_index = (command_buffer_index + 1) % COMMAND_BUFFER_SIZE;
        }
        next = getchar();
    }
    return 0;
}

/**
 * @brief Reads a line from the terminal without echo.
 * 
 * @param buffer Buffer to store the line
 * @param max_len Maximum length of the buffer
 * @return int Length of the read line
 */
int terminal_get_line(char *buffer, size_t max_len) {
    return terminal_get_line_echo(buffer, max_len, stdout);
}

/**
 * @brief Clears the terminal screen.
 */
inline void terminal_clear_screen(void) {
    fputs("\033[2J", stdout);
}

/**
 * @brief Resets the terminal cursor.
 */
inline void terminal_reset_cursor(void) {
    fputs("\033[f", stdout);
}

/**
 * @brief Disables the terminal cursor.
 */
inline void terminal_disable_cursor(void) {
    fputs("\033[?25l", stdout);
}

/**
 * @brief Enables the terminal cursor.
 */
inline void terminal_enable_cursor(void) {
    fputs("\033[?25h", stdout);
}