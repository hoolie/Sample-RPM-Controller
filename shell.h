#ifndef _SHELL_
#define _SHELL
#include <avr/io.h>
#include <string.h>
int get_line_echo(char *buffer, size_t max_len, FILE* echo_stream);
int get_line(char *buffer, size_t max_len);

typedef int (*CommandHandler)(char* buffer, uint8_t length);
#endif