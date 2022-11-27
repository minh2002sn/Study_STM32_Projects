#include "download_file.h"
#include "host_bootloader.h"

void setup() {
  download_file();
  BOOTLOADER_Init();
}

void loop() {
  BOOTLOADER_Handle();
}
