/*
 * CommsManager.h
 *
 *  Created on: 2013-11-30
 *      Author: Jean-Sebastien
 */

#ifndef COMMSMANAGER_H_
#define COMMSMANAGER_H_

typedef struct comms {
	int payloadid;
	int change;
	int type;
	int device;
	int status;
	int state;
	long int data;
} comms;

/*
comms comms_receive_array[5];
comms comms_transmit_array[5];

comms* ReceiveFirst = comms_receive_array;
comms* ReceivePush = comms_receive_array;
comms* ReceivePop = comms_receive_array;
comms* TransmitFirst = comms_transmit_array;
comms* TransmitPush = comms_transmit_array;
comms* TransmitPop = comms_transmit_array;
*/

int Pop(comms**, comms**, comms**);
void Push(comms**, comms**, comms);
void payloadToSend(comms*);

#define NEWCOMMS    1
#define NOCOMMS     0

#endif /* COMMSMANAGER_H_ */
