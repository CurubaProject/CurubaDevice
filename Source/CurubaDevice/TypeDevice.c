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
#include "typeDevice.h"
#include "commun.h"
#include "CommsManager.h"
#include "util.h"

#include "dimmer.h"
#include "outlet.h"

#include "board.h"
#include <msp430.h>

#include <stdlib.h>

int GetState(int deviceNumber, int* Tab_ADC10)
{
	static long int state = 0;
	ADCRead(deviceNumber);

	state = ComputationWattHour(Tab_ADC10) > 1 ? STATE_ON : STATE_OFF;

	return state;
}

int ChangeIO_Device(TYPEDEVICE* device, int commande, int deviceNumber, int* Tab_ADC10)
{
	static long int currentState = STATE_OFF;

	currentState = GetState(deviceNumber, Tab_ADC10);

	if(currentState != commande)
	{
		device->changeIO(deviceNumber, commande, Tab_ADC10);
	}

	currentState = GetState(deviceNumber, Tab_ADC10);

	if (currentState != commande)
	{
		currentState = STATE_NOLOAD;
	}

	return currentState;
}

TYPEDEVICE* createDimmer() {
	TYPEDEVICE* device = (TYPEDEVICE*) malloc(sizeof *device);

	device->initDevice = initDevice_dimmer;
	device->initListComms = initListComms_dimmer;
	device->heartBeat = heartBeat_dimmer;
	device->controlCommsReceive = controlCommsReceive_dimmer;
	device->infoCommsReceive = infoCommsReceive_dimmer;
	device->changeIO = changeIO_dimmer;
	device->initTIMER1 = initTIMER1_dimmer;
	device->initTIMER2 = initTIMER2_dimmer;
	device->timer2_execute = timer2_Execute_dimmer;

	return device;
}

TYPEDEVICE* createOutlet() {
	TYPEDEVICE* device = (TYPEDEVICE*) malloc(sizeof *device);

	device->initDevice = initDevice_outlet;
	device->initListComms = initListComms_outlet;
	device->heartBeat = heartBeat_outlet;
	device->controlCommsReceive = controlCommsReceive_outlet;
	device->infoCommsReceive = infoCommsReceive_outlet;
	device->changeIO = changeIO_outlet;
	device->initTIMER1 = initTIMER1_outlet;
	device->initTIMER2 = initTIMER2_outlet;
	device->timer2_execute = timer2_Execute_outlet;

	return device;
}

TYPEDEVICE* createTypeDevice(int typeModule) {
	TYPEDEVICE* device;

	switch (typeModule) {
		case TYPE_DIMMER :
			device = createDimmer();
			break;
		case TYPE_OUTLET :
			device = createOutlet();
			break;
		default :

			while (1)
			{
				P1OUT ^= BIT4;
				__delay_cycles(25000000);
			}

	}

	return device;
}

