
/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/

#include "cmsis_os.h"
#include "funciones.h"
#include "OneWire.h"
#include "MdE.h"
#include "liquidcrystal_i2c.h"


/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/

extern float Temp[MAXDEVICES_ON_THE_BUS];

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/
uint8_t estado_OK;
uint8_t estado_Cancel;
uint8_t estado_Sel;
uint8_t fin_preparacion;
uint8_t cursor=0;
uint8_t estado_Display;
uint8_t cafe_programado = 0;

uint8_t bufferTeclado = NO_KEY;

/**
*	\fn
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/

void Tarea_Lectura( void *p )
{
	TickType_t xLastWakeTime;

	while(1) {

		xLastWakeTime = xTaskGetTickCount();

		get_Temperature();

		//vTaskDelayUntil(&xLastWakeTime,ONE_SEC);
		vTaskDelayUntil(&xLastWakeTime, 1000);

	}

	vTaskDelete(NULL); // En caso de romperse el loop
}


void Error_Sensores( void *p )
{
	float dif=0, dif_s0=0, dif_s1=0, auxTemp[2] = {0, 0};

	while(1) {

		dif = Temp[1] - Temp[0];

		// Chequeamos que los sensores funcionen bien
		if( (dif>MAX_DIF_P) || (dif<MAX_DIF_N) ) {
			HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, OFF);
			HAL_GPIO_WritePin(RED_PORT, RED_PIN, ON);

		    HD44780_PrintStr("Error en");
		    HD44780_SetCursor(10,1);
		    HD44780_PrintStr("Sensores");
		    //vTaskDelay(2000); // Para que estaba

			//f_Error = 1; Comento porque de momento no se usa

			// En base a si la dif es positiva o negativa sabemos cual sensor fallo
		}

		vTaskDelay(ONE_SEC); // Tiene que ser 1s
	}

	vTaskDelete(NULL); // En caso de romperse el loop
}


void Error_Anafe(void *p)
{
	float dif_s0=0, dif_s1=0, auxTemp[2] = {0, 0};

	while(1)
	{

		dif_s0 = Temp[0]-auxTemp[0];
		dif_s1 = Temp[1]-auxTemp[1];

		auxTemp[0] = Temp[0];
		auxTemp[1] = Temp[1];
		// Chequeamos que el anafe funcione bien
		if( (dif_s0<MIN_DIF_CALENTAMIENTO) || (dif_s1<MIN_DIF_CALENTAMIENTO) ) {
			HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, OFF);
			HAL_GPIO_WritePin(RED_PORT, RED_PIN, ON);

		    HD44780_PrintStr("Error en");
		    HD44780_SetCursor(10,1);
		    HD44780_PrintStr("Anafe");
		    //vTaskDelay(2000); // Para que estaba

			//f_Error = 1; Comento porque de momento no se usa

		}

		vTaskDelay(5*ONE_SEC); // Que mida cada 5 secs
	}

	vTaskDelete(NULL); // En caso de romperse el loop
}



void MdE_Display(void *p)
{
	// Inicialmente debe saber si hay cafe programado o no
	if(cafe_programado) estado_Display = PROGRAMADO;
	else 				estado_Display = MENU_PPAL;

	print_Display(estado_Display);

	while(1)
	{
		switch(estado_Display)
		{
			case PROGRAMADO:
				if(getKey() == BOTON_OK)
				{
					estado_Display = MENU_PPAL;
					print_Display(estado_Display);
				}

			case MENU_PPAL:
				switch(getKey())
				{
					case BOTON_SEL:
						if(cursor == 0)
						{
							HD44780_Cursor();
							HD44780_SetCursor(0,1);
							HD44780_Blink();
							cursor = 1;
						}
						else if (cursor == 1)
						{
							HD44780_Cursor();
							HD44780_SetCursor(0,0);
							HD44780_Blink();
							cursor = 0;
						}
						break;

					case BOTON_OK:
						if(cursor == 0) estado_Display = PREPARACION;
						if(cursor == 1) estado_Display = SET_HORA;

						print_Display(estado_Display);
						break;
				}
				break;
//			case SEGURO:
//				if(estado_OK)
//				{
//					estado_Display = PREPARACION;
//					print_Display(estado_Display);
//					vTaskDelay(500);
//				}
//				if(estado_Cancel)
//				{
//					estado_Display = MENU_PPAL;
//					print_Display(estado_Display);
//					vTaskDelay(500);
//				}
//				break;
			case PREPARACION:
				if(getKey() == BOTON_OK) // || fin_preparacion
				{
					estado_Display = LISTO;
					fin_preparacion = 0;
					print_Display(estado_Display);
				}
				break;
			case LISTO:
				if(getKey() == BOTON_OK)
				{
					estado_Display = MENU_PPAL;
					print_Display(estado_Display);
				}
				break;
			default:
				estado_Display = MENU_PPAL;
				break;

		}

		vTaskDelay(500);
	}

	vTaskDelete(NULL);
}

void print_Display(uint8_t estado)
{
	switch (estado)
	{
		case PROGRAMADO:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Cafe para 66:66");
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Cancelar");
			HD44780_SetCursor(0,1);
			HD44780_Cursor();
			HD44780_Blink();
			break;
		case MENU_PPAL:
			HD44780_Clear();
			HD44780_SetCursor(2,0);
			HD44780_PrintStr("Cafe Rapido");
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Programar Cafe");
			HD44780_SetCursor(0,0);
			HD44780_Cursor();
			HD44780_Blink();
			break;
		case SEGURO:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Esta seguro?");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("OK:SI-CANCEL:NO");
			break;
		case PREPARACION:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Restante: ");
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Cancelar");
			HD44780_SetCursor(0,1);
			HD44780_Cursor();
			HD44780_Blink();
			break;
	}
}