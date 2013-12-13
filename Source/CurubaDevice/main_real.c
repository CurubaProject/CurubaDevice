#include "evnt_handler.h"  // TODO REMOVE THIS
#include "board.h"  // TODO REMOVE THIS
#include <msp430.h>  // TODO REMOVE THIS

#include <assert.h>

#include "domumapp.h"
#include "CommsManager.h"
#include "wifi.h"
#include "comms.h"
#include "network.h"
#include "typeDevice.h"

#include "commun.h"  // TODO REMOVE THIS

extern comms* ReceiveFirst;
extern comms* ReceivePush;
extern comms* ReceivePop;
extern comms* TransmitFirst;
extern comms* TransmitPush;
extern comms* TransmitPop;

void main(void) {
	//Application type of the module
	int typeModule = ReadAppSwitch();
	TYPEDEVICE* device = createTypeDevice(typeModule);

	//initCommunication();
	initDriver();
	initApp(device);

	InitListComms(device); // TODO PK BEFORE AND AFTER

	InitADC10();
	InitTIMER0();
	InitTIMER1(device);
	InitTIMER2(device);
	initTIMERB0();

	__bis_SR_register(GIE); //TODO Remove from main

	CTRL_OUT |= CTRL_1; //TODO Remove from main

	while (1) {
		if(connectNetwork())
		{
			receivePayLoad();

			if (Pop(&ReceiveFirst, &ReceivePush, &ReceivePop))
			{
				assert(ReceivePop != NULL);
				switch (ReceivePop->payloadid)
				{
					case PAYLOAD_INFO_REQUEST :
						InfoCommsReceive(device, TransmitFirst, TransmitPush); // TODO ?? Must be change to comms_receive.type
						break;
					case PAYLOAD_CONTROL_REQUEST :
						ControlCommsReceive(device, ReceivePop, TransmitFirst, TransmitPush); // TODO ??Must be change to comms_receive.type
						break;
					case PAYLOAD_CONFIG_REQUEST :
						//Not use for now
						break;
					default :
						assert(0);
						break;
				}
			}

			if (Pop(&TransmitFirst, &TransmitPush, &TransmitPop))
			{
				assert(TransmitPop != NULL);
				payloadToSend(TransmitPop);
			}
		}
	}
}


