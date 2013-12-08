/*
 * wifi.c
 *
 *  Created on: 2013-10-30
 *      Author: lemj2710
 */

#include "wlan.h"
#include "netapp.c"
#include "evnt_handler.h"
#include "board.h"
#include <msp430.h>

#include "host_driver_version.h"
#include "spi.h"

#include "wifi.h"
#include "CommsManager.h"
#include "commun.h"

extern comms* ReceiveFirst;
extern comms* ReceivePush;
extern comms* ReceivePop;
extern comms* TransmitFirst;
extern comms* TransmitPush;
extern comms* TransmitPop;

#define REQ_BUFFER_SIZE 100
#ifdef __CCS__
	unsigned char requestBuffer[REQ_BUFFER_SIZE];
#elif __IAR_SYSTEMS_ICC__
	__no_init char requestBuffer[REQ_BUFFER_SIZE];
#endif

// AJC DEBUG  ?????????
#define MST_DEBUG
#ifdef AJC_DEBUG
	unsigned char SSIDKey[] = "domumservuskey00";
	char SSIDName[] = "DSRouter";
#endif

#ifdef MST_DEBUG
	unsigned char SSIDKey[] = "domumservus";
	char SSIDName[] = "domumrff";
#endif

volatile unsigned long ulTimecount;
volatile unsigned long ulTimeRef;

volatile unsigned long  ulCC3000Connected,
                        ulCC3000DHCP,
                        ulCC3000SocketClosed,
                        ulPingNumber;

volatile long ulSocket, ulSocketTCP;
volatile short  sSocketConnected,
                sNetworkConnectAttempt;
volatile short sLedState;
netapp_pingreport_args_t pingReport;
tNetappIpconfigRetArgs CC3000ipconfig;
unsigned char DSServerIP[4] = { 192, 168, 20, 140 };
unsigned char DSServerPort[2] = { 0x13, 0x88 }; // Port 5000 or 0x1388
sockaddr tSocketAddr;

//
//! CC3000_UsynchCallback
//!
//! @param  lEventType   Event type
//! @param  data
//! @param  length
//!
//! @return none
//!
//! @brief  The function handles asynchronous events that come from CC3000
//!           device and operates a LED1 to have an on-board indication
//
//*****************************************************************************

int _system_pre_init(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
	return 1;
}

char *sendDriverPatch(unsigned long *Length) {
	*Length = 0;
	return NULL;
}

char *sendWLFWPatch(unsigned long *Length) {
	*Length = 0;
	return NULL;
}

char *sendBootLoaderPatch(unsigned long *Length) {
	*Length = 0;
	return NULL;
}

void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length)
{
    if (lEventType == HCI_EVNT_WLAN_UNSOL_CONNECT)
    {
        ulCC3000Connected = 1;
    }

    if (lEventType == HCI_EVNT_WLAN_UNSOL_DISCONNECT)
    {
        ulCC3000Connected = 0;
        ulCC3000DHCP = 0;
    }

    if (lEventType == HCI_EVNT_WLAN_UNSOL_DHCP)
    {
        // Notes:
        // 1) IP config parameters are received swapped
        // 2) IP config parameters are valid only if status is OK, i.e. ulCC3000DHCP becomes 1

        // only if status is OK, the flag is set to 1 and the addresses are valid
        if ( *(data + NETAPP_IPCONFIG_MAC_OFFSET) == 0)
        {
            ulCC3000DHCP = 1;
        }
        else
        {
            ulCC3000DHCP = 0;
        }
    }

    if (lEventType == HCI_EVNT_WLAN_ASYNC_PING_REPORT)
    {


        memcpy(&pingReport, data, length);

        if(pingReport.packets_received > 1) //All packets are received
        {
            ulPingNumber++;
        }

    }
    if (lEventType == HCI_EVNT_BSD_TCP_CLOSE_WAIT)
    {
        ulCC3000SocketClosed = 1;
    }

}

