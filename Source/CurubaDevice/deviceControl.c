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
#include "deviceControl.h"
#include "interuptDeviceControl.h"

#include "typeDevice.h"
#include "communication.h"
#include "eventManager.h"
#include "adcBuffer.h"
#include "deviceInfoState.h"

#include "board.h"

TYPEDEVICE* device = 0;

void callback_readCurrent();
void callback_heartbeatTimer();
void callback_dimmerTimer();
void callback_ledTimer();

void initApp()
{
	pio_init();

	//Type of module
	device = createTypeDevice(readAppSwitch());

	initReadCurrent(&callback_readCurrent);
	initTIMER0();
	initHearbeatTimer(&callback_heartbeatTimer);
	initDimmerTimer(&callback_dimmerTimer);
	initLedTimer(&callback_ledTimer);

	initCommunication(&device);
	device->initDevice();

	init_bis_register();
}

void callback_readCurrent()
{
	setValue(getValueCurrentVolt());

	if ( ! next() )
	{
		TimerStop(TIMER_0);
	}
}

void callback_heartbeatTimer()
{
	 notify(EVENT_HEARTBEAT_READYTOSEND);
}

void callback_dimmerTimer()
{
	device->timer2_execute();
}

void callback_ledTimer()
{
	getDeviceInfoState()->timeCounter = ++(getDeviceInfoState()->timeCounter);
}
