#include "Tecnica1.h"

//variables de uso interno de la libreria
unsigned int MedioPeriodoSet;
unsigned int  frecuencia = 1000;
unsigned char UpperByte=0xFF;
unsigned char LowerByte=0x00;
unsigned char OffsetPeriodo=32;
unsigned char Multiplicador = 0;
unsigned char MultiplicadorSet =0;
int desplazar_tout=0;


void TIMER0_INI(){
    T0CONbits.TMR0ON = 0;    //apago el timer
    T0CONbits.T08BIT = 1;       //selecciono el timer en 8 bits
    T0CONbits.T0CS = 0;          //el timer cuenta fosc/4
    T0CONbits.PSA = 0;            //habilito el preescaler
    T0CONbits.T0PS0 = 1;        //cargo el preescaler con 256
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;
    TMR0L = 209;                    //el timer contará 47 fosc/4 * 256 = 12032 * 0,0833us
    TMR0H = 0xFF;                 //en total aprox 1.0022ms  casi 1ms
    INTCONbits.TMR0IE = 1;   //Habilita la interrupción de timer 0
}
void TIMER1_INI(){
    T1CONbits.TMR1ON = 0;    //apago el timer
    T1CONbits.TMR1CS = 0;    //el timer cuenta fosc/4
    T1CONbits.nT1SYNC = 1;   //Timer no sincronizado en forma externa
    T1CONbits.T1OSCEN = 0;   //T1OS shut off
    T1CONbits.T1CKPS = 0;     //Preescaler en 1
    T1CONbits.RD16 = 0;         //LEctura en dos pasos de 8 bits
    T1CONbits.T1RUN = 0;      //Reloj interno del micro desde otra fuente
    TMR1L = 0x4F;                 //el timer contará 1200 fosc/4 * 1 = 12032 * 0,0833us
    TMR1H = 0xFB;                //en total aprox 99.96us  casi 100us
    PIR1bits.TMR1IF = 0;        //flag de interrupción borrado
    PIE1bits.TMR1IE = 1;        //interrupción de timer 1 habilitada
    INTCONbits.PEIE = 1;       //Interrupciones de perifericos habilitadas
    
}

void USART_INI (void){
    
    TXSTAbits.BRGH = 1;     // High Baud Rate Select bit
    BAUDCONbits.BRG16=1;
    TXSTAbits.SYNC = 0;     //modo asincrónico
    SPBRG = 0xE1;
    SPBRGH = 0x04;          //=(FOSC/9600)/4-1 donde FOSC=48MHz
    RCSTAbits.SPEN  = 1;    //Habilita el puerto serie
    
    //transmisión
    TXSTAbits.TX9 = 0;      //recepción y transmisión de 8 bits
    TXSTAbits.TXEN = 0;     //Inicialmente el transmisor está desactivado.
   
    //recepción
    (void) RCREG;           //Se vacia el buffer
    (void) RCREG;
    (void) RCREG;
    RCSTAbits.RX9 = 0 ;     //Recepción de 8 bits
    PIE1bits.RCIE = 1;      //habilita la interrupción de recepción
    RCSTAbits.CREN = 1;     //Habilita la recepción
    INTCONbits.PEIE = 1;    //Habilita las interrupciones parcialmente

    TRISCbits.RC6 = 1;      //RC6 y RC7 alta impedancia
    TRISCbits.RC7 = 1;
    BAUDCONbits.RXDTP = 1;  //La información se recive con polaridad invertida
                            //Esto es para poder funcionar con el ID-12LA, en
                            //caso de cambiar el selector de recepción comentar
                            //esta linea.
    

   
  }
void PWM_INI(void){
    PR2=0x00;               //PWM_Perior=[PR2+1]*4*Tosc*(TMR2 Preescaler)
    CCPR1L=0;               //Duty=(CCPR1L:CCP1CON<5:4>)*TOSC*(TMR2 Preescaler)
    CCP1CONbits.DC1B1=0;
    CCP1CONbits.DC1B0=1;
    TRISCbits.RC2=0;        //Salida de PWM RC2 activada
    T2CONbits.TMR2ON=1;     //Timer 2 encendido
    CCP1CONbits.CCP1M3 =1;
    CCP1CONbits.CCP1M2 =1;
}
/*pwm_set(periodo ,duty)  ********************************************************
 *Configura una modulación por ancho de pulso con frecuencia = 1/peiodo y ciclo de 
 * trabajo = Duty *****************************************************************/

void pwm_set(unsigned char periodo, unsigned char duty ){
    
    if(duty>0 && duty!=CCPR1L)CCPR1L=duty;      //si el valor de duty es correcto lo carga en el registro correspondiente
    if(periodo>0 && periodo!=PR2)PR2=periodo;       //si el valor del período es correcto lo carga en el registri correspondiente
    
}

