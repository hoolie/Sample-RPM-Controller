/**
 * @file shell.c
 * @brief Shell interface implementation.
 */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <shell.h>
#include <stdlib.h>
#include "terminal.h"

/**
 * @brief Structure to hold a command and its handler.
 */
typedef struct {
    const char *commandName; /**< Command name */
    CommandHandler function; /**< Function pointer to the command handler */
} Command;

Command *commandList = NULL; /**< List of commands */
size_t commandCount = 0; /**< Number of commands */

/**
 * @brief Adds a command to the shell.
 * 
 * @param name Command name
 * @param command_handler Function pointer to the command handler
 * @return int 0 on success, -1 on failure
 */
int shell_add_command(const char *name, CommandHandler command_handler) {
    Command *newCommandList = (Command *)realloc(commandList, (commandCount + 1) * sizeof(Command));
    if (newCommandList == NULL) {
        return -1; // Memory allocation failed
    }

    // Update the command list and increment the count
    commandList = newCommandList;
    commandList[commandCount].commandName = name; // Assign the command name
    if (commandList[commandCount].commandName == NULL) {
        return -1; // Memory allocation failed
    }

    // Set the function pointer
    commandList[commandCount].function = command_handler;

    // Increment the command count
    commandCount++;

    return 0; // Success
}

/**
 * @brief Retrieves a command handler for a given command name.
 * 
 * @param input Command name
 * @return CommandHandler Function pointer to the command handler
 */
CommandHandler shell_get_command(const char *input) {
    for (size_t i = 0; i < commandCount; i++) {
        if (strcmp(input, commandList[i].commandName) == 0) {
            return commandList[i].function;
        }
    }
    printf(">>> Command not found: %s\n-> ", input);
    return NULL;
}

CommandHandler program_ptr = NULL; /**< Pointer to the current command handler */
char *programArgs = NULL; /**< Arguments for the current command */
char *programName = NULL; /**< Name of the current command */

/**
 * @brief Main shell loop function.
 */
void shell_loop() {
    if (program_ptr == NULL) {
        char current_command[COMMAND_BUFFER_SIZE];
        if (terminal_get_line(current_command, COMMAND_BUFFER_SIZE) != 0) {
            // Extract command name and arguments
            char *beginArgs = strchr(current_command, ' ');
            if (beginArgs != NULL) {
                *beginArgs = '\0';
                beginArgs++;
                programArgs = beginArgs;
            } else {
                programArgs = "";
            }

            programName = (char *)malloc(strlen(current_command) + 1);
            strcpy(programName, current_command);
            program_ptr = shell_get_command(programName);
        }
    }
    if (program_ptr != NULL) {
        if (program_ptr(programArgs, strlen(programArgs)) == EOF) {
            free(programName);
            program_ptr = NULL;
            programName = NULL;
            programArgs = NULL;
            printf("-> ");
        }
    }
}