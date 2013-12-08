#ifndef UTIL_H
#define UTIL_H

#define ZONE_AMERICA
#ifdef ZONE_AMERICA
	#define GRID_VOLTAGE 120
#elif ZONE_EUROPE
	#define GRID_VOLTAGE 240
#endif

void TimerStart(int timer_number);
void TimerStop(int timer_number);
void ADCRead(int ADC_number);
int IsStateChange(int State, int StateComms);
int ComputationWattHour(int *Tab_ADC10); //TODO Rename Tab_ADC10

#endif
