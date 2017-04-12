/* 
 * File:   ConfShield2_1.h
 * Author: esteban
 *
 * Created on 10 de abril de 2016, 10:46
 */
#include <xc.h>
#include "lcd.h"

#ifndef CONFSHIELD2_1_H
#define	CONFSHIELD2_1_H



/*Definiciones de Hardware ***************************************************/

/*Pulsadores *****************************************************************/
#define BOT1    PORTBbits.RB4 
#define BOT2    PORTBbits.RB5
#define BOT3    PORTBbits.RB6
#define BOT4    PORTBbits.RB7
#define BOOT    PORTBbits.RB4
#define PULSADORES  (PORTB^0xF0)&0xF0

/*Display LCD ****************************************************************/
#define DISPLAY LATD

/*Entradasa Analógicas */

#define ANALOGICA_1 PORTAbits.RA0
#define ANALOGICA_2 PORTAbits.RA1

/*Puerto serie USART *********************************************************/

#define TX PORTCbits.TX
#define RX PORTCbits.RX

/*Salidas disponibles ********************************************************/

#define SAL0    PORTAbits.RA2
#define SAL1    PORTAbits.RA3
#define SAL2    PORTAbits.RA4
#define SAL3    PORTAbits.RA5

/*Configuración del triestado de las salidas puesrto A************************/

#define SAL0_ON     TRISAbits.TRISA2 = 0;
#define SAL0_OFF    TRISAbits.TRISA2 = 1;

#define SAL1_ON     TRISAbits.TRISA3 = 0;
#define SAL1_OFF    TRISAbits.TRISA3 = 1;

#define SAL2_ON     TRISAbits.TRISA4 = 0;
#define SAL2_OFF    TRISAbits.TRISA4 = 1;

#define SAL3_ON     TRISAbits.TRISA5 = 0;
#define SAL3_OFF    TRISAbits.TRISA5 = 1;




/*Funciones de Inicialización ************************************************/

void PicIni21(void);



#endif	/* CONFSHIELD2_1_H */

