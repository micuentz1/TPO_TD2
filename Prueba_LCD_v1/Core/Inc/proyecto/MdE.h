#ifndef MdE_H_
#define MdE_H_

/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES
 ************************************************************************************************************/
void Tarea_Lectura( void *p );
void Error_Sensores( void *p );
void Error_Anafe( void *p );
void Boton_Ok( void *p );
void Boton_Sel( void *p );
void Boton_Cancel( void *p );
void MdE_Display( void *p );
void print_Display( uint8_t );

enum estados_Display {INICIANDO , MENU_PPAL , SEGURO , PREPARACION , FINALIZACION};

#endif
