/*
 *
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <string.h>
#include "font_1.h"
#include "font_2.h"
#include "image.h"
#include "glcd.h"
#include "adc.h"
#include "timer.h"


/* Change Clock rate to 48 MHz*/
#undef Chip_Clock_GetIntOscRate
#define Chip_Clock_GetIntOscRate()      SYSCON_FRO48MHZ_FREQ




int main(void)
{

	SystemCoreClockUpdate();

	Board_Init();
	Board_LED_RGB_Set(LED_RGB_GREEN);
	
	/* SPI  pins initialization */
	SPI_Init_PinMux();

	/* Setup SPI controllers */
	SPI_setupMaster();

    /* Initiate TFT*/
	Glcd_Initial();
	Glcd_ClearScreen(AQUA);

	/* ADC initialization*/
	ADC_Initial();
	/* Timer initialization for touch status check*/
	Timer_Initialize();

	Glcd_show_image(30, 90, arrow_icon, RED, AQUA, 64, 64);
	Glcd_show_image(90, 90, arrow_icon, GREEN, AQUA, 64, 64);
	Glcd_show_image(150, 90, arrow_icon, BLUE, AQUA, 64, 64);
    delayms(200);
    Glcd_show_image(45, 200, button_icon, GREEN, AQUA, 152, 56);
    Glcd_show_str(80, 220, "START", WHITE, GREEN, 0);

    uint16_t chek_touch=0;
	while (1)
	{

        /* check the touch inside the points */
		chek_touch=ADC_check_point(45, 197, 200, 256);

		if(chek_touch)
			{
				  delayms(100); //double check
				  chek_touch=ADC_check_point(45, 197, 200, 256);

				  if(chek_touch && (!g_starts) )
					  {
					  	Glcd_show_image(45, 200, button_icon, RED, AQUA, 152, 56);
					  	Glcd_show_str(80, 220, "STOP", WHITE, RED, 1);
						chek_touch = 0;
						delayms(1500);
						g_starts = 1;
						Glcd_ClearScreen_part(30, 210, 90, 160, AQUA);
						g_current_num = 0;
					  }

				  else if(g_starts)
				  	  {
					  	Glcd_show_image(45, 200, button_icon, GREEN, AQUA, 152, 56);
					  	Glcd_show_str(80, 220, "START", WHITE, GREEN, 0);
						chek_touch = 0;
						delayms(1500);
						g_starts = 0;
				  	  }
			 }

		if(g_starts == 1)
		  {
			char string_disp[10];
			sprintf(string_disp, "%4d", g_current_num);

			Glcd_show_str_large(80, 90, string_disp, BLACK, AQUA, 0);
		  }

	}

	return 0;
}
