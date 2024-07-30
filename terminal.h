/**
 * @file terminal.h
 * @brief Header file for terminal functions.
 */

#ifndef _TERMINAL_
#define _TERMINAL_

#include <stdio.h>

#define COMMAND_BUFFER_SIZE 128 /**< Size of the command buffer */

/**
 * @brief Reads a line from the terminal with echo.
 * 
 * @param buffer Buffer to store the line
 * @param max_len Maximum length of the buffer
 * @param echo_stream Stream to echo the input
 * @return int Length of the read line
 */
int terminal_get_line_echo(char *buffer, size_t max_len, FILE* echo_stream);

/**
 * @brief Reads a line from the terminal without echo.
 * 
 * @param buffer Buffer to store the line
 * @param max_len Maximum length of the buffer
 * @return int Length of the read line
 */
int terminal_get_line(char *buffer, size_t max_len);

/**
 * @brief Clears the terminal screen.
 */
void terminal_clear_screen(void);

/**
 * @brief Resets the terminal cursor.
 */
void terminal_reset_cursor(void);

/**
 * @brief Disables the terminal cursor.
 */
void terminal_disable_cursor(void);

/**
 * @brief Enables the terminal cursor.
 */
void terminal_enable_cursor(void);

#endif // _TERMINAL_
