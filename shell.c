#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <shell.h>
#include <stdlib.h>
#define COMMAND_BUFFER_SIZE 128
char command_buffer[COMMAND_BUFFER_SIZE];
uint8_t command_buffer_index = 0;

int get_line(char *buffer, size_t max_len)
{
    return get_line_echo(buffer, max_len, stdout);
}

int get_line_echo(char *buffer, size_t max_len, FILE *echo_stream)
{
    int next = getchar();

    while (next != EOF)
    {
        putc(next, echo_stream);
        if (next == '\n')
        {
            command_buffer[command_buffer_index] = '\0';
            strncpy(buffer, command_buffer, max_len);
            command_buffer_index = 0;
            return strlen(buffer);
        }
        else
        {
            command_buffer[command_buffer_index] = (char)next;
            command_buffer_index = (command_buffer_index + 1) % COMMAND_BUFFER_SIZE;
        }
        next = getchar();
    }
    return 0;
}
enum SHELL_STATE
{
    SHELL,
    PROGRAM
};
typedef struct
{
    const char *commandName;
    CommandHandler function;
} Command;
Command *commandList = NULL;
size_t commandCount = 0;
int AddCommand(const char *name, CommandHandler command_handler)
{
    Command *newCommandList = (Command *)realloc(commandList, (commandCount + 1) * sizeof(Command));
    if (newCommandList == NULL)
    {
        return -1; // Memory allocation failed
    }

    // Update the command list and increment the count
    commandList = newCommandList;

    // Allocate memory for the command name
    commandList[commandCount].commandName = name; //(char*)malloc(strlen(name) + 1);
    if (commandList[commandCount].commandName == NULL)
    {
        return -1; // Memory allocation failed
    }

    // Copy the command name and set the function pointer
    //strcpy(commandList[commandCount].commandName, name);
    commandList[commandCount].function = command_handler;

    // Increment the command count
    commandCount++;

    return 0; // Success
}

CommandHandler GetCommand(const char *input)
{
    for (size_t i = 0; i < commandCount; i++)
    {
        if (strcmp(input, commandList[i].commandName) == 0)
        {
            return commandList[i].function;
        }
    }
    printf(">>> Command not found: %s\n-> ", input);
    return NULL;
}
CommandHandler program_ptr = NULL;
char *programArgs = NULL;
char *programName = NULL;
void loop()
{
    if (program_ptr == NULL)
    {
        char current_command[COMMAND_BUFFER_SIZE];
        if (get_line(current_command, COMMAND_BUFFER_SIZE) != 0) {
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
            program_ptr = GetCommand(programName);
        }
    }
    if (program_ptr != NULL)
        if (program_ptr(programArgs, strlen(programArgs)) == EOF)
        {
            free(programName);
            program_ptr = NULL;
            programName = NULL;
            programArgs = NULL;
            printf("-> ");
        }
}
