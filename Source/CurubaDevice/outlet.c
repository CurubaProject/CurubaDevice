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
#include "outlet.h"
#include "commun.h"

#include "device.h"
#include "typeDevice.h"
#include "deviceInfoState.h"

#include "util.h"
#include "adcBuffer.h"

#include "board.h"

void initDevice_outlet()
{
	setHeartbeatTime(HEARTBEAT_TIME);
}

void heartBeat_outlet()
{
	comms payload;

	payload.payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
	payload.status = STATUS_INACTIVE; // TODO OLD STATUS
	payload.state = STATE_OFF; // TODO OLD STATE
	payload.device = DEVICE_1;
	payload.type = TYPE_OUTLET;

	int state = GetState(DEVICE_1);
	if(IsStateChange(state, payload.state)) // TODO OLD STATE
	{
		payload.data = ComputationWattHour(getValues());
	}
	else
	{
		payload.state = state;
		payload.data = ComputationWattHour(getValues());
	}

	pushTransmit(&payload);

	payload.payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
	payload.status = STATUS_INACTIVE; // TODO OLD STATUS
	payload.state = STATE_OFF; // TODO OLD STATE
	payload.device = DEVICE_2;
	payload.type = TYPE_OUTLET;

	state = GetState(DEVICE_2);
	if(IsStateChange(state, payload.state)) // TODO OLD STATE
	{
		payload.data = ComputationWattHour(getValues()); //TODO ComputationWattHour depend on GetState call ADCREAD PROBLEM!
	}
	else
	{
		payload.state = state;
		payload.data = ComputationWattHour(getValues());
	}

	pushTransmit(&payload);
}

void controlCommsReceive_outlet(comms* receivePop)
{
	comms payload;

	payload.payloadid = PAYLOAD_CONTROL_RESPONSE;
	payload.status = receivePop->status;
	payload.state = changeIO_outlet(receivePop->device, receivePop->state);
	payload.device = receivePop->device;
	payload.type = TYPE_OUTLET;
	payload.data = ComputationWattHour(getValues());

	pushTransmit(&payload);
}
//TODO OPTIMIZE
long int changeIO_outlet(int deviceNumber, int command)
{
	static long int currentState = STATE_OFF;

	setState(deviceNumber, command);

	currentState = GetState(deviceNumber);

	if (currentState != command)
	{
		currentState = STATE_NOLOAD;
	}

	return currentState;
}

void infoCommsReceive_outlet()
{
	comms payload;

	payload.payloadid = PAYLOAD_INFO_RESPONSE;
	payload.status = STATUS_INACTIVE;
	payload.state = STATE_OFF;
	payload.device = DEVICE_1;
	payload.type = TYPE_OUTLET;
	payload.data = 0;

	pushTransmit(&payload);

	payload.device = DEVICE_2;
	pushTransmit(&payload);
}

// Interupt

void timer2_Execute_outlet()
{
	//Nothing to do
}
