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
	else if (*PtrPop == ((*PtrFirst)+5))
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
	if (*PtrPush == ((*PtrFirst)+5))
	{
		*PtrPush = *PtrFirst;
	}
	else
	{
		(*PtrPush)++;
	}

	**PtrPush = NewStruct;
}

comms* getTransmitFirst() {
	return TransmitFirst;
}

comms* getTransmitPush() {
	return TransmitPush;
}
