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
#include "device.h"
#include "util.h"
#include "commun.h"
#include "outlet.h"

#include "typeDevice.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

comms devices_outlet[2];

void initDevice_outlet(int* Tab_ADC10){
}

void initListComms_outlet() {
	comms d;

	d.payloadid = PAYLOAD_INFO_RESPONSE;
	d.status = STATUS_INACTIVE;
	d.state = STATE_OFF;
	d.device = DEVICE_1;
	d.type = TYPE_OUTLET;
	d.data = 0;

	devices_outlet[0] = d;

	d.device = DEVICE_2;

	devices_outlet[1] = d;
}

void heartBeat_outlet(comms** transmitFirst, comms** transmitPush, int* Tab_ADC10) {
	int state = GetState(devices_outlet[0].device, Tab_ADC10);
	if(IsStateChange(state, devices_outlet[0].state))
	{
		devices_outlet[0].payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
		devices_outlet[0].data = ComputationWattHour(Tab_ADC10);
	}
	else
	{
		devices_outlet[0].payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
		devices_outlet[0].state = state;
		devices_outlet[0].data = ComputationWattHour(Tab_ADC10);
	}

	Push(transmitFirst, transmitPush, devices_outlet[0]);//comms_transmit[0] = devices[0];

	if(IsStateChange(state, devices_outlet[1].state))
	{
		devices_outlet[1].payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
		devices_outlet[1].data = ComputationWattHour(Tab_ADC10);
	}
	else
	{
		devices_outlet[1].payloadid = PAYLOAD_HEARTBEAT_RESPONSE;
		devices_outlet[1].state = state;
		devices_outlet[1].data = ComputationWattHour(Tab_ADC10);
	}

	Push(transmitFirst, transmitPush, devices_outlet[1]);//comms_transmit[1] = devices[1];
}

void controlCommsReceive_outlet(TYPEDEVICE* device,
								 comms* ReceivePop, 
								 comms** transmitFirst, comms** transmitPush,
								int* Tab_ADC10) {

	if (ReceivePop->device == DEVICE_1)
	{
		devices_outlet[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_outlet[0].status = ReceivePop->status;
		devices_outlet[0].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_1, Tab_ADC10);
		devices_outlet[0].device = ReceivePop->device;
		devices_outlet[0].type = TYPE_OUTLET;
		devices_outlet[0].data = ComputationWattHour(Tab_ADC10);

		Push(transmitFirst, transmitPush, devices_outlet[0]);
	}
	else if (ReceivePop->device == DEVICE_2)
	{
		devices_outlet[1].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_outlet[1].status = ReceivePop->status;
		devices_outlet[1].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_2, Tab_ADC10);
		devices_outlet[1].device = ReceivePop->device;
		devices_outlet[1].type = TYPE_OUTLET;
		devices_outlet[1].data = ComputationWattHour(Tab_ADC10);

		Push(transmitFirst, transmitPush, devices_outlet[1]);
	}
}

void changeIO_outlet(int deviceNumber, int state, int* Tab_ADC10) {
	if (deviceNumber == DEVICE_1) {
		if (state == STATE_ON)
		{
			CTRL_OUT |= CTRL_1;
		}
		else if(state == STATE_OFF)
		{
			CTRL_OUT &= ~CTRL_1;
		}
	}
	else if (deviceNumber == DEVICE_2 )
	{
		if (state == STATE_ON)
		{
			CTRL_OUT |= CTRL_2;
		}
		else if(state == STATE_OFF)
		{
			CTRL_OUT &= ~CTRL_2;
		}
	}
}

void infoCommsReceive_outlet(comms** transmitFirst, comms** transmitPush, int* Tab_ADC10) {
	Push(transmitFirst, transmitPush, devices_outlet[0]);
	Push(transmitFirst, transmitPush, devices_outlet[1]);
}

void initTIMER1_outlet() {
	TA1CCR0 |= HEARTBEAT_TIME;
}

void initTIMER2_outlet() {
	//Nothing to do
}

// Interupt

void timer2_Execute_outlet(int* Tab_ADC10) {
	//Nothing to do
}
