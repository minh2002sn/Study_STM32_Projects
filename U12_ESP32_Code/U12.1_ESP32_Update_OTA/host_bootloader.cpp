#include "host_bootloader.h"

#define MIN_PORT    0
#define MIN_ID      1
#define MIN_Serial  Serial2

struct min_context min_ctx;

extern char hex_file[];

typedef enum{
  OTA_IDLE_STATE,
  OTA_START_STATE,
  OTA_SENDING_INFO_STATE,
  OTA_SENDING_DATA_STATE,
  OTA_END_STATE,
}OTA_State_t;
OTA_State_t ota_state;

void BOOTLOADER_Init(){
  min_init_context(&min_ctx, MIN_PORT);
  MIN_Serial.begin(115200);
  pinMode(2, OUTPUT);
  ota_state = OTA_IDLE_STATE;
}

void BOOTLOADER_Handle(){
  uint8_t t_data;
  uint8_t t_len = 0;
  if(MIN_Serial.available() > 0){
    t_data = MIN_Serial.read();
    t_len = 1;
  }
  min_poll(&min_ctx, &t_data, t_len);
}

void BOOTLOADER_Send_Data(void *p_data, uint8_t p_len){
  min_send_frame(&min_ctx, MIN_ID, (uint8_t *)p_data, p_len);
}

void OTA_Send_Code(OTA_Code_Name_t p_code){
  OTA_Code_t t_cmd;
  t_cmd.command_id = OTA_CODE;
  t_cmd.len = 1;
  t_cmd.code = p_code;
  BOOTLOADER_Send_Data(&t_cmd, sizeof(t_cmd));
}

void OTA_Send_Response(OTA_Response_Name_t p_response){
  OTA_Response_t t_response;
  t_response.command_id = OTA_RESPONSE;
  t_response.len = 1;
  t_response.ack = p_response;
  BOOTLOADER_Send_Data(&t_response, sizeof(t_response));
}

void OTA_Send_Info(char p_name[50], char p_version[5]){
  OTA_Info_t t_info;
  t_info.command_id = OTA_INFO;
  t_info.len = sizeof(t_info.name) + sizeof(t_info.version);
  strcpy((char *)&t_info.name, p_name);
  strcpy((char *)&t_info.version, p_version);
  BOOTLOADER_Send_Data(&t_info, sizeof(t_info));
}

void OTA_Send_Data(uint8_t *p_data, uint8_t p_len){
  OTA_Data_t t_data;
  t_data.command_id = OTA_DATA;
  t_data.len = p_len;
  memcpy(&t_data.data, p_data, p_len);
  BOOTLOADER_Send_Data(&t_data, sizeof(t_data));
}

void min_application_handler(uint8_t min_id, uint8_t const *min_payload, 
                                      uint8_t len_payload, uint8_t port){
  uint8_t t_hex_data[21] = {};
  switch(ota_state){
    case OTA_IDLE_STATE:
    {
      OTA_Code_t *t_ota_code = (OTA_Code_t *)min_payload;
      if(t_ota_code->command_id == OTA_CODE && t_ota_code->code == REQUEST_CODE){
        ota_state = OTA_START_STATE;
        OTA_Send_Code(START_CODE);
      } else{
        ota_state = OTA_IDLE_STATE;
      }
      break;
    }
    case OTA_START_STATE:
    {
      OTA_Response_t *t_ota_response = (OTA_Response_t *)min_payload;
      if(t_ota_response->command_id == OTA_RESPONSE && t_ota_response->ack == ACK){
        OTA_Send_Info("OTA_Test", "1.1.0");
      } else{
        ota_state = OTA_IDLE_STATE;
      }
      break;
    }
    case OTA_SENDING_INFO_STATE:
    {
      OTA_Response_t *t_ota_response = (OTA_Response_t *)min_payload;
      if(t_ota_response->command_id == OTA_RESPONSE && t_ota_response->ack == ACK){
        char *t_token = strtok(hex_file, "\n");
        convert_string_to_hex_array(t_token, t_hex_data);
        if(checksum(t_hex_data, t_hex_data[0] + 5) == -1){
          ota_state = OTA_END_STATE;
          return;
        }
        swap_4_bytes(&t_hex_data[4], t_hex_data[0]);
        OTA_Send_Data(&t_hex_data[4], t_hex_data[0]);
      } else{
        ota_state = OTA_IDLE_STATE;
      }
      break;
    }
    case OTA_SENDING_DATA_STATE:
    {
      OTA_Response_t *t_ota_response = (OTA_Response_t *)min_payload;
      if(t_ota_response->command_id == OTA_RESPONSE && t_ota_response->ack == ACK){
        char *t_token = strtok(hex_file, "\n");
        if(t_token != NULL){
          convert_string_to_hex_array(t_token, t_hex_data);
          if(t_hex_data[3] == 0x00){
            if(checksum(t_hex_data, t_hex_data[0] + 5) == -1){
              ota_state = OTA_END_STATE;
              return;
            } else{
              OTA_Send_Code(END_CODE);
              ota_state = OTA_END_STATE;
            }
            swap_4_bytes(&t_hex_data[4], t_hex_data[0]);
            OTA_Send_Data(&t_hex_data[4], t_hex_data[0]);
          } else{
            OTA_Send_Code(END_CODE);
            ota_state = OTA_END_STATE;
          }
        } else{
          OTA_Send_Code(END_CODE);
          ota_state = OTA_END_STATE;
        }
      } else{
        ota_state = OTA_IDLE_STATE;
      }
      break;
    }
    case OTA_END_STATE:
    {
      OTA_Response_t *t_ota_response = (OTA_Response_t *)min_payload;
      if (t_ota_response->command_id == OTA_RESPONSE && t_ota_response->ack == ACK)
      {
        ota_state = OTA_IDLE_STATE;
      }
      break;
    }
    default:
      break;
  }
}

