#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> 
#include "esp_mac.h"
#include "../bsp/bsp.h"
#include <math.h>
char state[100];

#define ACTIVO 0
#define DESACTIVO 1

TaskHandle_t adc_task_handle = NULL;
TaskHandle_t adc_2_task_handle = NULL;
TaskHandle_t adc_3_task_handle = NULL;
TaskHandle_t uart_task_handle = NULL;
TaskHandle_t activar_desactivar_task_handle = NULL;
TaskHandle_t K_means_task_handle = NULL;

int value_adc=0;
int value_adc_2=0;
int value_adc_3=0;
int bandera_control =0;
int count_btn=0;
int indice;

bool sistema_on =false;


void adc_task(void* pvParameters);
void adc_2_task(void* pvParameters);
void adc_3_task(void* pvParameters);
void uart_task(void* pvParameters);
void activar_desactivar_task(void* pvParameters);
void K_means_task(void* pvParameters);



void app_main(void)
{

    adc_init();
    uart_init();
    gpio_init();
    
    while(1){

        if(hal_btn_read(BTN1)==0){
            while(hal_btn_read(BTN1)==0);
            count_btn++;
            if((count_btn & 1)==1){
                hal_led_set_level(LED4,1);
                sistema_on = true;
                xTaskCreate(uart_task, "uart Task", 2048, NULL, 5, &uart_task_handle);
                xTaskCreate(activar_desactivar_task, "activar_desactivar_task", 2048, NULL, 5, &activar_desactivar_task_handle );
                xTaskCreate(K_means_task, "k_means_task", 2048, NULL, 5, &K_means_task_handle );
            }     
            else{
                 hal_led_set_level(LED4,0);
                 
                 
                 sistema_on=false;
                 sprintf(state, "7\n");
                 hal_terminal_send(state);

                 if(adc_task_handle != NULL){
                    vTaskDelete(adc_task_handle);
                    adc_task_handle = NULL; 
                }
                if(adc_2_task_handle != NULL){
                    vTaskDelete(adc_2_task_handle);
                   adc_2_task_handle = NULL; 
                }
                if(adc_3_task_handle != NULL){
                    vTaskDelete(adc_3_task_handle);
                   adc_3_task_handle = NULL; 
                }
                if(uart_task_handle != NULL){
                    vTaskDelete(uart_task_handle);
                   uart_task_handle = NULL; 
                }

                if(activar_desactivar_task_handle != NULL){
                    vTaskDelete(activar_desactivar_task_handle);
                   activar_desactivar_task_handle = NULL; 
                }

                if(K_means_task_handle!= NULL){
                    vTaskDelete(K_means_task_handle);
                   K_means_task_handle = NULL; 
                    
                }
                 

            }
        }

        vTaskDelay(100/ portTICK_PERIOD_MS);
    }

}

