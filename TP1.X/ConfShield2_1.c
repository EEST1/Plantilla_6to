#include "ConfShield2_1.h"

void PicIni21(void){
    
    //Se borran todos los puertos
    
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    
    //Por defecto se toman todos los pines como entradas para protegerlos.
    
    TRISA = 0xFF;           //RA0 y RA1 están conectadas a trimers de 10K.
                            //Los mismos puedes ser deconectados por jumpers
    TRISB = 0xFF;           //El puerto B usa 4 pulsadores.
    TRISC = 0xFF;           //RC0 , RC1, RC2 son entradas directas.
                            //RC4 y RC5 se conectan al puerto USB.
                            //RC6 y RC7 se conectan el puerto RS232.
                            //RC3 no está implementada.
    TRISD = 0x0C;           //El nibble superior se conecta al LCD.
                            //el nible inferior tiene dos LEDS RD0,1     
    TRISE = 0x00;           //RE0,1,2 se conectan al LCD RW, RS, E son salidas
    
     
    CMCON = 0x07;           //comparadores de entrada deshabilitados
    ADCON1 = 0x0F;          //Conversores AD deshabilitados la habilitación 
                            //de los periféricos se realiza por el usuario
}

