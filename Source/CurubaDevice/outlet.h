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
#ifndef OUTLET_H
#define OUTLET_H

#define HEARTBEAT_TIME 0x3C00

void initDevice_outlet(int* Tab_ADC10);
void initListComms_outlet();
void heartBeat_outlet(comms** transmitFirst, comms** transmitPush, int* Tab_ADC10);
void controlCommsReceive_outlet(TYPEDEVICE* device,
								 comms* ReceivePop, 
								 comms** transmitFirst, comms** transmitPush,
								int* Tab_ADC10);
void changeIO_outlet(int deviceNumber, int state, int* Tab_ADC10);
void infoCommsReceive_outlet(comms** transmitFirst, comms** transmitPush, int* Tab_ADC10);
void initTIMER1_outlet();
void initTIMER2_outlet();

// Interupt
void timer2_Execute_outlet(void);

#endif
