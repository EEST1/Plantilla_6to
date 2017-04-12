#ifndef _teclado_
#define _teclado_

#include <xc.h>
#include "ConfShield1_3.h"


//#define ResASCII		// Si el resultado será binario, comentar esta línea

// Definiciones del hardware según conexión del teclado
// FILAS (deben estar inicializadas como entradas en el MCU)
// Deben de ser lineas con interrupción externa disponible
#define fila1		PORTBbits.RB4     //conectar al PIN2 del teclado
#define fila2		PORTBbits.RB5     //conectar al PIN7 del teclado
#define fila3		PORTBbits.RB6     //conectar al PIN6 del teclado
#define fila4		PORTBbits.RB7     //conectar al PIN4 del teclado
// COLUMNAS (deben estar inicializadas como salidas en el MCU)

#define col1		TRISBbits.RB0     //conectar al PIN3 del teclado

#define col1		TRISBbits.RB0    //conectar al PIN3 del teclado

#define col2		TRISBbits.RB1     //conectar al PIN1 del teclado
#define col3		TRISBbits.RB2     //conectar al PIN5 del teclado

// Define un valor que devuelve la función si no hay tecla
#define NO_TECLA		0xFF

// Define el tiempo en mS de antirrebote y espera por liberación del teclado
#define DLY_DEBOUNCE			30
#define DLY_RELEASE				50

//define valores booleanos para las funciones incorporadas
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/****************************************************************************
 * La siguiente funcion inicializa el puerto y las interrupciones           *
 * para incorporar el teclado al puerto B                                   *
 * debe de llamarse luego de:                                               *
 *      PicIni13() y TIMER0_INI()                                           *
 * IMPORTANTE!! esta función solamente funciona con el Shield 1.3           *  
 *              y no debe de ser tenida en cuenta para otros micros o placas*       
 ****************************************************************************/
void tecladoIni13(void);         

/****************************************************************************
 *  Función que devuelve una tecla presionada o NO_TECLA=0xFF               *
 *  si no hay novedad                                                       *
 *  El dato puede configurarse como binario o como ASCII                    *
 *          definiendo o no ResASCII                                        *
 ****************************************************************************/
unsigned char tecla(void);      
/****************************************************************************
 * Funcion para uso de teclado la misma se incluye en la interrupción       *
 * de teclado.                                                              *
 * La misma pone un estado alto el flag que indica el evento de teclado     *   
 ****************************************************************************/
void marca_tecla(void);			// Usada para indicar que se presionó tecla

/****************************************************************************
 * Función para el tratamiento del rebote se incluye en la interrupción     *
 * del timer , que deberá estar configurado en 1ms un milisegundo.          *
 * La misma decrementa una variable que se encarga de definir los tiempos de*
 * estabilidad y así evitar el rebote.                                      * 
 ****************************************************************************/
 void tic_teclado(void);		// Para producir demora, debe llamarse 
                                // x int de timer cada 1mS

#endif