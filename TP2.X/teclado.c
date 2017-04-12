#include "teclado.h"
#include <xc.h>

// Valores iniciales de variables
#define NO_FILA			0xFF
#define NO_COL			0xFF

// Valores que tomará el estado del teclado
enum{
	KB_LIBRE,           //en espera de que se presione una tecla
	KB_ESPERA,          //un tiempo necesario para evitar rebotes
	KB_DETECTA,         //barribo del puerto en busqueda de la tecla
	KB_LIBERA,          //un tiempo de espera y una condición para iniciar 
                        //nuevamente el ciclo
};

// Variable global que queda guardada en flash. No cambia durante la ejecución

#ifdef ResASCII
//Resultado ASCII, util para usarse con LCDs o con puerto serie
const unsigned char teclas[4][3]={
 {'1','2','3'},{'4','5','6'},{'7','8','9'},{'E','0','C'}
};
#else
//Resultados binarios útiles para cualquier tipo de uso
const unsigned char teclas[4][3]={
 {1,2,3},{4,5,6},{7,8,9},{'*',0,'+'}
};
#endif


unsigned char fila=NO_FILA, col=NO_COL;		// Aquí guardará indice de matriz
volatile unsigned char flag_kb,delay_kb;	// Modificadas por hardware (interrupciones)

void tecladoIni13(void){
    
    LATB = 0;                               //Borrado de latches
    PORTB = 0;                              //borrado del puerto
    TRISB = 0xF0;                           //RB7_4 entradas , RB3_0 salidas
    RBPU = 0;                               //PULLUPs habilitados
    RBIP = 1;                               //Prioridad alta  
    RBIF = 0;                               //Flag de interrupción borrado
    RBIE = 1;                               //interrupción de Puerto B ON
}
unsigned char tecla(void){
	static unsigned char status_KB = KB_LIBRE;
	unsigned char tecla_KB = NO_TECLA;

	/**************************************************************************
     * Cada vez que se llama a la función tecla() se evalúa el estado de la 
     * variable status_KB, la misma irá evolucionando según se vayan cumpliendo
     * las distintas condiciones                                             
     **************************************************************************/
    switch (status_KB){                     

		case KB_LIBRE:
			if (flag_kb){                   //ocurrida la interrupción
				flag_kb = FALSE;            //se inicializan las variables     
				fila = NO_FILA;             //que luego contenbdrán    
				col = NO_COL;               //el valor de la tecla presionada    
				tecla_KB = NO_TECLA;
				delay_kb = DLY_DEBOUNCE;    //se establece un tiempo de esta_
				status_KB = KB_ESPERA;      //bilidad y se evoluciona al
                                            //próximo valor de la máquina de 
                                            //estado                 
			}//En caso contrario se vuelkve a la funcion anterior
			break;
		
		case KB_ESPERA:
            
			if (!delay_kb){                 //simplemente se espera un tiempo
                status_KB = KB_DETECTA;     //para evolucionar al próximo estado                
            }                               //de esta manera se estabiliza
			break;                          //el valor del pulsador
			
		/**********************************************************************
         *Se procede a barrer el puerto para detectar cual es la tecla que    *
         *se ha presionado                                                    *
         **********************************************************************/
        case KB_DETECTA:                    
            
			if (!fila1){        //Primera fila en 0
                fila = 0;
               
            }
			else if (!fila2){   //Segunda fila en 0
                fila = 1;
                
            }
			else if (!fila3){   //Tercera fila en 0
                fila = 2;
               
            }
			else if (!fila4){   //Cuarta fila en 0
                fila = 3;
                
            }
			/******************************************************************
             * Si ninguna fila se ha presionado se presupone que la detección *
             * constituye un falso dato, por lo que se procede a reiniciar    *
             ******************************************************************/
            if (fila == NO_FILA){
                status_KB = KB_LIBRE;                
            }
            /******************************************************************
             * En caso contrario se barren todas las columnas para identificar*
             * la tecla que se ha presionado                                  *                                                                    
             ******************************************************************/    
			else{
				col1 = 1;       //Se fuerza un estado alto en la columna 1
		                LATBbits.LB0=0;
				if (fila1 && fila2 && fila3 && fila4){
					col = 0;    //Si la multiplicación booleana de todas las 
                                //filas es verdadera, significa que se ha 
                                //forzado la entrada de la fila que corresponde 
                                //a la tecla presionada a un estado alto    
				}else{          //se repite el proceso para las columnas restantes
					col2 = 1;
                			LATBbits.LB1=0;
					if (fila1 && fila2 && fila3 && fila4){
						col = 1;
                      
					}else{
						col3 = 1;
			                        LATBbits.LB2=0;
						if (fila1 && fila2 && fila3 && fila4){
							col = 2;
                            
						}
					}
				}
                //Se vuelve a forzar a todas las columna a 0
				col1=0;
				col2=0;
				col3=0;
                
                if (col == NO_COL){         //Si ninguna columna cumplió la 
                    status_KB = KB_LIBRE;   //condición se devuelve el valor
                                            //NO_TECLA
                }
                /**************************************************************
                 * Si por el contrario se reconoce una tecla se devuelve el   *
                 * valor de la misma y se retiene la detección de teclado por *
                 * un tiempo para evitar rebotes en el proceso                *
                 * ************************************************************/
				else{                           
					delay_kb = DLY_RELEASE;
					status_KB = KB_LIBERA;
					tecla_KB = teclas[fila][col];
                    
				}
			}
        break;
			
		case KB_LIBERA:
			
            if ((fila1 && fila2 && fila3 && fila4)&&(!delay_kb)) status_KB = KB_LIBRE;
			break;
			
		default:
            /******************************************************************
            * Si por algún motivo la máquina de estados entro en un estado    *
            *no válido, se define una condición de reingreso a la máquina     *
            *******************************************************************/
			fila = NO_FILA;
			col = NO_COL;
			tecla_KB = NO_TECLA;
			status_KB = KB_LIBRE;
			break;
	}

	return tecla_KB;

}

void marca_tecla(void){
    (void)PORTB;            //Lee el puerto pero no efectua ninguna acción
    asm("NOP");             //Espera un ciclo sin hacer nada
	flag_kb = TRUE;         //Guarda el evento en el flag
    asm("NOP");             //Espera un ciclo sin hacer nada    
}

void tic_teclado(void){
	
    if (delay_kb) delay_kb--;   //Decrementa la variable utilizada para evitar el rebote
}





































