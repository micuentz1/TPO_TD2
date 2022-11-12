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

void chequeo_inicio()
{
	HD44780_Clear();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Iniciando");
	HD44780_SetCursor(0,1);
	HD44780_PrintStr("Aguarde...");

	HAL_Delay(ONE_SEC);
}

