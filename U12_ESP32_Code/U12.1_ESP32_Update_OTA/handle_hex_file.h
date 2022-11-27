#ifndef HANDLE_HEX_FILE_H
#define HANDLE_HEX_FILE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "download_file.h"

uint8_t char_to_byte(char p_char);
void convert_string_to_hex_array(char *p_input, uint8_t *p_output);
int8_t checksum(uint8_t *p_buffer, uint8_t p_len);
void swap(uint8_t *p_data_1, uint8_t *p_data_2);
void swap_4_bytes(uint8_t *p_data, uint8_t p_len);
void print_data(uint8_t *p_data, uint8_t p_len);

#endif