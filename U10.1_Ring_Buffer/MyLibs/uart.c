#include "uart.h"

#define UART_MAX_LEN 64

static uint8_t uart_buffer[UART_MAX_LEN];
static uint8_t uart_pointer = 0;
static uint8_t uart_flag = 0;
RING_BUFFER_HandleTypeDef uart_ring_buffer;

void UART_Init(){
	RING_BUFFER_Init(&uart_ring_buffer, uart_buffer, UART_MAX_LEN);
}

void UART_Receive(uint8_t Rx_Buffer){
	RING_BUFFER_Push(&uart_ring_buffer, Rx_Buffer);
}

void UART_Handle(){
	if(uart_flag){
		uart_pointer = 0;
		uart_flag = 0;
	}
}

uint8_t UART_Read(){
	uint8_t t_data;
	RING_BUFFER_Pop(&uart_ring_buffer, &t_data);
	return t_data;
}

uint16_t UART_Available(){
	return RING_BUFFER_Availble(&uart_ring_buffer);
}
