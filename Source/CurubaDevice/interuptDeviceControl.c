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
#include "interuptDeviceControl.h"
#include "util.h"
#include "commun.h"

#include "heartbeat.h"
#include "adcBuffer.h"
#include "deviceInfoState.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

static TYPEDEVICE* _device = 0x0;

volatile unsigned long ulTimeRef;		//use for Timer LED

void initInterupt(TYPEDEVICE** device)
{
	_device = *device;
}

/*********************
 *  UNTERUPT VERTOR  *
 ********************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	switch (__even_in_range(ADC10IV, 12))  // For the 12 RTFM
	{
		case 0:                            // No interrupt
			break;
		case ADC10OVIFG:                   // conversion result overflow
			break;
		case ADC10TOVIFG:                  // conversion time overflow
			break;
		case ADC10HIIFG:                   // conversion result is higher than treshold
			break;
		case ADC10LOIFG:                   // conversion result is lower than treshold
			break;
		case ADC10INIFG:                   // conversion result is within the treshold
			break;
		case 12:
			setValue(ADC10MEM0);

			if ( ! next() )
			{
				TimerStop(TIMER_0);
			}
			break;
		default:
			break;
	}
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
	switch (__even_in_range(TA0IV, 14))    // For the 14 RTFM
	{
		case 0:                            // No interrupt
			break;
		case TA0IV_TA0CCR1:                // Capture/Compare 1
			break;
		case TA0IV_TA0CCR2:                // Capture/Compare 2
			break;
		case TA0IV_TA0CCR3:                // Capture/Compare 3
			break;
		case TA0IV_TA0CCR4:                // Capture/Compare 4
			break;
		case TA0IV_5:                      // Capture/Compare 5
			break;
		case TA0IV_6:                      // Capture/Compare 6
			break;
		case TA0IV_TA0IFG:                 // Timer overflow
			ADC10CTL0 |= ADC10SC;
			TA0CTL &= ~TAIFG;
			break;
		default:
			break;
	}
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
	switch (__even_in_range(TA1IV, 14))    // For the 14 RTFM
	{
		case 0:                            // No interrupt
			break;
		case TA1IV_TA1CCR1:                // Capture/Compare 1
			break;
		case TA1IV_TA1CCR2:                // Capture/Compare 2
			break;
		case TA1IV_3:                      // Capture/Compare 3
			break;
		case TA1IV_4:                      // Capture/Compare 4
			break;
		case TA1IV_5:                      // Capture/Compare 5
			break;
		case TA1IV_6:                      // Capture/Compare 6
			break;
		case TA1IV_TA1IFG:                 // Timer overflow
			setHeartbeatFlag(TRUE);
			TA1CTL &= ~TAIFG;
			break;
		default:
			break;
	}
}

#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void)
{
	switch (__even_in_range(TA2IV, 14))          // For the 14 RTFM
	{
		case 0:                                  // No interrupt
			break;
		case TA2IV_TA2CCR1:                      // Capture/Compare 1
			break;
		case TA2IV_TA2CCR2:                      // Capture/Compare 2
			break;
		case TA2IV_3:                            // Capture/Compare 3
			break;
		case TA2IV_4:                            // Capture/Compare 4
			break;
		case TA2IV_5:                            // Capture/Compare 5
			break;
		case TA2IV_6:                            // Capture/Compare 6
			break;
		case TA2IV_TA2IFG:                       // Timer overflow
			_device->timer2_execute();
			ZERO_CROSS_IE |= ZERO_CROSS;
			break;
		default:
			break;
	}
}

//Timer used to interrupt at every 500 msec
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
    static short sFlag = 0;
    unsigned long ultime;
    unsigned long timeCounter = getDeviceInfoState()->timeCounter;

    switch (__even_in_range(TB0IV, 14))
    {
        case 0:                            // No interrupt
            break;
        case TB0IV_TB0CCR1:                            // Capture/Compare 1
            break;
        case TB0IV_TB0CCR2:                            // Capture/Compare 2
            break;
        case TB0IV_3:                            // Capture/Compare 3
            break;
        case TB0IV_4:                            // Capture/Compare 4
            break;
        case TB0IV_5:                           // Capture/Compare 5
            break;
        case TB0IV_6:                           // Capture/Compare 6
            break;
        case TB0IV_TB0IFG:                           // Timer overflow
        	timeCounter++;
            if(sFlag == 0)
            {
                ulTimeRef = timeCounter;
                sFlag = 1;
                ultime = getTimeElapsed(timeCounter, ulTimeRef);
            }
            else if(sFlag == 1)
            {
                ultime = getTimeElapsed(timeCounter, ulTimeRef);
            }
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
                        sFlag = 0;
                    }
                    break;
                case LED_STATE_CONFIGURING:
                    if(ultime >= 1) //Toggle LED at each 8 Hz
                    {
                        toggleLed(WARNING_LED);
                        sFlag = 0;
                    }
                    break;
                default:
                    sFlag = 0;
                    break;
            }
            TB0CTL &= ~TBIFG;
            break;
        default:
            break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void IntGPIOHandler(void)
{
	switch(__even_in_range(P1IV, P1IV_P1IFG7))
	{
		case P1IV_P1IFG7:
			CTRL_OUT &= ~CTRL_1 & ~CTRL_2;
			TA2CTL |= MC_1;
			ZERO_CROSS_IE &= ~ZERO_CROSS;
			ZERO_CROSS_IFG &= ~ZERO_CROSS;
			break;
		default:
			break;
	}
}
