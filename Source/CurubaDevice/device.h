#ifndef DEVICE_H
#define DEVICE_H

#include "CommsManager.h"

typedef struct typeDevice TYPEDEVICE;

typedef void (*InitDevice)(int* Tab_ADC10);
typedef void (*InitListCommsDevice)();
typedef void (*HeartBeatDevice)(comms* transmitFirst, comms* transmitPush, int* Tab_ADC10);
typedef void (*ControlCommsReceiveDevice)(TYPEDEVICE* device,
												comms* ReceivePop,
												comms* transmitFirst, comms* transmitPush,
												int* Tab_ADC10);
typedef void (*InfoCommsReceiveDevice)(comms* transmitFirst,comms* transmitPush);
typedef void (*ChangeIODevice)(int deviceNumber);
typedef void (*InitTIMER1Device)();
typedef void (*InitTIMER2Device)();

// Interupt
typedef void (*TIMER2_Execute)();

#endif
