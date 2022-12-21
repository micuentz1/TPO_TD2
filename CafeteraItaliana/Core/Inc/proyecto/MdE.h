#ifndef MdE_H_
#define MdE_H_
/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES
 ************************************************************************************************************/
void Tarea_Lectura( void *p );
void Error_Sensores( void *p );
void Error_Anafe( void *p );
void MdE_Principal( void *p );
void Tarea_Timer( void *p );
void Tarea_Timer2(void *p);
void print_Display( uint8_t estado);
void Tarea_BOTON_ON(void *p);

enum estados_comp {TODO_OK, FALTA_SD, ERROR_SD, ERROR_ANAFE, ERROR_SENSOR_1, ERROR_SENSOR_2, MULTIPLES_ERRORES};
enum estados_Display {DIS_PROGRAMADO, DIS_MENU_PPAL, DIS_PROGRAMAR_HORA, DIS_PREPARACION, DIS_LISTO, DIS_FALTA_SD, DIS_ERROR_SD, DIS_ERROR_ANAFE, DIS_ERROR_SENSOR_1, DIS_ERROR_SENSOR_2, DIS_MULTIPLES_ERRORES};
enum estados_MdE {PRENDIENDO, PROGRAMADO, MENU_PPAL, PROGRAMAR_HORA, PREPARACION, LISTO, E_ERROR};

#endif
