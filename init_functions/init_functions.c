
#include "init_functions.h"

void gpio_init(){
    //ENTRADAS
    hal_gpio_mode(BTN1,INPUT);
    hal_gpio_set_pullup(BTN1);

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

void init_isr(){
    gpio_config_t pGPIOconfig;
    pGPIOconfig.pin_bit_mask = (1ULL <<GPIO_SENSOR_1);
    pGPIOconfig.mode = GPIO_MODE_DEF_INPUT;
    pGPIOconfig.pull_up_en = GPIO_PULLUP_ENABLE;
    pGPIOconfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pGPIOconfig.intr_type=GPIO_INTR_ANYEDGE;

    gpio_config(&pGPIOconfig);

    // Configurar GPIO_SENSOR_2 para generar otra interrupción
    pGPIOconfig.pin_bit_mask = (1ULL << GPIO_SENSOR_2);
    gpio_config(&pGPIOconfig);  // Reusar la estructura para el segundo pin

    // Configurar GPIO_SENSOR_3 para generar otra interrupción
    pGPIOconfig.pin_bit_mask = (1ULL << GPIO_SENSOR_3);
    gpio_config(&pGPIOconfig);  // Reusar la estructura para el segundo pin


     // Instalar el servicio de interrupciones de GPIO
    gpio_install_isr_service(0);
    // Agregar handler para la interrupción en el pin de entrada
    gpio_isr_handler_add(GPIO_SENSOR_1, gpio_isr_handler, (void*) GPIO_SENSOR_1);
    gpio_isr_handler_add(GPIO_SENSOR_2, gpio_isr_handler, (void*) GPIO_SENSOR_2);
    gpio_isr_handler_add(GPIO_SENSOR_3, gpio_isr_handler, (void*) GPIO_SENSOR_3);

}



