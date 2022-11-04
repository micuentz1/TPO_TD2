#include "funciones.h"
#include "liquidcrystal_i2c.h"
#include "OneWire.h"

void InitAll(void) {

	HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, ON); // Sistema Prendido y funcionando

	get_ROMid();

	HD44780_Init(2);
	HD44780_NoCursor();
	HD44780_Clear();
	HD44780_SetCursor(0,0);
}

/*
 *
  if(f_mount(&fs, "", 0)) lectura_SD = 0; // Si no se monta la SD pedir que se reinicie el sistema

  if(f_open(&fil, "Sensores.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ)) lectura_SD = 0; // Da error si no hay tarjeta, pantalla error SD

  f_gets(buffer_SD, 4, &fil);

  if(f_truncate(&fil)) lectura_SD = 0;  // Si se desconecto la tarjeta o hay error, mandar a ventana error SD

  if(f_lseek(&fil, 0)) lectura_SD = 0;  // Si se desconecto la tarjeta o hay error, mandar a ventana error SD

  escritura_SD[0] = 117; // 0 - 48     1 - 49
  escritura_SD[1] = 119;
  escritura_SD[2] = 117;
  f_puts(escritura_SD, &fil);

  if(f_close(&fil)) lectura_SD = 0;  // Si se desconecto la tarjeta o hay error, mandar a ventana error SD
 */
