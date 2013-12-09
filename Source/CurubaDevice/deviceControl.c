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
#include "deviceControl.h"
#include "CommsManager.h"
#include "commun.h"
#include "util.h"
#include "typeDevice.h"
#include "interuptDeviceControl.h"

int Tab_ADC10[64] = { 0 };   //a isoler

void initApp(TYPEDEVICE* device) {
	initInterupt(device,Tab_ADC10);
	device->initDevice(Tab_ADC10);
}

void InitADC10(void)
{
	//ADC10 Init
	ADC10CTL0 |= ADC10SHT_2 + ADC10ON;
	ADC10CTL1 |= ADC10SHP + ADC10DIV_4 + ADC10SSEL_2;
	ADC10CTL2 |= ADC10RES;                            // 10 bits resolution
	ADC10MCTL0 |= ADC10SREF_7;
	ADC10IE |= ADC10IE0;                    		  // Interupt enable end of conversion
	ADC10IFG &= 0x0000;                               // Reset all interupt flag
	ADC10CTL0 |= ADC10ENC;                            // ADC enable conversion
}

void InitTIMER0(void)
{
	//Timer0 Init (pour ADC)
	TA0CTL |= TACLR;
	TA0CTL |= TASSEL_2 + ID_3 + TAIE;
	TA0CCR0 |= 0x00A4;                  // Value the timer count to //ps Jo veut modifier par une variable
	TA0EX0 |= TAIDEX_4;
}

void InitTIMER1(TYPEDEVICE* device)
{
	//Timer1 Init
	TA1CTL |= TACLR;
	TA1CTL |= TASSEL_1 + ID_3 + TAIE;

	device->initTIMER1();

	TA1EX0 |= TAIDEX_7;
}

//DOIT etre modifier badly en accordance avec MO
void InitTIMER2(TYPEDEVICE* device)
{
	//Timer2 Init Dimmer
	TA2CTL |= TACLR;

	device->initTIMER2();

	TA2CCR0 = 0;
	TA2EX0 |= TAIDEX_7;
}

void InitListComms(TYPEDEVICE* device)
{
	device->initListComms();
}

void InfoCommsReceive(TYPEDEVICE* device, comms* transmitFirst, comms* transmitPush)
{
	TimerStop(TIMER_1);
	device->infoCommsReceive(transmitFirst, transmitPush);
	TimerStart(TIMER_1);
}

void ControlCommsReceive(TYPEDEVICE* device, comms* ReceivePop, comms* transmitFirst,comms* transmitPush)
{
	TimerStop(TIMER_1);
	device->controlCommsReceive(device, ReceivePop, transmitFirst, transmitPush, Tab_ADC10);
	TimerStart(TIMER_1);
}

void HeartBeat(TYPEDEVICE* device)
{
	comms* transmitFirst = getTransmitFirst();
	comms* transmitPush = getTransmitPush();

	device->heartBeat(transmitFirst, transmitPush, Tab_ADC10);
}

unsigned short ReadAppSwitch(void) {
	return (SWITCH_B1_IN & 0x0F);
}
