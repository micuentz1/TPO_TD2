
/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/

#include <stdio.h>
#include "cmsis_os.h"
#include "funciones.h"
#include "OneWire.h"
#include "MdE.h"
#include "liquidcrystal_i2c.h"
#include "my_ftoa.h"

/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/
extern RTC_HandleTypeDef hrtc;
extern float Temp[MAXDEVICES_ON_THE_BUS];
extern char componentes[4];
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
uint8_t f_preparacion=0;

uint8_t escribirSD = 0;
uint8_t tiempoRestante=7;
uint32_t tiempoHervor = 1;
char horaStr[3], minutoStr[3], restanteStr[3], temp_dis[5];
RTC_TimeTypeDef horaActual, horaProgramada, horaInicio, horaFicticia;

uint8_t f_error=0;
uint8_t estado_comp = TODO_OK;

uint8_t timerRealTerminado=0;

uint8_t bufferTeclado = NO_KEY;

float temperatura;

//************************************************************************************************************

void Tarea_Lectura( void *p )
{
	TickType_t xLastWakeTime;

	while(1) {

		xLastWakeTime = xTaskGetTickCount();

		get_Temperature();

		temperatura = ( Temp[0] + Temp[1] ) / 2;

		vTaskDelayUntil(&xLastWakeTime,ONE_SEC);

	}

	vTaskDelete(NULL); // En caso de romperse el loop
}

//************************************************************************************************************

void Error_Sensores( void *p )
{
	float dif=0;

	while(1) {

		dif = Temp[1] - Temp[0];

		// Chequeamos que los sensores funcionen bien
		if((dif>MAX_DIF_P) || (dif<MAX_DIF_N))
		{
			SetLED(OFF, VERDE);
			SetLED(ON, ROJO);

		    // En base a si la dif es positiva o negativa sabemos cual sensor fallo
		    // Cuando el sensor no lee bien indica 85 grados, usamos eso para determinar cual fallo
		    if(dif>MAX_DIF_P)
		    {
		    	componentes[2] = '0'; // Fallo el sensor 2
		    	estado_comp = ERROR_SENSOR_2;

		    	// CARGAR EN LA SD EL ERROR
		    	//escribir_SD();
		    }
		    else if(dif<MAX_DIF_N){
		    	componentes[1] = '0'; // Fallo el sensor 1
		    	estado_comp = ERROR_SENSOR_1;

		    	// CARGAR EN LA SD EL ERROR
		    	//escribir_SD();
		    }

		    f_error = 1;
		}

		vTaskDelay(ONE_SEC); // Tiene que ser 1s
	}

	vTaskDelete(NULL); // En caso de romperse el loop
}

//************************************************************************************************************

void Error_Anafe(void *p)
{
	float dif_s0=0, dif_s1=0, auxTemp[2] = {0, 0};

	while(1)
	{
		if(f_preparacion && (tiempoRestante<=4) && (!faseFinal))
		{
			dif_s0 = Temp[0]-auxTemp[0];
			dif_s1 = Temp[1]-auxTemp[1];

			auxTemp[0] = Temp[0];
			auxTemp[1] = Temp[1];
			// Chequeamos que el anafe funcione bien
			if( (dif_s0<MIN_DIF_CALENTAMIENTO) || (dif_s1<MIN_DIF_CALENTAMIENTO) )
			{
				SetLED(OFF, VERDE);
				SetLED(ON, ROJO);

				componentes[0] = '0'; // Fallo el anafe
				estado_comp = ERROR_ANAFE;

				// CARGAR EN LA SD EL ERROR
				//escribir_SD();
				f_error = 1;
			}


		}
		vTaskDelay(15*ONE_SEC); // Que mida cada 15 secs
	}

	vTaskDelete(NULL); // En caso de romperse el loop
}

//************************************************************************************************************

void Tarea_Timer(void *p)
{
	TickType_t xLastWakeTime3;

	xLastWakeTime3 = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime3,1000);

		HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);

		if(faseFinal)
		{
			if((horaActual.Seconds >= horaInicio.Seconds) && (horaActual.Minutes == horaInicio.Minutes+tiempoHervor))
			{
				// Termino
				timerRealTerminado = 1;
			}
		}
	}
	vTaskDelete(NULL);
}

