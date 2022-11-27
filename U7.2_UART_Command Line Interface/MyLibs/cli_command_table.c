#include "cli_command_table.h"

const cli_command_info_t command_info_read_time =
{
		read_time,
		"Read time"
};

const cli_command_entry_t command_entry_table[] =
{
		{"TIME", &command_info_read_time},
		{NULL, NULL}
};
