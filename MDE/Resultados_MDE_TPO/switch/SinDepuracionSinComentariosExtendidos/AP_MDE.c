/**
*	\file       AP_MDE.c
*	\brief      Maquina/s de Estado
*	\details    Descripcion detallada del archivo
*	\author     Grupo6_2022
*	\date       11-10-2022 12:25:11
*/
/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/
#include "AP_MDE.h"
#include "AP_FuncionesMDE.h"

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
 *** MACROS PRIVADAS AL MODULO
 ************************************************************************************************************/
#define     m_ApagarAnafe()

#define     m_Buzz_Error()

#define     m_Buzz_Listo()

#define     m_ChequearSD()

#define     m_LED_Amarillo_OFF()

#define     m_LED_Amarillo_ON()

#define     m_LED_Rojo()

#define     m_LED_Verde_OFF()

#define     m_LED_Verde_ON()

#define     m_LeerTemp()

#define     m_Leer_Estado_Comp()

#define     m_Menu_Error()

#define     m_Menu_Init()

#define     m_Menu_Listo()

#define     m_Menu_Prep()

#define     m_Menu_Prep()

#define     m_Menu_SD()

#define     m_Menu_Time()

#define     m_PrenderAnafe()


/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/
static int MdE_Principal ( int );
static int MdE_Preparar ( int );

/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/
int TEMP ;		//

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

/**
*	\fn      static int MdE_Principal ( int Estado )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*   \param   [in] Estado: caso dentro de la maquina (estado actual)
*	\return  int : estado siguiente
*/

static int MdE_Principal ( int  Estado )
{
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
    return Estado ;
}

/**
*	\fn      static int MdE_Preparar ( int Estado )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*   \param   [in] Estado: caso dentro de la maquina (estado actual)
*	\return  int : estado siguiente
*/

static int MdE_Preparar ( int  Estado )
{
    switch ( Estado )
    {

        case RESET :
            m_Menu_Prep();
            m_PrenderAnafe();
            m_LeerTemp();
            m_LED_Amarillo_ON();

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
    return Estado ;
}

/************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void MaquinaDeEstados ( void )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*   \param   void
*	\return  void
*/
void MaquinaDeEstados ( void )
{
    static int estados_MdE_Principal = RESET;
    static int estados_MdE_Preparar = RESET;

    // Coloque su codigo aqui

    estados_MdE_Principal = MdE_Principal( estados_MdE_Principal );
    estados_MdE_Preparar = MdE_Preparar( estados_MdE_Preparar );


    return ;
}


void InitAll(void);

void InitAll(void) {

}
