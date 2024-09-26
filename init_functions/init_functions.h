#ifndef _INIT_FUNCTIONS_H_
#define _INIT_FUNCTIONS_H_
#include "../bsp/bsp.h"


void gpio_init();
void adc_init();
void uart_init();
void IRAM_ATTR gpio_isr_handler(void* arg);
void init_isr();


#endif