
#include "init_functions.h"

void gpio_init(){
    //ENTRADAS
    hal_gpio_mode(BTN1,INPUT);
    hal_gpio_set_pullup(BTN1);


    hal_gpio_mode(GPIO_SENSOR_1,INPUT);
    hal_gpio_set_pullup(GPIO_SENSOR_1);

    hal_gpio_mode(GPIO_SENSOR_2,INPUT);
    hal_gpio_set_pullup(GPIO_SENSOR_2);

    hal_gpio_mode(GPIO_SENSOR_3,INPUT);
    hal_gpio_set_pullup(GPIO_SENSOR_3);

    //SALIDAS
    hal_gpio_mode(LED4,OUTPUT);
}

void adc_init(){
    hal_adc_init();
    hal_adc_config(ADC1_CHANNEL_4);
    hal_adc_config(ADC1_CHANNEL_5);
     hal_adc_config(ADC1_CHANNEL_6);

}

void uart_init(){
    hal_comunicacion_terminal_config(UART_NUM_0,9600,UART_DATA_8_BITS,UART_PARITY_DISABLE,UART_STOP_BITS_1,UART_HW_FLOWCTRL_DISABLE);

}




