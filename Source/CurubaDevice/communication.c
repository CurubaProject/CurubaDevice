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
#include "network.h"
#include "wifiCommun.h"

#include "commsReceive.h"

#include "commsManager.h"
#include "commun.h"
#include "eventManager.h"

#include "string.h"

#define PAYLOAD_SIZE (25)

TYPEDEVICE* _device = 0;

/* EVENT FUNCTION */
void do_event_hearbeat_readyToSend()
{
	_device->heartBeat();
}

void do_event_payloadReceived()
{
	comms* receivePop;

	if ( popReceive(&receivePop) )
	{
		( receiveComms(receivePop->payloadid) )(_device, receivePop);
	}
}

void do_event_payloadToSend()
{
	comms* transmitPop;

	if ( popTransmit(&transmitPop) )
	{
		payloadToSend(transmitPop);
	}
}

/* END EVENT FUNCTION */

void initCommunication(TYPEDEVICE** device)
{
	_device = *device;
}

void payloadReceived(unsigned char* requestBuffer)
{
    char szResponse[PAYLOAD_SIZE];
    memset(szResponse, 0, sizeof(szResponse));

    comms comms_receive;

    switch (requestBuffer[0])
    {
        case PAYLOAD_INFO_REQUEST :
        	notify(EVENT_INFOREQUEST_RECEIVED);
            comms_receive.payloadid = PAYLOAD_INFO_REQUEST;
            pushReceive(&comms_receive);
            break;
        case PAYLOAD_CONTROL_REQUEST :
            comms_receive.payloadid = PAYLOAD_CONTROL_REQUEST;
            comms_receive.device = requestBuffer[1];
            comms_receive.status = requestBuffer[2];
            comms_receive.state = requestBuffer[3];
            comms_receive.data = requestBuffer[4];

            pushReceive(&comms_receive);
            break;
        case PAYLOAD_CONFIG_REQUEST:
            comms_receive.payloadid = PAYLOAD_CONFIG_REQUEST;
            break;
        case PAYLOAD_HEARTBEAT_REQUEST:
        	notify(EVENT_HEARTBEAT_RECEIVED);
            break;
        default:
            break;
    }

	notify(EVENT_PAYLOAD_RECEIVED);
}

void payloadToSend(comms* PtrPop)
{
    comms comms_transmit = *PtrPop;

	LanConfig* config;
	unsigned char* serverIP = 0;
	unsigned char* serverPort = 0;
	getLanConfig(serverIP, serverPort, &config);

    char szResponse[PAYLOAD_SIZE];
    memset(szResponse, 0, sizeof(szResponse));

    switch (comms_transmit.payloadid)
    {
        case PAYLOAD_INFO_RESPONSE :
            szResponse[0] = PAYLOAD_INFO_RESPONSE;
            szResponse[1] = config->uaMacAddr[2];
            szResponse[2] = config->uaMacAddr[1];
            szResponse[3] = config->uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;

            sendPayLoad(szResponse, 9);
            break;
        case PAYLOAD_CONTROL_RESPONSE :
            szResponse[0] = PAYLOAD_CONTROL_RESPONSE;
            szResponse[1] = config->uaMacAddr[2];
            szResponse[2] = config->uaMacAddr[1];
            szResponse[3] = config->uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;

            sendPayLoad(szResponse, 9);
            break;
        case PAYLOAD_CONFIG_RESPONSE:

            szResponse[0] = PAYLOAD_CONFIG_RESPONSE;

            szResponse[1] = config->uaMacAddr[5];
            szResponse[2] = config->uaMacAddr[4];
            szResponse[3] = config->uaMacAddr[3];
            szResponse[4] = config->uaMacAddr[2];
            szResponse[5] = config->uaMacAddr[1];
            szResponse[6] = config->uaMacAddr[0];

            szResponse[7] = config->aucIP[3];
            szResponse[8] = config->aucIP[2];
            szResponse[9] = config->aucIP[1];
            szResponse[10] = config->aucIP[0];

            szResponse[11] = config->aucSubnetMask[3];
            szResponse[12] = config->aucSubnetMask[2];
            szResponse[13] = config->aucSubnetMask[1];
            szResponse[14] = config->aucSubnetMask[0];

            szResponse[15] = config->aucDefaultGateway[3];
            szResponse[16] = config->aucDefaultGateway[2];
            szResponse[17] = config->aucDefaultGateway[1];
            szResponse[18] = config->aucDefaultGateway[0];

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
    		szResponse[1] = config->uaMacAddr[2];
    		szResponse[2] = config->uaMacAddr[1];
    		szResponse[3] = config->uaMacAddr[0];
    		szResponse[4] = comms_transmit.type;
    		szResponse[5] = comms_transmit.device;
    		szResponse[6] = comms_transmit.status;
    		szResponse[7] = comms_transmit.state;
    		szResponse[8] = comms_transmit.data;

    		sendPayLoad(szResponse, 9);
    		break;
        default:
        	//TODO SOMETHING
            break;
    }
}

