#include "port_min.h"

uint32_t min_time_ms(void){
  return millis();
}

uint16_t min_tx_space(uint8_t port){
  return 512;
}

void min_tx_byte(uint8_t port, uint8_t byte){
  Serial.write(byte);
}

void min_tx_start(uint8_t port){

}

void min_tx_finished(uint8_t port){

}
