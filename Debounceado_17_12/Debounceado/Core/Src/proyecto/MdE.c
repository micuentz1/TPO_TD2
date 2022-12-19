
/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/

#include <stdio.h>
#include "cmsis_os.h"
#include "funciones.h"
#include "OneWire.h"
#include "MdE.h"
#include "liquidcrystal_i2c.h"
//#include "handlers.h"


/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/

extern float Temp[MAXDEVICES_ON_THE_BUS];
extern uint8_t componentes[3];
extern uint8_t cafe_programado;
extern TaskHandle_t Handler_Lectura;
extern TaskHandle_t Handler_Sensores;
extern TaskHandle_t Handler_Anafe;
extern TaskHandle_t Handler_Timer;
/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

uint8_t fin_preparacion;
uint8_t cursor=0;
uint8_t faseFinal=0;

extern RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef horaActual, horaProgramada, horaInicio;
char horaStr[3], minutoStr[3];

uint8_t esLaHora=0;  // ?????


uint8_t bufferTeclado = NO_KEY;

uint8_t estado_comp = TODO_OK;

uint32_t tiempoPreparacion = 5;


/**
*	\fn
*	\brief   Máquina de estados principal
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/

void Tarea_Lectura( void *p )
{
	vTaskSuspend(NULL);   // La tarea inicia y se suspende hasta que la resuma la MdE ppal.

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
	vTaskSuspend(NULL); // La tarea inicia y se suspende hasta que la resuma la MdE ppal.

	float dif=0;//, dif_s0=0, dif_s1=0, auxTemp[2] = {0, 0};

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
	vTaskSuspend(NULL); // La tarea inicia y se suspende hasta que la resuma la MdE ppal.

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

void Tarea_Timer(void *p)
{
	vTaskSuspend(NULL);

	TickType_t xLastWakeTime;

	HAL_RTC_GetTime(&hrtc, &horaInicio, RTC_FORMAT_BIN);

	xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime,1000);

		HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);
		if((horaActual.Seconds >= horaInicio.Seconds) && (horaActual.Minutes == horaInicio.Minutes+tiempoPreparacion))
		{
			// Terminó
			//Supongo que se avisará con un flag
			//vTaskSuspend(NULL);
		}
	}
}

void MdE_Principal(void *p)
{
	static uint8_t estado_MdE = PRENDIENDO;

	while(1)
	{
		switch(estado_MdE)
		{
			case PRENDIENDO:

				if(estado_comp == TODO_OK)
				{
					if(cafe_programado)
					{
						estado_MdE = PROGRAMADO;
						print_Display(DIS_PROGRAMADO);
					}
					else
					{
						estado_MdE = MENU_PPAL;
						print_Display(DIS_MENU_PPAL);
						cursor = 0;
					}

				}
				else if(estado_comp == FALTA_SD)
				{
					print_Display(DIS_FALTA_SD);
					// Lea de nuevo
				}
				else if(estado_comp >1) // Identificar error
				{
					estado_MdE = ERROR;
					print_Display(estado_comp + 4); // La organizacion de la enumeracion
				}
				break;

			case PROGRAMADO: // Esperando a que sea la hora. Si se presiona el boton se cancela y se vuelve al menu ppal.

				HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);

				if( getKey() == BOTON_OK )
				{
					// Escribir en SD que no hay ninguno programado
					estado_MdE = MENU_PPAL;
					print_Display(DIS_MENU_PPAL);
					cursor = 0;
				}
				else if( (horaActual.Hours == horaProgramada.Hours) && (horaActual.Minutes == horaProgramada.Minutes) )
				{
					SetRelay(ON);
					SetLED(ON, AMARILLO);

					vTaskResume(Handler_Lectura); // Activar tarea medicion de temperatura
					vTaskResume(Handler_Sensores);
					vTaskResume(Handler_Anafe);// Activar tareas medicion de errores
					vTaskResume(Handler_Timer);// Activar tarea timer


					estado_MdE = PREPARACION;
					print_Display(DIS_PREPARACION);
				}
				break;

			case MENU_PPAL:
				switch(getKey())
				{
					case BOTON_SEL:
						if(cursor == 0) // Preparar ahora
						{
							cursor = 1;
							HD44780_SetCursor(0, 1);
						}
						else if (cursor == 1)	// Programar Cafe
						{
							cursor = 0;
							HD44780_SetCursor(0, 0);
						}
						break;

					case BOTON_OK:
						if(cursor == 0) // Preparar ahora
						{
							vTaskResume(Handler_Lectura);
							SetRelay(ON);
							SetLED(ON, AMARILLO);

							vTaskResume(Handler_Lectura); // Activar tarea medicion de temperatura
							vTaskResume(Handler_Sensores);
							vTaskResume(Handler_Anafe);// Activar tareas medicion de errores
							vTaskResume(Handler_Timer);// Activar tarea timer

							estado_MdE = PREPARACION;
							print_Display(DIS_PREPARACION);
						}
						if(cursor == 1) // Programar cafe
						{
							cursor = 0;

							HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);
							horaProgramada.Hours = horaActual.Hours;
							horaProgramada.Minutes = 0;
							horaProgramada.Seconds = 0;
							sprintf(horaStr, "%d", horaProgramada.Hours);
							sprintf(minutoStr, "%d", horaProgramada.Minutes);

							estado_MdE = PROGRAMAR_HORA;
							print_Display(DIS_PROGRAMAR_HORA);
						}

						break;
				}
				break;

			case PROGRAMAR_HORA:
				switch(getKey())
				{
					case BOTON_SEL:
						if(cursor == 0) // Hora
						{
							cursor = 1;
							HD44780_SetCursor(9, 1);
						}
						else if (cursor == 1)	// Minuto
						{
							cursor = 2;
							HD44780_SetCursor(13,1);
						}
						else if (cursor == 2)		// OK
						{
							cursor = 0;
							HD44780_SetCursor(2, 1);
						}
						break;

					case BOTON_OK:
						if(cursor == 0)
						{
							// Aumento la hora
							horaProgramada.Hours += 1;
							if(horaProgramada.Hours >23) horaProgramada.Hours = 0;
							sprintf(horaStr, "%d", horaProgramada.Hours);

							print_Display(DIS_PROGRAMAR_HORA);
							HD44780_SetCursor(2, 1);
						}
						if(cursor == 1)
						{
							// Aumento los minutos
							horaProgramada.Minutes += 10;
							if(horaProgramada.Minutes >50) horaProgramada.Minutes = 0;
							sprintf(minutoStr, "%d", horaProgramada.Minutes);

							print_Display(DIS_PROGRAMAR_HORA);
							HD44780_SetCursor(9, 1);
						}
						if(cursor == 2)
						{
							estado_MdE = PROGRAMADO;
							print_Display(DIS_PROGRAMADO);
						}
						break;

					case BOTON_CANCEL:
						estado_MdE = MENU_PPAL;
						print_Display(DIS_MENU_PPAL);
						cursor = 0;
						break;

					default:
						break;
					}

			case PREPARACION:
				if(getKey() == BOTON_OK) // Cancelan el cafe
				{
					SetRelay(OFF);
					SetLED(OFF, AMARILLO);

					vTaskSuspend(Handler_Lectura);// Apagar tarea medicion de temperatura

					estado_MdE = MENU_PPAL;
					print_Display(DIS_MENU_PPAL);
					cursor = 0;
				}
//				else if( (temp >= 100) && (!faseFinal) )
//				{
//					// Apagar medicion de temp
//					// Disparar timer real
//					faseFinal = 1;
//				}
//				else if(faseFinal)
//				{
//					if(timerRealTerminado)
//					{
//						// Activar tarea buzzer
//
//						SetRelay(OFF);
//						SetLED(OFF, AMARILLO);
//
//						estado_MdE = LISTO;
//						print_Display(LISTO);
//					}
//				}
				break;
			case LISTO:
				if(getKey() == BOTON_OK)
				{
					estado_MdE = MENU_PPAL;
					print_Display(DIS_MENU_PPAL);
					cursor = 0;
				}
				break;
			default:
				estado_MdE = PRENDIENDO;
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
		case DIS_PROGRAMADO:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Cafe para ");
			HD44780_SetCursor(10,0);
			HD44780_PrintStr(horaStr);
			HD44780_SetCursor(12,0);
			HD44780_PrintStr(":");
			HD44780_SetCursor(14,0);
			HD44780_PrintStr(minutoStr);
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Cancelar");
			HD44780_SetCursor(0,1);
			HD44780_Cursor();
			HD44780_Blink();
			break;
		case DIS_MENU_PPAL:
			HD44780_Clear();
			HD44780_SetCursor(2,0);
			HD44780_PrintStr("Cafe Rapido");
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Programar Cafe");
			HD44780_SetCursor(0,0);
			HD44780_Cursor();
			HD44780_Blink();
			break;

		case DIS_PROGRAMAR_HORA:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Programar para:");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr(horaStr);
			HD44780_SetCursor(2,1);
			HD44780_PrintStr(" : ");
			HD44780_SetCursor(7,1);
			HD44780_PrintStr(minutoStr);
			HD44780_SetCursor(13,1);
			HD44780_PrintStr(" OK");
			HD44780_SetCursor(2,1);
			HD44780_Cursor();
			HD44780_Blink();
			break;

		case DIS_PREPARACION:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Restante: ");
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Cancelar");
			HD44780_SetCursor(0,1);
			HD44780_Cursor();
			HD44780_Blink();
			break;

		case DIS_LISTO:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Cafe Listo!");
			HD44780_SetCursor(2,1);
			HD44780_PrintStr("Volver");
			HD44780_SetCursor(0,1);
			HD44780_Cursor();
			HD44780_Blink();
			break;

		case DIS_FALTA_SD:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Inserte una");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("SD valida");
			HD44780_NoCursor();
			HD44780_NoBlink();
			break;

		case DIS_ERROR_ANAFE:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Error en Anafe");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Consulte Manual");
			HD44780_NoCursor();
			HD44780_NoBlink();
			break;

		case DIS_ERROR_SENSOR_1:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Error Sensor 1");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Consulte Manual");
			HD44780_NoCursor();
			HD44780_NoBlink();
			break;

		case DIS_ERROR_SENSOR_2:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Error Sensor 2");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Consulte Manual");
			HD44780_NoCursor();
			HD44780_NoBlink();
			break;

		case DIS_MULTIPLES_ERRORES:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Multi Errores");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Consulte Manual");
			HD44780_NoCursor();
			HD44780_NoBlink();
			break;
	}
}
