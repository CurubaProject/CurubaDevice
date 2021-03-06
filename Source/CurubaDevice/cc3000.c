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

#include "board.h"
#include "evnt_handler.h"
#include "host_driver_version.h"
#include <msp430.h>
#include "netapp.h"
#include "spi.h"
#include "cc3000.h"
#include "wlan.h"

#include "CommsManager.h"
#include "commun.h"

#define REQ_BUFFER_SIZE 100
#ifdef __CCS__
	unsigned char requestBuffer[REQ_BUFFER_SIZE];
#elif __IAR_SYSTEMS_ICC__
	__no_init char requestBuffer[REQ_BUFFER_SIZE];
#endif


#define JO_DEBUG								//ToDo: Remove after test
#ifdef AJC_DEBUG
	unsigned long SSIDType = WLAN_SEC_WPA2;
	unsigned char SSIDKey[] = "domumservuskey00";
	char SSIDName[] = "DSRouter";
#endif

#ifdef MST_DEBUG
	unsigned long SSIDType = WLAN_SEC_WPA2;
	unsigned char SSIDKey[] = "domumservus";
	char SSIDName[] = "domumrff";
#endif

#ifdef JO_DEBUG
	unsigned long SSIDType = WLAN_SEC_WPA2;
	unsigned char SSIDKey[] = "testtest";//"15AB1437CDPA175387";
	char SSIDName[] = "MAIN";
#endif
volatile unsigned long  ulCC3000Connected,
                        ulCC3000DHCP,
                        ulCC3000SocketClosed,
                        ulPingReceived;
volatile long ulSocketTCP;
sockaddr tSocketAddr;
netapp_pingreport_args_t pingReport;
tNetappIpconfigRetArgs CC3000ipconfig;
unsigned char DSServerIP[4] = { 192, 168, 0, 104 };
unsigned char DSServerPort[2] = { 0x13, 0x88 }; // Port 5000 or 0x1388

//*****************************************************************************
//! CC3000_UsynchCallback
//!
//! @param  lEventType   Event type
//! @param  data
//! @param  length
//!
//! @return none
//!
//! @brief  The function handles asynchronous events that come from CC3000
//!           device and operates a LED1 to have an on-board indication
//
//*****************************************************************************

int _system_pre_init(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
	return 1;
}

char *sendDriverPatch(unsigned long *Length) {
	*Length = 0;
	return NULL;
}

char *sendWLFWPatch(unsigned long *Length) {
	*Length = 0;
	return NULL;
}

char *sendBootLoaderPatch(unsigned long *Length) {
	*Length = 0;
	return NULL;
}

void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length)
{
    if (lEventType == HCI_EVNT_WLAN_UNSOL_CONNECT)
    {
        ulCC3000Connected = 1;
    }

    if (lEventType == HCI_EVNT_WLAN_UNSOL_DISCONNECT)
    {
        ulCC3000Connected = 0;
        ulCC3000DHCP = 0;
    }

    if (lEventType == HCI_EVNT_WLAN_UNSOL_DHCP)
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

    if (lEventType == HCI_EVNT_WLAN_ASYNC_PING_REPORT)
    {
        memcpy(&pingReport, data, length);

        if(pingReport.packets_received > 1) //All packets are received
        {
        	ulPingReceived = 1;
        }

    }
    if (lEventType == HCI_EVNT_BSD_TCP_CLOSE_WAIT)
    {
        ulCC3000SocketClosed = 1;
    }

}

//*****************************************************************************
//
//
//*****************************************************************************
int configcc3000(char *ssidname, char* ssidkey, unsigned short ssidtype,
		unsigned short lengthssidname, unsigned short lengthssidkey)
{

	return 0;
}


//*****************************************************************************
//
//
//*****************************************************************************
void heartBeatSent(void) //ToDo: D�commenter dans le code � "dommumapp.c"
{


}


//*****************************************************************************
//
//
//*****************************************************************************
void initDriver(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	//  Board Initialization start
	ulCC3000DHCP = 0;
	ulCC3000Connected = 0;
	ulCC3000SocketClosed = 0;
	ulPingReceived = 0;

	// Initialize the RX Buffer
	memset(requestBuffer, 0, sizeof(requestBuffer));

	// Init GPIO's
	pio_init();

	//init all layers
	init_spi();

	initCC3000();
}


