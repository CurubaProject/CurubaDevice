/*****************************************************************************

 *****************************************************************************/

#include "domumapp.h"
#include "CommsManager.h"

int ADC_Result;                //Surrement changer 'a local
int SwitchDimmer;              //Jo n<aime vraiment pas, non mais vraiment vraiment pas, comme non.
int Tab_ADC10[64] = { 0 };   //a isoler
int *ptr = Tab_ADC10;                    //Peut etre local le faire pointer au d/but
comms devices[2] = {0};

/*
//long int sum_value_ADC = 0;
long int sum_analog_value_ADC = 0;
long int mean = 0;
long int max_value_ADC = 0;
long int max_analog_value_ADC = 0;
//long int volt = 0;
float current = 0;
long int power = 0;
*/

extern comms* ReceiveFirst;
extern comms* ReceivePush;
extern comms* ReceivePop;
extern comms* TransmitFirst;
extern comms* TransmitPush;
extern comms* TransmitPop;


/*******************************************************************************

 ******************************************************************************/

//Define fonction
void InitADC10(void)
{
	//ADC10 Init
	ADC10CTL0 |= ADC10SHT_2 + ADC10ON;
	ADC10CTL1 |= ADC10SHP + ADC10DIV_4 + ADC10SSEL_2;
	ADC10CTL2 |= ADC10RES;                            // 10 bits resolution
	ADC10MCTL0 |= ADC10SREF_7;
	ADC10IE |= ADC10IE0;                    		  // Interupt enable end of conversion
	ADC10IFG &= 0x0000;                               // Reset all interupt flag
	ADC10CTL0 |= ADC10ENC;                            // ADC enable conversion
}

void InitTIMER0(void)
{
	//Timer0 Init (pour ADC)
	TA0CTL |= TACLR;
	TA0CTL |= TASSEL_2 + ID_3 + TAIE;
	TA0CCR0 |= 0x00A4;                  // Value the timer count to //ps Jo veut modifier par une variable
	TA0EX0 |= TAIDEX_4;
}

void InitTIMER1(int switchValue)
{
	//Timer1 Init (pour 5 ou 60 sec.)
	TA1CTL |= TACLR;
	TA1CTL |= TASSEL_1 + ID_3 + TAIE;

	if (switchValue == TYPE_DIMMER)
	{
		TA1CCR0 |= 0x0A00;            // Value the timer count to //ps Jo veut modifier par une variable
	}
	else if (switchValue == TYPE_OUTLET)
	{
		TA1CCR0 |= 0x3C00;            // Value the timer count to //ps Jo veut modifier par une variable
	}

	TA1EX0 |= TAIDEX_7;
}

//DOIT etre modifier badly en accordance avec MO
void InitTIMER2(int switchValue)
{
	//Timer2 Init Dimmer
	TA2CTL |= TACLR;

	if (switchValue == TYPE_DIMMER)
	{
		TA2CTL |= TASSEL_2 + ID_3 + TAIE;     // Value the timer count to //ps Jo veut modifier par une variable
	}
	else if (switchValue == TYPE_OUTLET)
	{
		//TA2CTL |= TASSEL_1 + ID_3;          // Value the timer count to //ps Jo veut modifier par une variable
	}

	TA2CCR0 = 0;
	TA2EX0 |= TAIDEX_7;
}

void InitListComms(int TypeModule)
{
	comms d;
	switch (TypeModule)
	{
		case TYPE_DIMMER :
			d.payloadid = PAYLOAD_INFO_RESPONSE;
			d.status = STATUS_INACTIVE;
			d.state = STATE_OFF;
			d.device = DEVICE_1;
			d.type = TYPE_DIMMER;
			d.data = 0;
			d.change = 1;

			devices[0] = d;
			break;
		case TYPE_OUTLET :
			d.payloadid = PAYLOAD_INFO_RESPONSE;
			d.status = STATUS_INACTIVE;
			d.state = STATE_OFF;
			d.device = DEVICE_1;
			d.type = TYPE_OUTLET;
			d.data = 0;
			d.change = 1;

			devices[0] = d;

			d.device = DEVICE_2;

			devices[1] = d;
			break;
		default :
			break;
	}
}

//Autre fichier parce que JO chiale (sont g/n/rique sa l<air)
void TimerStart(int timer_number)
{
	if (timer_number == TIMER_0)
	{
		TA0CTL |= MC_1;
	}
	else if (timer_number == TIMER_1)
	{
		TA1CTL |= MC_1;
	}
	else if (timer_number == TIMER_2)
	{
		TA2CTL |= MC_1;
	}
}

