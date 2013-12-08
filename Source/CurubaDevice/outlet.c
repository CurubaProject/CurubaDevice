#include "device.h"
#include "util.h"
#include "commun.h"

#include "typeDevice.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

comms devices_outlet[2];

void initListComms_outlet() {
	comms d;

	d.payloadid = PAYLOAD_INFO_RESPONSE;
	d.status = STATUS_INACTIVE;
	d.state = STATE_OFF;
	d.device = DEVICE_1;
	d.type = TYPE_OUTLET;
	d.data = 0;
	d.change = 1;

	devices_outlet[0] = d;

	d.device = DEVICE_2;

	devices_outlet[1] = d;
}

void heartBeat_outlet(comms* transmitFirst, comms* transmitPush, int* Tab_ADC10) { // TODO Rename
	int state = 0; // TODO WHY NOT GET STATE (SEE OLD CODE)
	if(IsStateChange(state, devices_outlet[0].state))
	{
		devices_outlet[0].payloadid = 0;
		devices_outlet[0].data = ComputationWattHour(Tab_ADC10);
		devices_outlet[0].change = 1;
	}
	else
	{
		devices_outlet[0].payloadid = 0;
		devices_outlet[0].state = (1^state)+1;
		devices_outlet[0].data = ComputationWattHour(Tab_ADC10);
		devices_outlet[0].change = 1;
	}

	Push(&transmitFirst, &transmitPush, devices_outlet[0]);//comms_transmit[0] = devices[0];

	if(IsStateChange(state, devices_outlet[1].state))
	{
		devices_outlet[1].payloadid = 0;
		devices_outlet[1].data = ComputationWattHour(Tab_ADC10);
		devices_outlet[1].change = 1;
	}
	else
	{
		devices_outlet[1].payloadid = 0;
		devices_outlet[1].state = (1^state)+1;
		devices_outlet[1].data = ComputationWattHour(Tab_ADC10);
		devices_outlet[1].change = 1;
	}

	Push(&transmitFirst, &transmitPush, devices_outlet[1]);//comms_transmit[1] = devices[1];
}

void controlCommsReceive_outlet(TYPEDEVICE* device,
								 comms* ReceivePop, 
								 comms* transmitFirst, comms* transmitPush, 
								int* Tab_ADC10) { // TODO Rename

	if (ReceivePop->device == DEVICE_1)
	{
		devices_outlet[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_outlet[0].status = ReceivePop->status;
		devices_outlet[0].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_1, Tab_ADC10);
		devices_outlet[0].device = ReceivePop->device;
		devices_outlet[0].type = TYPE_OUTLET;
		devices_outlet[0].data = ComputationWattHour(Tab_ADC10);
		devices_outlet[0].change = 1;

		Push(&transmitFirst, &transmitPush, devices_outlet[0]);//comms_transmit[0] = devices[0];
	}
	else if (ReceivePop->device == DEVICE_2)
	{
		devices_outlet[1].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_outlet[1].status = ReceivePop->status;
		devices_outlet[1].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_2, Tab_ADC10);
		devices_outlet[1].device = ReceivePop->device;
		devices_outlet[1].type = TYPE_OUTLET;
		devices_outlet[1].data = ComputationWattHour(Tab_ADC10);
		devices_outlet[1].change = 1;

		Push(&transmitFirst, &transmitPush, devices_outlet[1]);//comms_transmit[1] = devices[1];
	}

//				ReceivePop.change = 0;                     ???

}

void changeIO_outlet(int deviceNumber) {
	if (deviceNumber == DEVICE_1)// && ReceivePop->state == STATE_ON)
	{
		CTRL_OUT ^= CTRL_1;
	}
/*
	else if(ReceivePop->device == DEVICE_1 && ReceivePop->state == STATE_OFF)
	{
		CTRL_OUT &= ~CTRL_1;
	}
*/
	if (deviceNumber == DEVICE_2)// && ReceivePop->state == STATE_ON)
	{
		CTRL_OUT ^= CTRL_2;
	}
/*
	else if(ReceivePop->device == DEVICE_2 && ReceivePop->state == STATE_OFF)
	{
		CTRL_OUT &= ~CTRL_2;
	}
*/
}

void infoCommsReceive_outlet(comms* transmitFirst, comms* transmitPush) {
	Push(&transmitFirst, &transmitPush, devices_outlet[0]);//comms_transmit[0] = devices[0];
	Push(&transmitFirst, &transmitPush, devices_outlet[1]);//comms_transmit[1] = devices[1];
}

void initTIMER1_outlet() {
	TA1CCR0 |= 0x3C00;// Value the timer count to //ps Jo veut modifier par une variable
}

void initTIMER2_outlet() {
	//Nothing to do
}

// Interupt

void timer2_Execute_outlet(void) {
	//Nothing to do
}
