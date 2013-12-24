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

/* Private Function */
unsigned long getRefTime(void);
/* End Private Function */

volatile unsigned long  ulHbTimeref,	//use to asset heartbeat response delay
						ulIrTimeref;	//use to asset inforesquest delay time
volatile short  sSocketConnected,
                sNetworkConnectAttempt;

void initNetwork(void)
{
    sSocketConnected = 0;
    sNetworkConnectAttempt = 0;
}

int connectNetwork(void)
{
    int iReturnConnect;
    int iReturnValue = 0; //Default return value if problem connecting to Network or Server
    unsigned long ulReftime;

    updateAsyncEvent();

    if(sNetworkConnectAttempt < 5) //After 5  attempt, stopped trying to connect
    {
        sNetworkConnectAttempt++;
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
                const unsigned long aucDHCP = 14400;
                const unsigned long aucARP = 3600;
                const unsigned long aucKeepalive = 10;
                const unsigned long aucInactivity = 0;

                configDHCP(aucDHCP, aucARP, aucKeepalive, aucInactivity);
                updateIPinfo(); //Get My IP address & MAC address
            }
        }
        if(wifiConnected() == 1)
        {
            if(openSocket())
            {
                iReturnValue = 1;
                sNetworkConnectAttempt = 0;
                getDeviceInfoState()->ledState = LED_STATE_CONNECTED;
                static short sflag = 0;
                static short sinfoflag = 0;
                if(getHeartbeatSentFlag())	//Heartbeat sent - Server must response back
                {
                	if(sflag == 0)
                	{
                		sflag = 1;
                		ulHbTimeref = getRefTime(); //Get time ref heartbeart sent
                	}
                	else
                	{
                		if(getTimeElapsed(getRefTime(), ulHbTimeref) > DELAY2SEC) // Waif 2 sec for heart beat response
                		{
                			callCloseSocket();
                			sSocketConnected = 0; 	//close socket and retry to reconnect
                			clearHeartbeatSentFlag();
                			sflag = 0;
                		}
                	}
                }
                else
                {
                	sflag = 0;

                }

                if(getDeviceInfoState()->infoResquestReceived == 0)
				{
					if(sinfoflag == 0)
					{
						sinfoflag = 1;
						ulIrTimeref = getRefTime(); //Get time ref heartbeart sent
					}
					else
					{
						if(getTimeElapsed(getRefTime(), ulIrTimeref) > DELAY2SEC) // Waif 2 sec for heart beat response
						{
							callCloseSocket();
							sSocketConnected = 0; 	//close socket and retry to reconnect
							getDeviceInfoState()->infoResquestReceived = 0;
							sinfoflag = 0;
						}
					}
				}
                else
                {
                	sinfoflag = 0;
                }
            }
            else
            {
            	getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;
            	getDeviceInfoState()->infoResquestReceived = 0;
            }
        }
    }
    else //Wait Before reconnecting to Wifi Network or Socket server
    {
        //Reset Variable to try to reconnect
        sNetworkConnectAttempt = 0;
        getDeviceInfoState()->ledState = LED_STATE_UNCONNECTED;
        iReturnValue = 0;
    }
    return (iReturnValue);
}

int openSocket(void)
{
    unsigned long ulReftime;
    unsigned long ulTime = 0;
    int iReturnping = 0;
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

        clearPingReceived();
        iReturnping = pingServer(PING_ATTEMPT, PING_SIZE, PING_TIMEOUT);
        ulReftime = getRefTime();
        //Check Server IP address
        while(iReturnping == 0 && pingReceived() == 0 && ulTime < DELAY5SEC) //Stop after 5 sec of finding the Server
        {
        	updateAsyncEvent();
            ulTime = getTimeElapsed(getRefTime(), ulReftime);
        }
        clearPingReceived();
        if(iReturnping != 0 || ulTime >= DELAY5SEC) //Problem finding server IP
        {
            iReturnValue = 0;
        }
        else //Connect to Server
        {
        	 int iReturnConnect = connectServer();
			__delay_cycles(100000); //TODO remove??__delay_cycles(12500000);

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
    }

    return (iReturnValue);
}

unsigned long getRefTime(void)
{
	return getDeviceInfoState()->timeCounter;
}
