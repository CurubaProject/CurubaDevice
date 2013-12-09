#ifndef OUTLET_H
#define OUTLET_H

#define HEARTBEAT_TIME 0x3C00

void initDevice_outlet(int* Tab_ADC10);
void initListComms_outlet();
void heartBeat_outlet(comms* transmitFirst, comms* transmitPush, int* Tab_ADC10);
void controlCommsReceive_outlet(TYPEDEVICE* device,
								 comms* ReceivePop, 
								 comms* transmitFirst, comms* transmitPush, 
								int* Tab_ADC10);
void changeIO_outlet(int deviceNumber);
void infoCommsReceive_outlet(comms* transmitFirst, comms* transmitPush);
void initTIMER1_outlet();
void initTIMER2_outlet();

// Interupt
void timer2_Execute_outlet(void);

#endif
