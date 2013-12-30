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

#include "boardMSP430.h"
#include <msp430.h>

#define XT1HFOFFG   0
#define PMM_STATUS_ERROR  1
#define PMM_STATUS_OK     0

/*******************************************************************************
 * \brief   Increase Vcore by one level
 *
 * \param level     Level to which Vcore needs to be increased
 * \return status   Success/failure
 ******************************************************************************/
static unsigned int SetVCoreUp(unsigned char level) {
	unsigned int PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

	// The code flow for increasing the Vcore has been altered to work around
	// the erratum FLASH37. 
	// Please refer to the Errata sheet to know if a specific device is affected
	// DO NOT ALTER THIS FUNCTION

	// Open PMM registers for write access   
	PMMCTL0_H = 0xA5;

	// Disable dedicated Interrupts
	// Backup all registers
	PMMRIE_backup = PMMRIE;
	PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE | SVSLPE | SVMHVLRIE | SVMHIE
			| SVSMHDLYIE | SVMLVLRIE | SVMLIE | SVSMLDLYIE);
	SVSMHCTL_backup = SVSMHCTL;
	SVSMLCTL_backup = SVSMLCTL;

	// Clear flags
	PMMIFG = 0;

	// Set SVM highside to new level and check if a VCore increase is possible
	SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);

	// Wait until SVM highside is settled
	while ((PMMIFG & SVSMHDLYIFG)== 0);

	// Clear flag
	PMMIFG &= ~SVSMHDLYIFG;

	// Check if a VCore increase is possible
	if ((PMMIFG & SVMHIFG)== SVMHIFG) { // -> Vcc is too low for a Vcore increase
		// recover the previous settings
		PMMIFG &= ~SVSMHDLYIFG;
		SVSMHCTL = SVSMHCTL_backup;

		// Wait until SVM highside is settled
		while ((PMMIFG & SVSMHDLYIFG)== 0);

		// Clear all Flags
		PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG
				| SVSMLDLYIFG);

		PMMRIE = PMMRIE_backup;         // Restore PMM interrupt enable register
		PMMCTL0_H = 0x00;                 // Lock PMM registers for write access
		return PMM_STATUS_ERROR;                // return: voltage not set
	}

	// Set also SVS highside to new level     
	// Vcc is high enough for a Vcore increase
	SVSMHCTL |= (SVSHRVL0 * level);

	// Wait until SVM highside is settled
	while ((PMMIFG & SVSMHDLYIFG)== 0);

	// Clear flag
	PMMIFG &= ~SVSMHDLYIFG;

	// Set VCore to new level
	PMMCTL0_L = PMMCOREV0 * level;

	// Set SVM, SVS low side to new level
	SVSMLCTL = SVMLE | (SVSMLRRL0 * level) | SVSLE | (SVSLRVL0 * level);

	// Wait until SVM, SVS low side is settled
	while ((PMMIFG & SVSMLDLYIFG)== 0);

	// Clear flag
	PMMIFG &= ~SVSMLDLYIFG;
	// SVS, SVM core and high side are now set to protect for the new core level

	// Restore Low side settings
	// Clear all other bits _except_ level settings
	SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

	// Clear level settings in the backup register,keep all other bits
	SVSMLCTL_backup &= ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1
			+ SVSMLRRL2);

	// Restore low-side SVS monitor settings
	SVSMLCTL |= SVSMLCTL_backup;

	// Restore High side settings
	// Clear all other bits except level settings
	SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

	// Clear level settings in the backup register,keep all other bits
	SVSMHCTL_backup &= ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1
			+ SVSMHRRL2);

	// Restore backup 
	SVSMHCTL |= SVSMHCTL_backup;

	// Wait until high side, low side settled
	while (((PMMIFG & SVSMLDLYIFG)== 0)&&((PMMIFG & SVSMHDLYIFG) == 0));

	// Clear all Flags
	PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG
			| SVSMLDLYIFG);

	PMMRIE = PMMRIE_backup;             // Restore PMM interrupt enable register
	PMMCTL0_H = 0x00;                     // Lock PMM registers for write access

	return PMM_STATUS_OK;
}

/*******************************************************************************
 * \brief  Decrease Vcore by one level
 *
 * \param  level    Level to which Vcore needs to be decreased
 * \return status   Success/failure
 ******************************************************************************/
