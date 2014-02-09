// ------------------------------------------------------------------------------------------------
// ----------------- Curuba Device ----------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Copyright (C) 2013 Mathieu B�langer (mathieu.b.belanger@usherbrooke.ca)
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
#include "board.h"
#include "adcBuffer.h"
#include "deviceInfoState.h"

int IsStateChange(int State, int StateComms)
{
	return ((State == STATE_ON && StateComms == STATE_ON) ||
			(State == STATE_OFF && (StateComms == STATE_OFF || StateComms == STATE_NOLOAD)));
}

int ComputationWattHour(int *Tab_ADC10)
{
	long int sum_value_ADC = 0;
	long long int sum_analog_value_ADC = 0;
	long int mean = 0;
	long int max_value_ADC = 0;
	long int max_analog_value_ADC = 0;
	long int volt = 0;
	float current = 0;
	long int power = 0;
	long int temp_max = 0;
	long int pente = 2;

	unsigned int i = 0;

	sum_value_ADC = 0;

	for (i = MAXTABADC-1; i >= 77; i--)
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

	max_analog_value_ADC = (1050 * max_value_ADC)>>10;
	sum_analog_value_ADC = (((long long int)1050) * sum_value_ADC)>>10;
	mean = (long int)(sum_analog_value_ADC/(MAXTABADC-77));

	volt = ((max_analog_value_ADC - mean) * 169) / 239;                            //Volt RMS value   sqrt(2) = 1.4142
	current = (float)volt / 37;    //                                                //Magic number from IC current sensor data sheet
	power = GRID_VOLTAGE * current;

	if (power < 8)
	{
		power = 0;
	}

	return power;
}

unsigned long getTimeElapsed(unsigned long ref, unsigned long lastcount)
{
	if(ref >= lastcount)
	{
		return(ref - lastcount);
	}
	else
	{
		return(((unsigned long) (0xFFFFFFFF) - lastcount) + ref);
	}
}

int GetState(int deviceNumber)
{
	static long int state = 0;
	readADC(deviceNumber);

	state = ComputationWattHour(getValues()) > 1 ? STATE_ON : STATE_OFF;

	return state;
}

void ledControl()
{
	static unsigned long ulTimeRef = 0;
    unsigned long timeCounter = getDeviceInfoState()->timeCounter;
    unsigned long ultime = getTimeElapsed(timeCounter, ulTimeRef);

	switch (getDeviceInfoState()->ledState)
	{
	  case LED_STATE_OFF:
		  turnLedOff(WARNING_LED);
		  break;
	  case LED_STATE_UNCONNECTED:
		  turnLedOn(WARNING_LED);
		  break;
	  case LED_STATE_CONNECTED:
		  if(ultime >= 16) // Toggle LED at each 1 Hz
		  {
			  toggleLed(WARNING_LED);
		  }
		  break;
	  case LED_STATE_CONFIGURING:
		  if(ultime >= 1) //Toggle LED at each 8 Hz
		  {
			  toggleLed(WARNING_LED);
		  }
		  break;
	  default:
		  break;
	}

	ulTimeRef = getDeviceInfoState()->timeCounter;
}
