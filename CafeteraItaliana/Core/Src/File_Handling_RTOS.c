/*
 * File_Handling_RTOS.c
 *
 *  Created on: 30-April-2020
 *      Author: Controllerstech
 */

#include "File_Handling_RTOS.h"

/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

FRESULT Mount_SD (const TCHAR* path)
{
	return(f_mount(&fs, path, 1));
}

void Unmount_SD (const TCHAR* path)
{
	fresult = f_mount(NULL, path, 1);
}


FRESULT Write_File (char *name, char *data, uint32_t size)
{

	fresult = f_stat (name, &fno);
	fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
	if (fresult != FR_OK)
	{
		return fresult;
	}

	else
	{
	  	fresult = f_write(&fil, data, size, &bw);
	   	if (fresult != FR_OK)
	   	{
	    		return fresult;
	    }

	    fresult = f_close(&fil);
	}
	    return fresult;
}

FRESULT Read_File (char *name,char *lectura)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
	    return fresult;
	}

	else
	{
		fresult = f_open(&fil, name, FA_READ);

		if (fresult != FR_OK)
		{

		    return fresult;
		}

		size_t tam = sizeof(f_size(&fil));
		char *buffer = pvPortMalloc(tam);
		fresult = f_read (&fil, buffer, sizeof(lectura), &br);
		if (fresult != FR_OK)
		{
			return fresult;
		}

		else
		{
			for(int i=0;i<=sizeof(lectura);i++)
			{
				if(buffer[i] == 0) lectura[i] = '0';
				else lectura[i] = buffer[i];
			}
			vPortFree(buffer);


		fresult = f_close(&fil);
		}
	    return fresult;
	}
	return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
	    return fresult;
	}

	else
	{
	    fresult = f_open(&fil, name, FA_OPEN_ALWAYS | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	        return fresult;
	    }

	    f_lseek(&fil, fil.fsize);
	    fresult = f_write(&fil, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	return fresult;
	    }

	    fresult = f_close(&fil);
	}
    return fresult;
}