static unsigned int SetVCoreDown(unsigned char level) {
	unsigned int PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

	// The code flow for decreasing the Vcore has been altered to work around
	// the erratum FLASH37. 
	// Please refer to the Errata sheet to know if a specific device is affected
	// DO NOT ALTER THIS FUNCTION

	// Open PMM registers for write access
	PMMCTL0_H = 0xA5;

	// Disable dedicated Interrupts 
	// Backup all registers
	PMMRIE_backup = PMMRIE;
	PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE | SVSLPE | SVMHVLRIE | SVMHIE
			| SVSMHDLYIE | SVMLVLRIE | SVMLIE | SVSMLDLYIE);
	SVSMHCTL_backup = SVSMHCTL;
	SVSMLCTL_backup = SVSMLCTL;

	// Clear flags
	PMMIFG &= ~(SVMHIFG | SVSMHDLYIFG | SVMLIFG | SVSMLDLYIFG);

	// Set SVM, SVS high & low side to new settings in normal mode
	SVSMHCTL = SVMHE | (SVSMHRRL0 * level) | SVSHE | (SVSHRVL0 * level);
	SVSMLCTL = SVMLE | (SVSMLRRL0 * level) | SVSLE | (SVSLRVL0 * level);

	// Wait until SVM high side and SVM low side is settled
	while ((PMMIFG & SVSMHDLYIFG)== 0 || (PMMIFG & SVSMLDLYIFG) == 0);

	// Clear flags
PMMIFG	&= ~(SVSMHDLYIFG + SVSMLDLYIFG);
	// SVS, SVM core and high side are now set to protect for the new core level

	// Set VCore to new level
	PMMCTL0_L = PMMCOREV0 * level;

	// Restore Low side settings
	// Clear all other bits _except_ level settings
	SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

	// Clear level settings in the backup register,keep all other bits
	SVSMLCTL_backup &= ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1
			+ SVSMLRRL2);

	// Restore low-side SVS monitor settings
	SVSMLCTL |= SVSMLCTL_backup;

	// Restore High side settings
	// Clear all other bits except level settings
	SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

	// Clear level settings in the backup register, keep all other bits
	SVSMHCTL_backup &= ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1
			+ SVSMHRRL2);

	// Restore backup 
	SVSMHCTL |= SVSMHCTL_backup;

	// Wait until high side, low side settled
	while (((PMMIFG & SVSMLDLYIFG)== 0)&&((PMMIFG & SVSMHDLYIFG) == 0));

	// Clear all Flags
	PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG
			| SVSMLDLYIFG);

	PMMRIE = PMMRIE_backup;             // Restore PMM interrupt enable register
	PMMCTL0_H = 0x00;                     // Lock PMM registers for write access
	return PMM_STATUS_OK;                     // Return: OK
}

//*****************************************************************************
//
//! ReadWlanInterruptPin
//!
//! @param  none
//!
//! @return none
//!
//! @brief  return wlan interrup pin
//
//*****************************************************************************
long ReadWlanInterruptPin(void) {
	// Return the status of IRQ
	return (SPI_IRQ_IN & SPI_IRQ_PIN);
}

//*****************************************************************************
//
//! WlanInterruptEnable
//!
//! @param  none
//!
//! @return none
//!
//! @brief  Enable waln IrQ pin
//
//*****************************************************************************

void WlanInterruptEnable() {
	SPI_IRQ_IES |= SPI_IRQ_PIN;
	SPI_IRQ_IE |= SPI_IRQ_PIN;
}

//*****************************************************************************
//
//! WlanInterruptDisable
//!
//! @param  none
//!
//! @return none
//!
//! @brief  Disable waln IrQ pin
//
//*****************************************************************************

void WlanInterruptDisable() {
	SPI_IRQ_IE &= ~SPI_IRQ_PIN;
}

//*****************************************************************************
//
//! WriteWlanPin
//!
//! @param  val value to write to wlan pin
//!
//! @return none
//!
//! @brief  write value to wlan pin
//
//*****************************************************************************

void WriteWlanPin(unsigned char trueFalse) {
	if (trueFalse) {
		WLAN_EN_OUT |= WLAN_EN_PIN;                 // RF_EN_PIN high
	} else {
		WLAN_EN_OUT &= ~WLAN_EN_PIN;                // RF_EN_PIN low
	}
}

void LFXT_Start(unsigned int xtdrive) {
	// If the drive setting is not already set to maximum
	// Set it to max for LFXT startup
	if ((UCSCTL6 & XT1DRIVE_3)!= XT1DRIVE_3) {
		UCSCTL6_L |= XT1DRIVE1_L + XT1DRIVE0_L; // Highest drive setting for XT1startup
	}

	while (SFRIFG1 & OFIFG) {   // Check OFIFG fault flag
		UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT1HFOFFG + XT2OFFG); // Clear OSC flaut Flags fault flags
		SFRIFG1 &= ~OFIFG;        // Clear OFIFG fault flag
	}

	UCSCTL6 = (UCSCTL6 & ~(XT1DRIVE_3)) | (xtdrive); // set requested Drive mode
}

