/*
global.h erstellt am 10.07.22 (c) Thomas Kaifer
für das Projekt lim Librarie-Manager enthalten sind
Bounce2.h    und CONTROLLINO.h
*/
#define INPUTMODE INPUT     //
// #define INPUTMODE INPUT_PULLLUP

boolean startsperre = true;
#ifdef CONTROLLINO_MAXI
   byte EGkallibriert = 30;
#endif

#ifdef CONTROLLINO_MEGA
   byte EGkallibriert = 254;
#endif


// Test des Array buttonPin[] nach Fehlermeldungen in L45AAuswerten.h
// ggf. später in L45Controllino.h aufnehmen, da dort alle spezifischen IO-Definitionen abgelegt sind

#ifdef CONTROLLINO_MAXI
byte  buttonPins[] = { CONTROLLINO_A0, CONTROLLINO_A1, CONTROLLINO_A2, CONTROLLINO_A3, CONTROLLINO_A4,
                        CONTROLLINO_A5, CONTROLLINO_A6, CONTROLLINO_A7, CONTROLLINO_A8, CONTROLLINO_A9,
                        CONTROLLINO_IN0, CONTROLLINO_IN1  };
#endif


const int NUMBUTTONS = sizeof(buttonPins);
const unsigned int entprellZeit = 500;                 // 500ms
#ifdef CONTROLLINO_MAXI
const unsigned int entprellZeitI0 = 1500;
const unsigned int entprellZeitI1 = 2500;
#endif

#ifdef CONTROLLINO_MEGA
const unsigned int entprellZeitI16 = 1500;     // 1500 ms = 1,5 s Auswertung eines langen Tasterndrucks für "Sonnenautimatik" an/aus
const unsigned int entprellZeitI17 = 2500;     // 2500 ms = 2,5 s Unterdrückung Störimpulse Lichtsensor Garage z.B. durch Blitz
#endif

unsigned long startZeit[NUMBUTTONS];
unsigned long endeZeit[NUMBUTTONS];