//*****************************************************************************
//
//! initDriver
//!
//!  \param[in] cRequestPatch 0 to load with EEPROM patches and 1 to load with no patches
//!
//!  \return none
//!
//!  \brief  The function initializes a CC3000 device and triggers it to start operation
//
//*****************************************************************************

void initCommunication (void)
{
    //  Board Initialization start
    ulCC3000DHCP = 0;
    ulCC3000Connected = 0;
    ulCC3000SocketClosed = 0;
    ulSocket = 0;
    ulPingNumber = 0;
    sSocketConnected = 0;
    sNetworkConnectAttempt = 0;
    ulTimecount = 0;

    // Initialize the UART RX Buffer
    memset(requestBuffer, 0, sizeof(requestBuffer));
}

//*****************************************************************************


//*****************************************************************************
void initDriver(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	// Init GPIO's
	pio_init();

	//init all layers
	init_spi();

	initCC3000();
}

//*****************************************************************************


//*****************************************************************************
void initCC3000(void)
{

	unsigned char pucIP_Addr[4] = { 0, 0, 0, 0 };
	unsigned char pucIP_DefaultGWAddr[4] = { 0, 0, 0, 0 };
	unsigned char pucSubnetMask[4] = { 0, 0, 0, 0 };
	unsigned char pucDNS[4] = { 0, 0, 0, 0 };

	// WLAN On API Implementation
	wlan_init(CC3000_UsynchCallback,
			sendWLFWPatch,
			sendDriverPatch,
			sendBootLoaderPatch,
			ReadWlanInterruptPin,
			WlanInterruptEnable,
			WlanInterruptDisable,
			WriteWlanPin);

	__delay_cycles(10000);

	// Trigger a WLAN device without Flag
	wlan_start(0);

	// DHCP is used by default

	//Force DHCP
	netapp_dhcp((unsigned long *) pucIP_Addr,
			(unsigned long *) pucSubnetMask,
			(unsigned long *) pucIP_DefaultGWAddr,
			(unsigned long *) pucDNS);

	wlan_ioctl_set_connection_policy(DISABLE, DISABLE, DISABLE);
	wlan_ioctl_del_profile(255); //clear all profiles

	//Wait until CC3000 is disconnected
	// COMPARAISON EST NÉCESSAIRE ?????????
	// delay ??
	while (ulCC3000Connected == 1)
	{
		__delay_cycles(100);
		hci_unsolicited_event_handler();
	}
	ulCC3000Connected = 0;
	ulCC3000DHCP = 0;

	//init device with new FAT
	wlan_stop();
	__delay_cycles(100000);
	wlan_start(0);

	// Mask out all non-required events from CC3000
	wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE | HCI_EVNT_WLAN_UNSOL_INIT);

}

//*****************************************************************************
//
//
//*****************************************************************************
void payloadReceived(unsigned char *usBuffer, signed long iReturnValue)
{
    char szResponse[25];
    comms comms_receive;
    memset(szResponse, 0, sizeof(szResponse));

    switch (usBuffer[0])
    {
        case PAYLOAD_INFO_REQUEST :
            comms_receive.payloadid = PAYLOAD_INFO_REQUEST;
            comms_receive.change = 1;

            Push(&ReceiveFirst, &ReceivePush, comms_receive);
            break;

        case PAYLOAD_CONTROL_REQUEST :
            comms_receive.payloadid = PAYLOAD_CONTROL_REQUEST;
            comms_receive.device = usBuffer[1];
            comms_receive.status = usBuffer[2];
            comms_receive.state = usBuffer[3];
            comms_receive.data = usBuffer[4];
            comms_receive.change = 1;

            Push(&ReceiveFirst, &ReceivePush, comms_receive);
            break;

        case PAYLOAD_CONFIG_REQUEST:
            comms_receive.payloadid = PAYLOAD_CONFIG_REQUEST;
            comms_receive.change = 1;

            //Push(ReceiveFirst, ReceivePush, comms_receive);
            break;

        default:
            break;
    }
}


