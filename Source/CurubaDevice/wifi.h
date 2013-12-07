/*
 * wiki.h
 *
 *  Created on: 2013-10-30
 */

#ifndef WIKI_H_
#define WIKI_H_

#define PING_ATTEMPT                    (5)
#define PING_SIZE                       (10)
#define PING_TIMEOUT                    (20)

#define LED_STATE_OFF                   (0)
#define LED_STATE_UNCONNECTED           (1)
#define LED_STATE_CONNECTED             (2)
#define LED_STATE_CONFIGURING           (3)

void initCommunication (void);
void initDriver(void);
void payloadReceived(unsigned char *usBuffer, signed long iReturnValue);
void initSocketComm();
void receivePayLoad();
void sendPayLoad(char* pcData, int length);
void initCOMMS(void);
void initCC3000(void);
int connectNetwork(void);
int connectServer(void);

void initTIMERB0(void);
void StartTIMERB0(void);
void StopTIMERB0(void);
unsigned long getTimeCollapse(unsigned long lastcount);
unsigned long getCountValue(void);

char *sendDriverPatch(unsigned long *Length);
char *sendWLFWPatch(unsigned long *Length);
char *sendBootLoaderPatch(unsigned long *Length);
void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length);


//Test Mohamed
int recvComm(void);

#endif /* WIKI_H_ */
