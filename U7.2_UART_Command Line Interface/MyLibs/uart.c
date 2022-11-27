#include "uart.h"

#define UART_MAX_LEN 100

static uint8_t uart_buffer[UART_MAX_LEN];
static uint8_t uart_pointer = 0;
static uint8_t uart_flag = 0;

void UART_Receive(uint8_t Rx_Buffer){
	if(Rx_Buffer == '\n'){
		uart_buffer[uart_pointer++] = '\0';
		uart_flag = 1;
	} else{
		uart_buffer[uart_pointer++] = Rx_Buffer;
	}
}

void UART_Handle(){
	if(uart_flag){
		cli_command_execute((char *)uart_buffer, uart_pointer);
		uart_pointer = 0;
		uart_flag = 0;
	}
}
