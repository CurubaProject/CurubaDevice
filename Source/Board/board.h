/*****************************************************************************
*
*  board.h - FRAM board definitions
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMERB_0 3

#define STATE_ON			(1)
#define STATE_OFF			(2)
#define STATE_NOLOAD		(3)

#define DEVICE_1 1
#define DEVICE_2 2

#define WARNING_LED       0x01

#define TIMER_OFF 0xFFCF
#define ADC_SEL_RESET 0xFFF0;

#include "interuptDeviceControl.h"

void pio_init();

void readADC(int ADC_number);
void TimerStart(int timer_number);
void TimerStop(int timer_number);
unsigned short readAppSwitch();

void setHeartbeatTime(unsigned int time);
void setDimmerTime(unsigned int time);

void disableDimmer();
void enableDimmer();

void setState(int deviceNumber, int command);
int setStateDimmer(int current_state);
int getCurrentStateDimmer();
void setCurrentStateDimmerOn();

void turnOffligth(int SwitchDimmer);
void turnOnlight(int SwitchDimmer);
void toggleControl();

void resetDimmerTimer();

void init_bis_register();

void initLEDs();

void turnLedOn(char ledNum);
void turnLedOff(char ledNum);
void toggleLed(char ledNum);

int getValueCurrentVolt();

long ReadWlanInterruptPin(void);
void WlanInterruptEnable();
void WlanInterruptDisable();
void WriteWlanPin(unsigned char trueFalse);

void initWatchDogTimer();
void stopWatchDogTimer();

#endif
