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
#include "communication.h"
#include <msp430.h>
#include "netapp.c"
#include "cc3000.h"
#include "wlan.h"

#include "CommsManager.h"
#include "commun.h"

extern unsigned char requestBuffer[];

extern comms* ReceiveFirst;
extern comms* ReceivePush;
extern comms* ReceivePop;
extern comms* TransmitFirst;
extern comms* TransmitPush;
extern comms* TransmitPop;

extern tNetappIpconfigRetArgs CC3000ipconfig;
extern unsigned char DSServerIP[];
extern unsigned char DSServerPort[];


//*****************************************************************************
//
//
//*****************************************************************************
void payloadReceived(unsigned char *usBuffer, signed long iReturnValue)
{
    char szResponse[25];
    comms comms_receive;
    memset(szResponse, 0, sizeof(szResponse));

    switch (usBuffer[0])
    {
        case PAYLOAD_INFO_REQUEST :
            comms_receive.payloadid = PAYLOAD_INFO_REQUEST;
            Push(&ReceiveFirst, &ReceivePush, comms_receive);
            break;
        case PAYLOAD_CONTROL_REQUEST :
            comms_receive.payloadid = PAYLOAD_CONTROL_REQUEST;
            comms_receive.device = usBuffer[1];
            comms_receive.status = usBuffer[2];
            comms_receive.state = usBuffer[3];
            comms_receive.data = usBuffer[4];

            Push(&ReceiveFirst, &ReceivePush, comms_receive);
            break;
        case PAYLOAD_CONFIG_REQUEST:
            comms_receive.payloadid = PAYLOAD_CONFIG_REQUEST;

            break;
        default:
            break;
    }
}

//*****************************************************************************
//
//
//*****************************************************************************
void payloadToSend(comms* PtrPop)
{
    char szResponse[25];
    memset(szResponse, 0, sizeof(szResponse));

    comms comms_transmit;

    comms_transmit = *PtrPop;

    switch (comms_transmit.payloadid)
    {
    	case PAYLOAD_HEARTBEAT_RESPONSE :
    		szResponse[0] = PAYLOAD_HEARTBEAT_RESPONSE;
    		szResponse[1] = CC3000ipconfig.uaMacAddr[2];
    		szResponse[2] = CC3000ipconfig.uaMacAddr[1];
    		szResponse[3] = CC3000ipconfig.uaMacAddr[0];
    		szResponse[4] = comms_transmit.type;
    		szResponse[5] = comms_transmit.device;
    		szResponse[6] = comms_transmit.status;
    		szResponse[7] = comms_transmit.state;
    		szResponse[8] = comms_transmit.data;

    		sendPayLoad(szResponse, 9);
    		break;
        case PAYLOAD_INFO_RESPONSE :
            szResponse[0] = PAYLOAD_INFO_RESPONSE;
            szResponse[1] = CC3000ipconfig.uaMacAddr[2];
            szResponse[2] = CC3000ipconfig.uaMacAddr[1];
            szResponse[3] = CC3000ipconfig.uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;

            sendPayLoad(szResponse, 9);
            break;
        case PAYLOAD_CONTROL_RESPONSE :
            szResponse[0] = PAYLOAD_CONTROL_RESPONSE;
            szResponse[1] = CC3000ipconfig.uaMacAddr[2];
            szResponse[2] = CC3000ipconfig.uaMacAddr[1];
            szResponse[3] = CC3000ipconfig.uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;

            sendPayLoad(szResponse, 9);
            break;
        case PAYLOAD_CONFIG_RESPONSE:

            szResponse[0] = PAYLOAD_CONFIG_RESPONSE;

            szResponse[1] = CC3000ipconfig.uaMacAddr[5];
            szResponse[2] = CC3000ipconfig.uaMacAddr[4];
            szResponse[3] = CC3000ipconfig.uaMacAddr[3];
            szResponse[4] = CC3000ipconfig.uaMacAddr[2];
            szResponse[5] = CC3000ipconfig.uaMacAddr[1];
            szResponse[6] = CC3000ipconfig.uaMacAddr[0];

            szResponse[7] = CC3000ipconfig.aucIP[3];
            szResponse[8] = CC3000ipconfig.aucIP[2];
            szResponse[9] = CC3000ipconfig.aucIP[1];
            szResponse[10] = CC3000ipconfig.aucIP[0];

            szResponse[11] = CC3000ipconfig.aucSubnetMask[3];
            szResponse[12] = CC3000ipconfig.aucSubnetMask[2];
            szResponse[13] = CC3000ipconfig.aucSubnetMask[1];
            szResponse[14] = CC3000ipconfig.aucSubnetMask[0];

            szResponse[15] = CC3000ipconfig.aucDefaultGateway[3];
            szResponse[16] = CC3000ipconfig.aucDefaultGateway[2];
            szResponse[17] = CC3000ipconfig.aucDefaultGateway[1];
            szResponse[18] = CC3000ipconfig.aucDefaultGateway[0];

            szResponse[19] = DSServerIP[0];
            szResponse[20] = DSServerIP[1];
            szResponse[21] = DSServerIP[2];
            szResponse[22] = DSServerIP[3];

            szResponse[23] = DSServerPort[0];
            szResponse[24] = DSServerPort[1];

            sendPayLoad(szResponse, 25);
            break;
        default:
            break;
    }
}


//*****************************************************************************
//
//
//*****************************************************************************
void receivePayLoad(void)
{
	int iReturnValue = receivePackets();

	if (iReturnValue > 0)
	{
		payloadReceived(requestBuffer, iReturnValue);
	}
}


//*****************************************************************************
//
//
//*****************************************************************************
void sendPayLoad(char* pcData, int length)
{
	sendPackets(pcData, length); // data pointer
}

