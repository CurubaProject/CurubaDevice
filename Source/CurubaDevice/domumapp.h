/*****************************************************************************

 *****************************************************************************/

#ifndef DOMUMAPP_H
#define DOMUMAPP_H

#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

#include "typeDevice.h"

//////////////////////////////////////
//            PROTOTYPES            //
//////////////////////////////////////
void initApp(TYPEDEVICE* device);

void InitADC10(void);
void InitTIMER0(void);
void InitTIMER1(TYPEDEVICE* device);
void InitTIMER2(TYPEDEVICE* device);

void InitListComms(TYPEDEVICE* device);
void InfoCommsReceive(TYPEDEVICE* device, comms* transmitFirst,comms* transmitPush);
void ControlCommsReceive(TYPEDEVICE* device, comms* ReceivePop, comms* transmitFirst,comms* transmitPush);
void HeartBeat(TYPEDEVICE* device);

#endif
