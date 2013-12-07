/*****************************************************************************

 *****************************************************************************/

#ifndef DOMUMAPP_H
#define DOMUMAPP_H

#include "wlan.h"
#include "evnt_handler.h"
#include "nvmem.h"
#include "socket.h"
#include "netapp.h"
#include "spi.h"
#include "board.h"
#include <msp430.h>
#include "security.h"
#include <string.h>

#include "host_driver_version.h"

#define DISABLE                                     (0)
#define ENABLE                                      (1)
#define NETAPP_IPCONFIG_MAC_OFFSET              	(20)

#define PALTFORM_VERSION							(1)
#define UART_COMMAND_CC3000_SIMPLE_CONFIG_START		(0x31)
#define UART_COMMAND_CC3000_CONNECT					(0x32)
#define UART_COMMAND_SOCKET_OPEN					(0x33)
#define UART_COMMAND_SEND_DATA						(0x34)
#define UART_COMMAND_RCV_DATA						(0x35)
#define UART_COMMAND_BSD_BIND						(0x36)
#define UART_COMMAND_SOCKET_CLOSE					(0x37)
#define UART_COMMAND_IP_CONFIG						(0x38)
#define UART_COMMAND_CC3000_DISCONNECT				(0x39)
#define UART_COMMAND_CC3000_DEL_POLICY				(0x61)
#define UART_COMMAND_SEND_DNS_ADVERTIZE				(0x62)
#define CC3000_APP_BUFFER_SIZE						(5)
#define CC3000_RX_BUFFER_OVERHEAD_SIZE				(20)
#define SL_VERSION_LENGTH							(11)
//#define NETAPP_IPCONFIG_MAC_OFFSET					(20)

#define IP_ALLOC_METHOD USE_DHCP

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

#define STATE_ON			(1)
#define STATE_OFF			(2)
#define STATE_NOLOAD		(3)

/*
#define CTRL_1      BIT6
#define CTRL_2      BIT5
#define CTRL_7      BIT7
#define CTRL_OUT    P1OUT
*/
//**********************************
#define TYPE_DIMMER 1
#define TYPE_OUTLET 2

#define DEVICE_1 1
#define DEVICE_2 2

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

//READ_TIME = 10ms
#define READ_TIME 250000;

//ADC voltage ref.
#define SUPPLY_VOLTAGE 3.3
#define VOLTAGE_REF 1

//ADC_STEP = number of step for 10 bits
#define ADC_STEP 1023;

//GRID_VOLTAGE = ?
#define GRID_VOLTAGE_AMERICA 120
#define GRID_VOLTAGE_EUROPE 240

//**********************************

//////////////////////////////////////
//            PROTOTYPES            //
//////////////////////////////////////

void InitADC10(void);
void InitTIMER0(void);
void InitTIMER1(int);
void InitTIMER2(int);

void InitListComms(int);
void InfoCommsReceive(int);
void ControlCommsReceive(int);

int GetState(int);
void HeartBeat(void);
int IsStateChange(int, int);
void TimerStart(int);
void TimerStop(int);
void ADCRead(int);
int ChangeIO(int, int);                   //OK//TODO//ChangeIO
int ComputationWattHour(void);          //OK//TODO//ComputationWattHour

#endif
