#ifndef FUNCIONES_H_
#define FUNCIONES_H_

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

#define BOTON_OK_Pin GPIO_PIN_15
#define BOTON_OK_GPIO_Port GPIOA
#define BOTON2_Pin GPIO_PIN_3
#define BOTON2_GPIO_Port GPIOB
#define BOTON1_Pin GPIO_PIN_4
#define BOTON1_GPIO_Port GPIOB

void InitAll(void);


#endif
