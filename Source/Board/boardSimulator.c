/*****************************************************************************
 *
 *  board.c - FRAM board functions
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

#ifdef __TESTDEBUG__

#include "board.h"
#include "boardSimulatorConfig.h"

long ReadWlanInterruptPin(void)
{
	return 0;
}

void WlanInterruptEnable(){}
void WlanInterruptDisable(){}
void WriteWlanPin(unsigned char trueFalse){}
void LFXT_Start(unsigned int xtdrive){}

void pio_init() {}
void pio_init_wifi() {}
void readADC(int ADC_number) {}
void TimerStart(int timer_number) {}
void TimerStop(int timer_number) {}

/****************************************/
unsigned short value_readAppSwitch = 0;
void config_readAppSwitch(unsigned short value)
{
	value_readAppSwitch = value;
}
unsigned short readAppSwitch()
{
	return value_readAppSwitch;
}
/****************************************/

void setHeartbeatTime(unsigned int time) {}
void setDimmerTime(unsigned int time) {}
void disableDimmer() {}
void enableDimmer(){}
void setState(int deviceNumber, int command){}

/*****************************************/
int value_setStateDimmer = 0;
void config_setStateDimmer(int value)
{
	value_setStateDimmer = value;
}
int setStateDimmer(int current_state)
{
	return value_setStateDimmer;
}
/*****************************************/

void setCurrentStateDimmerOn(){}
void turnOffligth(int SwitchDimmer){}


/*******************************************/
int value_getCurrentStateDimmer = 0;
void config_getCurrentStateDimmer(int value)
{
	value_getCurrentStateDimmer = value;
}
int getCurrentStateDimmer()
{
	return value_getCurrentStateDimmer;
}
/*******************************************/

void toggleControl(void){}
void turnOnlight(int SwitchDimmer){}
void resetDimmerTimer(){}
void init_bis_register(){}
void turnLedOn(char ledNum){}
void turnLedOff(char ledNum){}
void toggleLed(char ledNum){}


/******************************/
int value_getValueCurrentVolt = 0;
void config_getValueCurrentVolt(int value)
{
	value_getValueCurrentVolt = 0;
}
int getValueCurrentVolt()
{
	return value_getValueCurrentVolt;
}
/******************************/

#endif
