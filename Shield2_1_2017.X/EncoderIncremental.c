

#include "EncoderIncremental.h"

//variables de uso esclusivo de zla libreria
    unsigned char maximoEncoder=0;
    unsigned char minimoEncoder=0;
    unsigned char posicionEncoder=0;
    unsigned char encoderTout=0;
    unsigned char flagCanal=0;

//Funciones para el uso exclusivo de la libreria
void incrementarEncoder(void);
void decrementarEncoder(void);



void encoderIni(unsigned char setMAX, unsigned char setMin){
    maximoEncoder = setMAX;     //define el valor maximo del rango
    minimoEncoder = setMin;     //define el valor minimo del rango
    
    //Se configura la direccion del puerto, vea las definiciones de hardware
    setEntradaA
    setEntradaB
            
            
    INTCON2bits.RBPU = 0;       //en caso de contar con pullUps utilizarlos 
                                //de lo contrario conectarlos en forma externa.
    
    
    
    INTCON2bits.INTEDG0 = 0;    //selecciona el tipo de flanco de la INT0
    INTCON2bits.INTEDG1 = 0;    //selecciona el tipo de flanco de la INT0
    
    INTCONbits.INT0IF = 0;      //borra el flag de la INT0
    INTCON3bits.INT1F = 0;      //borra el flag de la INT1
    
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    
    
}
void putEncoderPos(unsigned char setPosicion){
    posicionEncoder = setPosicion;
}
unsigned char getEncoderPos(void){
    return posicionEncoder;   
}

void encoderInterrup(void){
    if(INT0IE) {       
        if(INT0IF){
            asm("NOP");
            flagCanal=0;
            INT0IF = 0; 
            INT1IF = 0;  
            INT0IE = 0;
            INT1IE = 0;
            LED1=~LED1;
        }
        else if(INT1IF){
            asm("NOP");
            flagCanal=1;
            INT0IF = 0; 
            INT1IF = 0;  
            INT0IE = 0;
            INT1IE = 0;
            LED1=~LED1;
        }
    }          
}
/*******************************************************************************
 * Funciones para la libreria                                                  *
 ******************************************************************************/

void incrementarEncoder(void){
    
    if (posicionEncoder<maximoEncoder) posicionEncoder++;
    else posicionEncoder = minimoEncoder;     
}
  
void decrementarEncoder(void){
    
        if (posicionEncoder>minimoEncoder)posicionEncoder--;
        else posicionEncoder = maximoEncoder;   
}
void tic_encoder(void){
    
    static unsigned char estado=4;
    
    if(encoderTout)encoderTout--;
    
    switch(estado){
        case 0:
            if(canalA==1 && canalB ==1){ 
                estado = 1;
                encoderTout = encoderToutSet;
            }
            break;
        case 1:
            if(!encoderTout){
                if(!flagCanal){
                    decrementarEncoder();                
                }
                 if(flagCanal){
                    incrementarEncoder();
                }
                estado = 2;
            }
            break;
        case 2:
            
                SendDisp(1,posicionEncoder);
                estado = 3;
            
            break;
        case 3:
            INT0IF=0;
            INT1IF=0;
            INT0IE=1;
            INT1IE=1;
            estado =4;
            break;
        case 4:
            if(!INT0IE){
                estado = 0;
            }
            break;
        default:
            estado = 4;
            break;
     }
}