void uart_task(void *params) {
    while (sistema_on==true) {
        if (bandera_control==0){
            sprintf(state, "0\n");
             hal_terminal_send(state);
        }
        else if (bandera_control==11){
            sprintf(state, "11,%d,%d\n",value_adc,indice);
            hal_terminal_send(state);
        }
        else if(bandera_control==12){
            sprintf(state, "12,%d,%d\n",value_adc_2,indice);
            hal_terminal_send(state);

        }
        else if(bandera_control==13){
            sprintf(state, "13,%d,%d\n",value_adc_3,indice);
            hal_terminal_send(state);

        }
        else if(bandera_control==212){
            sprintf(state, "212,%d,%d,%d\n",value_adc,value_adc_2,indice);
            hal_terminal_send(state);

        }
        else if(bandera_control==213){
            sprintf(state, "213,%d,%d,%d\n",value_adc,value_adc_3,indice);
            hal_terminal_send(state);

        }
        else if(bandera_control==223){
            sprintf(state, "223,%d,%d,%d\n",value_adc_2,value_adc_3,indice);
            hal_terminal_send(state);

        }
        else if(bandera_control==3){
            sprintf(state, "3,%d,%d,%d,%d\n",value_adc,value_adc_2,value_adc_3,indice);
            hal_terminal_send(state);

        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms
    }
}

void activar_desactivar_task(void *params){

    while(sistema_on==true){

         //SENSOR 1-----------------------
        if(gpio_get_level(GPIO_SENSOR_1)==ACTIVO){
            if (adc_task_handle == NULL) {
            xTaskCreate(adc_task, "ADC Task", 2048, NULL, 5, &adc_task_handle);
            vTaskResume(adc_task_handle);
        }
        }
        else if(gpio_get_level(GPIO_SENSOR_1)==DESACTIVO){
            if(adc_task_handle != NULL){
                vTaskDelete(adc_task_handle);
                adc_task_handle = NULL; 
        }
        }//---------------------------------------
        //SENSOR 2-------------------------------
        if(gpio_get_level(GPIO_SENSOR_2)==ACTIVO){
            if (adc_2_task_handle == NULL) {
            xTaskCreate(adc_2_task, "ADC_2 Task", 2048, NULL, 5, &adc_2_task_handle);
            vTaskResume(adc_2_task_handle);
        }
        }
        else if(gpio_get_level(GPIO_SENSOR_2)==DESACTIVO){
            if(adc_2_task_handle != NULL){
                vTaskDelete(adc_2_task_handle);
                adc_2_task_handle = NULL; 
        }
        }
        //---------------------------------------

        //SENSOR 3-------------------------------
        if(gpio_get_level(GPIO_SENSOR_3)==ACTIVO){
            if (adc_3_task_handle == NULL) {
            xTaskCreate(adc_3_task, "ADC_3 Task", 2048, NULL, 5, &adc_3_task_handle);
            vTaskResume(adc_3_task_handle);
        }
        }
        else if(gpio_get_level(GPIO_SENSOR_3)==DESACTIVO){
            if(adc_3_task_handle != NULL){
                vTaskDelete(adc_3_task_handle);
                adc_3_task_handle = NULL; 
        }
        }

        //---------------------------------------

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

        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }

}


void adc_task(void *params) {
    while (1) {
        value_adc = hal_read_adc(ADC1_CHANNEL_4);
        value_adc = (value_adc/12)-32;
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms
    }
}

void adc_2_task(void *params){
    while(1){
        value_adc_2 = hal_read_adc(ADC1_CHANNEL_5);
        value_adc_2 = (value_adc_2/12)-32;
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }
}
void adc_3_task(void *params){
    while(1){
        value_adc_3 = hal_read_adc(ADC1_CHANNEL_6);
        value_adc_3 = (value_adc_3/12)-32;
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }
}

// Estructura para almacenar un punto
typedef struct {
    double x, y, z,n;  // Coordenadas del punto
} Point;


void K_means_task(void *params){
    Point centroide[3] = {
        {21.96439839,  22.7299336, 21.92407344}, {28.06416244, 27.48584264,  27.42411371}, {30.99310449, 32.35361523, 32.94930469}
    };


    double d1=0;
    double d2=0;
    double d3=0;
    Point a;

    while(sistema_on==true){

        if(bandera_control==3){
            a.x=value_adc;
            a.y=value_adc_2;
            a.z=value_adc_3;
        
            d1 = sqrt(pow(a.x - centroide[0].x, 2) + pow(a.y - centroide[0].y, 2) + pow(a.z - centroide[0].z,2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2) + pow(a.y - centroide[1].y, 2) + pow(a.z - centroide[1].z,2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2) + pow(a.y - centroide[2].y, 2) + pow(a.z - centroide[2].z,2));
        }
        else if(bandera_control==11){
            a.x=value_adc;
            a.n=1;

            d1 = sqrt(pow(a.x - centroide[0].x, 2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2));

        }
        else if(bandera_control==12){
            a.x=value_adc_2;
            a.n=1;

            d1 = sqrt(pow(a.x - centroide[0].x, 2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2));

        }
        else if(bandera_control==13){
            a.x=value_adc_3;
            a.n=1;

            d1 = sqrt(pow(a.x - centroide[0].x, 2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2));

        }
        else if(bandera_control==212){
            a.x=value_adc;
            a.y=value_adc_2;
            
        
            d1 = sqrt(pow(a.x - centroide[0].x, 2) + pow(a.y - centroide[0].y, 2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2) + pow(a.y - centroide[1].y, 2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2) + pow(a.y - centroide[2].y, 2));
        }
        else if(bandera_control==213){
            a.x=value_adc;
            a.y=value_adc_3;
            
        
            d1 = sqrt(pow(a.x - centroide[0].x, 2) + pow(a.y - centroide[0].y, 2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2) + pow(a.y - centroide[1].y, 2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2) + pow(a.y - centroide[2].y, 2));
        }
        else if(bandera_control==223){
            a.x=value_adc_2;
            a.y=value_adc_3;
            
        
            d1 = sqrt(pow(a.x - centroide[0].x, 2) + pow(a.y - centroide[0].y, 2));
            d2 = sqrt(pow(a.x - centroide[1].x, 2) + pow(a.y - centroide[1].y, 2));
            d3 = sqrt(pow(a.x - centroide[2].x, 2) + pow(a.y - centroide[2].y, 2));
        }
        

        // Determinar el menor de los tres
        double menor = d1;
         indice = 0;

        if (d2 < menor) {
            menor = d2;
            indice = 1;
        }
        if (d3 < menor) {
            menor = d3;
            indice = 2;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }
}

