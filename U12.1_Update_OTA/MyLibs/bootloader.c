#include "bootloader.h"

struct min_context min_ctx;

typedef enum{
	OTA_IDLE_STATE,
	OTA_START_STATE,
	OTA_RECEIVING_INFO_STATE,
	OTA_RECEIVING_DATA_STATE,
	OTA_END_STATE,
}OTA_State_t;
OTA_State_t ota_state;

void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, uint8_t port){
	switch(ota_state){
		case OTA_IDLE_STATE:
		{
			OTA_Code_t *ota_code;
			ota_code = (OTA_Code_t *)min_payload;
			if(ota_code->command_id == OTA_CODE && ota_code->len == 1 && ota_code->code == START_CODE){
				ota_state = OTA_START_STATE;
				OTA_Send_Response(ACK);
			} else{
				OTA_Send_Response(NACK);
			}
			break;
		}
		case OTA_START_STATE:
		{
			OTA_Info_t *t_info = (OTA_Info_t *)min_payload;
			if(t_info->command_id == OTA_INFO){
				// Save name and version to flash
				ota_state = OTA_RECEIVING_INFO_STATE;
				OTA_Send_Response(ACK);
			} else{
				OTA_Send_Response(NACK);
			}
			break;
		}
		case OTA_RECEIVING_INFO_STATE:
		{
			OTA_Data_t *t_data = (OTA_Data_t *)min_payload;
			if(t_data->command_id == OTA_DATA){
				// Save data to flash
				ota_state = OTA_RECEIVING_DATA_STATE;
				OTA_Send_Response(ACK);
			} else {
				ota_state = OTA_IDLE_STATE;
				OTA_Send_Response(NACK);
			}
			break;
		}
		case OTA_RECEIVING_DATA_STATE:
		{
			OTA_Data_t *t_data = (OTA_Data_t *)min_payload;
			if(t_data->command_id == OTA_DATA){
				// Save data to flash
				OTA_Send_Response(ACK);
			} else if(t_data->command_id == OTA_CODE){
				OTA_Code_t *t_code = (OTA_Code_t *)min_payload;
				if(t_code->code == END_CODE){
					ota_state = OTA_END_STATE;
					OTA_Send_Response(ACK);
				} else{
					OTA_Send_Response(NACK);
				}
			} else{
				OTA_Send_Response(NACK);
			}
			break;
		}
		case OTA_END_STATE:
			ota_state = OTA_END_STATE;
//			run_app();
			break;
	}
}
void BOOTLOADER_Init(){
	min_init_context(&min_ctx, MIN_PORT);
}

void BOOTLOADER_Handle(){
	uint8_t data;
	uint8_t len = 0;
	if(UART_Available() > 0){
		data = UART_Read();
		len = 1;
	}
	min_poll(&min_ctx, &data, len);
}

void BOOTLOADER_Request_Update(){

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
