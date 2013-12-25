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
#include "commsManager.h"

/* Private Function */

int pop(comms**, comms**, comms**);
void push(comms*, comms**, comms**, comms*);

/* End Private Function */

comms comms_receive_array[5];
comms comms_transmit_array[5];

comms* ReceiveFirst = comms_receive_array;
comms* ReceivePush = comms_receive_array;
comms* TransmitFirst = comms_transmit_array;
comms* TransmitPush = comms_transmit_array;

int pop(comms** ptrFirst, comms** ptrLast, comms** ptrPop)
{
	int StateComms = NOCOMMS;

	if (*ptrFirst == *ptrLast)
	{
		StateComms = NOCOMMS;
	}
	else
	{
		*ptrPop = *ptrFirst;
		(*ptrFirst)++;
		StateComms = NEWCOMMS;
	}

	return StateComms;
}

void push(comms* tab, comms** ptrFirst, comms** ptrLast, comms* newComms)
{
	**ptrLast = *newComms;

	if (*ptrLast == (tab+4))
	{
		*ptrLast = tab;

		if (*ptrFirst == *ptrLast)
		{
			(*ptrFirst)++;
		}
	}
	else
	{
		(*ptrLast)++;
	}
}

int popTransmit(comms** ptrPop)
{
	return pop(&TransmitFirst, &TransmitPush, ptrPop);
}

void pushTransmit(comms* newComms)
{
	push(comms_transmit_array, &TransmitFirst, &TransmitPush, newComms);
}

int popReceive(comms** PtrPop)
{
	return pop(&ReceiveFirst, &ReceivePush, PtrPop);
}

void pushReceive(comms* newComms)
{
	push(comms_receive_array, &ReceiveFirst, &ReceivePush, newComms);
}