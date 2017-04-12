/* 
 * File:   confbits.h
 * Author: Administrador
 *
 * Created on 19 de febrero de 2014, 19:01
 */

#ifndef CONFBITS_H
#define	CONFBITS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFBITS_H */

    #pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
    #pragma config CPUDIV   = OSC1_PLL2
    #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
    #pragma config FOSC     = HSPLL_HS
    #pragma config FCMEN    = OFF
    #pragma config IESO     = OFF
    #pragma config PWRT     = OFF
    #pragma config BOR      = ON
    #pragma config BORV     = 3
    #pragma config VREGEN   = ON		//USB Voltage Regulator
    #pragma config WDT      = OFF
    #pragma config WDTPS    = 32768
    #pragma config MCLRE    = ON
    #pragma config LPT1OSC  = OFF
    #pragma config PBADEN   = OFF	//NOTE: modifying this value here won't have an effect
                                        //on the application.  See the top of the main() function.
                                        //By default the RB4 I/O pin is used to detect if the
                                        //firmware should enter the bootloader or the main application
                                        //firmware after a reset.  In order to do this, it needs to
                                        //configure RB4 as a digital input, thereby changing it from
                                        //the reset value according to this configuration bit.
    //#pragma config CCP2MX   = ON
    #pragma config STVREN   = ON
    #pragma config LVP      = OFF
    //#pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
    #pragma config XINST    = OFF       // Extended Instruction Set
    #pragma config CP0      = OFF
    #pragma config CP1      = OFF
    //#pragma config CP2      = OFF
    //#pragma config CP3      = OFF
    #pragma config CPB      = OFF
    //#pragma config CPD      = OFF
    #pragma config WRT0     = OFF
    #pragma config WRT1     = OFF
    //#pragma config WRT2     = OFF
    //#pragma config WRT3     = OFF
    #pragma config WRTB     = OFF       // Boot Block Write Protection
    #pragma config WRTC     = OFF
    //#pragma config WRTD     = OFF
    #pragma config EBTR0    = OFF
    #pragma config EBTR1    = OFF
    //#pragma config EBTR2    = OFF
    //#pragma config EBTR3    = OFF
    #pragma config EBTRB    = OFF