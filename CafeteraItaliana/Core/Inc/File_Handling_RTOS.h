/*
 * File_Handling_RTOS.h
 *
 *  Created on: 30-April-2020
 *      Author: Controllerstech
 */

#ifndef FILE_HANDLING_RTOS_H_
#define FILE_HANDLING_RTOS_H_

#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "fatfs_sd.h"


/* mounts the sd card*/
FRESULT Mount_SD (const TCHAR* path);

/* unmounts the sd card*/
void Unmount_SD (const TCHAR* path);

/* write the data to the file
 * @ name : is the path to the file*/
FRESULT Write_File (char *name, char *data, uint32_t size);

/* read data from the file
 * @ name : is the path to the file*/
FRESULT Read_File (char *name, char *lectura);

/* updates the file. write pointer is set to the end of the file
 * @ name : is the path to the file
 */
FRESULT Update_File (char *name, char *data);




#endif /* FILE_HANDLING_RTOS_H_ */
