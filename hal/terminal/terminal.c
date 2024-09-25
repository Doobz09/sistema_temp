
#include "terminal.h"

#ifdef ESP32
//--------------

/**************************************************************************
* Function: hal_comunicacion_terminal_config
* Preconditions: Ninguna
* Overview: Configura la UART en el ESP32 con los parámetros especificados como velocidad de transmisión, bits de datos, paridad, etc.
* Input: uart_num (número de UART a usar), baudrate (velocidad en baudios), 
         data_bits (número de bits de datos), paridad (habilitada o deshabilitada),
         stop_bits (número de bits de parada), flow_ctrl (control de flujo)
* Output: Ninguno
*****************************************************************************/

void hal_comunicacion_terminal_config(int uart_num, int baudrate, int data_bits, int paridad, int stop_bits, int flow_ctrl){
    uart_config_t uart_config = {
        .baud_rate = baudrate,                  /*9600 de velocidad de trasmision*/
        .data_bits = data_bits,           /*8 bits*/
        .parity = paridad,           /*paridad desabilitada*/
        .stop_bits = stop_bits,           /*un bit para paro */
        .flow_ctrl = flow_ctrl
    };

    uart_param_config(uart_num, &uart_config); /*el puerto de la uart a usar el el 0 le pasamos la estructura anterior */
    uart_set_pin(uart_num, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);  /*configuramos los pines */
    uart_driver_install(uart_num, 1024, 0, 0, NULL, 0);   /*le decimos el tamaño del buffer en eeste caso 1024*/
}

/**************************************************************************
* Function: hal_terminal_send
* Preconditions: La UART debe estar configurada.
* Overview: Envía un mensaje a través de la UART especificada.
* Input: mensaje (cadena de caracteres a enviar)
* Output: Ninguno
*****************************************************************************/
void hal_terminal_send(const char *mensaje){

    uart_write_bytes(UART_NUM_0, mensaje, strlen(mensaje));
}

#endif


#ifdef PIC18
/**************************************************************************
* Function: hal_comunicacion_terminal_config
* Preconditions: Ninguna
* Overview: Inicializa la pantalla LCD para mostrar mensajes como terminal.
* Input: Ninguno
* Output: Ninguno
*****************************************************************************/
void hal_comunicacion_terminal_config(){
   lcd_init();
}

/**************************************************************************
* Function: hal_terminal_send
* Preconditions: La pantalla LCD debe estar inicializada.
* Overview: Envía un mensaje a la pantalla LCD, limpiando la pantalla y posicionando el cursor.
* Input: mensaje (cadena de caracteres a enviar)
* Output: Ninguno
*****************************************************************************/

void hal_terminal_send(char* mensaje){
   lcd_putc("\f");  // Comando para limpiar la pantalla LCD
   lcd_gotoxy(1,1);
   printf(lcd_putc,mensaje);
  
}
#endif
