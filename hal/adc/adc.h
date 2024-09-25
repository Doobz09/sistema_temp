#ifndef _ADC_H_
#define _ADC_H_
#include "../../bsp/bsp.h"

#ifdef ESP32
//funciones adc
 void hal_adc_init();
 void hal_adc_config(int chanel);
 int hal_read_adc(int channel);

#endif


#ifdef PIC18

void hal_adc_config(int canal,int puerto);
int8 hal_read_adc(int channel);

#endif







#endif