void Init_FLL_Settle(unsigned int fsystem, unsigned int ratio) {
	volatile unsigned int x = ratio * 32;

	Init_FLL(fsystem, ratio);

	while (x--) {
		__delay_cycles(30);
	}
}

void Init_FLL(unsigned int fsystem, unsigned int ratio) {
	unsigned int d, dco_div_bits;
	unsigned int mode = 0;

	// Save actual state of FLL loop control, then disable it. This is needed to
	// prevent the FLL from acting as we are making fundamental modifications to
	// the clock setup.
	unsigned int srRegisterState = __get_SR_register() & SCG0;
	__bic_SR_register(SCG0);

	d = ratio;
	dco_div_bits = FLLD__2;        // Have at least a divider of 2

	if (fsystem > 16000) {
		d >>= 1;
		mode = 1;
	} else {
		fsystem <<= 1;               // fsystem = fsystem * 2
	}

	while (d > 512) {
		dco_div_bits = dco_div_bits + FLLD0;  // Set next higher div level
		d >>= 1;
	}

	UCSCTL0 = 0x0000;              // Set DCO to lowest Tap

	UCSCTL2 &= ~(0x03FF);          // Reset FN bits
	UCSCTL2 = dco_div_bits | (d - 1);

	if (fsystem <= 630)            //           fsystem < 0.63MHz
		UCSCTL1 = DCORSEL_0;
	else if (fsystem < 1250)      // 0.63MHz < fsystem < 1.25MHz
		UCSCTL1 = DCORSEL_1;
	else if (fsystem < 2500)      // 1.25MHz < fsystem <  2.5MHz
		UCSCTL1 = DCORSEL_2;
	else if (fsystem < 5000)      // 2.5MHz  < fsystem <    5MHz
		UCSCTL1 = DCORSEL_3;
	else if (fsystem < 10000)     // 5MHz    < fsystem <   10MHz
		UCSCTL1 = DCORSEL_4;
	else if (fsystem < 20000)     // 10MHz   < fsystem <   20MHz
		UCSCTL1 = DCORSEL_5;
	else if (fsystem < 40000)     // 20MHz   < fsystem <   40MHz
		UCSCTL1 = DCORSEL_6;
	else
		UCSCTL1 = DCORSEL_7;

	//while (SFRIFG1 & OFIFG) {                               // Check OFIFG fault flag
	UCSCTL7 &= ~(DCOFFG);                               // Clear OSC flaut Flags
	SFRIFG1 &= ~OFIFG;                                 // Clear OFIFG fault flag
	//}

	if (mode == 1) {                                        // fsystem > 16000
		UCSCTL4 = 0x0233; //ACLK = REF0 = 32768 Hz // MCLK  = DCOCLK (Select DCOCLK)
		UCSCTL5 = DIVS_3; // SMCLK = MCLK/8
	} else {
		UCSCTL4 = 0x0244;  //ACLK = REF0 = 32768 Hz // MCLK = SMCLK = DCOCLK (Select DCODIVCLK)
		UCSCTL5 = DIVS_3; // SMCLK = MCLK/8
	}

	__bis_SR_register(srRegisterState);
	// Restore previous SCG0
}

unsigned int SetVCore(unsigned char level) {
	unsigned int actlevel;
	unsigned int status = 0;

	level &= PMMCOREV_3;                       // Set Mask for Max. level
	actlevel = (PMMCTL0 & PMMCOREV_3);         // Get actual VCore
	// step by step increase or decrease
	while (((level != actlevel) && (status == 0)) || (level < actlevel)) {
		if (level > actlevel) {
			status = SetVCoreUp(++actlevel);
		} else {
			status = SetVCoreDown(--actlevel);
		}
	}

	return status;
}

//Catch interrupt vectors that are not initialized.
#ifdef __CCS__
#pragma vector=WDT_VECTOR, TIMER1_A0_VECTOR, TIMER0_A0_VECTOR, TIMER2_A0_VECTOR, COMP_B_VECTOR, UNMI_VECTOR,DMA_VECTOR, RTC_VECTOR, TIMER0_B0_VECTOR,SYSNMI_VECTOR, USCI_A0_VECTOR, USCI_A1_VECTOR, USCI_B0_VECTOR, USCI_B1_VECTOR, LDO_PWR_VECTOR
__interrupt void Trap_ISR(void) {
	while (1)
		;
}

#endif

