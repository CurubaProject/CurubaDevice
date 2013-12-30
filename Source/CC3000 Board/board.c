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

#include "board.h"
#include "boardMSP430.h"
#include <msp430.h>

/* PRIVATE FUNCTION */
void initClk();
/* END PRIVATE FUNCTION */

//*****************************************************************************
//
//! pio_init
//!
//! @param  none
//!
//! @return none
//!
//! @brief  Initialize the board's I/O
//
//*****************************************************************************
void pio_init()
{
	// Setup XT1 and XT2
	//XT1_XT2_PORT_SEL |= XT1_ENABLE + XT2_ENABLE;
	// Use internal REF0 32.768kHz XTAL as reference (fFLLREFCLK = fREF0)
	UCSCTL3 &= 0xFF88;
	UCSCTL3 |= 0x0020; //SELREF = REF0 et FFLREVDIV = 0

	// Init device clock
	initClk();

	//init Leds
	initLEDs();

	// WLAN enable
	WLAN_EN_OUT &= ~WLAN_EN_PIN;
	WLAN_EN_DIR |= WLAN_EN_PIN;

	// SPI IRQ enable
	SPI_IRQ_DIR &= ~SPI_IRQ_PIN;

	// Port initialization for SPI operation
	SPI_SEL |= SPI_CLK + SPI_SOMI + SPI_SIMO;
	SPI_DIR |= SPI_CLK + SPI_SIMO;
	SPI_REN |= SPI_SOMI;                        // Pull-Ups on RF Interface SOMI
	SPI_OUT |= SPI_SOMI;

	RF_CS_SEL &= ~RF_CS;
	RF_CS_OUT |= RF_CS;
	RF_CS_DIR |= RF_CS;

	// Port initialisation SWITCH_B1
	SWITCH_B1_SEL &= ~SWITCH_B1_1;
	SWITCH_B1_SEL &= ~SWITCH_B1_2;
	SWITCH_B1_SEL &= ~SWITCH_B1_3;
	SWITCH_B1_SEL &= ~SWITCH_B1_4;

	SWITCH_B1_DIR &= ~SWITCH_B1_1;
	SWITCH_B1_DIR &= ~SWITCH_B1_2;
	SWITCH_B1_DIR &= ~SWITCH_B1_3;
	SWITCH_B1_DIR &= ~SWITCH_B1_4;
	SWITCH_B1_REN |= SWITCH_B1_1 + SWITCH_B1_2 + SWITCH_B1_3 + SWITCH_B1_4;
	SWITCH_B1_OUT &= ~SWITCH_B1_1;
	SWITCH_B1_OUT &= ~SWITCH_B1_2;
	SWITCH_B1_OUT &= ~SWITCH_B1_3;
	SWITCH_B1_OUT &= ~SWITCH_B1_4;

	// Port initialisation SWITCH_B2
	SWITCH_B2_DIR &= ~SWITCH_B2;
	SWITCH_B2_REN |= SWITCH_B2;
	SWITCH_B2_OUT &= ~SWITCH_B2;

	// Port initialisation Zero_Cross
	ZERO_CROSS_SEL &= ~ZERO_CROSS;
	ZERO_CROSS_DIR &= ~ZERO_CROSS;
	ZERO_CROSS_IES &= ~ZERO_CROSS;
	ZERO_CROSS_IE &= ~ZERO_CROSS;

	//For the case that CTRL pin are inverse (by default)
	// Port initialisation CTRL
	CTRL_SEL &= ~CTRL_1;
	CTRL_SEL &= ~CTRL_2;
	CTRL_DIR |= CTRL_1 + CTRL_2;
	CTRL_OUT &= ~CTRL_1 & ~CTRL_2;

	// Port initialisation ADC
	ADC_SEL |= ADC_1 + ADC_2;

	// Globally enable interrupts
	__enable_interrupt();
}

//*****************************************************************************
//
//! initClk
//!
//!  @param  None
//!
//!  @return none
//!
//!  @brief  Init the device with 16 MHz DCOCLCK.
//
//*****************************************************************************
void initClk(void)
{
	// Set Vcore to accomodate for max. allowed system speed
	SetVCore(3);

	// Set system clock to max (25MHz)
	Init_FLL_Settle(25000, 762);
}

void readADC(int ADC_number)
{
	//to watch
	if ((TA0CTL>>4 & 0x0001) == 1) //(TA0CTL == MC_1)
	{
		//Wait for the end of sampling
		__delay_cycles(525000);
	}

	ADC10CTL0 &= ~ADC10ENC;

	ADC10MCTL0 &= ADC_SEL_RESET;

	ADC10MCTL0 |= (0x0000 & (ADC_number - 1));

	ADC10CTL0 |= ADC10ENC;

	__delay_cycles(2000000);

	TimerStart(TIMER_0);

	while((TA0CTL>>4 & 0x0001) == 1)
	{

	}
}

void TimerStart(int timer_number)
{
	//TODO Change to switch
	if (timer_number == TIMER_0)
	{
		TA0R = 0;
		TA0CTL |= MC_1;
	}
	else if (timer_number == TIMER_1)
	{
		TA1R = 0;
		TA1CTL |= MC_1;
	}
	else if (timer_number == TIMER_2)
	{
		TA2R = 0;
		TA2CTL |= MC_1;
	}
	else if (timer_number == TIMERB_0)
	{
		TB0CTL |= MC_1; //Start timer in Up-mode
	}
}

