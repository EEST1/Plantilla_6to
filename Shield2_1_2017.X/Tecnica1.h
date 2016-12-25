/* 
 * File:   Tecnica1.h
 * Author: esteban
 *
 * Created on 10 de abril de 2016, 11:53
 */
#include <xc.h>
#ifdef Placa1_3
#include "ConfShield1_3.h"
#else
#include "ConfShield2_1.h"
#endif


#ifndef TECNICA1_H
#define	TECNICA1_H

#define DebounceSet 10                   //tiempo en ms para chequear los botones
#ifdef  CONFSHIELD1_3_H
#define PULSADORES  (PORTD^0xF0)&0xF0    //Nibble superior del puertoB
#else
#define PULSADORES  (PORTB^0xF0)&0xF0
#endif

#define GRABAR  1
#define LEER    0



/*Variables globales de las funciones especiales *****************************/
unsigned char EstadoDeLosBotones;

/*Funciones especiales utilizadas para la resoluci�n de problemas comunes ****/

//devuelve un registro completo de 8 bits con 8 botones filtrados
unsigned char debounce(unsigned char sample);


void CalculoDeFrecuencia(void);
void AumentarPeriodo(unsigned char);
void DisminuirPeriodo(unsigned char);

/*Configuraci�n de los m�dulos internos del micro 18F4550 ********************/

void TIMER0_INI(void);
void TIMER1_INI(void);
void PWM_INI(void);
void USART_INI (void);

/*Uso de los modulos internos ************************************************/
void pwm_set(unsigned char periodo, unsigned char duty );
unsigned char obtener_ADC8(void);
void informar(const char *dato, char longitud);
void transmitir(unsigned char Dato);

/*funciones varias*/
#ifdef CONFSHIELD2_1_H
void demo_shield2_1_INI(void);
void demo_shield2_1(void);
#endif


/*sincronismo de diversas funciones                                          */
void tic_debounce(void);
void tic_timer1(void);

#ifdef CONFSHIELD2_1_H
void tic_demo_shield2_1(void);


/*funciones varias*/

void demo_shield2_1_INI(void);
void demo_shield2_1(void);
#endif




#endif	/* TECNICA1_H */
