/*
 * timer.h
 *
 *  Created on: Dec 21, 2017
 *      Author: user2
 */

#ifndef TIMER_H_
#define TIMER_H_

#define TICKRATE_HZ1 (20)/* 20 ticks per second */

uint16_t  g_starts=0, g_current_num=0;

/**
 * @brief	Handle interrupt from Timer 0
 * @return	Nothing
 */

void CT32B0_IRQHandler(void)
{
	static uint16_t tim = 0;
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1))
	{
		Chip_TIMER_ClearMatch(LPC_TIMER0, 1); //clear timer flag

		ADC_scan_task();

		if(g_starts == 1)
			tim++;
	    if(tim > 100)//n increment every 30 cycle
	     {
	    	tim = 0;
	    	g_current_num++;
	     }

	}
}

void Timer_Initialize(void)
{
	uint32_t timerBaseClock;
	/* Initialize Timer 0 */
	Chip_TIMER_Init(LPC_TIMER0);
	/* Setup prescale value on Timer 0 to PCLK */
	Chip_TIMER_PrescaleSet(LPC_TIMER0, 0);
	/* Reset timer */
	Chip_TIMER_Reset(LPC_TIMER0);
	/* Enable timer to generate interrupts when time matches */
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
	/* Get rate of timer base clock */
	timerBaseClock = Chip_Clock_GetAsyncSyscon_ClockRate();
	/* Setup Timer for a match */
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, (timerBaseClock / TICKRATE_HZ1));
	/* Setup timer to restart when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 1);
	/* Start timer */
	Chip_TIMER_Enable(LPC_TIMER0);
	/* Clear timer of any pending interrupts */
	NVIC_ClearPendingIRQ(CT32B0_IRQn);
	/* Enable timer interrupts */
	NVIC_EnableIRQ(CT32B0_IRQn);
	/* Set priority */
	NVIC_SetPriority(CT32B0_IRQn, 4);
}



#endif /* TIMER_H_ */
