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
#include "util.h"
#include "typeDevice.h"
#include "commun.h"

#include "adcBuffer.h"
#include "deviceInfoState.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

static int SwitchDimmer = 0;

void initDevice_dimmer()
{
	CTRL_OUT |= CTRL_1;

	if(GetState(DEVICE_1) == STATE_ON)
	{
		toggleControl();
	}
	SwitchDimmer = (CTRL_1 + CTRL_2) & ~CTRL_OUT;
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

void controlCommsReceive_dimmer(TYPEDEVICE* device, comms* receivePop)
{
	if (receivePop->status == STATUS_ACTIVE)
	{
		comms payload;

		payload.payloadid = PAYLOAD_CONTROL_RESPONSE;
		payload.status = STATUS_ACTIVE;
		payload.state = ChangeIO_Device(device, receivePop->state, DEVICE_1);
		payload.device = DEVICE_1;
		payload.type = TYPE_DIMMER;
		payload.data = ComputationWattHour(getValues());

		//Enable Zero cross for dimmer function
		ZERO_CROSS_IE |= ZERO_CROSS;
		if (receivePop->state == STATE_ON && receivePop->data > 10)
		{
			TA2CCR0 = (int) (101 - receivePop->data) / 100.0 * PERIODHZ;
		}
		else
		{
			ZERO_CROSS_IE &= ~ZERO_CROSS;
			TimerStop(TIMER_2);
			turnOffligth();
		}

		pushTransmit(&payload);
	}
	else if (receivePop->status == STATUS_INACTIVE)
	{
		//Disable Zero cross for dimmer function
		ZERO_CROSS_IE &= ~ZERO_CROSS;
		ZERO_CROSS_IFG &= ~ZERO_CROSS;

		//Timer2 count to 0
		TA2CCR0 = 0;

		comms payload;

		payload.payloadid = PAYLOAD_CONTROL_RESPONSE;
		payload.status = STATUS_INACTIVE;
		payload.state = ChangeIO_Device(device, receivePop->state, DEVICE_1);
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

void changeIO_dimmer(int deviceNumber, int state)
{
	int current_state = GetState(deviceNumber);
	if (current_state == STATE_ON && (CTRL_OUT & (CTRL_1 + CTRL_2)) != 0)
	{
		SwitchDimmer = (CTRL_1 + CTRL_2) & CTRL_OUT;
	}
	else if(CTRL_OUT & (CTRL_1 + CTRL_2) != 0)
	{
		SwitchDimmer = (CTRL_1 + CTRL_2) & ~CTRL_OUT;
	}

	getDeviceInfoState()->previousState = current_state;
}

void initTIMER1_dimmer()
{
	TA1CCR0 |= HEARTBEAT_5SEC;
}

void initTIMER2_dimmer()
{
	TA2CCR0 = 0;
}

// Interupt

void timer2_Execute_dimmer()
{
	turnOnlight();
	TimerStop(TIMER_2);
	// Reset the count
	TA2R &= 0x0000;
	TA2CTL &= ~TAIFG;
}

void turnOffligth(void)
{
	if(CTRL_OUT & CTRL_1)
	{
		CTRL_OUT &= (~SwitchDimmer & CTRL_1);
	}
	else
	{
		CTRL_OUT |= (~SwitchDimmer & CTRL_1);
	}

	if(CTRL_OUT & CTRL_2)
	{
		CTRL_OUT &= (~SwitchDimmer & CTRL_2);
	}
	else
	{
		CTRL_OUT |= (~SwitchDimmer & CTRL_2);
	}
}

void toggleControl(void)
{
	if(CTRL_OUT & CTRL_1)
	{
		CTRL_OUT &= ~CTRL_1;
	}
	else
	{
		CTRL_OUT |= CTRL_1;
	}

	if(CTRL_OUT & CTRL_2)
	{
		CTRL_OUT &= ~CTRL_2;
	}
	else
	{
		CTRL_OUT |= CTRL_2;
	}
}

void turnOnlight()
{
	if(CTRL_OUT & CTRL_1)
	{
		CTRL_OUT &= (SwitchDimmer & CTRL_1);
	}
	else
	{
		CTRL_OUT |= (SwitchDimmer & CTRL_1);
	}

	if(CTRL_OUT & CTRL_2)
	{
		CTRL_OUT &= (SwitchDimmer & CTRL_2);
	}
	else
	{
		CTRL_OUT |= (SwitchDimmer & CTRL_2);
	}
}
