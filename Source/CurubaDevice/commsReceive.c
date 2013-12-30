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
#include "commsReceive.h"
#include "commun.h"
#include "typeDevice.h"
#include "commsManager.h"

#include "board.h"

void infoCommsReceive(TYPEDEVICE* device, comms* receivePop)
{
	TimerStop(TIMER_1);
	device->infoCommsReceive();
	TimerStart(TIMER_1);
}

void controlCommsReceive(TYPEDEVICE* device, comms* receivePop)
{
	TimerStop(TIMER_1);
	device->controlCommsReceive(receivePop);
	TimerStart(TIMER_1);
}

CommsReceive receiveComms(int payloadId)
{
	CommsReceive result;

	switch (payloadId)
	{
		case PAYLOAD_INFO_REQUEST :
			result = &infoCommsReceive;
			break;
		case PAYLOAD_CONTROL_REQUEST :
			result = &controlCommsReceive;
			break;
		case PAYLOAD_CONFIG_REQUEST :
			//TODO PAYLOAD_CONFIG_REQUEST
			break;
		default :
			//TODO ON error receive comms
			break;
	}

	return result;
}
