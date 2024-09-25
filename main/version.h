#define ESP32


#if !defined(ESP32) && !defined(PIC18)
    #error "Debe definirse la macro ESP32 o PIC18 para compilar correctamente."
#endif