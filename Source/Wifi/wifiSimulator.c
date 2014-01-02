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
#ifdef __TESTDEBUG__

#include "cc3000.h"

volatile unsigned long  ulCC3000Connected,
                        ulCC3000DHCP,
                        ulCC3000SocketClosed,
                        ulPingReceived;

void initDriver(void)
{
	ulCC3000DHCP = 0;
	ulCC3000Connected = 0;
	ulCC3000SocketClosed = 0;
	ulPingReceived = 0;
}

void initSocket(void){}

void configDHCP(unsigned long aucDHCP, unsigned long aucARP, unsigned long aucKeepalive, unsigned long aucInactivity)
{
}

int receivePackets(unsigned char* requestBuffer)
{
	return 0;
}

void sendPackets(char* pcData, int length){}

int connectServer(unsigned char* serverIP, unsigned char* serverPort)
{
	return 0;
}

int connectWifi(char* SSIDName, unsigned char* SSIDKey, unsigned short SSIDType)
{
	return 0;
}

int pingServer(unsigned char* serverIP, unsigned long ulPingAttempts, unsigned long ulPingSize, unsigned long ulPingTimeout)
{
	return 0;
}


#include <string.h>

void updateIPinfo(LanConfig* lanconfig)
{
    unsigned char aucIP[4] = {10, 10, 10, 10};
	unsigned char aucSubnetMask[4] = {10, 10, 10, 10};
	unsigned char aucDefaultGateway[4] = {10, 10, 10, 10};
	unsigned char aucDHCPServer[4] = {10, 10, 10, 10};
	unsigned char aucDNSServer[4] = {10, 10, 10, 10};
	unsigned char uaMacAddr[6] = {10, 10, 10, 10, 10, 10};
	unsigned char uaSSID[32] = "TEST";

	memcpy (lanconfig->aucSubnetMask, aucSubnetMask, sizeof(aucIP)+1 );
	memcpy (lanconfig->aucDefaultGateway, aucDefaultGateway, sizeof(aucDefaultGateway)+1 );
	memcpy (lanconfig->aucDHCPServer, aucDHCPServer, sizeof(aucDHCPServer)+1 );
	memcpy (lanconfig->aucDNSServer, aucDNSServer, sizeof(aucDNSServer)+1 );
	memcpy (lanconfig->uaMacAddr, uaMacAddr, sizeof(uaMacAddr)+1 );
	memcpy (lanconfig->uaSSID, uaSSID, sizeof(uaSSID)+1 );
}

void updateAsyncEvent(void){}

void callCloseSocket(void)
{
	ulCC3000SocketClosed = 1;
}

unsigned long wifiConnected(void)
{
	return(ulCC3000Connected);
}

unsigned long pingReceived(void)
{
	return (ulPingReceived);
}

unsigned long socketclosed(void)
{
	return(ulCC3000SocketClosed);
}

#endif
