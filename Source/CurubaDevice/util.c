#include "util.h"
#include "commun.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

void TimerStart(int timer_number)
{
	if (timer_number == TIMER_0)
	{
		TA0CTL |= MC_1;
	}
	else if (timer_number == TIMER_1)
	{
		TA1CTL |= MC_1;
	}
	else if (timer_number == TIMER_2)
	{
		TA2CTL |= MC_1;
	}
}

void TimerStop(int timer_number)
{
	if (timer_number == TIMER_0)
	{
		TA0CTL &= 0xFFCF;                 //TODO MC_0 //variable
	}
	else if (timer_number == TIMER_1)
	{
		TA1CTL &= 0xFFCF;                 //TODO MC_0 //variable
	}
	else if (timer_number == TIMER_2)
	{
		TA2CTL &= 0xFFCF;                 //TODO MC_0 //variable
	}
}

void ADCRead(int ADC_number)
{
	//to watch
	if ((TA0CTL>>4 & 0x0001) == 1) //(TA0CTL == MC_1)
	{
		//Wait for the end of sampling
		__delay_cycles(500000);
	}

	ADC10CTL0 &= ~ADC10ENC;

	ADC10MCTL0 &= 0xFFF0;                  //TODO Remettre les bit s/lection ADC

	ADC10MCTL0 |= (0x0000 & (ADC_number - 1));

	ADC10CTL0 |= ADC10ENC;

	TimerStart(TIMER_0);

	__delay_cycles(500000);
}

// TODO ADD NOLOAD RETURN
int IsStateChange(int State, int StateComms)
{
	return ((State == STATE_ON && StateComms == STATE_ON) ||
			(State == STATE_OFF && (StateComms == STATE_OFF || StateComms == STATE_NOLOAD)));
}

int ComputationWattHour(int *Tab_ADC10) //TODO Rename
{
	static long int sum_value_ADC = 0;
	static long int sum_analog_value_ADC = 0;
	static long int mean = 0;
	static long int max_value_ADC = 0;
	static long int max_analog_value_ADC = 0;
	static long int volt = 0;
	static float current = 0;
	static long int power = 0;
	static long int temp_max = 0;
	static long int pente = 2;

	int i = 0;

	for (i = 63; i--; )
	{
		if(max_value_ADC < Tab_ADC10[i])
		{
			temp_max = Tab_ADC10[i];
			if ((temp_max - Tab_ADC10[i-1] <= pente) && (temp_max - Tab_ADC10[i-2] <= 2*pente))
			{
				max_value_ADC = Tab_ADC10[i];
			}
		}
		sum_value_ADC += Tab_ADC10[i];
	}

	max_analog_value_ADC = (1000 * max_value_ADC)>>10;
	sum_analog_value_ADC = (1000 * sum_value_ADC)>>10;
	mean = sum_analog_value_ADC>>6;

	volt = ((max_analog_value_ADC - mean) * 169) / 239;                            //Volt RMS value   sqrt(2) = 1.4142
	current = (float)volt / 37;                                                    //Magic number from IC current sensor data sheet
	power = GRID_VOLTAGE * current;

	if (power < 30)
	{
		power = 0;
	}

	return power;
}
