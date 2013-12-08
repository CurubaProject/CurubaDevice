#ifndef COMMUN_H
#define COMMUN_H

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define STATE_ON			(1)
#define STATE_OFF			(2)
#define STATE_NOLOAD		(3)

#define TYPE_DIMMER 1
#define TYPE_OUTLET 2

#define DEVICE_1 1
#define DEVICE_2 2

#define PAYLOAD_INFO_REQUEST		(10)
#define PAYLOAD_INFO_RESPONSE		(20)
#define PAYLOAD_CONTROL_REQUEST		(30)
#define PAYLOAD_CONTROL_RESPONSE	(40)
#define PAYLOAD_CONFIG_REQUEST		(50)
#define PAYLOAD_CONFIG_RESPONSE		(60)
#define PAYLOAD_HEARTBEAT_RESPONSE  (80)

#define STATUS_ACTIVE 		(1)
#define STATUS_INACTIVE 	(2)
#define STATUS_OVERRIDE 	(3)

#endif