/****************************************************************************
 *unsigned char debounce(unsigned char sample)                              *
 *                                                                          *
 *Recibe la variable sample que contiene 8 bits y devuelve el estado de los *
 *pulsadores                                                                *
 *Es necesario llamar a la función tic_debounce() cada 1ms                  * 
 ***************************************************************************/ 

unsigned char debounce(unsigned char sample){
    static unsigned char state, cnt0, cnt1;         //contador de 4 pasos dado por cnt1 y cnt0
    unsigned char delta;                                 //variable que regista los cambios
    delta = (sample) ^ state;                          // delta = sample(dato recibido) XOR state(estado anterior)
    cnt1 = (cnt1 ^ cnt0) & delta;                    // si delta es 1 incrementa el contador
    cnt0 = ~cnt0 & delta;                               
    state ^= delta & (~(cnt0 | cnt1));              //cuando el contador desborda guarda el nuevo estado
    return state;                                            //devuelve el estado de los pulsadores
}
void informar(const char *dato, char longitud){
    
    char i=0;
    for(i=0;i<longitud;i++) transmitir(*(dato+i));   //transmite el dato
    
}
void transmitir(unsigned char Dato){                //transmite un char
    TXSTAbits.TXEN=1;                                 //habilita el transmisor
   
    TXREG = Dato;                                        //carga el transmisor con el Dato a transmitir
    while(!TXSTAbits.TRMT);                         //espera a que se vacie el registro de transmisión
    TXSTAbits.TXEN=0;                                //apaga el transmisor
    
}
unsigned char obtener_ADC8(void){
    unsigned char   GuardoPorta,GuardoTrisa;
    
    GuardoPorta = PORTA;
    GuardoTrisa = TRISA;
    
    TRISAbits.RA0=1;        //RA0 se transforma en AN0
    ADCON1 = 0x0E;          //selección de entradas analógicas
                            //canal 0 por defecto
    ADCON2 = 0x2D;
    ADCON0bits.ADON=1;
    ADCON0bits.GO=1;        //inicia la conversión
    
    while(ADCON0bits.GO);
    
    ADCON0bits.ADON=0;      //se apaga el conversor
    ADCON1 = 0x0F;
    LATA = GuardoPorta;
    TRISA = GuardoTrisa;
                    
    return ADRESH;
}

void AumentarPeriodo(unsigned char valor){
    if(frecuencia>1){
        frecuencia-=valor;
        CalculoDeFrecuencia();
    }
}
void DisminuirPeriodo(unsigned char valor){
    if(frecuencia<1000){
        frecuencia+=valor;
        CalculoDeFrecuencia();
    }
}
void CalculoDeFrecuencia(void){
    if(frecuencia>100){
        MultiplicadorSet = 0;
        MedioPeriodoSet=(2000000/frecuencia);
        MedioPeriodoSet = (MedioPeriodoSet-OffsetPeriodo)*3;
     }
    /*preescaler *************************************************************/
    else if(frecuencia<=100 && frecuencia>60 ){
        MultiplicadorSet = 0;
        T1CONbits.T1CKPS = 1;   //Preescaler en 2
        MedioPeriodoSet=(1000000/frecuencia);
        MedioPeriodoSet = (MedioPeriodoSet-OffsetPeriodo)*3;
      
    }
    else if(frecuencia<=60 && frecuencia > 30){
        MultiplicadorSet = 0;
        T1CONbits.T1CKPS0 = 0;   //Preescaler en 4
        T1CONbits.T1CKPS1 = 1;   //Preescaler en 4
        MedioPeriodoSet=(500000/frecuencia-OffsetPeriodo)*3;
              
    }
    else if(frecuencia<=30 && frecuencia > 20){
        MultiplicadorSet = 0;
        T1CONbits.T1CKPS0 = 1;   //Preescaler en 8
        T1CONbits.T1CKPS1 = 1;   //Preescaler en 8
        MedioPeriodoSet=(250000/frecuencia-OffsetPeriodo)*3;
               
    }
    else if(frecuencia<=20){
        MultiplicadorSet = 12;
        T1CONbits.T1CKPS0 = 1;   //Preescaler en 8
        T1CONbits.T1CKPS1 = 1;   //Preescaler en 8
        MedioPeriodoSet=(62500/frecuencia - OffsetPeriodo);
   
    }
    MedioPeriodoSet = 65535 - MedioPeriodoSet;
    UpperByte =(unsigned char)(MedioPeriodoSet>>8);
    LowerByte =(unsigned char)MedioPeriodoSet;
}


void tic_debounce(void){
    static unsigned char debounce_tic = DebounceSet;
     if (debounce_tic)debounce_tic--;
    else {
        debounce_tic = DebounceSet;
        EstadoDeLosBotones = debounce(PULSADORES);      
    }
}

