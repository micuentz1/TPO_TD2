/**
*	\file       main.c
*	\brief      Proyecto: MDE_TPO
*	\details    Descripcion detallada del archivo
*	\author     Grupo6_2022
*	\date       11-10-2022 12:25:11
*/

/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/

#include "AP_MDE.h"
#include "AP_Inicializacion.h"

void main ( void )
{
    Inicializar( );

    while ( 1 )
    {
        MaquinaDeEstados();
    }
    return ;
}

