#ifndef HOST_BOOTLOADER_H
#define HOST_BOOTLOADER_H

extern "C"{
  #include "min.h"
}
#include "Arduino.h"
#include "bootloader_command.h"
#include "download_file.h"
#include "handle_hex_file.h"

void BOOTLOADER_Init();
void BOOTLOADER_Handle();

#endif
