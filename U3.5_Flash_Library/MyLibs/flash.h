#ifndef FLASH_H
#define FLASH_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

#define FIRST_PAGE_ADD 0x08000000
#define BYTE_PER_PAGE  1024

void FLASH_Erase(uint32_t page);
void FLASH_Write(uint32_t page, uint8_t *data, uint16_t len);
void FLASH_Read(uint32_t add, uint8_t *data, uint16_t len);

#endif
