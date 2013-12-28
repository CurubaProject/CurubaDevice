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
#include "communication.h"
#include "cc3000.h"

#include "commsManager.h"
#include "commun.h"

#include "deviceInfoState.h"
#include "heartbeat.h"

#include "string.h"

#define PAYLOAD_SIZE (25)
#define SIZE (sizeof(char) * 25)

extern unsigned char requestBuffer[];

void payloadReceived(unsigned char *usBuffer)
{
    char szResponse[PAYLOAD_SIZE];
    memset(szResponse, 0, sizeof(szResponse));

    comms comms_receive;

    switch (usBuffer[0])
    {
        case PAYLOAD_INFO_REQUEST :
        	getDeviceInfoState()->infoResquestReceived = 1;
            comms_receive.payloadid = PAYLOAD_INFO_REQUEST;
            pushReceive(&comms_receive);
            break;
        case PAYLOAD_CONTROL_REQUEST :
            comms_receive.payloadid = PAYLOAD_CONTROL_REQUEST;
            comms_receive.device = usBuffer[1];
            comms_receive.status = usBuffer[2];
            comms_receive.state = usBuffer[3];
            comms_receive.data = usBuffer[4];

            pushReceive(&comms_receive);
            break;
        case PAYLOAD_CONFIG_REQUEST:
            comms_receive.payloadid = PAYLOAD_CONFIG_REQUEST;
            break;
        case PAYLOAD_HEARTBEAT_REQUEST:
        	setHeartbeatSentFlag(0);
            break;
        default:
            break;
    }
}

void payloadToSend(comms* PtrPop)
{
	tNetappIpconfigRetArgs* CC3000config;
	unsigned char* serverIP;
	unsigned char* serverPort;
	getConfigInfo(serverIP, serverPort, &CC3000config);

    char szResponse[PAYLOAD_SIZE];
    memset(szResponse, 0, sizeof(szResponse));

    comms comms_transmit = *PtrPop; // TODO USEFUL?

    switch (comms_transmit.payloadid)
    {
        case PAYLOAD_INFO_RESPONSE :
            szResponse[0] = PAYLOAD_INFO_RESPONSE;
            szResponse[1] = CC3000config->uaMacAddr[2];
            szResponse[2] = CC3000config->uaMacAddr[1];
            szResponse[3] = CC3000config->uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;

            sendPayLoad(szResponse, 9);
            break;
        case PAYLOAD_CONTROL_RESPONSE :
            szResponse[0] = PAYLOAD_CONTROL_RESPONSE;
            szResponse[1] = CC3000config->uaMacAddr[2];
            szResponse[2] = CC3000config->uaMacAddr[1];
            szResponse[3] = CC3000config->uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;

            sendPayLoad(szResponse, 9);
            break;
        case PAYLOAD_CONFIG_RESPONSE:

            szResponse[0] = PAYLOAD_CONFIG_RESPONSE;

            szResponse[1] = CC3000config->uaMacAddr[5];
            szResponse[2] = CC3000config->uaMacAddr[4];
            szResponse[3] = CC3000config->uaMacAddr[3];
            szResponse[4] = CC3000config->uaMacAddr[2];
            szResponse[5] = CC3000config->uaMacAddr[1];
            szResponse[6] = CC3000config->uaMacAddr[0];

            szResponse[7] = CC3000config->aucIP[3];
            szResponse[8] = CC3000config->aucIP[2];
            szResponse[9] = CC3000config->aucIP[1];
            szResponse[10] = CC3000config->aucIP[0];

            szResponse[11] = CC3000config->aucSubnetMask[3];
            szResponse[12] = CC3000config->aucSubnetMask[2];
            szResponse[13] = CC3000config->aucSubnetMask[1];
            szResponse[14] = CC3000config->aucSubnetMask[0];

            szResponse[15] = CC3000config->aucDefaultGateway[3];
            szResponse[16] = CC3000config->aucDefaultGateway[2];
            szResponse[17] = CC3000config->aucDefaultGateway[1];
            szResponse[18] = CC3000config->aucDefaultGateway[0];

            szResponse[19] = serverIP[0];
            szResponse[20] = serverIP[1];
            szResponse[21] = serverIP[2];
            szResponse[22] = serverIP[3];

            szResponse[23] = serverPort[0];
            szResponse[24] = serverPort[1];

            sendPayLoad(szResponse, 25);
            break;
    	case PAYLOAD_HEARTBEAT_RESPONSE :
    		szResponse[0] = PAYLOAD_HEARTBEAT_RESPONSE;
    		szResponse[1] = CC3000config->uaMacAddr[2];
    		szResponse[2] = CC3000config->uaMacAddr[1];
    		szResponse[3] = CC3000config->uaMacAddr[0];
    		szResponse[4] = comms_transmit.type;
    		szResponse[5] = comms_transmit.device;
    		szResponse[6] = comms_transmit.status;
    		szResponse[7] = comms_transmit.state;
    		szResponse[8] = comms_transmit.data;

    		setHeartbeatSentFlag(1);
    		sendPayLoad(szResponse, 9);
    		break;
        default:
        	//TODO SOMETHING
            break;
    }
}

void receivePayLoad()
{
	if (receivePackets() > 0)
	{
		payloadReceived(requestBuffer);
	}
}

void sendPayLoad(char* pcData, int length)
{
	sendPackets(pcData, length); // data pointer
	__delay_cycles(7500000); //Important to make sure All packet sent
}
