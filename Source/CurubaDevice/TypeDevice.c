#include "typeDevice.h"
#include "commun.h"
#include "CommsManager.h"
#include "util.h"

#include "dimmer.h"
#include "outlet.h"

#include <stdlib.h>

int GetState(int deviceNumber, int* Tab_ADC10)
{
	static long int state = 0;
	ADCRead(deviceNumber);

	state = ComputationWattHour(Tab_ADC10) > 1 ? STATE_ON : STATE_OFF;

	return state;
}

int ChangeIO_Device(TYPEDEVICE* device, int commande, int deviceNumber, int* Tab_ADC10)
{
	static long int state = STATE_OFF;

	state = GetState(deviceNumber, Tab_ADC10);

	if(state != commande)
	{
		device->changeIO(deviceNumber);
	}

	state = GetState(deviceNumber, Tab_ADC10);

	if (state != commande)
	{
		state = STATE_NOLOAD;
	}

	return state;
}

TYPEDEVICE* createDimmer() {
	TYPEDEVICE* device = (TYPEDEVICE*) malloc(sizeof *device);

	device->initListComms = initListComms_dimmer;
	device->heartBeat = heartBeat_dimmer;
	device->controlCommsReceive = controlCommsReceive_dimmer;
	device->infoCommsReceive = infoCommsReceive_dimmer;
	device->changeIO = changeIO_dimmer;
	device->initTIMER1 = initTIMER1_dimmer;
	device->initTIMER2 = initTIMER2_dimmer;
	device->initDevice = initDevice_dimmer;
	return device;
}

TYPEDEVICE* createOutlet() {
	TYPEDEVICE* device = (TYPEDEVICE*) malloc(sizeof *device);

	//TODO Assign New Dimmer Method

	return device;
}

TYPEDEVICE* createTypeDevice(int typeModule) {
	TYPEDEVICE* device;

	switch (typeModule) {
	case TYPE_DIMMER :
		device = createDimmer();
		break;
	case TYPE_OUTLET :
		device = createOutlet();
		break;
	}

	return device;
}