//Autre fichier parce que JO chiale (sont g/n/rique sa l<air)
//Test pour modifier la facon d arretter
void TimerStop(int timer_number)
{
	if (timer_number == TIMER_0)
	{
		TA0CTL &= 0xFFCF;                 // MC_0 //ps Jo veut modifier par une variable
	}
	else if (timer_number == TIMER_1)
	{
		TA1CTL &= 0xFFCF;                 // MC_0 //ps Jo veut modifier par une variable
	}
	else if (timer_number == TIMER_2)
	{
		TA2CTL &= 0xFFCF;                 // MC_0 //ps Jo veut modifier par une variable
	}
}

void ADCRead(int ADC_number)
{
	//to watch
	if ((TA0CTL>>4 & 0x0001) == 1) //(TA0CTL == MC_1)
	{
		//Wait for the end of sampling
		__delay_cycles(500000);
	}

	ADC10CTL0 &= ~ADC10ENC;

	ADC10MCTL0 &= 0xFFF0;                  //TODO CALLISS de JO Remettre les bit s/lection ADC

	ADC10MCTL0 |= (0x0000 & (ADC_number - 1));

	ADC10CTL0 |= ADC10ENC;

	TimerStart(TIMER_0);

	__delay_cycles(500000);
}

int ComputationWattHour(void)
{
	static long int sum_value_ADC = 0;
	static long int sum_analog_value_ADC = 0;
	static long int mean = 0;
	static long int max_value_ADC = 0;
	static long int max_analog_value_ADC = 0;
	static long int volt = 0;
	static float current = 0;
	static long int power = 0;
	static long int temp_max = 0;
	static long int pente = 2;

	int i = 0;
/*
	for (i = 63; i--; )
	{
		sum_value_ADC += Tab_ADC10[i];
	}
	sum_analog_value_ADC = (1000 * sum_value_ADC)>>10;
	mean = sum_analog_value_ADC>>6;
*/
	for (i = 63; i--; )
	{
		//max_value_ADC = max_value_ADC < Tab_ADC10[i] ? Tab_ADC10[i] : max_value_ADC;
		if(max_value_ADC < Tab_ADC10[i])
		{
			temp_max = Tab_ADC10[i];
			if ((temp_max - Tab_ADC10[i-1] <= pente) && (temp_max - Tab_ADC10[i-2] <= 2*pente))
			{
				max_value_ADC = Tab_ADC10[i];
			}
		}
		sum_value_ADC += Tab_ADC10[i];
	}

	max_analog_value_ADC = (1000 * max_value_ADC)>>10;
	sum_analog_value_ADC = (1000 * sum_value_ADC)>>10;
	mean = sum_analog_value_ADC>>6;

	volt = ((max_analog_value_ADC - mean) * 169) / 239;                            //Volt RMS value   sqrt(2) = 1.4142
	current = (float)volt / 37;                                                    //Magic number from IC current sensor data sheet
	power = GRID_VOLTAGE_AMERICA * current;

	if (power < 30)
	{
		power = 0;
	}

	return power;
}

int GetState(int index)
{
	static long int state = 0;
	ADCRead(devices[index-1].device);

	state = ComputationWattHour() > 1 ? STATE_ON : STATE_OFF;

	return state;
}

int ChangeIO(int commande, int device)
{
	static long int state = STATE_OFF;

	state = GetState(device);

	switch (ReadAppSwitch())                          //TODO si possible en variable ReadAppSwitch
	{
		case TYPE_DIMMER :
			CTRL_OUT ^= CTRL_1 + CTRL_2;
			SwitchDimmer = 0x0060 & CTRL_OUT;
			break;

		case TYPE_OUTLET :
			if (ReceivePop->device == DEVICE_1)// && ReceivePop->state == STATE_ON)
			{
				CTRL_OUT ^= CTRL_1;
			}
/*
			else if(ReceivePop->device == DEVICE_1 && ReceivePop->state == STATE_OFF)
			{
				CTRL_OUT &= ~CTRL_1;
			}
*/
			if (ReceivePop->device == DEVICE_2)// && ReceivePop->state == STATE_ON)
			{
				CTRL_OUT ^= CTRL_2;
			}
/*
			else if(ReceivePop->device == DEVICE_2 && ReceivePop->state == STATE_OFF)
			{
				CTRL_OUT &= ~CTRL_2;
			}
*/
			break;
		default :
			break;
	}

	state = GetState(device);

	if (state != commande)
	{
		state = STATE_NOLOAD;
	}

	return state;
}

int IsStateChange(int State, int StateComms)
{
	return ((State == STATE_ON && StateComms == STATE_ON) ||
			(State == STATE_OFF && (StateComms == STATE_OFF || StateComms == STATE_NOLOAD)));
}

