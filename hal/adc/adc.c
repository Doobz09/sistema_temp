
#include "adc.h"

#ifdef ESP32
//funciones adc
/**************************************************************************
* Function: hal_adc_init
* Preconditions: Ninguna
* Overview: Inicializa el ADC1 con una resolución de 12 bits. 
* Input: Ninguno
* Output: Ninguno
*****************************************************************************/
 void hal_adc_init(){
    /*CONFIGURAMOS el ADC USAMOS EL CANAL 4  */
    /*RESOLUCION DE 12 BITS MAXIMO VALOR 4095 ATENUACION 11 PARA TENER UN VALOR DE 3.3 EN VOLTAJE*/
    //adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);/*pinVp*/
    
    adc1_config_width(ADC_WIDTH_BIT_10);


}

/**************************************************************************
* Function: hal_adc_config
* Preconditions: hal_adc_init debe haberse llamado previamente.
* Overview: Configura el canal ADC con una atenuación de 11dB para un rango de voltaje de 0 a 3.3V.
* Input: chanel (canal ADC a configurar)
* Output: Ninguno
*****************************************************************************/
void hal_adc_config(int chanel){
    adc1_config_channel_atten(chanel, ADC_ATTEN_DB_11);/*PIN32*/
}

/**************************************************************************
* Function: hal_read_adc
* Preconditions: El canal ADC debe estar configurado.
* Overview: Lee el valor ADC (sin procesar) de un canal específico.
* Input: channel (canal ADC a leer)
* Output: Valor de lectura ADC  (0-4095 para 12 bits)
*****************************************************************************/

int hal_read_adc(int channel){
    return adc1_get_raw(channel);
}

#endif

#ifdef PIC18
/**************************************************************************
* Function: hal_adc_config
* Preconditions: Ninguna
* Overview: Configura el puerto y el canal ADC con un reloj interno para la conversión.
* Input: canal (canal ADC a configurar), puerto (puerto ADC a usar)
* Output: Ninguno
*****************************************************************************/
void hal_adc_config(int canal,int puerto){
   setup_adc_ports(puerto);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(canal);
}
/**************************************************************************
* Function: hal_read_adc
* Preconditions: hal_adc_config debe haberse llamado previamente.
* Overview: Lee el valor convertido por el ADC en el canal especificado.
* Input: channel (canal ADC a leer)
* Output: Valor de lectura ADC (8 bits en este caso)
*****************************************************************************/
int8 hal_read_adc(int channel){
   set_adc_channel(channel);
   return read_adc();

}

#endif