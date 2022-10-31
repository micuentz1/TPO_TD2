#include "funciones.h"
#include "liquidcrystal_i2c.h"
#include "OneWire.h"

void InitAll(void) {

	HAL_GPIO_WritePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin, ON); // Sistema Prendido y funcionando

	get_ROMid();

	HD44780_Init(2);
	HD44780_NoCursor();
	HD44780_Clear();
}