void InfoCommsReceive(int TypeModule)
{
	TimerStop(TIMER_1);

		switch (TypeModule)
		{
			case TYPE_DIMMER :
				Push(&TransmitFirst, &TransmitPush, devices[0]);//comms_transmit[0] = devices[0];
//				comms_receive.change = 0;                     //???
				break;
			case TYPE_OUTLET :
				Push(&TransmitFirst, &TransmitPush, devices[0]);//comms_transmit[0] = devices[0];
				Push(&TransmitFirst, &TransmitPush, devices[1]);//comms_transmit[1] = devices[1];
//				comms_receive.change = 0;                     //???
				break;
			default :
				break;

	}

	TimerStart(TIMER_1);
}

void ControlCommsReceive(int TypeModule)
{
		TimerStop(TIMER_1);
		switch (TypeModule)
		{
			case TYPE_DIMMER :
				if (ReceivePop->status == STATUS_ACTIVE)
				{
					//Enable Zero cross for dimmer function
					ZERO_CROSS_IE |= ZERO_CROSS;

					//Timer2 count to 0
					TA2CCR0 = 0;

					//Status of the module
					devices[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
					devices[0].status = STATUS_ACTIVE;
					devices[0].state = ChangeIO(ReceivePop->state,ReceivePop->device);
					devices[0].device = ReceivePop->device;
					devices[0].type = TYPE_DIMMER;
					devices[0].data = ComputationWattHour();
					devices[0].change = 1;                   //TODO define 1

					if (ReceivePop->state == STATE_ON)
					{
						TA2CCR0 = (int) (101 - ReceivePop->data) / 100.0 * 3253;       //TODO define 3253
					}
					Push(&TransmitFirst, &TransmitPush, devices[0]);
				}
				else if (ReceivePop->status == STATUS_INACTIVE)
				{
					//Disable Zero cross for dimmer function
					P1IE &= ~BIT7;
					P1IFG &= 0x0000;

					//Timer2 count to 0
					TA2CCR0 = 0;

					//Status of the module
					devices[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
					devices[0].status = STATUS_INACTIVE;
					devices[0].state = ChangeIO(ReceivePop->state,ReceivePop->device);
					devices[0].device = ReceivePop->device;
					devices[0].type = TYPE_DIMMER;
					devices[0].data = ComputationWattHour();
					devices[0].change = 1;                           //TODO define 1

					Push(&TransmitFirst, &TransmitPush, devices[0]);//comms_transmit[0] = devices[0];
				}

//				ReceivePop->change = 0;                         //TODO define 0


				break;
			case TYPE_OUTLET :
				if (ReceivePop->device == DEVICE_1)
				{
					devices[0].payloadid = PAYLOAD_CONTROL_RESPONSE;
					devices[0].status = ReceivePop->status;
					devices[0].state = ChangeIO(ReceivePop->state,ReceivePop->device);
					devices[0].device = ReceivePop->device;
					devices[0].type = TYPE_OUTLET;
					devices[0].data = ComputationWattHour();
					devices[0].change = 1;

					Push(&TransmitFirst, &TransmitPush, devices[0]);//comms_transmit[0] = devices[0];
				}
				else if (ReceivePop->device == DEVICE_2)
				{
					devices[1].payloadid = PAYLOAD_CONTROL_RESPONSE;
					devices[1].status = ReceivePop->status;
					devices[1].state = ChangeIO(ReceivePop->state,ReceivePop->device);
					devices[1].device = ReceivePop->device;
					devices[1].type = TYPE_OUTLET;
					devices[1].data = ComputationWattHour();
					devices[1].change = 1;

					Push(&TransmitFirst, &TransmitPush, devices[1]);//comms_transmit[1] = devices[1];
				}

//				ReceivePop.change = 0;                     ???

				break;
			default:
				break;
		}
	TimerStart(TIMER_1);
}

void HeartBeat(void)
{
	int state = 0;

	int TypeModule = 0;

	TypeModule = ReadAppSwitch();

	switch (TypeModule)
	{
		case TYPE_DIMMER :
			GetState(devices[0].device);
			if(IsStateChange(state, devices[0].state))
			{
				devices[0].payloadid = 0;
				devices[0].data = ComputationWattHour();
				devices[0].change = 1;
			}
			else
			{
				devices[0].payloadid = 0;
				devices[0].state = (1^state)+1;                             //TODO remove ducktape
				devices[0].data = ComputationWattHour();
				devices[0].change = 1;
			}

			Push(&TransmitFirst, &TransmitPush, devices[0]);//comms_transmit[0] = devices[0];
			break;
		case TYPE_OUTLET :
		{
			if(IsStateChange(state, devices[0].state))
			{
				devices[0].payloadid = 0;
				devices[0].data = ComputationWattHour();
				devices[0].change = 1;
			}
			else
			{
				devices[0].payloadid = 0;
				devices[0].state = (1^state)+1;
				devices[0].data = ComputationWattHour();
				devices[0].change = 1;
			}

			Push(&TransmitFirst, &TransmitPush, devices[0]);//comms_transmit[0] = devices[0];

			if(IsStateChange(state, devices[1].state))
			{
				devices[1].payloadid = 0;
				devices[1].data = ComputationWattHour();
				devices[1].change = 1;
			}
			else
			{
				devices[1].payloadid = 0;
				devices[1].state = (1^state)+1;
				devices[1].data = ComputationWattHour();
				devices[1].change = 1;
			}

			Push(&TransmitFirst, &TransmitPush, devices[1]);//comms_transmit[1] = devices[1];
		}
		break;
		default :
			break;
	}
}

/*********************
 *  UNTERUPT VERTOR  *
 ********************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
	switch (__even_in_range(ADC10IV, 12))  // For the 12 RTFM
	{
		case 0:                            // No interrupt
			break;
		case ADC10OVIFG:                   // conversion result overflow
			break;
		case ADC10TOVIFG:                  // conversion time overflow
			break;
		case ADC10HIIFG:                   // conversion result is higher than treshold
			break;
		case ADC10LOIFG:                   // conversion result is lower than treshold
			break;
		case ADC10INIFG:                   // conversion result is within the treshold
			break;
		case 12:
			*ptr = ADC10MEM0;
			ptr++;

			if (*ptr > Tab_ADC10[63])
			{
				ptr = Tab_ADC10;
				TimerStop(TIMER_0);
			}
			break;
		default:
			break;
	}
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void) {
	switch (__even_in_range(TA0IV, 14))    // For the 14 RTFM
	{
		case 0:                            // No interrupt
			break;
		case TA0IV_TA0CCR1:                // Capture/Compare 1
			break;
		case TA0IV_TA0CCR2:                // Capture/Compare 2
			break;
		case TA0IV_TA0CCR3:                // Capture/Compare 3
			break;
		case TA0IV_TA0CCR4:                // Capture/Compare 4
			break;
		case TA0IV_5:                      // Capture/Compare 5
			break;
		case TA0IV_6:                      // Capture/Compare 6
			break;
		case TA0IV_TA0IFG:                 // Timer overflow
			ADC10CTL0 |= ADC10SC;
			TA0CTL &= ~TAIFG;
			break;
		default:
			break;
	}
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void) {
	switch (__even_in_range(TA1IV, 14))    // For the 14 RTFM
	{
		case 0:                            // No interrupt
			break;
		case TA1IV_TA1CCR1:                // Capture/Compare 1
			break;
		case TA1IV_TA1CCR2:                // Capture/Compare 2
			break;
		case TA1IV_3:                      // Capture/Compare 3
			break;
		case TA1IV_4:                      // Capture/Compare 4
			break;
		case TA1IV_5:                      // Capture/Compare 5
			break;
		case TA1IV_6:                      // Capture/Compare 6
			break;
		case TA1IV_TA1IFG:                 // Timer overflow
			HeartBeat();
			TA1CTL &= ~TAIFG;
			break;
		default:
			break;
	}
}

#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void) {
	switch (__even_in_range(TA2IV, 14))          // For the 14 RTFM
	{
		case 0:                                  // No interrupt
			break;
		case TA2IV_TA2CCR1:                      // Capture/Compare 1
			break;
		case TA2IV_TA2CCR2:                      // Capture/Compare 2
			break;
		case TA2IV_3:                            // Capture/Compare 3
			break;
		case TA2IV_4:                            // Capture/Compare 4
			break;
		case TA2IV_5:                            // Capture/Compare 5
			break;
		case TA2IV_6:                            // Capture/Compare 6
			break;
		case TA2IV_TA2IFG:                       // Timer overflow
			CTRL_OUT |= SwitchDimmer;
			TimerStop(TIMER_2);
			TA2R &= 0x0000;                      // Reset the count
			TA2CTL &= ~TAIFG;
			break;
		default:
			break;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void IntGPIOHandler(void)
{
	switch(__even_in_range(P1IV, P1IV_P1IFG7))
	{
	case P1IV_P1IFG7:
		CTRL_OUT &= ~CTRL_1 & ~CTRL_2;
		TA2CTL |= MC_1;
		P1IFG &= ~ZERO_CROSS;
		break;
	default:
		break;
	}
}

