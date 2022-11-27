#include "read_time.h"

void read_time(char **arg_value, uint8_t arg_num){
	if(arg_num > 4){
		response_print("Too much arguments [%d]", arg_num);
		return;
	} else if(arg_num < 4){
		response_print("Don't enough arguments [%d]", arg_num);
		return;
	}
	response_print("%dh : %dm %d day\n", atoi((const char *)arg_value[1]), atoi((const char *)arg_value[2]), atoi((const char *)arg_value[3]));
}
