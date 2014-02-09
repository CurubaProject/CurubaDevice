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
#ifndef __TESTDEBUG__

#include "wifi.h"

#include "board.h"

#include "evnt_handler.h"
#include "host_driver_version.h"
#include "netapp.h"

#include "spi.h"
#include "wlan.h"

netapp_pingreport_args_t pingReport;

volatile unsigned long  ulCC3000Connected,
                        ulCC3000DHCP,
                        ulCC3000SocketClosed,
                        ulPingReceived;
volatile long ulSocketTCP;

void initDriver(void)
{
	initWatchDogTimer();

	//  Board Initialization start
	ulCC3000DHCP = 0;
	ulCC3000Connected = 0;
	ulCC3000SocketClosed = 0;
	ulPingReceived = 0;

	// Init GPIO's
	pio_init_wifi();

	//init all layers
	init_spi();

	initCC3000();
}

void initCC3000(void)
{
	// WLAN On API Implementation
	wlan_init(CC3000_UsynchCallback,
			sendWLFWPatch,
			sendDriverPatch,
			sendBootLoaderPatch,
			ReadWlanInterruptPin,
			WlanInterruptEnable,
			WlanInterruptDisable,
			WriteWlanPin);

	// Trigger a WLAN device without Flag
	wlan_start(0);

	wlan_ioctl_set_connection_policy(DISABLE, DISABLE, DISABLE);
	wlan_ioctl_del_profile(255); //clear all profiles

	// Mask out all non-required events from CC3000
	wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE | HCI_EVNT_WLAN_UNSOL_INIT);
}

void initSocket(void)
{
	ulSocketTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	unsigned long socketTimeout = 10;
	int iReturnValue = setsockopt(ulSocketTCP,
									SOL_SOCKET,
									SOCKOPT_RECV_TIMEOUT,
									&socketTimeout,
									sizeof(socketTimeout));
}

void configDHCP(unsigned long aucDHCP, unsigned long aucARP, unsigned long aucKeepalive, unsigned long aucInactivity)
{
	netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity);
}

void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length)
{
	//TODO Why not else if
    if (lEventType == HCI_EVNT_WLAN_UNSOL_CONNECT)
    {
        ulCC3000Connected = 1;
    }
    else if (lEventType == HCI_EVNT_WLAN_UNSOL_DISCONNECT)
    {
        ulCC3000Connected = 0;
        ulCC3000DHCP = 0;
    }
    else if (lEventType == HCI_EVNT_WLAN_UNSOL_DHCP)
    {
        if ( *(data + NETAPP_IPCONFIG_MAC_OFFSET) == 0)
        {
            ulCC3000DHCP = 1;
        }
        else
        {
            ulCC3000DHCP = 0;
        }
    }
    else if (lEventType == HCI_EVNT_WLAN_ASYNC_PING_REPORT)
    {
        memcpy(&pingReport, data, length);

        if(pingReport.packets_received > 1) //All packets are received
        {
        	ulPingReceived = 1;
        }
    }
    else if (lEventType == HCI_EVNT_BSD_TCP_CLOSE_WAIT)
    {
        ulCC3000SocketClosed = 1;
    }
}

int receivePackets(unsigned char* requestBuffer)
{
	return (recv(ulSocketTCP, requestBuffer, sizeof(requestBuffer), 0));
}

void sendPackets(char* pcData, int length)
{
	send(ulSocketTCP, pcData, length, 0);
	__delay_cycles(7501000); //Important to make sure All packet sent
}

int connectServer(unsigned char* serverIP, unsigned char* serverPort)
{
	sockaddr tSocketAddr;
	// the family is always AF_INET
	tSocketAddr.sa_family = AF_INET;

	// the destination port
	tSocketAddr.sa_data[0] = serverPort[0];
	tSocketAddr.sa_data[1] = serverPort[1];

	// the destination IP address
	tSocketAddr.sa_data[2] = serverIP[0];
	tSocketAddr.sa_data[3] = serverIP[1];
	tSocketAddr.sa_data[4] = serverIP[2];
	tSocketAddr.sa_data[5] = serverIP[3];

	return (connect(ulSocketTCP, &tSocketAddr, sizeof(sockaddr)));
}

int connectWifi(char* SSIDName, unsigned char* SSIDKey, unsigned short SSIDType)
{
	return(wlan_connect(SSIDType, SSIDName, strlen(SSIDName), NULL, SSIDKey, strlen((char*) SSIDKey)));
}

int pingServer(unsigned char* serverIP, unsigned long ulPingAttempts, unsigned long ulPingSize, unsigned long ulPingTimeout)
{
	return(netapp_ping_send((unsigned long *) serverIP, ulPingAttempts, ulPingSize, ulPingTimeout));
}

void updateLanConfig(LanConfig* lanconfig)
{
	tNetappIpconfigRetArgs cc3000config;
	netapp_ipconfig(&cc3000config);

	memcpy(lanconfig, &cc3000config, sizeof(LanConfig));
}

void updateAsyncEvent(void)
{
	hci_unsolicited_event_handler();
}

void callCloseSocket(void)
{
	closesocket(ulSocketTCP);
	ulCC3000SocketClosed = 1;
}

unsigned long wifiConnected(void)
{
	return(ulCC3000Connected && ulCC3000DHCP);
}

unsigned long pingReceived(void)
{
	return (ulPingReceived);
}

unsigned long socketclosed(void)
{
	return(ulCC3000SocketClosed);
}

int _system_pre_init(void)
{
	stopWatchDogTimer();
	return 1;
}

char *sendDriverPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}

char *sendWLFWPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}

char *sendBootLoaderPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}

#endif
