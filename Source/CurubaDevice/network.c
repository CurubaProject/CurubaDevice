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
#include "network.h"

#include "cc3000.h"

#include "util.h"
#include "commun.h"
#include "deviceInfoState.h"
#include "heartbeat.h"

#include "eventManager.h"

#include "wlan.h"

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

/* Private Function */
unsigned long getRefTime(void);

void callback_wificonnected();
void callback_wifiConnectedContinue();
/* End Private Function */

volatile unsigned long  ulHbTimeref,	//use to asset heartbeat response delay
						ulIrTimeref;	//use to asset inforesquest delay time
volatile short  sSocketConnected;


/* EVENT FUNCTION */
void do_event_wifi_connected()
{
	const unsigned long aucDHCP = 14400;
	const unsigned long aucARP = 3600;
	const unsigned long aucKeepalive = 10;
	const unsigned long aucInactivity = 0;

	configDHCP(aucDHCP, aucARP, aucKeepalive, aucInactivity);
	updateIPinfo(); //Get My IP address & MAC address

	callback_wifiConnectedContinue();
}
/* END EVENT FUNCTION */

void initNetwork(void)
{
	configCC3000(_SSIDName, _SSIDKey, _SSIDType);

	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;

	initDriver();
    sSocketConnected = 0;
}

int connectNetwork(void)
{
    int iReturnConnect;
    int iReturnValue = 0; //Default return value if problem connecting to Network or Server
    unsigned long ulReftime;

    updateAsyncEvent();

	if (wifiConnected() == 0)
	{
		iReturnConnect = connectWifi();

		ulReftime = getRefTime(); //Wait until connected to Network
		while (iReturnConnect == 0 && wifiConnected() == 0 && getTimeElapsed(getRefTime(), ulReftime) < DELAY5SEC) //Wait 5 sec
		{
			updateAsyncEvent();
		}

		if(wifiConnected() == 1)
		{
			notify(EVENT_WIFI_CONNECTED);
			doEvent();
		}
	}

	if(wifiConnected() == 1)
	{
		iReturnValue = 1;
		callback_wifiConnectedContinue();
	}

    return (iReturnValue);
}

void callback_wificonnected()
{
	const unsigned long aucDHCP = 14400;
	const unsigned long aucARP = 3600;
	const unsigned long aucKeepalive = 10;
	const unsigned long aucInactivity = 0;

	configDHCP(aucDHCP, aucARP, aucKeepalive, aucInactivity);
	updateIPinfo(); //Get My IP address & MAC address

	callback_wifiConnectedContinue();
}

void callback_wifiConnectedContinue()
{
	if(openSocket())
	{
		getDeviceInfoState()->ledState = LED_STATE_CONNECTED;
	}
	else
	{
		getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;
	}
}

int openSocket(void)
{
    int iReturnValue = 1; //Default fucntion return value

    if(socketclosed() == 1)
    {
    	callCloseSocket();
    	clearSocketClosedflag();
    	sSocketConnected  = 0;
    }

    if (sSocketConnected == 0)
    {
    	getDeviceInfoState()->infoResquestReceived = 0;
    	TimerStop(TIMER_1);
    	setHeartbeatFlag(FALSE);
    	initSocket();
        sSocketConnected = 0;

		int iReturnConnect = connectServer();

		if (iReturnConnect == 0)
		{
			iReturnValue = 1;
			sSocketConnected = 1;
		}
		else //Server present but not listening
		{
			iReturnValue = 0;
		}
    }

    return (iReturnValue);
}

unsigned long getRefTime(void)
{
	return getDeviceInfoState()->timeCounter;
}
