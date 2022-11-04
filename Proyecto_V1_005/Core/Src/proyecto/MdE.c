
/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/
#define		RESET		0
//Maquina: MdE_Principal
#define		PRENDIENDO		1	// < Maquina: MdE_Principal >
#define		IDLE		2	// < Maquina: MdE_Principal >
#define		PREPARANDO		3	// < Maquina: MdE_Principal >
#define		ERROR		4	// < Maquina: MdE_Principal >

//Maquina: MdE_Preparar
#define		CALENTAR		1	// < Maquina: MdE_Preparar >
#define		HIRVIENDO		2	// < Maquina: MdE_Preparar >
#define		LISTO		3	// < Maquina: MdE_Preparar >

#include "OneWire.h"
#include "funciones.h"

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
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/

void MdE_Principal ( void *p )
{
    static uint8_t Estado = RESET;

	while(1) {

		switch ( Estado )
		{

			case RESET :
				m_Leer_Estado_Comp();
				m_LED_Verde_ON();

				Estado = PRENDIENDO;

				break;

			case PRENDIENDO :
				if ( s_NoHaySD() )
				{
					m_Leer_Estado_Comp();
					m_Menu_SD();

					Estado = PRENDIENDO;
				}

				if ( s_TodoOK() )
				{
					m_Menu_Init();
					m_ChequearSD();

					Estado = IDLE;
				}

				if ( f_Error )
				{
					m_Buzz_Error();
					m_LED_Rojo();
					m_Menu_Error();
					m_LED_Verde_OFF();

					Estado = ERROR;
				}

				break;

			case IDLE :
				if ( s_EsLaHora() || s_PrepararAhora() )
				{
					m_Menu_Prep();

					Estado = PREPARANDO;
				}

				if ( s_NoHaySD() )
				{
					m_Menu_SD();

					Estado = PRENDIENDO;
				}

				break;

			case PREPARANDO :
				if ( s_Cancelar() || s_Cafe_Listo() )
				{

					Estado = IDLE;
				}

				if ( f_Error )
				{
					m_Buzz_Error();
					m_LED_Rojo();
					m_Menu_Error();
					m_LED_Verde_OFF();

					Estado = ERROR;
				}

				break;

			case ERROR :
				if ( s_Tarea_Aparte_Chequeo() )
				{

					Estado = ERROR;
				}

				break;

			default:
				Estado = RESET ;
				break;

		}

    }

    vTaskDelete(NULL); // En caso de romperse el loop

}

/**
*	\fn
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/

void MdE_Preparar ( void *p )
{
	static uint8_t Estado = RESET;
	char temp_dis[5];

	while(1) {

		switch ( Estado )
		{

			case RESET :
				m_Menu_Prep();
				m_PrenderAnafe();
				m_LeerTemp();
				m_LED_Amarillo_ON();
 //--------------------------------------------
				get_Temperature();
				my_ftoa(Temp[0], temp_dis, 2);

				HD44780_Clear();
				HD44780_SetCursor(0,0);
				HD44780_PrintStr("Temp: ");
				HD44780_SetCursor(6,0);
				HD44780_PrintStr(temp_dis);

				vTaskDelay(1000);
				//HAL_Delay(1000);
//---------------------------------------------
				Estado = CALENTAR;

				break;

			case CALENTAR :
				if ( TEMP == 100 )
				{
					t_TimerCafe_100_S();
					m_Menu_Time();

					Estado = HIRVIENDO;
				}

				break;

			case HIRVIENDO :
				if ( e_TimerCafe() )
				{
					m_Menu_Listo();
					m_ApagarAnafe();
					m_Buzz_Listo();
					m_LED_Amarillo_OFF();

					Estado = LISTO;
				}

				break;

			case LISTO :
				break;

			default:
				Estado = RESET ;
				break;

		}

    }

    vTaskDelete(NULL); // En caso de romperse el loop
}

/**
*	\fn
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*/

void Tarea_Errores( void *p ) {
	float dif=0, dif_s0=0, dif_s1=0, auxTemp[2] = {0, 0};

	while(1) {

		get_Temperature();

		dif0 = Temp[1] - Temp[0];

		dif_s0 = Temp[0]-auxTemp[0];
		dif_s1 = Temp[1]-auxTemp[1];

		auxTemp[0] = Temp[0];
		auxTemp[1] = Temp[1];

		// Chequeamos que los sensores funcionen bien
		if( (dif0>MAX_DIF_P) || (dif0<MAX_DIF_N) ) {
			f_Error = 1;

			// En base a si la dif es positiva o negativa sabemos cual sensor fallo
		}

		// Chequeamos que el anafe funcione bien
		if( (dif_s0<MIN_DIF_CALENTAMIENTO) || (dif_s1<MIN_DIF_CALENTAMIENTO) ) {
			f_Error = 1;

		} // Aca o en otra tarea?? ---> probablemente si, que chequee cada 5s por ejemplo?

		vTaskDelay(1SEC); // Tiene que ser 1s
	}

	vTaskDelete(NULL); // En caso de romperse el loop
}
