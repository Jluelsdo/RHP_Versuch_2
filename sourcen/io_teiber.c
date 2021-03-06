﻿#include "simuc.h"
#include "io_treiber.h"

typedef struct BHandle_Data {
	BYTE Board_allocated;	// 1=allocated, 0=free
	BYTE Port_A_Direction;	// 1=Output, 0=Input
	BYTE Port_B_Direction;
	BYTE Port_C_Direction;
	BYTE Port_D_Direction;
} BHandle;


BHandle BoardHandle_Data;
DSCB GlobalBoardHandle = &BoardHandle_Data;


// - INIT - METHODE
BYTE Init(DSCB BoardHandle, unsigned long int Steuerwort) {

    BYTE a,b,c,d;

        if (BoardHandle->Board_allocated==0)//Wenn Board nicht bereits reserviert
        {

            BoardHandle->Board_allocated=1;//Reserviere Boardhardware

            switch(Steuerwort){
                case 0x80: a=1;b=1;c=1;d=1; break;
                case 0x81: a=1;b=1;c=1;d=0; break;
                case 0x88: a=1;b=1;c=0;d=1; break;
                case 0x89: a=1;b=1;c=0;d=0; break;
                case 0x82: a=1;b=0;c=1;d=1; break;
                case 0x83: a=1;b=0;c=1;d=0; break;
                case 0x8A: a=1;b=0;c=0;d=1; break;
                case 0x8B: a=1;b=0;c=0;d=0; break;
                case 0x90: a=0;b=1;c=1;d=1; break;
                case 0x91: a=0;b=1;c=1;d=0; break;
                case 0x98: a=0;b=1;c=0;d=1; break;
                case 0x99: a=0;b=1;c=0;d=0; break;
                case 0x92: a=0;b=0;c=1;d=1; break;
                case 0x93: a=0;b=0;c=1;d=0; break;
                case 0x9A: a=0;b=0;c=0;d=1; break;
                case 0x9B: a=0;b=0;c=0;d=0; break;
            }

            //Schreibrechte gemäß Steuerwort setzen

            if(a==1){io_out16(DIR0,0x00ff);}// PORT B = 0; PORT A = 1
                else{io_out16(DIR0,0x0000);}// PORT B und A = 0
            if(b==1){io_out16(DIR0, (io_in16(DIR0)|0xff00));}// PORT B = 1; PORT A bleibt


            if(c==1){io_out16(DIR1,0x00ff);}// PORT D = 0; PORT C = 1
                else{io_out16(DIR0,0x0000);}// PORT D und C = 0
            if(d==1){io_out16(DIR1, (io_in16(DIR1)|0xff00));}// PORT D = 1; PORT C bleibt



            //Ports als Ein- oder Ausgang definieren

            BoardHandle->Port_A_Direction=a;
            BoardHandle->Port_B_Direction=b;
            BoardHandle->Port_C_Direction=c;
            BoardHandle->Port_D_Direction=d;
            return 0;
        }

        else{return 1;}

}


// - INPUT BYTE - METHODE
BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue) {
    unsigned short int stat=0;

        if(BoardHandle->Board_allocated==0)
        {   //Port A
            if     (Port == 0)
            {
                if (BoardHandle->Port_A_Direction==0) //PORT Direction abfragen
                {
                    *DigitalValue   = io_in16(IN0) & 0xFF;

                }else{stat=1;} //falsches Boardhandle
            }


            //Port B
            else if(Port == 1)
            {
                if(BoardHandle->Port_B_Direction==0)
                {
                    *DigitalValue = io_in16(IN0) >> 8;

                }else{stat=1;} //falsches Boardhandle
            }


            //Port C
            else if(Port == 2)
            {
                if(BoardHandle->Port_C_Direction==0)
                {
                    *DigitalValue = io_in16(IN1)& 0xFF;

                }else{stat=1;} //falsches Boardhandle
            }

            //Port D
            else if(Port == 3)
            {
                if(BoardHandle->Port_D_Direction==0)
                {
                    *DigitalValue = io_in16(IN1) >> 8;

                }else{stat=1;} //falsches Boardhandle
            }


            else{stat=2;} //falscher Port

        }else{stat=1;} //falsches Boardhandle (bereits in Benutzung)


        return stat;
    }




// - OUTPUT BYTE - METHODE

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigitalValue)
{
	
    // Mit Leben fuellen
    unsigned short int maske,temp,stat=0;



    if(BoardHandle->Board_allocated==0)
    {


    // PORT A
        if(Port == 0)
        {
            if(BoardHandle->Port_A_Direction==1)
            {
                temp = io_in16(OUT0) & 0xFF00;   // PORT B bleibt - PORT A = 00000000
                maske = 0xFF & DigitalValue;     // maske wird "00000000DigitalValue"
                io_out16(OUT0,temp | maske);    //Ausgabe auf OUT0  Möglich wäre auch: temp | DigitalValue - ohne Maske

            }else{stat=3;} //PORT FALSCH KONFIGURIERT
        }



    //PORT B
        else if(Port == 1)
        {
            if(BoardHandle->Port_B_Direction==1)
            {
                temp = io_in16(OUT0) & 0xFF;    // PORT B = 00000000 - PORT A bleibt
                maske = 0xFF00 & (DigitalValue<<8); // maske wird "DigitalValue00000000"
                io_out16(OUT0,temp | maske);    //Ausgabe auf OUT0

            }else{stat=3;} //PORT FALSCH KONFIGURIERT
        }



    //PORT C
        else if(Port == 2)
        {
            if(BoardHandle->Port_C_Direction==1)
            {
                temp = io_in16(OUT1) & 0xFF00;   // PORT D bleibt - PORT C = 00000000
                maske = 0xFF & DigitalValue;     // maske wird "00000000DigitalValue"
                io_out16(OUT1,temp | maske);    //Ausgabe auf OUT0  Möglich wäre auch: temp | DigitalValue - ohne Maske

            }else{stat=3;} //PORT FALSCH KONFIGURIERT
        }



    //PORT D
        else if(Port == 3)
        {
            if(BoardHandle->Port_D_Direction==1)
            {
                temp = io_in16(OUT1) & 0xFF;    // PORT D = 00000000 - PORT C bleibt
                maske = 0xFF00 & (DigitalValue<<8); // maske wird "DigitalValue00000000"
                io_out16(OUT1,temp | maske);    //Ausgabe auf OUT0

            }else{stat=3;} //PORT FALSCH KONFIGURIERT
        }


        else{stat=2;} // UNGÜLTIGER PORT

    }else{stat=1;} // UNGÜLTIGES BOARDHANDLE

    return stat;
}





// - FREE - METHODE
BYTE Free(DSCB BoardHandle)
{
    if (BoardHandle->Board_allocated==1)
    {
        BoardHandle->Board_allocated=0;

        return 0;
    }
    else{return 1;}
}

