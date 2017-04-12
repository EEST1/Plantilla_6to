/* 
 * File:   EncoderIncremental.h
 * Author: ESteban
 *
 * Created on 20 de noviembre de 2016, 11:52
 */

#include <xc.h>
#include "ConfShield1_3.h"

#ifndef ENCODERINCREMENTAL_H
#define	ENCODERINCREMENTAL_H

#ifdef	__cplusplus
extern "C" {
#endif
    //Definiciones de Hardware cambiar si es necesario
    #define canalA          PORTBbits.RB0
    #define canalB          PORTBbits.RB1
    #define setEntradaA     TRISBbits.RB0 = 1;
    #define setEntradaB     TRISBbits.RB1 = 1;
    #define encoderToutSet  30
    
   /**
    * Funcion necesaria para inicializar el encoder, en la misma se define el 
    * rango del encoder
    * @param setMAX    necesario para definir la posicion maxima
    * @param setMin    necesario para definir la posicion minima
    */
    void encoderIni(unsigned char setMAX, unsigned char setMin);
    /**
     * Llamando a esta funcion se obtiene un numero entre el valor MAX y el min
     * @return 
     */
    unsigned char getEncoderPos(void);
    /**
     * Llamando a esta funcion se inicializa la posicion del encoder 
     * @param posicion  indica que posicion tiene el encoder fisicamente
     */
    void putEncoderPos(unsigned char posicion);
     /**
      * Agregar a la funcion de interrupcion
      */
    void encoderInterrup(void);
    /**
     * Agregar en la interrupcion del timer0 cada 1ms
     */
    void tic_encoder(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ENCODERINCREMENTAL_H */

