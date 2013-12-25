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
#include "cc3000.h"

#include "board.h"
#include <msp430.h>

#include "evnt_handler.h"
#include "host_driver_version.h"
#include "wlan.h"
#include "netapp.h"

#include "spi.h"

/* Private Function */
char *sendDriverPatch(unsigned long *Length);
char *sendWLFWPatch(unsigned long *Length);
char *sendBootLoaderPatch(unsigned long *Length);
/* End Private Function */

#define MAX_KEY_SIZE 32
#define MAX_SSID_NAME_SIZE 16

#define REQ_BUFFER_SIZE 100
#ifdef __CCS__
	unsigned char requestBuffer[REQ_BUFFER_SIZE];
#elif __IAR_SYSTEMS_ICC__
	__no_init char requestBuffer[REQ_BUFFER_SIZE];
#endif

volatile long ulSocketTCP;
netapp_pingreport_args_t pingReport;

volatile unsigned long  ulCC3000Connected,
                        ulCC3000DHCP,
                        ulCC3000SocketClosed,
                        ulPingReceived = 0;

volatile CallbackWifiConnected callbackWifiConnected = NULL;
volatile CallbackWifiDisconnected callbackWifiDisconnected = NULL;
volatile CallbackPingResult callbackPingResult = NULL;
volatile CallbackDHCPConfig callbackDHCPConfig = NULL;

void initDriver(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	//  Board Initialization start
	ulCC3000DHCP = 0;
	ulCC3000Connected = 0;
	ulCC3000SocketClosed = 0;

	// Initialize the RX Buffer
	memset(requestBuffer, 0, sizeof(requestBuffer));

	//init all layers
	init_spi();

	initCC3000();
}

void initCC3000(void)
{
	const unsigned char pucIP_Addr[4] = { 0, 0, 0, 0 };
	const unsigned char pucIP_DefaultGWAddr[4] = { 0, 0, 0, 0 };
	const unsigned char pucSubnetMask[4] = { 0, 0, 0, 0 };
	const unsigned char pucDNS[4] = { 0, 0, 0, 0 };

	// WLAN On API Implementation
	wlan_init(CC3000_UsynchCallback,
			sendWLFWPatch,
			sendDriverPatch,
			sendBootLoaderPatch,
			ReadWlanInterruptPin,
			WlanInterruptEnable,
			WlanInterruptDisable,
			WriteWlanPin);

	__delay_cycles(10000);

	// Trigger a WLAN device without Flag
	wlan_start(0);

	// DHCP is used by default
	//Force DHCP
	netapp_dhcp((unsigned long *) pucIP_Addr,
			(unsigned long *) pucSubnetMask,
			(unsigned long *) pucIP_DefaultGWAddr,
			(unsigned long *) pucDNS);

	wlan_ioctl_set_connection_policy(DISABLE, DISABLE, DISABLE);
	wlan_ioctl_del_profile(255); //clear all profiles

	//Wait until CC3000 is connected
	while (ulCC3000Connected == 1)
	{
		__delay_cycles(100);
		hci_unsolicited_event_handler();
	}

	ulCC3000Connected = 0;
	ulCC3000DHCP = 0;

	//init device
	wlan_stop();
	__delay_cycles(100000);
	wlan_start(0);

	// Mask out all non-required events from CC3000
	wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE | HCI_EVNT_WLAN_UNSOL_INIT);
}

void initSocket(void)
{
	int iReturnValue = -1;
	unsigned long socketTimeout = 10;

	ulSocketTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	while (iReturnValue != 0)
	{
		iReturnValue = setsockopt(ulSocketTCP,
									SOL_SOCKET,
									SOCKOPT_RECV_TIMEOUT,
									&socketTimeout,
									sizeof(socketTimeout));
		__delay_cycles(10000);
	}
}


void clearSocketClosedflag(void)
{
	ulCC3000SocketClosed = 0;
}

