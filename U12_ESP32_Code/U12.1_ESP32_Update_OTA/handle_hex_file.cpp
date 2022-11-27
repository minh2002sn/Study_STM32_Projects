#include "handle_hex_file.h"

uint8_t char_to_byte(char p_char){
    if(p_char >= '0' && p_char <= '9') return p_char - '0';
    if(p_char >= 'A' && p_char <= 'F') return p_char - 'A' + 10;
    if(p_char >= 'a' && p_char <= 'f') return p_char - 'a' + 10;
    return -1;
}

void convert_string_to_hex_array(char *p_input, uint8_t *p_output){
    uint8_t t_index = 0;
    if(*p_input == ':'){
        p_input++;
        while(*p_input != '\0'){
            uint8_t t_hex_value = char_to_byte(*(p_input++)) << 4;
            t_hex_value |= char_to_byte(*(p_input++));
            p_output[t_index++] = t_hex_value;
        }
    }
}

int8_t checksum(uint8_t *p_buffer, uint8_t p_len){
    uint8_t t_sum = 0;
    for(int i = 0; i < p_len - 1; i++){
        t_sum += p_buffer[i];
    }
    t_sum = ~t_sum + 1;
    if(t_sum == p_buffer[p_len - 1]){
        return 0;
    } else{
        return -1;
    }
}

void swap(uint8_t *p_data_1, uint8_t *p_data_2){
    uint8_t t_data = *p_data_1;
    *p_data_1 = *p_data_2;
    *p_data_2 = t_data;
}

void swap_4_bytes(uint8_t *p_data, uint8_t p_len){
    for(int i = 0; i < p_len - 1; i += 4){
        swap(&p_data[i], &p_data[i + 3]);
        swap(&p_data[i + 1], &p_data[i + 2]);
    }
}

void print_data(uint8_t *p_data, uint8_t p_len){
    for(int i = 0; i < p_len - 1; i++){
        printf("%02X ", p_data[i]);
    }
    printf("\n");
}
