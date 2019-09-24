#include "lcd.h"


/* esta librería solo puede utilizar las cuatro líneas menos significativas de 
 * un puerto en caso de querer utilizar otras líneas se debe de reprogramar
 * es necesarío este habilitada la interrupción de timer en la cual se debe de 
 * llamar a la función LCD_tic() la misma establece las demoras que de otra
 * manera habría que implementarlas como una función aparte 
 */
#define ENTRADA	0xF0	// Esto es para invertir el bus de datos y poder
#define SALIDA	0x0F    //leer cuando necesito ver si está busy..
#define BUSY	PORTDbits.RD7	// Con estos defines me abstraigo del hardware
#define BUS_DIR TRISD

#define TRUE    0x1
#define FALSE   0x0

unsigned short LCD_tout;								// Contador para decrementar cada 1mS

		// Verifica si terminó la operación en proceso

// Esta rutina recibe un puntero a char e imprime en el LCD ese string

void msg2LCD(const char *datos){

	while (*datos != 0){
		write_DATA(*datos);
		read_BUSY();
		datos++;
	}
}

// Esta rutina recibe un byte e imprime en el LCD
void char2LCD(unsigned char dato){

	write_DATA(dato);
	read_BUSY();
}

// Envía el comando de borrar display (cmd 0x01)
void clear_LCD(void){
	write_CMD(0x01);
	read_BUSY();
}

// Lleva el cursor a la primera posición del primer renglón
void ret_HOME(void){
	write_CMD(0x02);
	read_BUSY();
}

// Pone el cursor donde quiero (0x00 a 0x0F el 1er renglón, 0x40 a 0x4F el 2do)
void set_CURSOR(unsigned char posi){
	posi |= 0x80;
	write_CMD(posi);
	read_BUSY();
}
void DesplazarIzquierda(void){
	write_CMD(24);
	read_BUSY();
}

// Inicialización de display
void LCD_init(void){
    BUS_DIR &= SALIDA;	// Hace el bus salida para escribir en el LCD
    RS_BUS_DIR = 0;
    RW_BUS_DIR = 0;
    E_BUS_DIR = 0;
        

	LCD_tout = 30;          // Espera 30 interrup de timer
	while (LCD_tout);

	write_CMD(0x02);        // trabajar en 4bits 0010****
	LCD_tout = 1;		// Al menos 50uS 
	while (LCD_tout);	// Espera acá hasta que LCD_tout se hace 0

	write_CMD(0x28);	// 4 bits 2 lineas caracter 5*8
	LCD_tout = 1;
	while (LCD_tout);

	write_CMD(0x0C);        //disp ON Cursor OFF Blink OFF
	LCD_tout = 1;
	while (LCD_tout);

	write_CMD(0x01);        // borra el display
	LCD_tout = 1;
	while (LCD_tout);

	write_CMD(0x06);        //Incrementar la pos auto SCROLL OFF
	LCD_tout = 1;
	while (LCD_tout);

	read_BUSY();												// Esta rutina lee el bit de busy a ver si el LCD se liberó..

}
// Pone las señales correspondientes para enviar un dato a mostrar
void write_DATA(unsigned char dato){

	RS = TRUE;
	RW = FALSE;
	write_LCD(dato);	// Envía efectivamente el dato

}

// Pone las señales correspondientes para enviar un comando
void write_CMD(unsigned char dato){

	RS = FALSE;
	RW = FALSE;
	write_LCD(dato);	// Envía efectivamente el comando

}

// Pone un byte en el bus de a un nibble por vez
void write_LCD (unsigned char dato){
		    
    DISPLAY &= 0x0F;
    E = TRUE;
    
    
    DISPLAY |= (dato & 0xF0);		// Pone el nibble alto en el bus
    
    E = FALSE;
    
    DISPLAY &= 0x0F;
    E = TRUE;
    
    
    DISPLAY |= (dato<<4);		// Pone el nibble alto en el bus
    
    E = FALSE;
        
}

// Lee el bit de busy (LCD terminó de procesar byte recibido..)
void read_BUSY(void){
	unsigned char aux;

	BUS_DIR |= ENTRADA;	// Pone el bus como entrada (hay que leer) D7 a D4
	RS = FALSE;         // Pone las señales para indicar que va a leer
	RW = TRUE;
	do{             // Hace
		E = TRUE;	// E=1
		aux = BUSY;	// Lee bit de busy
		E = FALSE;	// E=0
		E = TRUE;	// E=1
		(void)BUSY;	// Lectura dummy para completar el byte
		E = FALSE;	// E=0, completo byte
	}while (aux);		// Mientras busy = 1, cuando busy = 0 (LCD terminó), sale de este do-while
	RW = FALSE;		// Normaliza las señales
	BUS_DIR &= SALIDA;	// Hace el bus salida para seguir escribiendo al LCD
}

void init_user_chars(void) {
        // Carcateres definidos por suario de 5 x 7 
        unsigned char i; //variable de cuenta
        // ** crea un vector de caracteres especiales para las barras  **** 
        const unsigned char user_char[24] = {
            0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, //caracter 0x00 '|  ' una barra
            0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, //caracter 0x01 '|| ' dos barras
            0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x00  //caracter 0x02 '|||' tres barras
        };
        write_CMD(0x41);//envio a la CGRAM posicion 1 pero deberia ser 0 

        for (i = 0; i < 24; i++) //rellena las filas de la cgram con el vector definido arriba
            char2LCD(user_char[i]);
        write_CMD(0x80); //vuelve a DDRAM
        set_CURSOR(0x00);//vuelve a posicion 1ra fila primer caracter
    }
/******************************************************************************
     *    La funcion bargraph debe recibir un entero en numeo de barras
     *  se calcula con el valor del AD (entre 0 y 1023) dividido por 21
     */
    void bargraph(unsigned int percent) {
        //' ** Declaro Variables **    
        int Bars; // Number of full ||| bars to draw.
        int Balance; //   Balance left after all |||s are drawn.
        int i; // counting variable
   
        Bars = percent / 3;    // cuantas barras completas (de |||) escribo?)
        Balance = percent % 3; // se necesitan barras parciales?
        set_CURSOR(0x40);      //   me ubico en la 2da linea primera posicion

        for (i = 0; i < Bars; i++)
             char2LCD(0x02);
        switch (Balance) {
            case 0: char2LCD(' '); //     caso 0: manda un espacio que sirve para borrar el arrastre
                break;
            case 1: char2LCD(0x00);//     caso 1: escribe el caracter de una barra
                break;
            case 2: char2LCD(0x01);//    caso 2: escribe el caracter de 2 barras
                break;
        }
    }



// Sólo decrementa una variable si esta está cargada con algún valor
void tic_LCD(void){
	 if (LCD_tout) LCD_tout--;
     
}