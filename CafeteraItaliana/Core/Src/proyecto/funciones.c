#include "funciones.h"
#include "liquidcrystal_i2c.h"
#include "OneWire.h"
#include "main.h"
#include "MdE.h"
#include <stdio.h>
#include <stdlib.h>
#include "File_Handling_RTOS.h"

//************************************************************************************************************

extern uint8_t faseFinal;
extern uint8_t f_preparacion;
extern uint8_t toggle_reset;

extern uint8_t bufferTeclado;
extern uint8_t tiempoRestante;
extern RTC_TimeTypeDef horaActual, horaProgramada, horaInicio, horaFicticia;
extern char horaStr[3], minutoStr[3], restanteStr[3];

extern RTC_HandleTypeDef hrtc;
extern TaskHandle_t Handler_Lectura;
extern TaskHandle_t Handler_Sensores;
extern TaskHandle_t Handler_Anafe;
extern TaskHandle_t Handler_Timer;
extern TaskHandle_t Handler_Timer2;

extern uint8_t f_error;
extern uint8_t estado_comp;

char componentes[4];
uint8_t cafe_programado;

//************************************************************************************************************

void InitAll(void) {

	SetBuzzer(ON);

	HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, ON); // Sistema Prendido y funcionando

	get_ROMid();

	InitLCD();

	LecturaSD();

	SetBuzzer(OFF);
}

//************************************************************************************************************

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

//************************************************************************************************************

void LecturaSD()
{
	char estado_anafe,estado_sensor1,estado_sensor2,programado,stop,hora_prog[6],hora[2],minutos[2];

	// Leo la SD y actualizo el vector componentes y la variable estado_comp y cafe_programado
	FRESULT resultado = Mount_SD("");
	if(resultado == FR_OK)
	{
		if(Read_File("est.txt", componentes) == FR_OK)
		{
			//Chequeo que este todo en orden

			estado_anafe = componentes[0];
			estado_sensor1 = componentes[1];
			estado_sensor2 = componentes[2];
			programado = componentes[3];
			if(estado_anafe == 0) estado_anafe = '0';
			if(estado_sensor1 == 0) estado_sensor1 = '0';
			if(estado_sensor2 == 0) estado_sensor2 = '0';
			if(programado == 0) programado = '0';

			if((estado_anafe == '0' && estado_sensor1 == '0') || (estado_anafe == '0' && estado_sensor2 == '0') || (estado_sensor2 == '0' && estado_sensor1 == '0'))
			{
				f_error = 1;
				estado_comp = MULTIPLES_ERRORES;
				print_Display(estado_comp + 4);
				SetLED(OFF, VERDE);
				SetLED(ON, ROJO);
			} else
			{
				if(estado_anafe == '0')
				{
					f_error = 1;
					estado_comp = ERROR_ANAFE;
					print_Display(estado_comp + 4);
					SetLED(OFF, VERDE);
					SetLED(ON, ROJO);
				}
				if(estado_sensor1 == '0')
				{
					f_error = 1;
					estado_comp = ERROR_SENSOR_1;
					print_Display(estado_comp + 4);
					SetLED(OFF, VERDE);
					SetLED(ON, ROJO);
				}
				if(estado_sensor2 == '0')
				{
					f_error = 1;
					estado_comp = ERROR_SENSOR_2;
					print_Display(estado_comp + 4);
					SetLED(OFF, VERDE);
					SetLED(ON, ROJO);
				}
			}

			if(programado == '1')
			{
				cafe_programado = 1;
				FRESULT resultado = Read_File("prog.txt", hora_prog);
				for(int i=0;i<strlen(hora_prog);i++)
				{
					if(hora_prog[i] == 0) hora_prog[i] = '0';
				}
				if(resultado == FR_OK)
				{
					hora[0] = hora_prog[0];
					hora[1] = hora_prog[1];
					minutos[0] = hora_prog[3];
					minutos[1] = hora_prog[4];

					horaProgramada.Hours = atoi(hora);
					horaProgramada.Minutes = atoi(minutos);

					UNUSED(stop);
				}

			} else cafe_programado = 0;
		} else
		{
			f_error = 1;
			estado_comp = ERROR_SD;
			print_Display(estado_comp + 4);
		}
	} else
	{
		f_error = 1;
		estado_comp = FALTA_SD;
		print_Display(estado_comp + 4);
	}
	Unmount_SD("");

	horaProgramada.Seconds = 0;
	sprintf(horaStr, "%d", horaProgramada.Hours);
	sprintf(minutoStr, "%d", horaProgramada.Minutes);
}

//************************************************************************************************************

