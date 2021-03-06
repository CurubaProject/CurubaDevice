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
#ifndef DIMMER_H
#define DIMMER_H

#include "CommsManager.h"
#include "typeDevice.h"

#define HEARTBEAT_5SEC 0x0A00 //5 sec //0A00

void initDevice_dimmer();
void initListComms_dimmer();
void heartBeat_dimmer(comms** transmitFirst, comms** transmitPush);
void controlCommsReceive_dimmer(TYPEDEVICE* device,
								comms* ReceivePop, 
								comms** transmitFirst, comms** transmitPush);
void infoCommsReceive_dimmer(comms** transmitFirst, comms** transmitPush);
void changeIO_dimmer(int deviceNumber, int state);
void initTIMER1_dimmer();
void initTIMER2_dimmer() ;

// Interupt
void timer2_Execute_dimmer();
void turnOffligth();
void toggleControl();
void turnOnlight();

#endif
