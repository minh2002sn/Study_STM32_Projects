#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "read_time.h"
#include "response.h"
#include "cli_type.h"
#include "cli_command_table.h"

void cli_command_execute(char *uart_buffer, uint8_t len);

#endif
