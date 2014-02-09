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
#ifndef __TESTDEBUG__

#ifndef BOARDMSP430_H
#define BOARDMSP430_H

// Pins from MSP430 connected to the RF Header
#define SPI_IRQ_PIN       BIT0  //P2.0
#define WLAN_EN_PIN       BIT1  //PJ.1

#define SPI_SIMO        BIT1    //P4.1
#define SPI_SOMI        BIT2    //P4.2
#define SPI_CLK         BIT3    //P4.3
#define RF_CS           BIT0    //P4.0

// Ports
#define SPI_SEL         P4SEL
#define SPI_DIR         P4DIR
#define SPI_OUT         P4OUT
#define SPI_REN         P4REN

#define RF_CS_SEL       P4SEL
#define RF_CS_OUT       P4OUT
#define RF_CS_DIR       P4DIR

#define SPI_IRQ_DIR       P2DIR
#define SPI_IRQ_IN        P2IN
#define SPI_IRQ_IES       P2IES
#define SPI_IRQ_IE        P2IE
#define SPI_IFG_PORT      P2IFG

#define WLAN_EN_DIR       PJDIR
#define WLAN_EN_OUT       PJOUT

#define LED1BIT        	  BIT4    	//P1.4
#define LED1_PORT_DIR     P1DIR
#define LED1_PORT_OUT     P1OUT

// Pins from MSP430 connected to switch 4 dib

#define SWITCH_B1_1          BIT0
#define SWITCH_B1_2          BIT1
#define SWITCH_B1_3          BIT2
#define SWITCH_B1_4          BIT3

#define SWITCH_B1_DIR        P1DIR
#define SWITCH_B1_REN        P1REN
#define SWITCH_B1_SEL        P1SEL
#define SWITCH_B1_IN         P1IN
#define SWITCH_B1_OUT        P1OUT

// Pins from MSP430 connected to interrupt B2
#define SWITCH_B2         BIT0		//PJ.0

#define SWITCH_B2_DIR     PJDIR
#define SWITCH_B2_REN     PJREN
#define SWITCH_B2_SEL     PJSEL
#define SWITCH_B2_IN      PJIN
#define SWITCH_B2_OUT     PJOUT
#define SWITCH_B2_IE      PJIE
#define SWITCH_B2_IES     PJIES

// Pins from MSP430 connected to interrupt zero-cross
#define ZERO_CROSS          BIT7  	//P1.7

#define ZERO_CROSS_DIR      P1DIR
#define ZERO_CROSS_SEL      P1SEL
#define ZERO_CROSS_IN       P1IN
#define ZERO_CROSS_IE       P1IE
#define ZERO_CROSS_IES      P1IES
#define ZERO_CROSS_IFG      P1IFG

// PIN from MSP430 connected to CTRL
#define CTRL_1      BIT5	//P1.5
#define CTRL_2      BIT6	//P1.6

#define CTRL_DIR    P1DIR
#define CTRL_SEL    P1SEL
#define CTRL_OUT    P1OUT

// PIN from MSP430 connected to ADC
#define ADC_1      BIT0		//P6.0
#define ADC_2      BIT1		//P6.1

// PIN from MSP430 in option for I/O or ADC
#define OPTION1    BIT2		//P6.2
#define OPTION2    BIT3		//P6.3

#define ADC_SEL    P6SEL

void LFXT_Start(unsigned int xtdrive);
void Init_FLL_Settle(unsigned int fsystem, unsigned int ratio);
void Init_FLL(unsigned int fsystem, unsigned int ratio);
unsigned int SetVCore(unsigned char level);

#endif

#endif
