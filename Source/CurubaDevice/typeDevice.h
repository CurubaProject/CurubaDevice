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
#ifndef TYPEDEVICE_H
#define TYPEDEVICE_H

#include "CommsManager.h"
#include "device.h"

typedef struct typeDevice {
	InitDevice initDevice;
	InitListCommsDevice initListComms;
	HeartBeatDevice heartBeat;
	ControlCommsReceiveDevice controlCommsReceive;
	InfoCommsReceiveDevice infoCommsReceive;
	ChangeIODevice changeIO;
	InitTIMER1Device initTIMER1;
	InitTIMER2Device initTIMER2;
    TIMER2_Execute timer2_execute;
} TYPEDEVICE;

int GetState(int deviceNumber);
int ChangeIO_Device(TYPEDEVICE* device, int commande, int deviceNumber);
TYPEDEVICE* createTypeDevice(int typeModule);
TYPEDEVICE* createDimmer();

#endif
