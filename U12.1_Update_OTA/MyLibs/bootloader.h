#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#define	MIN_PORT	0
#define MIN_ID		1

#include "min.h"
#include "uart.h"
#include "bootloader_command.h"

void BOOTLOADER_Init();
void BOOTLOADER_Handle();
void BOOTLOADER_Request_Update();
void OTA_Send_Code(OTA_Code_Name_t p_code);
void OTA_Send_Response(OTA_Response_Name_t p_response);

#endif