//*****************************************************************************
//
//
//*****************************************************************************
void payloadToSend(comms* PtrPop)
{
    char szResponse[25];
    memset(szResponse, 0, sizeof(szResponse));

    comms comms_transmit;

    comms_transmit = *PtrPop;

    switch (comms_transmit.payloadid)
    {
    	case 0 :
    		szResponse[0] = 5;
    		szResponse[1] = CC3000ipconfig.uaMacAddr[2];
    		szResponse[2] = CC3000ipconfig.uaMacAddr[1];
    		szResponse[3] = CC3000ipconfig.uaMacAddr[0];
    		szResponse[4] = comms_transmit.type;
    		szResponse[5] = comms_transmit.device;
    		szResponse[6] = comms_transmit.status;
    		szResponse[7] = comms_transmit.state;
    		szResponse[8] = comms_transmit.data;
    		sendPayLoad(szResponse, strlen(szResponse));
//          comms_transmit.change = 0;                     //???

    		break;
        case PAYLOAD_INFO_RESPONSE :
            szResponse[0] = PAYLOAD_INFO_RESPONSE;
            szResponse[1] = CC3000ipconfig.uaMacAddr[2];
            szResponse[2] = CC3000ipconfig.uaMacAddr[1];
            szResponse[3] = CC3000ipconfig.uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;
            sendPayLoad(szResponse, strlen(szResponse));
//          comms_transmit.change = 0;                     //???

            break;
        case PAYLOAD_CONTROL_RESPONSE :
            szResponse[0] = PAYLOAD_CONTROL_RESPONSE;
            szResponse[1] = CC3000ipconfig.uaMacAddr[2];
            szResponse[2] = CC3000ipconfig.uaMacAddr[1];
            szResponse[3] = CC3000ipconfig.uaMacAddr[0];
            szResponse[4] = comms_transmit.type;
            szResponse[5] = comms_transmit.device;
            szResponse[6] = comms_transmit.status;
            szResponse[7] = comms_transmit.state;
            szResponse[8] = comms_transmit.data;
            sendPayLoad(szResponse, strlen(szResponse));
//          comms_transmit.change = 0;                     //???

            break;
        case PAYLOAD_CONFIG_RESPONSE:

            szResponse[0] = PAYLOAD_CONFIG_RESPONSE;

            szResponse[1] = CC3000ipconfig.uaMacAddr[5];
            szResponse[2] = CC3000ipconfig.uaMacAddr[4];
            szResponse[3] = CC3000ipconfig.uaMacAddr[3];
            szResponse[4] = CC3000ipconfig.uaMacAddr[2];
            szResponse[5] = CC3000ipconfig.uaMacAddr[1];
            szResponse[6] = CC3000ipconfig.uaMacAddr[0];

            szResponse[7] = CC3000ipconfig.aucIP[3];
            szResponse[8] = CC3000ipconfig.aucIP[2];
            szResponse[9] = CC3000ipconfig.aucIP[1];
            szResponse[10] = CC3000ipconfig.aucIP[0];

            szResponse[11] = CC3000ipconfig.aucSubnetMask[3];
            szResponse[12] = CC3000ipconfig.aucSubnetMask[2];
            szResponse[13] = CC3000ipconfig.aucSubnetMask[1];
            szResponse[14] = CC3000ipconfig.aucSubnetMask[0];

            szResponse[15] = CC3000ipconfig.aucDefaultGateway[3];
            szResponse[16] = CC3000ipconfig.aucDefaultGateway[2];
            szResponse[17] = CC3000ipconfig.aucDefaultGateway[1];
            szResponse[18] = CC3000ipconfig.aucDefaultGateway[0];

            szResponse[19] = DSServerIP[0];
            szResponse[20] = DSServerIP[1];
            szResponse[21] = DSServerIP[2];
            szResponse[22] = DSServerIP[3];

            szResponse[23] = DSServerPort[0];
            szResponse[24] = DSServerPort[1];
            sendPayLoad(szResponse, strlen(szResponse));
            break;

        default:
            break;
    }
}

