#ifndef BOOTLOADER_COMMAND_H
#define BOOTLOADER_COMMAND_H

#include "stdint.h"

enum
{
  OTA_CODE,
  OTA_INFO,
  OTA_DATA,
  OTA_RESPONSE,
};

typedef enum
{
  START_CODE,
  END_CODE,
  REQUEST_CODE,
}OTA_Code_Name_t;

typedef enum
{
  ACK,
  NACK,
}OTA_Response_Name_t;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t code;
} __attribute__((packed)) OTA_Code_t;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t ack;
} __attribute__((packed)) OTA_Response_t;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t name[51];
  uint8_t version[10];
} __attribute__((packed)) OTA_Info_t;

typedef struct{
  uint8_t command_id;
  uint8_t len;
  uint8_t data[16];
} __attribute__((packed)) OTA_Data_t;


#endif
