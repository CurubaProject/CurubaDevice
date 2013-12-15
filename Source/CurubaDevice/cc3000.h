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

#ifndef CC3000_H
#define CC3000_H

#include "netapp.h"

#define DISABLE                                     (0)
#define ENABLE                                      (1)
#define NETAPP_IPCONFIG_MAC_OFFSET              	(20)

void initCC3000(void);
void initDriver(void);
void initSocket();

void clearpingReceived(void);
void configDHCP(unsigned long aucDHCP, unsigned long aucARP, unsigned long aucKeepalive,unsigned long aucInactivity);
int configcc3000(char *ssidname, char* ssidkey, unsigned short ssidtype, unsigned short lengthssidname, unsigned short lengthssidkey);
int connectWifi(void);
int connectServer(void);
void getConfigInfo (unsigned char* dsServerIP, unsigned char* dsServerPort, tNetappIpconfigRetArgs** cc3000config);
void heartBeatSent(void);
unsigned long pingReceived(void);
int pingServer(unsigned long ulPingAttempts, unsigned long ulPingSize, unsigned long ulPingTimeout);
void sendPackets(char* pcData, int length);
int receivePackets(void);
char *sendDriverPatch(unsigned long *Length);
char *sendWLFWPatch(unsigned long *Length);
char *sendBootLoaderPatch(unsigned long *Length);
unsigned long socketclosed(void);
void updateAsyncEvent(void);
void updateIPinfo(void);
void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length);
unsigned long wifiConnected(void);

#endif /* CC3000_H */
