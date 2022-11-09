
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

/**
*	\fn
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/


void Tarea_Lectura( void *p ) {

	TickType_t xLastWakeTime;

	while(1) {

		xLastWakeTime = xTaskGetTickCount();

		get_Temperature();

		//vTaskDelayUntil(&xLastWakeTime,ONE_SEC);
		vTaskDelayUntil(&xLastWakeTime, 1000);

	}

	vTaskDelete(NULL); // En caso de romperse el loop
}


void Error_Sensores( void *p ) {

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

void Boton_Ok(void *p)
{

	while(1)
	{
		if(HAL_GPIO_ReadPin(BOTON_OK_GPIO_Port,BOTON_OK_Pin))
		{
			estado_OK = 1;
		}
	}

	vTaskDelete(NULL);
}
