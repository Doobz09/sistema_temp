#ifndef _BSP_H_
#define _BSP_H_

#include "../main/version.h"
#include <driver/gpio.h>
#include<driver/uart.h>
#include <driver/adc.h>
#include "../hal/gpio/gpio.h"
#include "../hal/adc/adc.h"
#include "../hal/terminal/terminal.h"
#include "../init_functions/init_functions.h"
#include "string.h"


enum LEDS{
    LED1=17, LED2=16, LED3=4, LED4=2,
    LED5=14, LED6=12, LED7=13, LED8=5,
    LEDR=14, LEDG=13, LEDB=12,

};
typedef enum GPIOS{
    GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8, GPIO9,
    GPIO10, GPIO11, GPIO12, GPIO13, GPIO14, GPIO15, GPIO16, GPIO17, GPIO18, GPIO19,
    GPIO20, GPIO21, GPIO22, GPIO23, GPIO24, GPIO25, GPIO26, GPIO27, GPIO28, GPIO29,
    GPIO30, GPIO31, GPIO32, GPIO33, GPIO34, GPIO35, GPIO36, GPIO37, GPIO38, GPIO39,
}GPIOS;

enum BTNS{
    BTN1=18,
    BTN2,
};

typedef enum MODE{
    INPUT,
    OUTPUT,

}MODE;


// Definir UART número y pines
#define TXD_PIN (UART_PIN_NO_CHANGE)
#define RXD_PIN (UART_PIN_NO_CHANGE)

// Tamaños de buffer
#define BUF_SIZE (1024)
extern char mensaje[100];






#endif