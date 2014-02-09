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
#include <string.h>

#include "network.h"
#include "communication.h"
#include "wifi.h"
#include "wifiCommun.h"

#include "util.h"
#include "commun.h"
#include "deviceInfoState.h"
#include "eventManager.h"
#include "timer.h"

#include "board.h"

#define JO_DEBUG
#if defined(AJC_DEBUG)
	unsigned long _SSIDType = SECURITY_WPA2;
	unsigned char _SSIDKey[] = "domumservuskey00";
	char _SSIDName[] = "DSRouter";
#elif defined(MST_DEBUG)
	unsigned long _SSIDType = SECURITY_WPA2;
	unsigned char _SSIDKey[] = "domumservus";
	char _SSIDName[] = "domumrff";
#elif defined(JO_DEBUG)
	unsigned long _SSIDType = SECURITY_WPA2;
	unsigned char _SSIDKey[] = "testtest";
	char _SSIDName[] = "MAIN";
#endif

unsigned char _serverIP[4] = { 192, 168, 0, 100 };
unsigned char _serverPort[2] = { 0x13, 0x88 }; // Port 5000 or 0x1388
unsigned char requestBuffer[REQ_BUFFER_SIZE];

unsigned short connecting = 0;

/* Private Function */

void connectToWifi();
void connectToServer();

/* End Private Function */


/* EVENT FUNCTION */
void do_event_wifi_connected()
{
	stopTimer(EVENT_WIFI_CONNECTION_TIMEOUT);

	const unsigned long aucDHCP = 14400;
	const unsigned long aucARP = 3600;
	const unsigned long aucKeepalive = 10;
	const unsigned long aucInactivity = 0;

	configDHCP(aucDHCP, aucARP, aucKeepalive, aucInactivity);
	updateLanConfig(&(getDeviceInfoState()->lanConfig));

	connectToServer();
}

void do_event_wifi_disconnected()
{
	connectToWifi();
}

void do_event_socket_connected()
{
	stopTimer(EVENT_SOCKET_CONNECTION_TIMEOUT);
	getDeviceInfoState()->ledState = LED_STATE_CONNECTED;
}

void do_event_socket_disconnected()
{
	callCloseSocket();
}

void do_event_heartbeat_received()
{
	stopTimer(EVENT_HEARTBEAT_TIMEOUT);
}

void do_event_inforequest_received()
{
	stopTimer(EVENT_INFOREQUEST_TIMEOUT);
}

void do_event_packetsReceived()
{
	payloadReceived(requestBuffer);
}

/* END EVENT FUNCTION */

void initNetwork(void)
{
	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;

	initDriver();
	initTimer();

	// Initialize the RX Buffer
	memset(requestBuffer, 0, sizeof(requestBuffer));
}

void connectToWifi()
{
	connecting = 1;

    int iReturnConnect = connectWifi(_SSIDName, _SSIDKey, _SSIDType);

    if (!iReturnConnect)
    {
    	startTimer(EVENT_WIFI_CONNECTION_TIMEOUT);
    }
    else
    {
    	connectToWifi();
    }
}

void connectToServer()
{
	TimerStop(TIMER_1);
	initSocket();

	if (connectServer(_serverIP, _serverPort) == 0)
	{
		notify(EVENT_SOCKET_CONNECTED);
	}
	else
	{
		//TODO Notify fail connected
	}
}

void checkNetwork()
{
    updateAsyncEvent();

    if ( connecting == 1 )
    {
    	if(wifiConnected() == 1)
    	{
    		notify(EVENT_WIFI_CONNECTED);
        	connecting = 0;
    	}
    }
    else
    {
    	if (wifiConnected() == 0)
    	{
    		connectToWifi();
    	}

        if(socketclosed() == 1)
        {
    		notify(EVENT_SOCKET_DISCONNECTED);
        }

    	if (receivePackets(requestBuffer) > 0)
    	{
    		notify(EVENT_PACKETS_RECEIVED);
    	}
    }
}

#ifndef __TESTDEBUG__
#pragma diag_suppress=552
#endif
void getLanConfig(unsigned char* serverIP, unsigned char* serverPort, LanConfig** config)
{
	serverIP = _serverIP;
	serverPort = _serverPort;
	*config = &(getDeviceInfoState()->lanConfig);
}
#ifndef __TESTDEBUG__
#pragma diag_default=552
#endif

void sendPayLoad(char* pcData, int length)
{
	sendPackets(pcData, length);
}
