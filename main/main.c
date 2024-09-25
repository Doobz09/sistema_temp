#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> 
#include "esp_mac.h"
#include "../bsp/bsp.h"
char state[100];

TaskHandle_t adc_task_handle = NULL;
TaskHandle_t adc_2_task_handle = NULL;
TaskHandle_t adc_3_task_handle = NULL;
TaskHandle_t uart_task_handle = NULL;
TaskHandle_t bandera_task_handle = NULL;

int value_adc=0;
int value_adc_2=0;
int value_adc_3=0;
int bandera_control =0;
int last_interrupt_time = 0; // Tiempo de la última interrupción


void adc_task(void* pvParameters);
void adc_2_task(void* pvParameters);
void adc_3_task(void* pvParameters);
void uart_task(void* pvParameters);
void bandera_task(void* pvParameters);

void IRAM_ATTR gpio_isr_handler(void* arg);
void init_isr();


void app_main(void)
{

    adc_init();
    uart_init();
    init_isr();
    xTaskCreate(uart_task, "uart Task", 2048, NULL, 5, &uart_task_handle);
    xTaskCreate(bandera_task, "bandera Task", 2048, NULL, 5, &bandera_task_handle);

    
    while(1){
        vTaskDelay(500/ portTICK_PERIOD_MS);
    }

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

void uart_task(void *params) {
    while (1) {
        if (bandera_control==0){
            sprintf(state, "0\n");
             hal_terminal_send(state);
        }
        else if (bandera_control==11){
            sprintf(state, "11,%d\n",value_adc);
            hal_terminal_send(state);
        }
        else if(bandera_control==12){
            sprintf(state, "12,%d\n",value_adc_2);
            hal_terminal_send(state);

        }
        else if(bandera_control==13){
            sprintf(state, "13,%d\n",value_adc_3);
            hal_terminal_send(state);

        }
        else if(bandera_control==212){
            sprintf(state, "212,%d,%d\n",value_adc,value_adc_2);
            hal_terminal_send(state);

        }
        else if(bandera_control==213){
            sprintf(state, "213,%d,%d\n",value_adc,value_adc_3);
            hal_terminal_send(state);

        }
        else if(bandera_control==223){
            sprintf(state, "223,%d,%d\n",value_adc_2,value_adc_3);
            hal_terminal_send(state);

        }
        else if(bandera_control==3){
            sprintf(state, "3,%d,%d,%d\n",value_adc,value_adc_2,value_adc_3);
            hal_terminal_send(state);

        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms
    }
}


void bandera_task(void *params) {
    while (1) {
        if(gpio_get_level(GPIO_SENSOR_2)==0 && gpio_get_level(GPIO_SENSOR_1)==0 && gpio_get_level(GPIO_SENSOR_3)==1){
                bandera_control=212;
            }
        else if(gpio_get_level(GPIO_SENSOR_2)==1 && gpio_get_level(GPIO_SENSOR_1)==0 && gpio_get_level(GPIO_SENSOR_3)==0){
            bandera_control=213;

        }
        else if(gpio_get_level(GPIO_SENSOR_2)==0 && gpio_get_level(GPIO_SENSOR_1)==1 && gpio_get_level(GPIO_SENSOR_3)==0){
            bandera_control=223;

        }
        else if(gpio_get_level(GPIO_SENSOR_2)==0 && gpio_get_level(GPIO_SENSOR_1)==1 && gpio_get_level(GPIO_SENSOR_3)==1){
            bandera_control=12;

        }
         else if(gpio_get_level(GPIO_SENSOR_2)==1 && gpio_get_level(GPIO_SENSOR_1)==0 && gpio_get_level(GPIO_SENSOR_3)==1){
            bandera_control=11;

        }
        else if(gpio_get_level(GPIO_SENSOR_2)==1 && gpio_get_level(GPIO_SENSOR_1)==1 && gpio_get_level(GPIO_SENSOR_3)==0){
            bandera_control=13;

        }
        else if(gpio_get_level(GPIO_SENSOR_2)==1 && gpio_get_level(GPIO_SENSOR_1)==1 && gpio_get_level(GPIO_SENSOR_3)==1){
            bandera_control=0;

        }
        else if(gpio_get_level(GPIO_SENSOR_2)==0 && gpio_get_level(GPIO_SENSOR_1)==0 && gpio_get_level(GPIO_SENSOR_3)==0){
            bandera_control=3;

        }

        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}


// Función de la tarea que leerá el ADC
void adc_task(void *params) {
    while (1) {
        value_adc = hal_read_adc(ADC1_CHANNEL_4);
        value_adc = value_adc/12;

        //sprintf(state, "Valor ADC: %d\n", value_adc);
        //hal_terminal_send(state);
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms
    }
}

void adc_2_task(void *params){
    while(1){
        value_adc_2 = hal_read_adc(ADC1_CHANNEL_5);
        value_adc_2 = value_adc_2/12;

        //sprintf(state, "Valor ADC: %d\n", value_adc);
        //hal_terminal_send(state);
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }
}
void adc_3_task(void *params){
    while(1){
        value_adc_3 = hal_read_adc(ADC1_CHANNEL_6);
        value_adc_3 = value_adc_3/12;

        //sprintf(state, "Valor ADC: %d\n", value_adc);
        //hal_terminal_send(state);
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }
}

void IRAM_ATTR gpio_isr_handler(void* arg) {

    // Control de debounce
    int current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
    if ((current_time - last_interrupt_time) < DEBOUNCE_TIME_MS) {
        return;  // Ignorar si ocurre dentro del tiempo de debounce
    }
    last_interrupt_time = current_time;  // Actualizar el tiempo de la última interrupción

    // Cada vez que se presiona el botón (flanco descendente), se invoca esta función
    int pin = (int) arg;
    
    if(pin == GPIO_SENSOR_1){
        // Crear tarea si no existe
        if (adc_task_handle == NULL) {
            xTaskCreate(adc_task, "ADC Task", 2048, NULL, 5, &adc_task_handle);
            vTaskResume(adc_task_handle);
        }
        else if(adc_task_handle != NULL){
            vTaskDelete(adc_task_handle);
            adc_task_handle = NULL; 

        }

    }
    if(pin == GPIO_SENSOR_2){
        // Crear tarea si no existe
        if (adc_2_task_handle == NULL) {
            xTaskCreate(adc_2_task, "ADC_2 Task", 2048, NULL, 5, &adc_2_task_handle);
            vTaskResume(adc_2_task_handle); 
            
        }
        else if(adc_2_task_handle != NULL){
            vTaskDelete(adc_2_task_handle);
            adc_2_task_handle = NULL;
        }
    }

    if(pin == GPIO_SENSOR_3){
        // Crear tarea si no existe
        if (adc_3_task_handle == NULL) {
            xTaskCreate(adc_3_task, "ADC_3 Task", 2048, NULL, 5, &adc_3_task_handle);
            vTaskResume(adc_3_task_handle); 
            
        }
        else if(adc_3_task_handle != NULL){
            vTaskDelete(adc_3_task_handle);
            adc_3_task_handle = NULL;
        }
    }
    
}