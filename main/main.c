#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> 
#include "esp_mac.h"
#include "../bsp/bsp.h"
#include <math.h>
typedef struct {
    double x, y, z;  // Coordenadas del punto
    int label;    // Clase del punto (0 o 1)
} Point;


#include"datos.h"




char state[100];

#define ACTIVO 0
#define DESACTIVO 1

TaskHandle_t adc_task_handle = NULL;
TaskHandle_t adc_2_task_handle = NULL;
TaskHandle_t adc_3_task_handle = NULL;
TaskHandle_t uart_task_handle = NULL;
TaskHandle_t activar_desactivar_task_handle = NULL;
TaskHandle_t knn_task_handle = NULL;

int value_adc=0;
int value_adc_2=0;
int value_adc_3=0;
int bandera_control =0;
int count_btn=0;
int indice =0;

bool sistema_on =false;


void adc_task(void* pvParameters);
void adc_2_task(void* pvParameters);
void adc_3_task(void* pvParameters);
void uart_task(void* pvParameters);
void activar_desactivar_task(void* pvParameters);
void knn_task(void* pvParameters);


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
                xTaskCreate(knn_task, "knn_task", 32768, NULL, 5, &knn_task_handle );
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
                if(knn_task_handle != NULL){
                    vTaskDelete(knn_task_handle);
                   knn_task_handle = NULL; 
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
            //sprintf(state, "3,%d,%d,%d\n",value_adc,value_adc_2,value_adc_3);
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

#define K 3
#define NUM_POINTS 3000 

// Función para calcular la distancia euclidiana entre dos puntos
double euclidean_distance(Point a, Point b, int n) {
    if(n==3)
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)+ pow(a.z - b.z, 2));
    
    else if(n==2)
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    
    else if(n==1)
        return sqrt(pow(a.x - b.x, 2));

    return 0;
        
}

// Función para encontrar los K vecinos más cercanos
void find_k_nearest_neighbors(Point dataset[], Point test_point, int k, int neighbors[],int n) {
    double distances[NUM_POINTS];
    int i, j;

    // Calcular las distancias entre el punto de prueba y todos los puntos en el dataset
    for (i = 0; i < NUM_POINTS; i++) {
        distances[i] = euclidean_distance(dataset[i], test_point,n);
    }

    // Ordenar los puntos por distancia usando una selección simple de k vecinos
    for (i = 0; i < k; i++) {
        double min_distance = 1e9;
        int min_index = -1;
        for (j = 0; j < NUM_POINTS; j++) {
            if (distances[j] < min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }
        neighbors[i] = dataset[min_index].label;  // Almacenar la etiqueta del vecino
        distances[min_index] = 1e9;               // Excluir el punto ya seleccionado
    }
}

int classify(Point dataset[], Point test_point, int k,int n) {
    int neighbors[K];
    int i, label0 = 0, label1 = 0,label2 = 0;

    // Encontrar los k vecinos más cercanos
    find_k_nearest_neighbors(dataset, test_point, k, neighbors,n);

    // Contar las etiquetas de los vecinos
    for (i = 0; i < k; i++) {
        if (neighbors[i] == 1) label0++;
        else if (neighbors[i] == 2) label1++;
        else if (neighbors[i] == 3) label2++;
    }

	    // Devolver la etiqueta mayoritaria
	if (label0 > label1 && label0 > label2) {
	    return 0;
	} else if (label1 > label0 && label1 > label2) {
	    return 1;
	} else {
	    return 2;
	}
}


void knn_task(void *params){
    Point a;
    int n=0;

    while(sistema_on==true){
        if(bandera_control==3){
            a.x = value_adc;
            a.y = value_adc_2;
            a.z = value_adc_3;
            n=3;

            indice = classify(dataset, a, K,n);
        }
        if(bandera_control==11){
            a.x = value_adc;
            n=1;
            indice = classify(dataset, a, K,n);
        }
        if(bandera_control==12){
            a.x = value_adc_2;
            n=1;
            indice = classify(dataset, a, K,n);
        }
        if(bandera_control==13){
            a.x = value_adc_3;
            n=1;
            indice = classify(dataset, a, K,n);
        }
        if(bandera_control==212){
            a.x = value_adc;
            a.y = value_adc_2;
            n=2;
            indice = classify(dataset, a, K,n);
        }
        if(bandera_control==213){
            a.x = value_adc;
            a.y = value_adc_3;
            n=2;
            indice = classify(dataset, a, K,n);
        }
        if(bandera_control==223){
            a.x = value_adc_2;
            a.y = value_adc_3;
            n=2;
            indice = classify(dataset, a, K,n);
        }

        
        vTaskDelay(100 / portTICK_PERIOD_MS); // Espera 100ms

    }
}


