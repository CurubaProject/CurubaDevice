/*****************************************************************************
*
*  board.h - FRAM board definitions
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
#include "dimmer.h"

#include "device.h"
#include "commsManager.h"
#include "deviceInfoState.h"
#include "typeDevice.h"
#include "commun.h"

#include "util.h"
#include "adcBuffer.h"

#include "board.h"

static int switchDimmer = 0;

void initDevice_dimmer()
{
	setHeartbeatTime(HEARTBEAT_5SEC);

	setCurrentStateDimmerOn();

	if(GetState(DEVICE_1) == STATE_ON)
	{
		toggleControl();
	}

	switchDimmer = getCurrentStateDimmer();
}

void heartBeat_dimmer()
{
	comms payload;

	payload.payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
	payload.device = DEVICE_1;
	payload.type = TYPE_DIMMER;
	payload.status = getDeviceInfoState()->currentStatus;
	payload.state = getDeviceInfoState()->previousState;

	int state = GetState(DEVICE_1);

	if(IsStateChange(state, payload.state))
	{
		payload.data = ComputationWattHour(getValues());
	}
	else
	{
		payload.state = state;
		payload.data = ComputationWattHour(getValues());
	}

	pushTransmit(&payload);
}

void controlCommsReceive_dimmer(comms* receivePop)
{
	if (receivePop->status == STATUS_ACTIVE)
	{
		comms payload;

		payload.payloadid = PAYLOAD_CONTROL_RESPONSE;
		payload.status = STATUS_ACTIVE;
		payload.state = changeIO_dimmer(DEVICE_1, receivePop->state);
		payload.device = DEVICE_1;
		payload.type = TYPE_DIMMER;
		payload.data = ComputationWattHour(getValues());

		enableDimmer();
		if (receivePop->state == STATE_ON && receivePop->data > 10)
		{
			setDimmerTime((unsigned int) (101 - receivePop->data) / 100.0 * PERIODHZ);
		}
		else
		{
			disableDimmer();
			TimerStop(TIMER_2);
			turnOffligth(switchDimmer);
		}

		pushTransmit(&payload);
	}
	else if (receivePop->status == STATUS_INACTIVE)
	{
		disableDimmer();
		setDimmerTime(0);

		comms payload;

		payload.payloadid = PAYLOAD_CONTROL_RESPONSE;
		payload.status = STATUS_INACTIVE;
		payload.state = changeIO_dimmer(DEVICE_1, receivePop->state);
		payload.device = DEVICE_1;
		payload.type = TYPE_DIMMER;
		payload.data = ComputationWattHour(getValues());

		pushTransmit(&payload);
	}

	getDeviceInfoState()->currentStatus = receivePop->status;
}

void infoCommsReceive_dimmer()
{
	comms payload;

	payload.payloadid = PAYLOAD_INFO_RESPONSE;
	payload.status = STATUS_INACTIVE;
	payload.state = GetState(DEVICE_1);
	payload.device = DEVICE_1;
	payload.type = TYPE_DIMMER;
	payload.data = ComputationWattHour(getValues());

	pushTransmit(&payload);
}
//TODO OPTIMIZE
long int changeIO_dimmer(int deviceNumber, int command)
{
	static long int currentState = STATE_OFF;

	int current_state = GetState(deviceNumber);
	setStateDimmer(current_state);
	getDeviceInfoState()->previousState = current_state;

	currentState = GetState(deviceNumber);

	if (currentState != command)
	{
		currentState = STATE_NOLOAD;
	}

	return currentState;
}

// Interupt
void timer2_Execute_dimmer()
{
	turnOnlight(switchDimmer);
	TimerStop(TIMER_2);
	resetDimmerTimer();
}
