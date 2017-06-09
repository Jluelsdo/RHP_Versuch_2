#include "simuc.h"
#include "io_treiber.h"

// Die Verwendung der folgenden Zustandsnamen ist verbindlich
typedef enum{Steht,StarteRunter,StarteRauf,Notaus,FahreRauf_MIT_OF,FahreRauf_OHNE_OF,FahreRunter_MIT_OF,FahreRunter_OHNE_OF} state_t;

// Die folgenden Defines muessen sinnvoll genutzt werden
#define BIT_T2		5
#define BIT_T1		4
#define BIT_ESR		3
#define BIT_ESL		2

#define BIT_M_Re	2
#define BIT_M_Li	1
#define BIT_M_An	0


// Hier die Steuerungsfunktion definieren
// ...

void emain(void* arg) 
{
	// Hier alle benoetigten Variablen deklarieren
	// ...
BYTE DigitalValue ;


	INIT_BM_WITH_REGISTER_UI; // Hier unbedingt einen Break-Point setzen !!!

	
	// Hier die Treiberfunktionen aufrufen und testen (Aufgabe 1)
	// ...

//TEST 1

    printf(Init(GlobalBoardHandle,0x9B));
    printf("\n");

    printf(InputByte(GlobalBoardHandle,3,DigitalValue)); //PORT D  auslesen
    printf("\n");
    printf("Wert in DigitalValue: ");
    printf(DigitalValue);
    printf("\n");

	// Ab hier beginnt die Endlosschleife fuer den Automaten (Aufgabe 2)

	while(1) {

		SYNC_SIM; 

		// Hier die Eingabesignale einlesen
		// ...

		

		// Hier die Steuerungsfunktion aufrufen
		// ...




		// Hier die Ausgabesignale ausgeben
		// ...


	} // while(1)..

	
}

  
