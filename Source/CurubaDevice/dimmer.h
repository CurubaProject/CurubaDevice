#ifndef DIMMER_H
#define DIMMER_H

#define HEARTBEAT_TIME 0x0A00 //5 sec

void initDevice_dimmer(int* Tab_ADC10);
void initListComms_dimmer();
void heartBeat_dimmer(comms* transmitFirst, comms* transmitPush, int* Tab_ADC10);
void controlCommsReceive_dimmer(TYPEDEVICE* device,
								comms* ReceivePop, 
								comms* transmitFirst,comms* transmitPush,
								int* Tab_ADC10);
void infoCommsReceive_dimmer(comms* transmitFirst,comms* transmitPush);
void changeIO_dimmer(int deviceNumber);
void initTIMER1_dimmer();
void initTIMER2_dimmer() ;

// Interupt
void timer2_Execute_dimmer(void);

#endif