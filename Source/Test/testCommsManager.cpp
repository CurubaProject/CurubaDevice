#include "testCommsManager.h"
#include "cute.h"
#include <stdio.h>

extern "C"
{
	#include "commsManager.c"
}

void test_pushTransmit() {
	comms newComms;
	newComms.data = 10;

	pushTransmit(&newComms);

	comms* newComms2;

	popTransmit(&newComms2);

	ASSERTM("start writing tests", newComms.data = 10);
	ASSERTM("start writing tests", newComms2->data = 10);
}

void test_MultiplePushTransmit() {
	comms newComms;

	newComms.data = 10;
	newComms.device = 1;
	pushTransmit(&newComms);

	newComms.data = 20;
	newComms.device = 2;
	pushTransmit(&newComms);

	newComms.data = 30;
	pushTransmit(&newComms);

	newComms.data = 40;
	pushTransmit(&newComms);

	newComms.data = 50;
	pushTransmit(&newComms);

	newComms.data = 60;
	pushTransmit(&newComms);

	newComms.data = 70;
	pushTransmit(&newComms);

	newComms.data = 80;
	pushTransmit(&newComms);

	comms* newComms2;
	popTransmit(&newComms2);

	comms* newComms3;
	popTransmit(&newComms3);

	char buffer [50];
	sprintf (buffer, "Fifth data send erase first Data=%ld", newComms2->data);

	ASSERTM("Last data send Data=" + newComms.data, newComms.data == 80);
	ASSERTM(buffer, newComms2->data == 60);
	sprintf (buffer, "Sixth data send erase seciond Data=%ld", newComms3->data);
	ASSERTM(buffer, newComms3->data == 70);
}

void test_receiveComms() {
	comms newComms;
	newComms.data = 10;

	pushTransmit(&newComms);

	comms* newComms2;

	popTransmit(&newComms2);

	ASSERTM("start writing tests", newComms.data = 10);
	ASSERTM("start writing tests", newComms2->data = 10);
}

void test_MultipleReceiveComms() {
	comms newComms;

	newComms.data = 10;
	newComms.device = 1;
	pushReceive(&newComms);

	newComms.data = 20;
	newComms.device = 2;
	pushReceive(&newComms);

	newComms.data = 30;
	pushReceive(&newComms);

	newComms.data = 40;
	pushReceive(&newComms);

	newComms.data = 50;
	pushTransmit(&newComms);

	newComms.data = 60;
	pushReceive(&newComms);

	newComms.data = 70;
	pushReceive(&newComms);

	newComms.data = 80;
	pushReceive(&newComms);

	comms* newComms2;
	popReceive(&newComms2);

	comms* newComms3;
	popReceive(&newComms3);

	char buffer [50];
	sprintf (buffer, "Fifth data send erase first Data=%ld", newComms2->data);

	ASSERTM("Last data send Data=" + newComms.data, newComms.data == 80);
	ASSERTM(buffer, newComms2->data == 60);
	sprintf (buffer, "Sixth data send erase seciond Data=%ld", newComms3->data);
	ASSERTM(buffer, newComms3->data == 70);
}
