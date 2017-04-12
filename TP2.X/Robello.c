#include <xc.h>
#include "Robello.h"
#include"ConfShield1_3.h"


void mux_display(void)
{static unsigned char contdisp=0;
 unsigned char datoenv;
if(contdisp==0)
         {
            DISP4=0;
            DISP1=1;
            datoenv=UMIL;
        }
        if(contdisp==1)
         {
            DISP1=0;
            DISP2=1;
            datoenv=CENT;
        }

        if(contdisp==2)
         {
            DISP2=0;
            DISP3=1;
            datoenv=DEC;
        }


        if(contdisp==3)
         {
            DISP3=0;
            DISP4=1;
            datoenv=UNI;
        }

        LATA = (LATA & 0xF0)|(datoenv & 0x0F);


        contdisp++;

        //se hace para implementar  un contador limitado de 0 a 3
        contdisp=(contdisp & 3);



}
