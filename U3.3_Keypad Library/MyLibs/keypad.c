#include "Keypad.h"
//-----------keypad------------


const uint8_t key_code[KEYPAD_ROW][KEYPAD_COL] =
{ 
	{'1', '2', '3' , 'A'},
	{'4', '5', '6' , 'B'},
	{'7', '8', '9' , 'C'},
	{'*', '0', '#' , 'D'},
};
static uint8_t key_current;
static uint8_t key_last = 0;
static uint8_t key_debounce = 0;
static uint8_t debouncing =0;
static uint32_t t_debounce;
static uint32_t t_start_press;
static uint8_t is_press;


static GPIO_TypeDef *Keypad_RowPort[KEYPAD_ROW] = {GPIOA,GPIOA,GPIOA,GPIOA};
static uint16_t Keypad_RowPin[KEYPAD_ROW] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11};

static GPIO_TypeDef *Keypad_ColPort[KEYPAD_COL] = {GPIOB,GPIOB,GPIOB,GPIOB};
static uint16_t Keypad_ColPin[KEYPAD_COL] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};


__weak void KeypadPressingCallback(uint8_t key)
{

}
__weak void KeypadPressedShortCallback(uint8_t key)
{

}
__weak void KeypadReleaseCallback(uint8_t key)
{

}

__weak void KeypadPressingTimeoutCallback(uint8_t key)
{

}

void KeyPad_Select_Row(uint8_t row)
{
	HAL_GPIO_WritePin(Keypad_RowPort[row],Keypad_RowPin[row], GPIO_PIN_RESET);
}
void KeyPad_UnSelect_Row()
{
	for(uint8_t row=0; row < KEYPAD_ROW; row++)
	{
		HAL_GPIO_WritePin(Keypad_RowPort[row],Keypad_RowPin[row], GPIO_PIN_SET);
	}
}

static uint8_t KeyPad_GetKey()
{
	for(uint8_t row = 0; row < KEYPAD_ROW; row++)
	{
		KeyPad_UnSelect_Row();
		KeyPad_Select_Row(row);
		for(uint8_t col =0; col < KEYPAD_COL; col++)
		{
			if(HAL_GPIO_ReadPin(Keypad_ColPort[col],Keypad_ColPin[col]) == 0)
			{
				return key_code[row][col];
			}
		}
	}
	return 0;
}

static void Keypad_Filter()
{
	uint8_t key = KeyPad_GetKey();
	if(key != key_debounce)
	{
		debouncing = 1;
		t_debounce = HAL_GetTick();
		key_debounce = key;
	}
	//trang thai da xac lap
	if(debouncing && (HAL_GetTick() - t_debounce >= 15))
	{
		key_current = key_debounce;
		debouncing = 0;
	}
}

void Keypad_Handle()
{
	Keypad_Filter();
	
	if(key_current != key_last)
	{
		
		if(key_current != 0)
		{
			is_press = 1;
			t_start_press = HAL_GetTick();
			KeypadPressingCallback(key_current);
		}
		else
		{
			is_press = 0;
			if(HAL_GetTick() - t_start_press <=1000)//nhan nha nhanh
			{
				KeypadPressedShortCallback(key_last);
			}
			KeypadReleaseCallback(key_last);
		}
		key_last = key_current;
	}
	
	if(is_press && (HAL_GetTick() - t_start_press >= 3000)) //nhan giu phim
	{
		KeypadPressingTimeoutCallback(key_current);
		is_press = 0;
	}
}

void Keypad_Init()
{
	
}
