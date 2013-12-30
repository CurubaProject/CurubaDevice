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
#include "board.h"
#include "boardMSP430.h"
#include <msp430.h>

Callback _callback_readCurrent = 0;
Callback _callback_hearbeatTimer = 0;
Callback _callback_dimmerTimer = 0;
Callback _callback_ledTimer = 0;

void initReadCurrent(Callback callback_readCurrent)
{
	//ADC10 Init
	ADC10CTL0 |= ADC10SHT_2 + ADC10ON;
	ADC10CTL1 |= ADC10SHP + ADC10DIV_4 + ADC10SSEL_2;
	ADC10CTL2 |= ADC10RES;                            // 10 bits resolution
	ADC10MCTL0 |= ADC10SREF_7;
	ADC10IE |= ADC10IE0;                    		  // Interupt enable end of conversion
	ADC10IFG &= 0x0000;                               // Reset all interupt flag
	ADC10CTL0 |= ADC10ENC;                            // ADC enable conversion

	_callback_readCurrent = callback_readCurrent;
}

void initTIMER0(void)
{
	//Timer0 Init (for ADC)
	TA0CTL |= TACLR;
	TA0CTL |= TASSEL_2 + TAIE;
	TA0CCR0 &= 0x0000;
	TA0CCR0 = 812;
}

void initHearbeatTimer(Callback callback_hearbeatTimer)
{
	//Timer1 Init (for Heartbeat)
	TA1CTL |= TACLR;
	TA1CTL |= TASSEL_1 + ID_3 + TAIE;
	TA1EX0 |= TAIDEX_7;

	_callback_hearbeatTimer = callback_hearbeatTimer;
}

void initDimmerTimer(Callback callback_dimmerTimer)
{
	//Timer2 Init (for Dimmer)
	TA2CTL &= MC_0;
	TA2CTL |= TACLR;
	TA2CTL |= TASSEL_2 + TAIE;

	TA2CCR0 = 0;

	_callback_dimmerTimer = callback_dimmerTimer;
}

void initLedTimer(Callback callback_ledTimer)
{
	//Timer for Server connection delay time
	TB0CTL |= TBCLR;
	TB0CTL |= TBSSEL_1 + ID_3 + TBIE;
	TB0EX0 |= TBIDEX_7; //Timer clock = ACLK/64 = 512 Hz
	TB0CCR0 = (int) 32;

	_callback_ledTimer = callback_ledTimer;

	TimerStart(TIMERB_0);//Never stop this timer
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
			_callback_readCurrent();
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
			_callback_hearbeatTimer();
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
			_callback_dimmerTimer();
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
        	_callback_ledTimer();
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