void TimerStop(int timer_number)
{
	switch (timer_number)
	{
		case TIMER_0:
			TA0CTL &= TIMER_OFF;
			break;
		case TIMER_1:
			TA1CTL &= TIMER_OFF;
			break;
		case TIMER_2:
			TA2CTL &= TIMER_OFF;
			break;
		case TIMERB_0:
			TB0CTL &= TIMER_OFF; //Halt timer
			TB0R = 0;
			break;
		default:
			break;
	}
}

unsigned short readAppSwitch()
{
	return (SWITCH_B1_IN & 0x0F);
}

void setHeartbeatTime(unsigned int time)
{
	TA1CCR0 |= time;
}

void setDimmerTime(unsigned int time)
{
	TA2CCR0 |= time;
}

void disableDimmer()
{
	ZERO_CROSS_IE &= ~ZERO_CROSS;
	ZERO_CROSS_IFG &= ~ZERO_CROSS;
}

void enableDimmer()
{
	//Enable Zero cross for dimmer function
	ZERO_CROSS_IE |= ZERO_CROSS;
}


void setState(int deviceNumber, int command)
{
	if (deviceNumber == DEVICE_1)
	{
		if (command == STATE_ON)
		{
			CTRL_OUT |= CTRL_1;
		}
		else if(command == STATE_OFF)
		{
			CTRL_OUT &= ~CTRL_1;
		}
	}
	else if (deviceNumber == DEVICE_2 )
	{
		if (command == STATE_ON)
		{
			CTRL_OUT |= CTRL_2;
		}
		else if(command == STATE_OFF)
		{
			CTRL_OUT &= ~CTRL_2;
		}
	}
}

int setStateDimmer(int current_state)
{
	int switchDimmer = 0;
	if (current_state == STATE_ON && (CTRL_OUT & (CTRL_1 + CTRL_2)) != 0)
	{
		switchDimmer = (CTRL_1 + CTRL_2) & CTRL_OUT;
	}
	else if(CTRL_OUT & (CTRL_1 + CTRL_2) != 0)
	{
		switchDimmer = (CTRL_1 + CTRL_2) & ~CTRL_OUT;
	}

	return switchDimmer;
}

void setCurrentStateDimmerOn()
{
	CTRL_OUT |= CTRL_1;
}

void turnOffligth(int SwitchDimmer)
{
	if(CTRL_OUT & CTRL_1)
	{
		CTRL_OUT &= (~SwitchDimmer & CTRL_1);
	}
	else
	{
		CTRL_OUT |= (~SwitchDimmer & CTRL_1);
	}

	if(CTRL_OUT & CTRL_2)
	{
		CTRL_OUT &= (~SwitchDimmer & CTRL_2);
	}
	else
	{
		CTRL_OUT |= (~SwitchDimmer & CTRL_2);
	}
}

int getCurrentStateDimmer()
{
	return (CTRL_1 + CTRL_2) & ~CTRL_OUT;
}


void toggleControl(void)
{
	if(CTRL_OUT & CTRL_1)
	{
		CTRL_OUT &= ~CTRL_1;
	}
	else
	{
		CTRL_OUT |= CTRL_1;
	}

	if(CTRL_OUT & CTRL_2)
	{
		CTRL_OUT &= ~CTRL_2;
	}
	else
	{
		CTRL_OUT |= CTRL_2;
	}
}

void turnOnlight(int SwitchDimmer)
{
	if(CTRL_OUT & CTRL_1)
	{
		CTRL_OUT &= (SwitchDimmer & CTRL_1);
	}
	else
	{
		CTRL_OUT |= (SwitchDimmer & CTRL_1);
	}

	if(CTRL_OUT & CTRL_2)
	{
		CTRL_OUT &= (SwitchDimmer & CTRL_2);
	}
	else
	{
		CTRL_OUT |= (SwitchDimmer & CTRL_2);
	}
}

void resetDimmerTimer()
{
	// Reset the count
	TA2R &= 0x0000;
	TA2CTL &= ~TAIFG;
}

void init_bis_register()
{
	__bis_SR_register(GIE);
}


//*****************************************************************************
//
//! initLEDs
//!
//! @param  none
//!
//! @return none
//!
//! @brief  Initializes LED Ports and Pins
//
//*****************************************************************************
void initLEDs() {
	LED1_PORT_OUT &= ~(LED1BIT);
	LED1_PORT_DIR |= LED1BIT;
}

//*****************************************************************************
//
//!  turnLedOn
//!
//! @param  ledNum is the LED Number
//!
//! @return none
//!
//! @brief  Turns a specific LED on
//
//*****************************************************************************
void turnLedOn(char ledNum) {
	switch (ledNum) {
	case WARNING_LED:
		LED1_PORT_OUT |= (LED1BIT);
		break;
	}
}

//*****************************************************************************
//
//! turnLedOff
//!
//! @param  ledNum is the LED Number
//!
//! @return none
//!
//! @brief  Turns a specific LED Off
//
//*****************************************************************************
void turnLedOff(char ledNum) {
	switch (ledNum) {
	case WARNING_LED:
		LED1_PORT_OUT &= ~(LED1BIT);
		break;
	}
}

//*****************************************************************************
//
//! toggleLed
//!
//! @param  ledNum is the LED Number
//!
//! @return none
//!
//! @brief  Toggles a board LED
//
//*****************************************************************************
void toggleLed(char ledNum) {
	switch (ledNum)
	{
		case WARNING_LED:
			LED1_PORT_OUT ^= (LED1BIT);
			break;
	}
}

int getValueCurrentVolt()
{
	return ADC10MEM0;
}
