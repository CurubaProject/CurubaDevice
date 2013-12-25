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

#ifndef CC3000_H
#define CC3000_H

#define DISABLE                                     (0)
#define ENABLE                                      (1)
#define NETAPP_IPCONFIG_MAC_OFFSET              	(20)

typedef void (*CallbackWifiConnected)();
typedef void (*CallbackWifiDisconnected)();
typedef void (*CallbackPingResult)();
typedef void (*CallbackDHCPConfig)();

typedef struct lanConfig
{
    unsigned char aucIP[4];
	unsigned char aucSubnetMask[4];
	unsigned char aucDefaultGateway[4];
	unsigned char aucDHCPServer[4];
	unsigned char aucDNSServer[4];
	unsigned char uaMacAddr[6];
	unsigned char uaSSID[32];
} LanConfig;

void initCC3000(void);
void initDriver(void);
void initSocket();

void callCloseSocket(void);
void clearPingReceived(void);
void clearSocketClosedflag(void);
unsigned long wifiConnected(void);

void configDHCP(unsigned long aucDHCP, unsigned long aucARP,
				unsigned long aucKeepalive, unsigned long aucInactivity,
				CallbackDHCPConfig callbackConfig);
void updateIPinfo(LanConfig* lanconfig);

int connectWifi(char *SSIDName, unsigned char* SSIDKey, unsigned short SSIDType,
				CallbackWifiConnected callbackConnect,
				CallbackWifiDisconnected callbackDisConnect);
int connectServer(unsigned char* serverPort, unsigned char* serverIP);

unsigned long pingReceived(void);
int pingServer(unsigned char* serverIP,
				unsigned long ulPingAttempts, unsigned long ulPingSize, unsigned long ulPingTimeout,
				CallbackPingResult callbackPing);

void sendPackets(char* pcData, int length);
int receivePackets(void);

unsigned long socketclosed(void);

void updateAsyncEvent(void);

void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length);

#endif /* CC3000_H */