void callCloseSocket(void)
{
	closesocket(ulSocketTCP);
}

unsigned long socketclosed(void)
{
	return(ulCC3000SocketClosed);
}

unsigned long wifiConnected(void)
{
	return(ulCC3000Connected);
}

unsigned long pingReceived(void)
{
	return (ulPingReceived);
}

void clearPingReceived(void)
{
	ulPingReceived = 0;
}

void configDHCP(unsigned long aucDHCP, unsigned long aucARP,
				unsigned long aucKeepalive, unsigned long aucInactivity,
				CallbackDHCPConfig callbackConfig)
{
	callbackDHCPConfig = callbackConfig;
	netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity);
}

void updateAsyncEvent(void)
{
	hci_unsolicited_event_handler();
	__delay_cycles(100);
}

int pingServer(unsigned char* serverIP,
				unsigned long ulPingAttempts, unsigned long ulPingSize, unsigned long ulPingTimeout,
				CallbackPingResult callbackPing)
{
	callbackPingResult = callbackPing;
	int iReturnping = netapp_ping_send((unsigned long *) serverIP, ulPingAttempts, ulPingSize, ulPingTimeout);
	__delay_cycles(100000);
	return(iReturnping);
}

int connectServer(unsigned char* serverPort, unsigned char* serverIP)
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

int connectWifi(char *SSIDName, unsigned char* SSIDKey, unsigned short SSIDType,
				CallbackWifiConnected callbackConnect,
				CallbackWifiDisconnected callbackDisconnect)
{
	callbackWifiConnected = callbackConnect;
	callbackWifiDisconnected = callbackDisconnect;

	return wlan_connect(SSIDType, SSIDName, strlen(SSIDName), NULL, SSIDKey, strlen((char *) SSIDKey));
}

int receivePackets(void)
{
	return (recv(ulSocketTCP, requestBuffer, sizeof(requestBuffer), 0));
}

void sendPackets(char* pcData, int length)
{
	send(ulSocketTCP, pcData, length, 0);
	__delay_cycles(1000);
}

void updateIPinfo(LanConfig* lanconfig)
{
	tNetappIpconfigRetArgs cc3000config;
	__delay_cycles(3000000); //Wait Important the CC3000 get IP from DHCP //__delay_cycles(50000000);
	netapp_ipconfig(&cc3000config); //Get My IP address

	memcpy(lanconfig, &cc3000config, sizeof(LanConfig));
}

void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length)
{
	switch (lEventType)
	{
		case HCI_EVNT_WLAN_UNSOL_CONNECT:
			ulCC3000Connected = 1;
			if (callbackWifiConnected != NULL)
			{
				callbackWifiConnected();
			}
			break;
		case HCI_EVNT_WLAN_UNSOL_DISCONNECT:
			ulCC3000Connected = 0;
			ulCC3000DHCP = 0;

			if (callbackWifiDisconnected != NULL)
			{
				callbackWifiDisconnected();
			}
			break;
		case HCI_EVNT_WLAN_UNSOL_DHCP:
			if ( *(data + NETAPP_IPCONFIG_MAC_OFFSET) == 0)
			{
				ulCC3000DHCP = 1;

				if (callbackDHCPConfig != NULL )
				{
					callbackDHCPConfig();
				}
			}
			else
			{
				ulCC3000DHCP = 0;
			}
			break;
		case HCI_EVNT_WLAN_ASYNC_PING_REPORT:
			memcpy(&pingReport, data, length);

			if (callbackPingResult != NULL)
			{
				callbackPingResult();
			}

			if(pingReport.packets_received > 1) //All packets are received
			{
				ulPingReceived = 1;
			}
			break;
		case HCI_EVNT_BSD_TCP_CLOSE_WAIT:
			ulCC3000SocketClosed = 1;
			break;
		default:
			break;
	}
}


int _system_pre_init(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
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
