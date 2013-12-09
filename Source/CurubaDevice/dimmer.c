#include "device.h"
#include "CommsManager.h"
#include "util.h"
#include "typeDevice.h"
#include "commun.h"

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

comms devices_dimmer[2]; // TODO IMPROVE list devices
int SwitchDimmer = 0;

void initDevice_dimmer(int* Tab_ADC10){

	CTRL_OUT |= CTRL_1;

	if(GetState(DEVICE_1, Tab_ADC10))
	{
		CTRL_OUT ^= CTRL_1 + CTRL_2;
	}
}

void initListComms_dimmer() { // TODO REMOVE THIS FUNCTION, NOT GOOD
	comms d;

	d.payloadid = PAYLOAD_INFO_RESPONSE;
	d.status = STATUS_INACTIVE;
	d.state = STATE_OFF;
	d.device = DEVICE_1;
	d.type = TYPE_DIMMER;
	d.data = 0;

	devices_dimmer[0] = d;
}

void heartBeat_dimmer(comms* transmitFirst, comms* transmitPush, int* Tab_ADC10) {
	int state = GetState(devices_dimmer[0].device, Tab_ADC10);

	if(IsStateChange(state, devices_dimmer[0].state))
	{
		devices_dimmer[0].payloadid = 0;
		devices_dimmer[0].data = ComputationWattHour(Tab_ADC10);
	}
	else
	{
		devices_dimmer[0].payloadid = 0;
		devices_dimmer[0].state = (1^state)+1;                             //TODO remove ducktape
		devices_dimmer[0].data = ComputationWattHour(Tab_ADC10);
	}

	Push(&transmitFirst, &transmitPush, devices_dimmer[0]);//comms_transmit[0] = devices[0];
}

void controlCommsReceive_dimmer(TYPEDEVICE* device,
								comms* ReceivePop, 
								comms* transmitFirst,comms* transmitPush,
								int* Tab_ADC10) {
	if (ReceivePop->status == STATUS_ACTIVE)
	{
		//Enable Zero cross for dimmer function
		ZERO_CROSS_IE |= ZERO_CROSS;

		//Timer2 count to 0
		TA2CCR0 = 0;

		//Status of the module
		devices_dimmer[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_dimmer[0].status = STATUS_ACTIVE;
		devices_dimmer[0].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_1, Tab_ADC10);
		devices_dimmer[0].device = ReceivePop->device;
		devices_dimmer[0].type = TYPE_DIMMER;
		devices_dimmer[0].data = ComputationWattHour(Tab_ADC10);

		if (ReceivePop->state == STATE_ON)
		{
			TA2CCR0 = (int) (101 - ReceivePop->data) / 100.0 * PERIODHZ;
		}
		Push(&transmitFirst, &transmitPush, devices_dimmer[0]);
	}
	else if (ReceivePop->status == STATUS_INACTIVE)
	{
		//Disable Zero cross for dimmer function
		P1IE &= ~BIT7;
		P1IFG &= 0x0000;

		//Timer2 count to 0
		TA2CCR0 = 0;

		//Status of the module
		devices_dimmer[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
		devices_dimmer[0].status = STATUS_INACTIVE;
		devices_dimmer[0].state = ChangeIO_Device(device, ReceivePop->state, DEVICE_1, Tab_ADC10);
		devices_dimmer[0].device = ReceivePop->device;
		devices_dimmer[0].type = TYPE_DIMMER;
		devices_dimmer[0].data = ComputationWattHour(Tab_ADC10);

		Push(&transmitFirst, &transmitPush, devices_dimmer[0]);//comms_transmit[0] = devices[0];
	}
}

void infoCommsReceive_dimmer(comms* transmitFirst,comms* transmitPush) {
	devices_dimmer[0].payloadid = PAYLOAD_INFO_RESPONSE;
	devices_dimmer[0].status = STATUS_INACTIVE;
	devices_dimmer[0].state = STATE_OFF;
	devices_dimmer[0].device = DEVICE_1;
	devices_dimmer[0].type = TYPE_DIMMER;

	Push(&transmitFirst, &transmitPush, devices_dimmer[0]);
}

void changeIO_dimmer(int deviceNumber) {
	CTRL_OUT ^= CTRL_1 + CTRL_2;
	SwitchDimmer = 0x0060 & CTRL_OUT;
}

void initTIMER1_dimmer() {
	TA1CCR0 |= HEARTBEAT_TIME;
}

void initTIMER2_dimmer() {
	TA2CTL |= TASSEL_2 + ID_3 + TAIE;
	TA2CCR0 = 0;
	TA2EX0 |= TAIDEX_7;
}

// Interupt

void timer2_Execute_dimmer(void) {
	CTRL_OUT |= SwitchDimmer;
	TimerStop(TIMER_2);
	// Reset the count
	TA2R &= 0x0000;
	TA2CTL &= ~TAIFG;
}
