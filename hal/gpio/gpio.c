#include "gpio.h"

#ifdef ESP32
//GPIOS

/************************************************************************** 
* Function: hal_gpio_mode 
* Preconditions: Ninguna
* Overview: Configura el GPIO especificado como entrada o salida. 
             Primero se restablece el pin antes de asignarle la dirección.
* Input: gpio (número de pin), mode (INPUT o OUTPUT)
* Output: Ninguno
*****************************************************************************/


void hal_gpio_mode(int gpio, int mode){
    if(mode == OUTPUT){
        gpio_reset_pin(gpio);
        gpio_set_direction(gpio,GPIO_MODE_OUTPUT);
    }
    else if(mode == INPUT){
        gpio_reset_pin(gpio);
        gpio_set_direction(gpio,GPIO_MODE_INPUT);
    }
}

/**************************************************************************
* Function: hal_led_set_level
* Preconditions: El pin debe haberse configurado previamente como salida.
* Overview: Establece el nivel lógico (alto o bajo) de un pin GPIO.
* Input: gpio (número de pin), state (1 = alto, 0 = bajo)
* Output: Ninguno
*****************************************************************************/

void hal_led_set_level(int gpio,bool state){
    gpio_set_level(gpio,state);
}

/**************************************************************************
* Function: hal_btn_read
* Preconditions: El pin debe haberse configurado previamente como entrada.
* Overview: Lee el valor lógico del pin GPIO especificado.
* Input: gpio (número de pin)
* Output: Valor leído en el GPIO (1 = alto, 0 = bajo)
*****************************************************************************/


bool hal_btn_read(int gpio){

    return gpio_get_level(gpio);
}
/**************************************************************************
* Function: hal_gpio_set_pullup
* Preconditions: El pin debe haberse configurado previamente como entrada.
* Overview: Configura el GPIO con pull-up
* Input: gpio (número de GPIO)
* Output: Ninguno
*****************************************************************************/
void hal_gpio_set_pullup(int gpio){
     gpio_set_pull_mode(gpio,GPIO_PULLUP_ONLY);
}

#endif
//PIC 18F4550
#ifdef PIC18
/************************************************************************** 
* Function: hal_gpio_mode 
* Preconditions: Ninguna
* Overview: Configura el GPIO especificado como entrada o salida. 
* Input: gpio (número de pin), mode (INPUT o OUTPUT)
* Output: Ninguno
*****************************************************************************/

void hal_gpio_mode(int pin, int mode){

   if(pin>=D0 && pin<=D7){
      set_tris_d(mode);
   }
   if(pin>=B0 && pin<=B7){
   set_tris_b(mode);
   }
   if(pin==BTN1){
      set_tris_b(mode);
   }
   
   
}
/**************************************************************************
* Function: hal_led_set_level
* Preconditions: El pin debe haberse configurado previamente como salida.
* Overview: Establece el nivel lógico (alto o bajo) de un pin GPIO.
* Input: gpio (número de pin), state (1 = alto, 0 = bajo)
* Output: Ninguno
*****************************************************************************/

   
void hal_led_set_level(int puerto,int state){
   if(state==1)
      output_high(puerto);
   else if(state==0)
      output_low(puerto);
}

/**************************************************************************
* Function: hal_btn_read
* Preconditions: El pin debe haberse configurado previamente como entrada.
* Overview: Lee el valor lógico del pin GPIO especificado.
* Input: gpio (número de pin)
* Output: Valor leído en el GPIO (1 = alto, 0 = bajo)
*****************************************************************************/


int hal_btn_read(int btn){
 if(btn==PIN_B0){
  return INPUT_STATE(PIN_B0);
 }

}
#endif