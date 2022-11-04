#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include "stm32f1xx_hal_gpio.h"

#define		ON		1
#define		OFF		0

#define		MAX_DIF_P	5
#define		MAX_DIF_N	-5
#define		MIN_DIF_CALENTAMIENTO	0.5
#define		ONE_SEC		1000 // Para contar 1 segundo con vTaskDelay, CHEQUEAR -> Chequeado, vTaskDelay cuenta en ms

#define		RED_PORT	GPIOB
#define		RED_PIN		GPIO_PIN_14
#define		GREEN_PORT	GPIOB
#define		GREEN_PIN	GPIO_PIN_12

void InitAll(void);


#endif
