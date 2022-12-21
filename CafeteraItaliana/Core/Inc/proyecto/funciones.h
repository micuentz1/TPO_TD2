#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include "FreeRTOS.h"
#include "task.h"

#define		ON		1
#define		OFF		0

#define		VERDE		1
#define		AMARILLO	2
#define		ROJO		3

#define		MAX_DIF_P	5
#define		MAX_DIF_N	-5
#define		MIN_DIF_CALENTAMIENTO	0.5
#define		ONE_SEC		1000 // Para contar 1 segundo con vTaskDelay, CHEQUEAR -> Chequeado, vTaskDelay cuenta en ms
#define		ONE_MIN		(60*ONE_SEC)

#define		TIEMPO_CAFE_MS	(1*ONE_MIN)
#define		TIEMPO_CAFE_SEC	(60)
#define		TIEMPO_CAFE_MIN	(0)

#define		RED_PORT	GPIOB
#define		RED_PIN		GPIO_PIN_14
#define		GREEN_PORT	GPIOB
#define		GREEN_PIN	GPIO_PIN_12

#define BOTON_OK_Pin 				BOTON1_Pin
#define BOTON_OK_GPIO_Port 			BOTON1_GPIO_Port
#define BOTON_SEL_Pin 				BOTON2_Pin
#define BOTON_SEL_GPIO_Port 		BOTON2_GPIO_Port
#define BOTON_CANCEL_Pin 			BOTON3_Pin
#define BOTON_CANCEL_GPIO_Port 		BOTON3_GPIO_Port

// ****************** Teclado *****************************
void DriverTeclado(void *p);
char getKey(void);
char BarridoTeclado(void);

#define	ESPERANDO_TECLA			1
#define	VALIDAR_TECLA			2
#define	TECLA_PRESIONADA		3

#define CANT_PARA_VALIDAR		10 // 100 ms para validar la tecla

#define NO_KEY			0
#define BOTON_OK		1
#define BOTON_SEL		2
#define BOTON_CANCEL	3
#define	BOTON_ON		4

enum registroSD {EXITOSO = 0, CANCELADO, ERRONEO};
// ********************************************************

void InitAll(void);
void InitLCD( void );
void LecturaSD(void);
void SetRelay(char estado);
void SetBuzzer(char estado);
void SetLED(char estado, char led);
void IniciarPreparacion(void);
void FinalizarPreparacion(void);
void escribir_SD(void);
void hora_SD(void);
void registro_SD(int);

#endif
