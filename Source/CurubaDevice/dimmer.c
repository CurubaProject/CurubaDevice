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
#include "CommsManager.h"
#include "util.h"
#include "typeDevice.h"
#include "commun.h"

#include "adcBuffer.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

comms devices_dimmer[2]; // TODO IMPROVE list devices
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

void initListComms_dimmer()
{ // TODO REMOVE THIS FUNCTION, NOT GOOD
	comms d;

	d.payloadid = PAYLOAD_INFO_RESPONSE;
	d.status = STATUS_INACTIVE;
	d.state = STATE_OFF;
	d.device = DEVICE_1;
	d.type = TYPE_DIMMER;
	d.data = 0;

	devices_dimmer[0] = d;
}

void heartBeat_dimmer(comms** transmitFirst, comms** transmitPush)
{
	int state = GetState(devices_dimmer[0].device);

	if(IsStateChange(state, devices_dimmer[0].state))
	{
		devices_dimmer[0].payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
		devices_dimmer[0].data = ComputationWattHour(getValues());
	}
	else
	{
		devices_dimmer[0].payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
		devices_dimmer[0].state = state;                             //TODO remove ducktape
		devices_dimmer[0].data = ComputationWattHour(getValues());
	}

	Push(transmitFirst, transmitPush, devices_dimmer[0]);
}

void controlCommsReceive_dimmer(TYPEDEVICE* device,
								comms* ReceivePop, 
								comms** transmitFirst,comms** transmitPush)
{
	if (ReceivePop->status == STATUS_ACTIVE)
	{
		//Status of the module
		devices_dimmer[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_dimmer[0].status = STATUS_ACTIVE;
		devices_dimmer[0].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_1);
		devices_dimmer[0].device = ReceivePop->device;
		devices_dimmer[0].type = TYPE_DIMMER;
		devices_dimmer[0].data = ComputationWattHour(getValues());

		//Enable Zero cross for dimmer function
		ZERO_CROSS_IE |= ZERO_CROSS;
		if (ReceivePop->state == STATE_ON && ReceivePop->data > 10)
		{
			TA2CCR0 = (int) (101 - ReceivePop->data) / 100.0 * PERIODHZ;
		}
		else
		{
			ZERO_CROSS_IE &= ~ZERO_CROSS;
			TimerStop(TIMER_2);
			turnOffligth();
		}

		Push(transmitFirst, transmitPush, devices_dimmer[0]);
	}
	else if (ReceivePop->status == STATUS_INACTIVE)
	{
		//Disable Zero cross for dimmer function
		ZERO_CROSS_IE &= ~ZERO_CROSS;
		ZERO_CROSS_IFG &= ~ZERO_CROSS;

		//Timer2 count to 0
		TA2CCR0 = 0;

		//Status of the module
		devices_dimmer[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_dimmer[0].status = STATUS_INACTIVE;
		devices_dimmer[0].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_1);
		devices_dimmer[0].device = ReceivePop->device;
		devices_dimmer[0].type = TYPE_DIMMER;
		devices_dimmer[0].data = ComputationWattHour(getValues());

		Push(transmitFirst, transmitPush, devices_dimmer[0]);
	}
}

void infoCommsReceive_dimmer(comms** transmitFirst,comms** transmitPush)
{
	devices_dimmer[0].payloadid = PAYLOAD_INFO_RESPONSE;
	devices_dimmer[0].status = STATUS_INACTIVE;
	devices_dimmer[0].state = GetState(DEVICE_1);
	devices_dimmer[0].device = DEVICE_1;
	devices_dimmer[0].type = TYPE_DIMMER;
	devices_dimmer[0].data = ComputationWattHour(getValues());

	Push(transmitFirst, transmitPush, devices_dimmer[0]);
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
