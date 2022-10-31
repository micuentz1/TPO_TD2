
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


/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/


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

				if ( s_Error() )
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

				if ( s_Error() )
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

	while(1) {

	}

	vTaskDelete(NULL); // En caso de romperse el loop
}
