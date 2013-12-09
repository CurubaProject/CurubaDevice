/*
 * CommsManager.h
 *
 *  Created on: 2013-11-30
 *      Author: Jean-Sebastien
 */

#ifndef COMMSMANAGER_H_
#define COMMSMANAGER_H_

#define NEWCOMMS    1
#define NOCOMMS     0

typedef struct comms {
	int payloadid;
	int type;
	int device;
	int status;
	int state;
	long int data;
} comms;

int Pop(comms**, comms**, comms**);
void Push(comms**, comms**, comms);
void payloadToSend(comms*);

comms* getTransmitFirst();
comms* getTransmitPush();

#endif /* COMMSMANAGER_H_ */
