// ------------------------------------------------------------------------------------------------
// ----------------- Curuba Device ----------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Copyright (C) 2013 Mathieu Bélanger (mathieu.b.belanger@usherbrooke.ca)
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses>.
//
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this Program, or any covered work, by linking or combining it
// with "CC3000 Host Driver Implementation" (or a modified version of that
// library), containing parts covered by the terms of Texas Instruments
// Incorporated's BSD style license, the licensors of this Program grant you
// additional permission to convey the resulting work. {Corresponding Source
// for a non-source form of such a combination shall include the source code
// for the parts of "CC3000 Host Driver Implementation" used as well as that
// of the covered work.}
// ------------------------------------------------------------------------------------------------
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
		TA0CTL &= TIMER_OFF;
	}
	else if (timer_number == TIMER_1)
	{
		TA1CTL &= TIMER_OFF;
	}
	else if (timer_number == TIMER_2)
	{
		TA2CTL &= TIMER_OFF;
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

	ADC10MCTL0 &= ADC_SEL_RESET;

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

int ComputationWattHour(int *Tab_ADC10)
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

	for (i = (sizeof(Tab_ADC10)-1); i--; )
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