void tic_timer1(void){
    
        
    
}
void EEwrite(unsigned char addr,unsigned char n) {    // non uso puntatore, xché va a finire a 16bit! SPRECONI!

    EEADR = (char)addr;
    EEDATA=n;

    EECON1bits.EEPGD=0;        // Point to Data Memory
    EECON1bits.CFGS=0;        // Access EEPROM
    EECON1bits.WREN=1;

    INTCONbits.GIE = 0;            // disattiva interrupt globali
    EECON2=0x55;         // Write 55h
    EECON2=0xAA;         // Write AAh
    EECON1bits.WR=1;                                    // abilita write.
    INTCONbits.GIE = 1;            // attiva interrupt globali
    do {
        ClrWdt();
        } while(EECON1bits.WR);                            // occupato ? 


    EECON1bits.WREN=0;                                // disabilita write.
  }

unsigned char EEread(unsigned char addr) {

    EEADR=(char)addr;            // Address to read
    EECON1bits.EEPGD=0;        // Point to Data Memory
    EECON1bits.CFGS=0;          // Access EEPROM
    EECON1bits.RD=1;            // EE Read
    return EEDATA;                // W = EEDATA
    }

#ifdef CONFSHIELD2_1_H
void demo_shield2_1_INI(void){
    CalculoDeFrecuencia();
    PicIni21();             //inicializa la placa de 6to
    TIMER0_INI();           //Configura el timer0 en 1ms
    TIMER1_INI();
    ei();                   //Hhabilita las interrupciones
    T0CONbits.TMR0ON = 1;   //Enciende el time0
    T1CONbits.TMR1ON = 1;   //apago el timer
    LCD_init();             //Configura el LCD 16x2 en 4 lineas para PORTD
    clear_LCD();            //Borra el LCD
    USART_INI();            //Inicia el puerto serie
    msg2LCD("Hola Alumnos");//Mensaje unico de bienvenida
    
    SAL0_ON;                //habilito la salida por RA2
    SAL1_ON;                //HAbilito la salida por RA3
    
}
void demo_shield2_1(void){
    static unsigned char flag0=0,flag1=0,flag2=0,flag3=0,PrimeraVez=0,Desplazar=0;
    static unsigned char cont=0,cont1,cont2,cont3;
        
    if(EstadoDeLosBotones == 16 && flag0==0){
            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
            ret_HOME();
            msg2LCD("BOT1");
            flag0=1;
            Desplazar = 0;
            AumentarPeriodo(10);
            if(cont<9)cont++;
            else cont =0;
            set_CURSOR(0x40);
            char2LCD(cont+0x30);
            informar("BOT1 ",sizeof("BOT1 "));
            transmitir(cont+0x30);
            transmitir('\n');
            transmitir('\r');
        }
        if(EstadoDeLosBotones == 32 && !flag1){
            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
            ret_HOME();
            msg2LCD("BOT2");
            flag1=1;
            DisminuirPeriodo(10);
            if(cont1<9)cont1++;
            else cont1 =0;   
            set_CURSOR(0x42);
            char2LCD(cont1+0x30);
             
        }
        if(EstadoDeLosBotones == 64 && flag2==0){
            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
            ret_HOME();
            msg2LCD("BOT3");
            flag2=1;
            AumentarPeriodo(100);
            if(cont2<9)cont2++;
            else cont2 =0;
            set_CURSOR(0x44);
            char2LCD(cont2+0x30);
            
        }       
        if(EstadoDeLosBotones == 128 && flag3==0){
            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
            ret_HOME();
            msg2LCD("BOT4");
            flag3=1;
            
            if(cont3<9)cont3++;
            else cont3 =0;
            set_CURSOR(0x46);
            char2LCD(cont3+0x30);
            
            obtener_ADC8();
            set_CURSOR(0x4A);
            char2LCD(ADRESH/100+0x30);
            char2LCD((ADRESH%100)/10+0x30);
            char2LCD((ADRESH%10)+0x30); 
            msg2LCD("   **Frase escondida :-) **");
        }
        if(EstadoDeLosBotones == (16+32) && (!flag0 || !flag1)){
            Desplazar = 1;
            flag0=1;flag1=1;
        }            
        if(!EstadoDeLosBotones && flag3==1)flag3=0;
        if(!EstadoDeLosBotones && flag2==1)flag2=0;
        if(!EstadoDeLosBotones && flag1==1)flag1=0;
        if(!EstadoDeLosBotones && flag0==1)flag0=0;
        /*Funcion escondida *************************************************/
        if(Desplazar && !desplazar_tout){
            DesplazarIzquierda();
            desplazar_tout=400;
            
        }
}
void tic_demo_shield2_1(void){
    if (desplazar_tout)desplazar_tout--;
    tic_debounce();
    tic_LCD();
}
#endif