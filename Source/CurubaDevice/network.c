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
#include "board.h"
#include <msp430.h>

#include "cc3000.h"
#include "communication.h"

#include "util.h"
#include "commun.h"

#include "heartbeat.h"


volatile unsigned long ulTimecount;
volatile unsigned long ulTimeRef,		//use for Timer LED
						ulHbTimeref,	//use to asset heartbeat response delay
						ulIrTimeref;	//use to asset inforesquest delay time
volatile short  sSocketConnected,
                sNetworkConnectAttempt;
volatile short sLedState;


void initNetwork(void)
{
	initTIMERB0();

    sSocketConnected = 0;
    sNetworkConnectAttempt = 0;
    ulTimecount = 0;
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
            while (iReturnConnect == 0 && wifiConnected() == 0 && getTimeElapsed(ulReftime) < DELAY5SEC) //Wait 5 sec
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
                sLedState = LED_STATE_CONNECTED;
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
                		if(getTimeElapsed(ulHbTimeref) > DELAY2SEC) // Waif 2 sec for heart beat response
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

                if(getInfoResquestflag() == 0)
				{
					if(sinfoflag == 0)
					{
						sinfoflag = 1;
						ulIrTimeref = getRefTime(); //Get time ref heartbeart sent
					}
					else
					{
						if(getTimeElapsed(ulIrTimeref) > DELAY2SEC) // Waif 2 sec for heart beat response
						{
							callCloseSocket();
							sSocketConnected = 0; 	//close socket and retry to reconnect
							clearInfoResquestflag();
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
                sLedState = LED_STATE_UNCONNECTED;
                clearHeartbeatSentFlag();
            }
        }
    }
    else //Wait Before reconnecting to Wifi Network or Socket server
    {
        //Reset Variable to try to reconnect
        sNetworkConnectAttempt = 0;
        sLedState = LED_STATE_UNCONNECTED;
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
    	clearInfoResquestflag();
    	TimerStop(TIMER_1);
    	setHeartbeatFlag(FALSE);
    	initSocket();
        sSocketConnected = 0;

        clearpingReceived();
        iReturnping = pingServer(PING_ATTEMPT, PING_SIZE, PING_TIMEOUT);
        ulReftime = getRefTime();
        //Check Server IP address
        while(iReturnping == 0 && pingReceived() == 0 && ulTime < DELAY5SEC) //Stop after 5 sec of finding the Server
        {
        	updateAsyncEvent();
            ulTime = getTimeElapsed(ulReftime);
        }
        clearpingReceived();
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

void initTIMERB0(void)
{
	//Timer for Server connection delay time
	TB0CTL |= TBCLR;
	TB0CTL |= TBSSEL_1 + ID_3 + TBIE;
	TB0EX0 |= TBIDEX_7; //Timer clock = ACLK/64 = 512 Hz
	TB0CCR0 = (int) 32;
	StartTIMERB0(); //Never stop this timer
}

void StartTIMERB0(void)
{
	TB0CTL |= MC_1; //Start timer in Up-mode
}

void StopTIMERB0(void)
{
	TB0CTL &= 0xFFCF; //Halt timer
	TB0R = 0;
}

unsigned long getRefTime(void)
{
	return(ulTimecount);
}

unsigned long getTimeElapsed(unsigned long lastcount)
{
	if(getRefTime() >= lastcount)
	{
		return(getRefTime() - lastcount);
	}
	else
	{
		return(((unsigned long) (0xFFFFFFFF) - lastcount) + getRefTime());
	}
}

//Timer used to interrupt at every 500 msec
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
    static short sFlag = 0;
    unsigned long ultime;
    switch (__even_in_range(TB0IV, 14))
    {
        case 0:                            // No interrupt
            break;
        case TB0IV_TB0CCR1:                            // Capture/Compare 1
            break;
        case TB0IV_TB0CCR2:                            // Capture/Compare 2
            break;
        case TB0IV_3:                            // Capture/Compare 3
            break;
        case TB0IV_4:                            // Capture/Compare 4
            break;
        case TB0IV_5:                           // Capture/Compare 5
            break;
        case TB0IV_6:                           // Capture/Compare 6
            break;
        case TB0IV_TB0IFG:                           // Timer overflow
            ulTimecount++;
            if(sFlag == 0)
            {
                ulTimeRef = getRefTime();
                sFlag = 1;
                ultime = getTimeElapsed(ulTimeRef);
            }
            else if(sFlag == 1)
            {
                ultime = getTimeElapsed(ulTimeRef);
            }
            switch (sLedState)
            {
                case LED_STATE_OFF:
                    turnLedOff(WARNING_LED);
                    break;
                case LED_STATE_UNCONNECTED:
                    turnLedOn(WARNING_LED);
                    break;
                case LED_STATE_CONNECTED:
                    if(ultime >= 16) // Toggle LED at each 1 Hz
                    {
                        toggleLed(WARNING_LED);
                        sFlag = 0;
                    }
                    break;
                case LED_STATE_CONFIGURING:
                    if(ultime >= 1) //Toggle LED at each 8 Hz
                    {
                        toggleLed(WARNING_LED);
                        sFlag = 0;
                    }
                    break;
                default:
                    sFlag = 0;
                    break;
            }
            TB0CTL &= ~TBIFG;
            break;
        default:
            break;
    }
}
