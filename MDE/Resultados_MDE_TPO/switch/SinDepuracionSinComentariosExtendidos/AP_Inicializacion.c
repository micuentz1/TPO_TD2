/**
*	\file       AP_Inicializacion.c
*	\brief      Funciones de inicializacion de todo el proyecto
*	\details    Descripcion detallada del archivo
*	\author     Grupo6_2022
*	\date       11-10-2022 12:25:11
*/

/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/
#include "AP_Inicializacion.h"
#include "AP_MDE.h"

/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/

/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/
static void Inicializar_MdE_Principal( void ) ;
static void Inicializar_MdE_Preparar( void ) ;

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void Inicializar_MdE_Principal( void )
*	\brief   Inicializacion de las diferentes maquinas de estado
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*   \param   void
*	\return  void
*/
static void Inicializar_MdE_Principal( void )
{
    //!< Coloque aqui su codigo

    return ;
}

/**
*	\fn      void Inicializar_MdE_Preparar( void )
*	\brief   Inicializacion de las diferentes maquinas de estado
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*   \param   void
*	\return  void
*/
static void Inicializar_MdE_Preparar( void )
{
    //!< Coloque aqui su codigo
	TEMP = 0 ;

    return ;
}

/************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void Inicializar( void )
*	\brief   Inicializacion de las diferentes maquinas de estado
*	\details Amplie la descripcion
*	\author  Grupo6_2022
*	\date    11-10-2022 12:25:11
*   \param   void
*	\return  void
*/
void Inicializar( void )
{
    //Coloque aqui su codigo


	Inicializar_MdE_Principal( );

	Inicializar_MdE_Preparar( );

	
    return ;
}

