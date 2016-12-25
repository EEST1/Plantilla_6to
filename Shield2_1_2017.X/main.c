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

/* descomentar aquella placa que usted desea utilizar */
//#define Placa1_3            
#define Placa2_1
#include "Tecnica1.h"       //segun el shield habilita distintas librerias



/*variables globales ********************************************************/


void main(void) {
    
    demo_shield2_1_INI();
    
    while(1){
        demo_shield2_1();
    
    }    
}

void __interrupt myISR(void){
  
    if(TMR0IF){
        INTCONbits.TMR0IF = 0;  //borra el flag de la interrupción de timer
        TMR0L = 209;            //reinicia el timer
        TMR0H = 0xFF;
        
   
        /*Llamada a funciones TIC 1ms ********************/
        tic_demo_shield2_1();
    }   
}