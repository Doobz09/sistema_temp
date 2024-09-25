#ifndef _GPIO_H_
#define _GPIO_H_
#include "../../bsp/bsp.h"


#ifdef ESP32
void hal_gpio_mode(int gpio, int mode);
void hal_led_set_level(int gpio,bool state);
bool hal_btn_read(int gpio);
void hal_gpio_set_pullup(int gpio);

#endif
#ifdef PIC18
void hal_gpio_mode(int pin, int mode);
void hal_led_set_level(int puerto,int state);
int hal_btn_read(int btn);

#endif






#endif