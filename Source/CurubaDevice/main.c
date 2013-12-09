#include "board.h"  // TODO REMOVE THIS

#include <assert.h>

#include "domumapp.h"
#include "CommsManager.h"
#include "wifi.h"
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

	InitListComms(device);

	InitADC10();
	InitTIMER0();
	InitTIMER1(device);
	InitTIMER2(device);
	initTIMERB0();

	initCommunication();
	initDriver();
	initApp(device);

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