//************************************************************************************************************

void Tarea_Timer2(void *p)
{
	TickType_t xLastWakeTime2;

	xLastWakeTime2 = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime2,1000);

		if(f_preparacion)
		{
			if((horaActual.Seconds >= horaFicticia.Seconds) && (horaActual.Minutes == horaFicticia.Minutes+1))
			{
				HAL_RTC_GetTime(&hrtc, &horaFicticia, RTC_FORMAT_BIN);

				tiempoRestante --;

				if(tiempoRestante == 2) SetRelay(OFF);
				if(tiempoRestante == 0) tiempoRestante = 1;
				sprintf(restanteStr, "%d", tiempoRestante);
				print_Display(DIS_PREPARACION);
			}
		}

	}

	vTaskDelete(NULL);
}

//************************************************************************************************************

void Tarea_BOTON_ON(void *p)
{
	while(1)
	{
		HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);

		if(bufferTeclado == BOTON_ON) {
			HD44780_Clear();
			HAL_NVIC_SystemReset();
		}

		vTaskDelay(100);
	}

	vTaskDelete(NULL);
}

//************************************************************************************************************

void MdE_Principal(void *p)
{
	static uint8_t estado_MdE = PRENDIENDO;

	while(1)
	{
		//HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);

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
					LecturaSD();
					// Lea de nuevo
				}
				else if(estado_comp >1) // Identificar error
				{
					estado_MdE = E_ERROR;
					print_Display(estado_comp + 4); // La organizacion de la enumeracion
				}
				break;

			case PROGRAMADO: // Esperando a que sea la hora. Si se presiona el boton se cancela y se vuelve al menu ppal.

				if( getKey() == BOTON_OK )
				{
					// Escribir en SD que no hay ninguno programado
					estado_MdE = MENU_PPAL;
					print_Display(DIS_MENU_PPAL);
					cursor = 0;
					componentes[3] = '0';
					escribir_SD();
				}
				else if( (horaActual.Hours == horaProgramada.Hours) && (horaActual.Minutes == horaProgramada.Minutes) )
				{
					IniciarPreparacion();

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
							IniciarPreparacion();

							estado_MdE = PREPARACION;
							print_Display(DIS_PREPARACION);
						}
						if(cursor == 1) // Programar cafe
						{
							cursor = 0;

							HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);
							horaProgramada.Hours = horaActual.Hours;
							horaProgramada.Minutes = (horaActual.Minutes/10)*10;
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
							componentes[3] = '1';
							escribir_SD();
							hora_SD();
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
				break;

			case PREPARACION:

				if(f_error)
				{
					FinalizarPreparacion();
					estado_MdE = E_ERROR;
					print_Display(estado_comp + 4);
					escribir_SD();
					registro_SD(ERRONEO);
				}
				else if(getKey() == BOTON_OK) // Cancelan el cafe
				{
					FinalizarPreparacion();

					estado_MdE = MENU_PPAL;
					print_Display(DIS_MENU_PPAL);
					cursor = 0;
					registro_SD(CANCELADO);
				}
				else if( (temperatura >= 95) && (!faseFinal) )
				{
					faseFinal = 1;
				}
				else if(faseFinal)
				{
					if(timerRealTerminado)
					{
						FinalizarPreparacion();

						estado_MdE = LISTO;
						print_Display(LISTO);
					}
				}
				break;
			case LISTO:
				if(getKey() == BOTON_OK)
				{
					estado_MdE = MENU_PPAL;
					print_Display(DIS_MENU_PPAL);
					cursor = 0;
					registro_SD(EXITOSO);
				}
				break;

			case E_ERROR:
				// Necesita que lo arregle el tecnico, no va a salir de aca
				break;
			default:
				estado_MdE = PRENDIENDO;
				break;

		}

		vTaskDelay(1000);
	}

	vTaskDelete(NULL);
}

//************************************************************************************************************

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
			HD44780_SetCursor(10,0);
			HD44780_PrintStr(restanteStr);
			HD44780_SetCursor(13,0);
			HD44780_PrintStr("Min");
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

		case DIS_ERROR_SD:
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("Error en SD");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Consulte Manual");
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

