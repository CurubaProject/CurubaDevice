#ifndef TYPEDEVICE_H
#define TYPEDEVICE_H

#include "CommsManager.h"
#include "device.h"

typedef struct typeDevice {
	InitDevice initDevice;
	InitListCommsDevice initListComms;
	HeartBeatDevice heartBeat;
	ControlCommsReceiveDevice controlCommsReceive;
	InfoCommsReceiveDevice infoCommsReceive;
	ChangeIODevice changeIO;
	InitTIMER1Device initTIMER1;
	InitTIMER2Device initTIMER2;
    TIMER2_Execute timer2_execute;
} TYPEDEVICE;

int GetState(int deviceNumber, int* Tab_ADC10);
int ChangeIO_Device(TYPEDEVICE* device, int commande, int deviceNumber, int* Tab_ADC10);
TYPEDEVICE* createTypeDevice(int typeModule);
TYPEDEVICE* createDimmer();

#endif