void initSocketComm()
{
	unsigned long socketTimeout = 10;
	int iReturnValue = -1;
	ulSocketTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	while (iReturnValue != 0)
	{
		iReturnValue = setsockopt(ulSocketTCP,
								SOL_SOCKET,
								SOCKOPT_RECV_TIMEOUT,
								&socketTimeout,
								sizeof(socketTimeout));
		__delay_cycles(1000);
	}

	// the family is always AF_INET
	tSocketAddr.sa_family = AF_INET;

	// the destination port
	tSocketAddr.sa_data[0] = DSServerPort[0];
	tSocketAddr.sa_data[1] = DSServerPort[1];

	// the destination IP address
	tSocketAddr.sa_data[2] = DSServerIP[0];
	tSocketAddr.sa_data[3] = DSServerIP[1];
	tSocketAddr.sa_data[4] = DSServerIP[2];
	tSocketAddr.sa_data[5] = DSServerIP[3];
}

void receivePayLoad() {
	int iReturnValue = recv(ulSocketTCP, requestBuffer, sizeof(requestBuffer), 0);

	if (iReturnValue > 0)
	{
		payloadReceived(requestBuffer, iReturnValue);
	}
}

void sendPayLoad(char* pcData, int length)
{
	send(ulSocketTCP, pcData, length, 0); // data pointer
	__delay_cycles(100000);
}
/*
void initCOMMS(void) {
	comms_receive.change = 1;
	comms_receive.status = 1;
	comms_receive.state = 1;
	comms_receive.data = 60;
	comms_receive.type = 0;

	comms_transmit.change = 0;
	comms_transmit.status = 0;
	comms_transmit.state = 0;
	comms_transmit.data = 0;
	comms_transmit.type = 0;
}
*/
int connectNetwork(void)
{
    int iReturnConnect;
    int iReturnValue = 0; //Default return value if problem connecting to Network or Server
    unsigned long ulReftime;

    hci_unsolicited_event_handler();
    __delay_cycles(100);
    if(sNetworkConnectAttempt < 5) //After 5  attempt, stopped trying to connect
    {
        sNetworkConnectAttempt++;
        if (ulCC3000Connected == 0)
        {
            iReturnConnect = wlan_connect(WLAN_SEC_WPA2,
                        SSIDName,
                        strlen(SSIDName),
                        NULL,
                        SSIDKey,
                        strlen((char *) SSIDKey));

            ulReftime = getCountValue(); //Wait until connected to Network
            while (iReturnConnect == 0 && ulCC3000Connected == 0 && getTimeCollapse(ulReftime) < 20) //Wait 10 sec
            {
                __delay_cycles(100);
                hci_unsolicited_event_handler();
            }
            if(ulCC3000Connected == 1)
            {
                unsigned long aucDHCP = 14400;
                unsigned long aucARP = 3600;
                unsigned long aucKeepalive = 10;
                unsigned long aucInactivity = 0;

                netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity);
                netapp_ipconfig(&CC3000ipconfig); //Get My IP address
                __delay_cycles(50000000); //Wait Important (Make sure all events have been received)
            }

        }
        if(ulCC3000Connected == 1)
        {
            if(connectServer())
            {
                iReturnValue = 1;
                sNetworkConnectAttempt = 0;
                sLedState = LED_STATE_CONNECTED;
            }
            else
            {
                sLedState = LED_STATE_UNCONNECTED;
            }
        }
    }
    else //Wait Before reconnecting to Wifi Network or Socket server
    {
        //Reset Variable to try to reconnect
        sNetworkConnectAttempt = 0;
        sLedState = LED_STATE_UNCONNECTED;
        iReturnValue = 0;

    }
    return (iReturnValue);
}

