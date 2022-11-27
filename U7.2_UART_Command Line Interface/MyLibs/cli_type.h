#ifndef CLI_TYPE_H
#define CLI_TYPE_H

#include "stdint.h"

typedef void (*cli_command_func_t)(char **argv, uint8_t arg_num);

typedef struct{
	cli_command_func_t function;
	char *help;
} cli_command_info_t;

typedef struct{
	const char *name;
	const cli_command_info_t *command_info;
}cli_command_entry_t;

#endif
