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
	static long int state = STATE_OFF;

	state = GetState(deviceNumber, Tab_ADC10);

	if(state != commande)
	{
		device->changeIO(deviceNumber);
	}

	state = GetState(deviceNumber, Tab_ADC10);

	if (state != commande)
	{
		state = STATE_NOLOAD;
	}

	return state;
}

TYPEDEVICE* createDimmer() {
	TYPEDEVICE* device = (TYPEDEVICE*) malloc(sizeof *device);

	device->initListComms = initListComms_dimmer;
	device->heartBeat = heartBeat_dimmer;
	device->controlCommsReceive = controlCommsReceive_dimmer;
	device->infoCommsReceive = infoCommsReceive_dimmer;
	device->changeIO = changeIO_dimmer;
	device->initTIMER1 = initTIMER1_dimmer;
	device->initTIMER2 = initTIMER2_dimmer;
	device->initDevice = initDevice_dimmer;
	return device;
}

TYPEDEVICE* createOutlet() {
	TYPEDEVICE* device = (TYPEDEVICE*) malloc(sizeof *device);

	//TODO Assign New Dimmer Method

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
	}

	return device;
}

