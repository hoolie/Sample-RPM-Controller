/**
 * @file shell.h
 * @brief Header file for the shell interface.
 */

#ifndef _SHELL_
#define _SHELL_

#include <avr/io.h>
#include <string.h>

/**
 * @brief Type definition for command handler functions.
 */
typedef int (*CommandHandler)(char* buffer, size_t length);

/**
 * @brief Adds a command to the shell.
 * 
 * @param name Command name
 * @param command_handler Function pointer to the command handler
 * @return int 0 on success, -1 on failure
 */
int shell_add_command(const char *name, CommandHandler command_handler);

/**
 * @brief Main shell loop function.
 */
void shell_loop(void);

#endif // _SHELL_