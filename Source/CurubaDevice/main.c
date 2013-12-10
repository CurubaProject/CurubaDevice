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
#include <assert.h>

#include "deviceControl.h"
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
	initCommunication();
	initDriver();

	//Application type of the module
	int typeModule = ReadAppSwitch();
	TYPEDEVICE* device = createTypeDevice(typeModule);

	InitListComms(device);

	InitADC10();
	InitTIMER0();
	InitTIMER1(device);
	InitTIMER2(device);
	initTIMERB0();

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
						InfoCommsReceive(device, &TransmitFirst, &TransmitPush);
						break;
					case PAYLOAD_CONTROL_REQUEST :
						ControlCommsReceive(device, ReceivePop, &TransmitFirst, &TransmitPush);
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
