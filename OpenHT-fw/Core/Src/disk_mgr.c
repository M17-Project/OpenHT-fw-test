#include "disk_mgr.h"
#include "fatfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FATFS SDFatFs;  /* File system object for SD disk logical drive */
char SDPath[4]; /* SD disk logical drive path */

FIL the_file;     /* File object */
//static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

static void fat_error_handler(void);


void test_fat(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "This is OpenHT writing to a txt file!!"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */

  /*##-1- Link the SD disk I/O driver ########################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
	/*##-2- Register the file system object to the FatFs module ##############*/
	if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
	{
	  /* FatFs Initialization Error */
	  fat_error_handler();
	}
	else
	{
//	  /*##-3- Create a FAT file system (format) on the logical drive #########*/
//	  if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer)) != FR_OK)
//	  {
//		fat_error_handler();
//	  }
//	  else
//	  {
		/*##-4- Create and Open a new text file object with write access #####*/
		if(f_open(&the_file, "test456.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
		{
		  /* 'STM32.TXT' file Open for write Error */
			fat_error_handler();
		}
		else
		{
		  /*##-5- Write data to the text file ################################*/
		  res = f_write(&the_file, wtext, (UINT)strlen(wtext), (void *)&byteswritten);

		  if((byteswritten == 0) || (res != FR_OK))
		  {
			/* 'STM32.TXT' file Write or EOF Error */
			  fat_error_handler();
		  }
		  else
		  {
			/*##-6- Close the open text file #################################*/
			f_close(&the_file);

			/*##-7- Open the text file object with read access ###############*/
			if(f_open(&the_file, "test456.txt", FA_READ) != FR_OK)
			{
			  /* 'STM32.TXT' file Open for read Error */
				fat_error_handler();
			}
			else
			{
			  /*##-8- Read data from the text file ###########################*/
			  res = f_read(&the_file, rtext, sizeof(rtext), (UINT*)&bytesread);

			  if((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
			  {
				/* 'STM32.TXT' file Read or EOF Error */
				  fat_error_handler();
			  }
			  else
			  {
				/*##-9- Close the open text file #############################*/
				f_close(&the_file);

				/*##-10- Compare read data with the expected data ############*/
				if ((bytesread != byteswritten))
				{
				  /* Read data is different from the expected data */
					fat_error_handler();
				}
				else
				{
				  /* Success of the demo: no error occurrence */
				  BSP_LED_On(LED_GREEN);
				}
			  }
			}
//		  }
		}
	  }
	}
  }

  /*##-11- Unlink the SD disk I/O driver ####################################*/
  FATFS_UnLinkDriver(SDPath);
}

static void fat_error_handler(void)
{
	BSP_LED_On(LED_ORANGE);
}
