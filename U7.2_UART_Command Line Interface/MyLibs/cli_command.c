#include "cli_command.h"

extern const cli_command_entry_t command_entry_table[];

const cli_command_info_t *find_command_info(char *cmd){
	const cli_command_entry_t *command_entry = command_entry_table;

	while(command_entry->name != NULL){
		if(strcmp(command_entry->name, cmd) == 0){
			return command_entry->command_info;
		}
		command_entry++;
	}
	return NULL;
}

void cli_command_execute(char *uart_buffer, uint8_t len){
	char *arg_value[10];
	uint8_t arg_num = 0;

	char *token = strtok((char *)uart_buffer, " ");
	while(token != NULL){
		arg_value[arg_num++] = token;
		token = strtok(NULL, " ");
	}

	const cli_command_info_t *command_info = find_command_info(arg_value[0]);
	if(command_info != NULL){
		command_info->function(arg_value, arg_num);
	} else{
		response_print("Cannot find command.");
	}
}
