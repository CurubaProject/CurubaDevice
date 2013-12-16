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
#include "CommsManager.h"

comms comms_receive_array[5];
comms comms_transmit_array[5];

comms* ReceiveFirst = comms_receive_array;
comms* ReceivePush = comms_receive_array;
comms* ReceivePop = comms_receive_array;
comms* TransmitFirst = comms_transmit_array;
comms* TransmitPush = comms_transmit_array;
comms* TransmitPop = comms_transmit_array;

int Pop(comms** PtrFirst, comms** PtrPush, comms** PtrPop)
{
	int StateComms = 0;

	if (*PtrPop == *PtrPush)
	{
		StateComms = NOCOMMS;
	}
	else if (*PtrPop == ((*PtrFirst)+4))
	{
		*PtrPop = *PtrFirst;
		StateComms = NEWCOMMS;
	}
	else
	{
		(*PtrPop)++;
		StateComms = NEWCOMMS;
	}

	return StateComms;
}

void Push(comms** PtrFirst, comms** PtrPush, comms NewStruct)
{
	if (*PtrPush == ((*PtrFirst)+4))
	{
		*PtrPush = *PtrFirst;
	}
	else
	{
		(*PtrPush)++;
	}

	**PtrPush = NewStruct;
}

comms** getTransmitFirst() {
	return &TransmitFirst;
}

comms** getTransmitPush() {
	return &TransmitPush;
}