//*****************************************************************************
//
//
//*****************************************************************************
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

	//Wait until CC3000 is disconnected
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


//*****************************************************************************
//
//
//*****************************************************************************
void updateAsyncEvent(void)
{
	hci_unsolicited_event_handler();
	__delay_cycles(100);
}



//*****************************************************************************
//
//
//*****************************************************************************
int pingServer(unsigned long ulPingAttempts, unsigned long ulPingSize, unsigned long ulPingTimeout)
{
	int iReturnping = netapp_ping_send((unsigned long *) DSServerIP, ulPingAttempts, ulPingSize, ulPingTimeout);
	__delay_cycles(100000);
	return(iReturnping);

}


//*****************************************************************************
//
//
//*****************************************************************************
void updateIPinfo(void)
{
	__delay_cycles(3000000); //Wait Important the CC3000 get IP from DHCP //__delay_cycles(50000000);
	netapp_ipconfig(&CC3000ipconfig); //Get My IP address
}


//*****************************************************************************
//
//
//*****************************************************************************
void configDHCP(unsigned long aucDHCP, unsigned long aucARP,
		unsigned long aucKeepalive,unsigned long aucInactivity)
{
	netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity);

}


//*****************************************************************************
//
//
//*****************************************************************************
unsigned long wifiConnected(void)
{
	return(ulCC3000Connected);
}


//*****************************************************************************
//
//
//*****************************************************************************
unsigned long socketclosed(void)
{
	return(ulCC3000SocketClosed);
}


//*****************************************************************************
//
//
//*****************************************************************************
unsigned long pingReceived(void)
{
	return (ulPingReceived);
}


//*****************************************************************************
//
//
//*****************************************************************************
void clearpingReceived(void)
{
	ulPingReceived = 0;
}


//*****************************************************************************
//
//
//*****************************************************************************
int connectServer(void)
{
	return (connect(ulSocketTCP, &tSocketAddr, sizeof(sockaddr)));

}

//*****************************************************************************
//
//
//*****************************************************************************
int connectWifi(void)
{
	return(wlan_connect(SSIDType, SSIDName, strlen(SSIDName),
			NULL, SSIDKey, strlen((char *) SSIDKey)));
}

//*****************************************************************************
//
//
//*****************************************************************************
void initSocket(void)
{
	unsigned long socketTimeout = 10;
	int iReturnValue = -1;

	//closesocket(ulSocketTCP);
	//ulCC3000SocketClosed = 0;
	ulPingReceived = 0;
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

	// the family is always AF_INET
	tSocketAddr.sa_family = AF_INET;

	// the destination port
	tSocketAddr.sa_data[0] = DSServerPort[0];
	tSocketAddr.sa_data[1] = DSServerPort[1];

	// the destination IP address
	tSocketAddr.sa_data[2] = DSServerIP[0];
	tSocketAddr.sa_data[3] = DSServerIP[1];
	tSocketAddr.sa_data[4] = DSServerIP[2];
	tSocketAddr.sa_data[5] = DSServerIP[3];
}



//*****************************************************************************
//
//
//*****************************************************************************
void clearSocketClosedflag(void)
{
	ulCC3000SocketClosed = 0;
}


//*****************************************************************************
//
//
//*****************************************************************************
void callCloseSocket(void)
{
	closesocket(ulSocketTCP);
}


//*****************************************************************************
//
//
//*****************************************************************************
int receivePackets(void)
{
	return (recv(ulSocketTCP, requestBuffer, sizeof(requestBuffer), 0));
}

//*****************************************************************************
//
//
//*****************************************************************************
void sendPackets(char* pcData, int length)
{
	send(ulSocketTCP, pcData, length, 0);
	__delay_cycles(1000);//__delay_cycles(100000);
}


//*****************************************************************************
//
//
//*****************************************************************************
void getConfigInfo (unsigned char* dsServerIP, unsigned char* dsServerPort, tNetappIpconfigRetArgs** cc3000config)
{
	dsServerIP = DSServerIP;
	dsServerPort = DSServerPort;
	*cc3000config = &CC3000ipconfig;
}