int connectServer(void)
{
    unsigned long ulReftime;
    unsigned long ulTime = 0;
    int iReturnping = 0;
    int iReturnValue = 1;

    if(ulCC3000SocketClosed == 1)   //Server closed the socket connection
    {
        //closesocket (ulSocketTCP);
        sSocketConnected = 0;
        ulCC3000SocketClosed = 0;
    }

    if (sSocketConnected == 0)
    {
        initSocketComm();
        iReturnping = netapp_ping_send((unsigned long *) DSServerIP, PING_ATTEMPT, PING_SIZE, PING_TIMEOUT);

        ulReftime = getCountValue();
        //Check Server IP address
        while(iReturnping == 0 && ulPingNumber == 0 && ulTime < 40) //Stop after 5 sec of finding the Server
        {
            hci_unsolicited_event_handler();
            __delay_cycles(100);
            ulTime = getTimeCollapse(ulReftime);
        }

        if(iReturnping != 0 || ulTime >= 10) //Problem finding server IP
        {
            iReturnValue = 0;
        }
    }

    //Connect to Server
    if (sSocketConnected == 0 && iReturnValue == 1)
    {
        int iReturnConnect = connect(ulSocketTCP, &tSocketAddr, sizeof(sockaddr));
        __delay_cycles(12500000);

        if (iReturnConnect == 0)
        {
            iReturnValue = 1;
            sSocketConnected = 1;
        }
        else //Server present but not listening
        {
            //closesocket (ulSocketTCP);
            iReturnValue = 0;
        }
    }

    return (iReturnValue);
}

void initTIMERB0(void)
{
	//Timer for Server connection delay time
	TB0CTL |= TBCLR;
	TB0CTL |= TBSSEL_1 + ID_3 + TBIE;
	TB0EX0 |= TBIDEX_7; //Timer clock = ACLK/64 = 512 Hz
	TB0CCR0 = (int) 256;
	StartTIMERB0(); //Never stop this timer
}

void StartTIMERB0(void)
{
	TB0CTL |= MC_1; //Start timer in Up-mode
}

void StopTIMERB0(void)
{
	TB0CTL &= 0xFFCF; //Halt timer
	TB0R = 0;
}

unsigned long getCountValue(void)
{
	return(ulTimecount);
}


unsigned long getTimeCollapse(unsigned long lastcount)
{
	if(getCountValue() >= lastcount)
	{
		return(getCountValue() - lastcount);
	}
	else
	{
		return(((unsigned long) (0xFFFFFFFF) - lastcount) + getCountValue());
	}
}

//Timer used to interrupt at every 500 msec
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
    static short sFlag = 0;
    unsigned long ultime;
    switch (__even_in_range(TB0IV, 14))
    {
        case 0:                            // No interrupt
            break;
        case TB0IV_TB0CCR1:                            // Capture/Compare 1
            break;
        case TB0IV_TB0CCR2:                            // Capture/Compare 2
            break;
        case TB0IV_3:                            // Capture/Compare 3
            break;
        case TB0IV_4:                            // Capture/Compare 4
            break;
        case TB0IV_5:                           // Capture/Compare 5
            break;
        case TB0IV_6:                           // Capture/Compare 6
            break;
        case TB0IV_TB0IFG:                           // Timer overflow
            ulTimecount++;
            if(sFlag == 0)
            {
                ulTimeRef = getCountValue();
                sFlag = 1;
                ultime = getTimeCollapse(ulTimeRef);
            }
            else if(sFlag == 1)
            {
                ultime = getTimeCollapse(ulTimeRef);
            }
            switch (sLedState)
            {
                case LED_STATE_OFF:
                    turnLedOff(WARNING_LED);
                    break;
                case LED_STATE_UNCONNECTED:
                    turnLedOn(WARNING_LED);
                    break;
                case LED_STATE_CONNECTED:
                    if(ultime >= 8) // 1 Hz ON of toggle
                    {
                        toggleLed(WARNING_LED);
                        sFlag = 0;
                    }
                    break;
                case LED_STATE_CONFIGURING:
                    if(ultime >= 80 )
                    {
                        toggleLed(WARNING_LED);
                        sFlag = 0;
                    }
                    break;
                default:
                    sFlag = 0;
                    break;
            }
            TB0CTL &= ~TBIFG;
            break;
        default:
            break;


    }
}
