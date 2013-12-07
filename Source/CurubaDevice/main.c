/*****************************************************************************
 *
 * {DomumWifi.c}
 *
 *
 *****************************************************************************/
#include "evnt_handler.h"
#include "nvmem.h"
#include "socket.h"
#include "netapp.h"
#include "spi.h"
#include "board.h"
#include "host_driver_version.h"
#include <msp430.h>
#include "security.h"
#include <string.h>
#include <msp430.h>

#include <assert.h>

#include "domumapp.h"
#include "CommsManager.h"
#include "wifi.h"

extern comms* ReceiveFirst;
extern comms* ReceivePush;
extern comms* ReceivePop;
extern comms* TransmitFirst;
extern comms* TransmitPush;
extern comms* TransmitPop;

//*****************************************************************************
//
//! main
//!
//!  \param  None
//!
//!  \return none
//!
//!  \brief  The main loop is executed here
//
//*****************************************************************************
void main(void) {
	int TypeModule = 0;

	initCommunication();
	initDriver();

	//Application type of the module
	TypeModule = ReadAppSwitch();

	InitListComms(TypeModule);

	InitADC10();
	InitTIMER0();
	InitTIMER1(TypeModule);
	InitTIMER2(TypeModule);
	initTIMERB0();

	__bis_SR_register(GIE);

//	TimerStart(TIMER_1);

	InitListComms(TypeModule);

	CTRL_OUT |= CTRL_1;

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
						InfoCommsReceive(TypeModule); //Must be change to comms_receive.type
						break;
					case PAYLOAD_CONTROL_REQUEST :
						ControlCommsReceive(TypeModule); //Must be change to comms_receive.type
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
