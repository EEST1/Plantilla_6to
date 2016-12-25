/* 
 * File:   ConfShiel1_3.h
 * Author: esteban
 *
 * Created on 10 de abril de 2016, 10:44
 */
#include <xc.h>

/*Definiciones de hardware*/
#ifndef CONFSHIELD1_3_H
#define	CONFSHIELD1_3_H

#define LED5    LATDbits.LD0
#define LED6    LATDbits.LD1
#define LED1    LATDbits.LD2
#define LED2    LATDbits.LD3
#define LED3    LATCbits.LC6
#define LED4    LATCbits.LC7

#define BOT1    PORTDbits.RD4
#define BOT2    PORTDbits.RD5
#define BOT3    PORTDbits.RD6
#define BOT4    PORTDbits.RD7

#define DISP1   LATAbits.LA4
#define DISP2   LATAbits.LA5
#define DISP3   LATEbits.LATE0
#define DISP4   LATEbits.LATE1
#define DOT     LATEbits.LATE2
#define PULSADORES  (PORTD^0xF0)&0xF0    //Nibble superior del puertoD


/*Funciones de inicialización ************************************************/
/**
 * Función requerida para la correcta inicialización de los puertos para ser
 * utilizados por el Shield1_3.
 */
void PicIni13(void);


/*Funciones Incluidas ********************************************************/


/**
 * Función para enviar un dato a un display 7 segmentos
 * @param NroDisp : nro entre 1 y 4 que indica cual es el display a utilizar
 * @param Dato : dato entre 0 y 9 a ser mostrado
 */
void SendDisp(unsigned char NroDisp,unsigned char Dato);
/**
 * Función para el envío de información multiplexada
 * @param Umil: Unidad de mil DISP1
 * @param Cent: Centenas DISP2
 * @param Dec: Decenas DISP3
 * @param Uni: Unidades DISP4
 * @Requiere tic_mux() en interrupción de timer se aconseja 1ms
 */
void Send4Disp(unsigned char Umil,unsigned char Cent,unsigned char Dec,unsigned char Uni);
// para usar con Send4Disp(a,b,c,d), debe de ser llamada por interrupción
/**
 * Función requerida para el multiplezado de displays
 */
void tic_mux(void);


//Los leds hacer un efecto de desplazamiento ajustando la demora
void Leds13(unsigned int Periodo);
// para usar con Leds13(), debe de ser llamada por interrupción
void tic_led(void);


//Devuelve un el numero de boton presionado entre 1 y 4 0=ninguno
unsigned char Botones13(void);
// para usar con Botones13(), debe de ser llamada por interrupción
void tic_bot(void);

/*****************************************************************************/


/*Variables globales utilizadas por las funciones incluidas ******************/
unsigned char mux_tout,bot_tout;
unsigned int  led_tout;


#define MuxSet 4;               //Tiempo en ms para el multiplexado
#define BotonReleaseSet 40;     //Tiempo en ms para la liberación de botones
#define NoPresiona 0;           //valor nulo para ningun boton presionado


enum{
    BotonLibre,
    BotonEnEspera,
    BotonDetectado,
    BotonEsperaLiberarce,
         
};

#endif	/* CONFSHIEL1_3_H */

