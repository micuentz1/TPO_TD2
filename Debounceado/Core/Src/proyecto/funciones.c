#include "funciones.h"
#include "liquidcrystal_i2c.h"
#include "OneWire.h"
#include "main.h"

extern uint8_t bufferTeclado;

void InitAll(void) {

	HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, ON); // Sistema Prendido y funcionando

	get_ROMid();

	InitLCD();
}

void InitLCD()
{
	HD44780_Init(2);
	HD44780_NoCursor();
	HD44780_SetBacklight(100);
	HD44780_Clear();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Iniciando");
	HD44780_SetCursor(0,1);
	HD44780_PrintStr("Aguarde...");

	HAL_Delay(500);
}

// ************************ Teclado ************************************************************

char getKey(void)
{
	uint8_t aux = NO_KEY;

	if (bufferTeclado != NO_KEY)
	{
		aux = bufferTeclado;
		bufferTeclado = NO_KEY;
	}
	return aux;
}

char BarridoTeclado(void)
{
	if(!HAL_GPIO_ReadPin(BOTON_OK_GPIO_Port,BOTON_OK_Pin)) 				return BOTON_OK;
	else if(!HAL_GPIO_ReadPin(BOTON_SEL_GPIO_Port,BOTON_SEL_Pin))		return BOTON_SEL;
	else if(!HAL_GPIO_ReadPin(BOTON_CANCEL_GPIO_Port,BOTON_CANCEL_Pin))	return BOTON_CANCEL;
	else																return NO_KEY;
}

void DriverTeclado(void *p)
{
	static uint8_t codigoAnterior = NO_KEY;
	static uint8_t codigoActual = NO_KEY;
	static uint8_t estado = ESPERANDO_TECLA;
	static uint8_t contTeclaEstable = 0;

	while(1)
	{
		codigoActual = BarridoTeclado();

		switch (estado)
		{
			case ESPERANDO_TECLA:
					if (codigoActual != NO_KEY)
					{
						codigoAnterior = codigoActual;
						contTeclaEstable = 0;
						estado = VALIDAR_TECLA;
					}
					break;
			case VALIDAR_TECLA:
					if (codigoActual != codigoAnterior)
					{
						estado = ESPERANDO_TECLA;
					}
					else
					{
						contTeclaEstable++;
						if (contTeclaEstable == CANT_PARA_VALIDAR)
						{
							bufferTeclado = codigoActual;
							estado = TECLA_PRESIONADA;
						}
					}
					break;
			case TECLA_PRESIONADA:
					if (codigoActual != codigoAnterior)
					{
						estado = ESPERANDO_TECLA;
					}
					break;
			default:
					estado = ESPERANDO_TECLA;
					break;
		}
		vTaskDelay(10);
	}
	vTaskDelete(NULL); // En caso de romperse el loop
}

// ************************** Timer **********************************

void TimerCafe(void *p)
{
	TickType_t 	xLastWakeTime;
	int		 	contador_sec = TIEMPO_CAFE_SEC;
	int		 	contador_min = TIEMPO_CAFE_MIN;
	uint8_t 	estado = ESPERANDO;
	char		time_display_sec[3];
	char		time_display_min[3];

	while(1) {

		xLastWakeTime = xTaskGetTickCount();

		switch(estado)
		{
			case ESPERANDO:
				if(estado_Display == PREPARACION)
				{
					estado = COUNTDOWN;
				}
				break;

			case COUNTDOWN:
				itoa(contador_min, time_display_min, 10);
				HD44780_SetCursor(10,0);
				HD44780_PrintStr(time_display_min);
				HD44780_SetCursor(13,0);
				HD44780_PrintStr(":");
				itoa(contador_sec, time_display_sec, 10);
				HD44780_SetCursor(14,0);
				HD44780_PrintStr(time_display_min);

				HD44780_SetCursor(0,1);
				HD44780_Cursor();
				HD44780_Blink();

				if(contador_sec) contador_sec--;
				else if(contador_min)
				{
					contador_sec = TIEMPO_CAFE_SEC;
					contador_min--;
				}

				if( (contador_sec == -1 && contador_min == -1) || (estado_Display != PREPARACION) )
				{
					estado = ESPERANDO;
				}
				break;

		}


		vTaskDelayUntil(&xLastWakeTime, TIEMPO_CAFE_MS);

	}

	vTaskDelete(NULL); // En caso de romperse el loop
}
