/*
 * adc.h
 *
 *  Created on: Dec 19, 2017
 *      Author: user2
 */

#ifndef ADC_H_
#define ADC_H_

#define TICKRATE_HZ (500)

void ADC_Initial(void);
void ADC_2(void);
void ADC_1(void);
int check_touch_point(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);

uint16_t g_adcOut_x, g_adcOut_y, g_adcOut;
int16_t g_val_x, g_val_y;
char g_string_disp[10];
uint8_t g_axis=0, g_touch_flag=0;


void SysTick_Handler(void)
{
    /* Start ADC conversion for SeqB */
	Chip_ADC_StartSequencer(LPC_ADC, ADC_SEQB_IDX);
}



void ADC_SEQB_IRQHandler(void)
{
    /* If SeqB flags is set i.e. data in global register is valid then read it */
    if(Chip_ADC_GetFlags(LPC_ADC) & ADC_FLAGS_SEQB_INT_MASK) {
        g_adcOut = (Chip_ADC_GetGlobalDataReg(LPC_ADC, ADC_SEQB_IDX)
        		& ADC_SEQ_GDAT_RESULT_MASK) >> ADC_SEQ_GDAT_RESULT_BITPOS;

        if (g_axis==0)// ADC reading  in y axis
         {
        	g_adcOut_y=(Chip_ADC_GetDataReg(LPC_ADC, 5)& ADC_SEQ_GDAT_RESULT_MASK)>> ADC_SEQ_GDAT_RESULT_BITPOS;

         }

        else//ADC reading in x axis
        {
        	g_adcOut_x=(Chip_ADC_GetDataReg(LPC_ADC, 1)& ADC_SEQ_GDAT_RESULT_MASK)>> ADC_SEQ_GDAT_RESULT_BITPOS;
        }

    }

}



void ADC_Initial(void)
{

	/* Power up ADC and enable ADC base clock as system clock , 12-bit resolution.*/
	Chip_ADC_Init(LPC_ADC, ADC_CR_RESOL(3) | ADC_CR_CLKDIV(0));
    /*Select sequence B and channel 1 and 5*/
	Chip_ADC_SetSequencerBits(LPC_ADC, ADC_SEQB_IDX,
			ADC_SEQ_CTRL_SEQ_ENA | ADC_SEQ_CTRL_CHANNEL_EN(5) | ADC_SEQ_CTRL_CHANNEL_EN(1));
	/*Enable ADC interrupt*/
	Chip_ADC_EnableInt(LPC_ADC, ADC_INTEN_SEQB_ENABLE);

    /* Calibrate ADC */
    if(Chip_ADC_Calibration(LPC_ADC) == LPC_OK)
    {
        /* Enable ADC SeqA Interrupt */
        NVIC_EnableIRQ(ADC_SEQB_IRQn);
        /* Set priority*/
		NVIC_SetPriority(ADC_SEQB_IRQn, 1);
        /* Enable SysTick Timer */
        SysTick_Config(SystemCoreClock / TICKRATE_HZ);

    }
}

void ADC_2(void)
{
	/*digital output with 3.3 v */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2,
	    						 (IOCON_FUNC0 | IOCON_DIGITAL_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 1, 2, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 2, 1);

	/*analog input */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 30,
	    						 (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 30, 0);

	/*digital output with 0 v */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 2,
	    						 (IOCON_FUNC0 | IOCON_DIGITAL_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 2, 1);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, 0);

	/*digital input */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 3,
	    						 (IOCON_FUNC0 | IOCON_DIGITAL_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 3, 0);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 3, 1);

}

void ADC_1(void)
{
	/*digital output with 3.3 v */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 30,
	    						 (IOCON_FUNC0 | IOCON_DIGITAL_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 30, 1);
	 Chip_GPIO_SetPinState(LPC_GPIO, 0, 30, 1);

		/*analog input */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2,
	    						 (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_ANALOG_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 1, 2, 0);



	/*digital output with 0 v */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 3,
	    						 (IOCON_FUNC0 | IOCON_DIGITAL_EN));

	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 3,1);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 3, 0);

	/*digital input */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 2,
	    						 (IOCON_FUNC0 | IOCON_DIGITAL_EN));
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 2, 0);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, 0);

}

/*Scan the touch screen, calling in timer*/
void ADC_scan_task(void)
{
    if(g_axis == 1)
    {

    	/* change pins for ADC in channel 1*/
    	ADC_2();

    	delayms(100);
    	/*Calculation for 320 pixels*/
    	g_val_y = (int)(g_adcOut_y-440)/9;

        g_axis = 0;

    }

    else
    {

    	/* change pins for ADC in channel 5*/
    	ADC_1();
    	delayms(100);
    	/*Calculation for 240 pixels*/
    	g_val_x = (int)(g_adcOut_x-1000)/9.5;

        g_axis = 1;

        /*If touches the ADC value reduced below 4000*/
        if(g_adcOut_x < 4000)
        	g_touch_flag = 1;

        else
        	g_touch_flag = 0;

    }


}

/* check the touch inside the points */

int ADC_check_point(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{

	uint8_t k;
	k=( ((x0<g_val_x) && (x1>g_val_x))  &&  ((y0<g_val_y) && (y1>g_val_y)) );
   	return(k);
}

#endif /* ADC_H_ */