void escribir_SD()
{
	if(Mount_SD("") == FR_OK)
	{
		if(Write_File("est.txt", componentes, strlen(componentes)) != FR_OK)
		{
			estado_comp = ERROR_SD;
		}
		Unmount_SD("");
	} else
	{
		estado_comp = FALTA_SD;
	}
}

void hora_SD()
{
	char hora_prog[5];

	sprintf(hora_prog,"%d:%d",horaProgramada.Hours,horaProgramada.Minutes);

	if(Mount_SD("") == FR_OK)
	{
		if(Write_File("prog.txt", hora_prog, 5) != FR_OK)
		{
			estado_comp = ERROR_SD;
		}
		Unmount_SD("");
	} else
	{
		estado_comp = FALTA_SD;
	}
}

void registro_SD(int resultado)
{
	char str[40];
	//uint32_t len;

	HAL_RTC_GetTime(&hrtc, &horaActual, RTC_FORMAT_BIN);

	if(Mount_SD("") == FR_OK)
	{
		switch(resultado)
		{
			case EXITOSO:
				sprintf(str,"PROCESO OK - %d:%d \n\0",horaActual.Hours,horaActual.Minutes);
				//len = strlen(str);
				break;
			case CANCELADO:
				sprintf(str,"PROCESO CANCELADO - %d:%d \n\0",horaActual.Hours,horaActual.Minutes);
				//len = strlen(str);
				break;
			case ERRONEO:
				sprintf(str,"ERROR EN COMPONENTES - %d:%d \n\0",horaActual.Hours,horaActual.Minutes);
				//len = strlen(str);
				break;
			default:
				sprintf(str,"ERROR AL GRABAR PROCESO - %d:%d \n\0",horaActual.Hours,horaActual.Minutes);
				//len = strlen(str);
				break;
		}
		if(Update_File("reg.txt", str) != FR_OK)
		{
			estado_comp = ERROR_SD;
		}
		Unmount_SD("");
	} else
	{
		estado_comp = FALTA_SD;
	}
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
	else if(!HAL_GPIO_ReadPin(BOTON_ON_GPIO_Port,BOTON_ON_Pin))			return BOTON_ON;
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

//********************* RELE *******************************

void SetRelay(char estado)
{
	if(estado == ON) HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, ON);
	if(estado == OFF) HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, OFF);
}

//************************************************************************************************************

void SetBuzzer(char estado)
{
	if(estado == ON) HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, ON);
	if(estado == OFF) HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, OFF);
}

//************************************************************************************************************

void SetLED(char estado, char led)
{
	uint16_t pin;

	if(led == VERDE) 	pin = LED_VERDE_Pin;
	if(led == AMARILLO) pin = LED_AMARILLO_Pin;
	if(led == ROJO) 	pin = LED_ROJO_Pin;

	if(estado == ON) HAL_GPIO_WritePin(GPIOB, pin, ON);
	if(estado == OFF) HAL_GPIO_WritePin(GPIOB, pin, OFF);
}

//************************************************************************************************************

void IniciarPreparacion(void)
{
	SetBuzzer(ON);

	SetRelay(ON);
	SetLED(ON, AMARILLO);

	f_preparacion = 1;

	tiempoRestante=7;
	sprintf(restanteStr, "%d", tiempoRestante);
	print_Display(DIS_PREPARACION);

	HAL_RTC_GetTime(&hrtc, &horaInicio, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &horaFicticia, RTC_FORMAT_BIN);

//	vTaskPrioritySet(Handler_Lectura, 1);
//	vTaskPrioritySet(Handler_Sensores, 1);
//	vTaskPrioritySet(Handler_Anafe, 1);
//	vTaskPrioritySet(Handler_Timer, 1);
//	vTaskPrioritySet(Handler_Timer2, 1);

	SetBuzzer(OFF);
}

//************************************************************************************************************

void FinalizarPreparacion(void)
{
	faseFinal = 0;
	f_preparacion = 0;

	SetRelay(OFF);
	SetLED(OFF, AMARILLO);

//	vTaskPrioritySet(Handler_Lectura, 2);
//	vTaskPrioritySet(Handler_Sensores, 3);
//	vTaskPrioritySet(Handler_Anafe, 4);
//	vTaskPrioritySet(Handler_Timer, 2);
//	vTaskPrioritySet(Handler_Timer2, 2);

	// Aviso sonoro
	SetBuzzer(ON);
	HAL_Delay(500);
	SetBuzzer(OFF);
	HAL_Delay(500);

	SetBuzzer(ON);
	HAL_Delay(500);
	SetBuzzer(OFF);
	HAL_Delay(500);

	SetBuzzer(ON);
	HAL_Delay(500);
	SetBuzzer(OFF);

	f_preparacion = 0;
}

