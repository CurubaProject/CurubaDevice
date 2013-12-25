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
#include "wifiCommun.h"

#include "util.h"
#include "commun.h"
#include "deviceInfoState.h"
#include "heartbeat.h"
#include "timer.h"

#define PING_ATTEMPT                    (5)
#define PING_SIZE                       (40)
#define PING_TIMEOUT                    (20)

#define DELAY5SEC						(40)
#define DELAY2SEC						(20)

#define JO_DEBUG

#if defined(AJC_DEBUG)
	unsigned long SSIDType = SECURITE_WPA2;
	unsigned char SSIDKey[] = "domumservuskey00";
	char SSIDName[] = "DSRouter";
#elif defined(MST_DEBUG)
	unsigned long SSIDType = SECURITE_WPA2;
	unsigned char SSIDKey[] = "domumservus";
	char SSIDName[] = "domumrff";
#elif defined(JO_DEBUG)
	unsigned long SSIDType = SECURITE_WPA2;
	unsigned char SSIDKey[] = "testtest";
	char SSIDName[] = "MAIN";
#endif

unsigned char serverIP[4] = { 192, 168, 0, 104 };
unsigned char serverPort[2] = { 0x13, 0x88 }; // Port 5000 or 0x1388

TIMEOUT infoRequestTimerOut, heartbeatTimerOut, wifiTimerOut, pingTimerOut;

/* Private Function */
void connectToWifi();
void connectSocket();

void addTimerWifiConnect();
void addTimerPing();

void callback_timerHeartbeat();
void callback_timerInfoRequest();
void callback_timerWifiConnect();
void callback_timerPing();

void callback_DHCPConfig();
void callback_wifiDisconnect();
void callback_wifiConnect();
void callback_ping();
/* End Private Function */

void initNetwork(void)
{
	initDriver();

	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;

	connectToWifi();
}

int checkNetwork(void)
{
	timeOut();
    updateAsyncEvent();

    return (wifiConnected() && socketclosed() == 0);
}

void connectToWifi()
{
	connectWifi(SSIDName, SSIDKey, SSIDType,
				&callback_wifiConnect, &callback_wifiDisconnect);
	addTimerWifiConnect();
}

void callback_wifiDisconnect()
{
	TimerStop(TIMER_1);
	stopTimerInfoRequest();
	stopTimerHeartbeat();
	stopTimer(&pingTimerOut);
	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;
	callCloseSocket();
	setHeartbeatFlag(FALSE);

	connectToWifi();
}

void callback_wifiConnect()
{
	stopTimer(&wifiTimerOut);
	getDeviceInfoState()->ledState = LED_STATE_CONNECTED;

	const unsigned long aucDHCP = 14400;
	const unsigned long aucARP = 3600;
	const unsigned long aucKeepalive = 10;
	const unsigned long aucInactivity = 0;

	configDHCP(aucDHCP, aucARP, aucKeepalive, aucInactivity, &callback_DHCPConfig);
}

void callback_DHCPConfig()
{
	updateIPinfo(&(getDeviceInfoState()->config)); //Get My IP address & MAC address
	connectSocket();
}

void connectSocket()
{
	initSocket();

	pingServer(serverIP, PING_ATTEMPT, PING_SIZE, PING_TIMEOUT, &callback_ping);
	addTimerPing();
}

void callback_ping()
{
	stopTimer(&pingTimerOut);

	if ( connectServer(serverPort, serverIP) )
	{
		addTimerInfoRequest();
	}
}

void addTimerHeartbeat()
{
	heartbeatTimerOut.callback = &callback_timerHeartbeat;
	heartbeatTimerOut.refTime = getDeviceInfoState()->timeCounter;
	heartbeatTimerOut.timeOut = DELAY2SEC;

	addTimer(&heartbeatTimerOut);
}
void addTimerInfoRequest()
{
	infoRequestTimerOut.callback = &callback_timerInfoRequest;
	infoRequestTimerOut.refTime = getDeviceInfoState()->timeCounter;
	infoRequestTimerOut.timeOut = DELAY2SEC;

	addTimer(&infoRequestTimerOut);
}
void addTimerWifiConnect()
{
	wifiTimerOut.callback = &callback_timerWifiConnect;
	wifiTimerOut.refTime = getDeviceInfoState()->timeCounter;
	wifiTimerOut.timeOut = DELAY5SEC;

	addTimer(&wifiTimerOut);
}

void addTimerPing()
{
	pingTimerOut.callback = &callback_timerPing;
	pingTimerOut.refTime = getDeviceInfoState()->timeCounter;
	pingTimerOut.timeOut = DELAY5SEC;

	addTimer(&pingTimerOut);
}


void stopTimerHeartbeat()
{
	stopTimer(&heartbeatTimerOut);
}

void stopTimerInfoRequest()
{
	stopTimer(&infoRequestTimerOut);
}

void callback_timerHeartbeat()
{
	TimerStop(TIMER_1);
	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;
	callCloseSocket();
	connectSocket();
}


void callback_timerWifiConnect()
{
	connectToWifi();
}


void callback_timerInfoRequest()
{
	TimerStop(TIMER_1);
	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;
	callCloseSocket();
	connectSocket();
}


void callback_timerPing()
{
	TimerStop(TIMER_1);
	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;

	pingServer(serverIP, PING_ATTEMPT, PING_SIZE, PING_TIMEOUT, &callback_ping);
	addTimerPing();
}
