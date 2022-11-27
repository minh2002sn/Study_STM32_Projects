#include "flash.h"

void FLASH_Erase(uint32_t page){
	HAL_FLASH_Unlock();
	uint32_t PageError;
	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.Banks = 1;
	eraseInit.NbPages = 1;
	eraseInit.PageAddress = FIRST_PAGE_ADD + BYTE_PER_PAGE * page;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	HAL_FLASHEx_Erase(&eraseInit, &PageError);
	HAL_FLASH_Lock();
}

void FLASH_Write(uint32_t page, uint8_t *data, uint16_t len){
	uint32_t add = FIRST_PAGE_ADD + BYTE_PER_PAGE * page;
	HAL_FLASH_Unlock();
	for(int i = 0; i < len; i += 2){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, add + i, data[i] | (uint16_t)((i+1 >= len) ? 0xFF : data[i+1]) << 8);
	}
	HAL_FLASH_Lock();
}

void FLASH_Read(uint32_t add, uint8_t *data, uint16_t len){
	for(int i = 0; i < len; i += 2){
		uint16_t data_temp = *(volatile uint32_t *)(add + i);
		data[i] = data_temp;
		data[i+1] = data_temp >> 8;
	}
}
