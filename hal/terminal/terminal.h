#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "../../bsp/bsp.h"

#ifdef ESP32

//funciones uart
void hal_comunicacion_terminal_config(int uart_num, int baudrate, int data_bits, int paridad, int stop_bits, int flow_ctrl);
void hal_terminal_send(const char *mensaje);

#endif


#ifdef PIC18

void hal_comunicacion_terminal_config();
void hal_terminal_send(char* mensaje);
#endif


#endif