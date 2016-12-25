/****************************************************************************
 * File:   main.c                                                           *
 * Author: Esteban Lemos                                                    *
 * Created on 5 de febrero de 2014, 20:49                                   *
 * En caso de necesitar el proyecto completo ingresar en                    *
 * https://github.com/EEST1/Plantilla_6to                                   *
 ****************************************************************************/

#include <xc.h>
#include "confbits.h"       //no se debe de modificar este archivo para
                            //garantizar el funcionamiento del BOOTLOADER

#include "Tecnica1.h"       //segun el shield habilita distintas librerias



/*variables globales ********************************************************/
void main(void) {
     
    while(1){
       
 
    }    
}

void __interrupt myISR(void){
  
    if(TMR0IF){
        INTCONbits.TMR0IF = 0;  //borra el flag de la interrupción de timer
        TMR0L = 209;            //reinicia el timer
        TMR0H = 0xFF;
 
   
        /*Llamada a funciones TIC 1ms ********************/
        
    }   
}