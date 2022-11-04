
/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/

#include "OneWire.h"
#include "funciones.h"
#include "MdE.h"
#include "liquidcrystal_i2c.h"

/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/
extern float Temp[MAXDEVICES_ON_THE_BUS];

uint8_t f_Error=0;

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

/**
*	\fn
*	\brief   Control de Errores
*	\details Lo que hice fue separar el control de error en los sensores y anafe. Cada tarea chequea uno de los dos. En caso de detectarlos, cambia el led e imprime
*	en LCD. La lógica de cómo se detecta el error no la cambié. Agregué defines para los leds, no sé si hacía falta pero lo hice xd.
*	En el main lo único que cambié fue agregar un xTaskCreate y que prenda el led verde.
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/

void Error_Sensores( void *p ) {
	float dif=0, dif_s0=0, dif_s1=0, auxTemp[2] = {0, 0};

	while(1) {

		get_Temperature();

		dif = Temp[1] - Temp[0];

		// Chequeamos que los sensores funcionen bien
		if( (dif>MAX_DIF_P) || (dif<MAX_DIF_N) ) {
			HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, OFF);
			HAL_GPIO_WritePin(RED_PORT, RED_PIN, ON);

		    HD44780_PrintStr("Error en");
		    HD44780_SetCursor(10,1);
		    HD44780_PrintStr("Sensores");
		    vTaskDelay(2000);

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
		get_Temperature();

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
		    vTaskDelay(2000);

			//f_Error = 1; Comento porque de momento no se usa

		}

		vTaskDelay(5*ONE_SEC); // Que mida cada 5 secs
	}

	vTaskDelete(NULL); // En caso de romperse el loop
}
