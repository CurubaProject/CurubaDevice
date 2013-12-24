// ------------------------------------------------------------------------------------------------
// ----------------- Curuba Device ----------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Copyright (C) 2013 Mathieu B�langer (mathieu.b.belanger@usherbrooke.ca)
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

#include "wlan.h"

#ifndef NULL
#define NULL (void*)0
#endif

#define JO_DEBUG

#if defined(AJC_DEBUG)
	unsigned long _SSIDType = WLAN_SEC_WPA2;
	unsigned char _SSIDKey[] = "domumservuskey00";
	char _SSIDName[] = "DSRouter";
#elif defined(MST_DEBUG)
	unsigned long _SSIDType = WLAN_SEC_WPA2;
	unsigned char _SSIDKey[] = "domumservus";
	char _SSIDName[] = "domumrff";
#elif defined(JO_DEBUG)
	unsigned long _SSIDType = WLAN_SEC_WPA2;
	unsigned char _SSIDKey[] = "testtest";
	char _SSIDName[] = "MAIN";
#endif

#include "deviceControl.h"
#include "commsManager.h"
#include "commsReceive.h"
#include "typeDevice.h"

#include "heartbeat.h"

#include "communication.h"
#include "network.h"
#include "cc3000.h"

void main(void) {
	configCC3000(_SSIDName, _SSIDKey, _SSIDType);

	comms* receivePop;
	comms* transmitPop;

	initDriver();
	initNetwork();

	//Type of module
	TYPEDEVICE* device = createTypeDevice(readAppSwitch());

	initApp(&device);

	while (1)
	{
		if( connectNetwork() )
		{
			// Receive payload from buffer
			receivePayLoad();

			// Do reception of payload
			if ( popReceive(&receivePop) )
			{
				assert(receivePop != NULL);
				( receiveComms(receivePop->payloadid) )(device, receivePop);
			}

			// Transmit payload to send
			if ( popTransmit(&transmitPop) )
			{
				assert(transmitPop != NULL);
				payloadToSend(transmitPop);
			}

			// Do heartbeat
			if( getHeartbeatFlag() )
			{
				setHeartbeatFlag(0);
				device->heartBeat();
			}
		}
	}
}
