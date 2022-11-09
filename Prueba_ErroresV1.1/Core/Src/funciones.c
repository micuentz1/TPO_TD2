#include "funciones.h"
#include "liquidcrystal_i2c.h"
#include "OneWire.h"

void InitAll(void) {

	HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, ON); // Sistema Prendido y funcionando

	get_ROMid();

	HD44780_Init(2);
	HD44780_NoCursor();
	HD44780_Clear();
}
