/*
 L45AShutters.h
	als Include-Datei erstellt am 15.01.21
  am 04.07.22 zum Test mit dem Controllino Maxi mit der Preprozessor-Anweisung #ifdef CONTROLLINO_MAXi
  und #ifdef CONTROLLINO_MEGA IO Definitionen geändert

Release 25.02.22, Laufzeiten Aufw�rts gemessen und mit Konstentendefinition course...Time �bernommen

In dieser Datei werden alle Definitionen/Funktionen abgelegt, die f�r die Funktion der
Rolladensteuerung Version 2 unter Verwendung der LIB Shutters V2.1.1. erforderlich sind.
Erste Test in Verbindung mit sketch_jan15b_controllinino_BasicShutters_Index

unten sind die Shutters Definitionen als Kommentar eingef�gt, diese m�ssen im Hauptprogramm stehem
*/

// test automatisches backup
// The calibration ratio. If the full course is 30 sec. and the ratio is 0.1, the calibration time will be 30 * 0.1 = 3 sec. Defaults to 0.1
const float calibrationRatio = 0.1;

// byte flagsEGcalibrate = 0;
byte flagsOGcalibrate = 0;

// mit Ratio 0.2 kann folgender Wert max. auf 32 eigestellt werden, mit 33 bleibt der Ausgang aud der void Zentral.begin()
// dauerhaft gesetzt
const unsigned long courseZentralTime = 32 * 1000;

// hier kann die Hardwarezuordnung der Ausg�nge / Relais umdefiniert werden
enum ZentralRelaisExtern {  ZentralAuf = CONTROLLINO_D0 , ZentralAb  = CONTROLLINO_D1 };
// enum ZentralRelaisIntern {  ZentralAufIntern = CONTROLLINO_R0 , ZentralAbIntern  = CONTROLLINO_R1 };

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  EG  +++++++  Erdgeschoss
const unsigned long courseGWcTime = 14 * 1000;
const unsigned long courseKuecheTime = 17 * 1000;
const unsigned long courseEssenTime = 30 * 1000;
const unsigned long courseTerasseTime = 31 * 1000;
const unsigned long courseErker1Time = 26 * 1000;
const unsigned long courseErker2Time = 27 * 1000;
const unsigned long courseErker3Time = 28 * 1000;

enum EGRolladenNamen { dummiEG, nameGWC, nameKueche, nameEssen, nameTerasse , nameErker1, nameErker2, nameErker3 };
enum OGRolladenNamen { nameBad, nameSchlaf, nameArbSued, nameArbWest };    // 15.04.21 dummiOG entfernt

// hier kann die Hardwarezuordnung der Ausg�nge / Relais umdefiniert werden
#ifdef CONTROLLINO_MAXI
enum EGRolladen {  GWcAuf = CONTROLLINO_D2 , GWcAb = CONTROLLINO_D3,
               KuecheAuf  = CONTROLLINO_D4 , KuecheAb  = CONTROLLINO_D5,
			   	EssenAuf  = CONTROLLINO_D6 , EssenAb   = CONTROLLINO_D7,
			  TerasseAuf  = CONTROLLINO_D8 , TerasseAb = CONTROLLINO_D9 };
#endif

#ifdef CONTROLLINO_MEGA

enum EGRolladen {  GWcAuf = CONTROLLINO_D2 , GWcAb = CONTROLLINO_D3,
               KuecheAuf  = CONTROLLINO_D4 , KuecheAb  = CONTROLLINO_D5,
			   	EssenAuf  = CONTROLLINO_D6 , EssenAb   = CONTROLLINO_D7,
			  TerasseAuf  = CONTROLLINO_D8 , TerasseAb = CONTROLLINO_D9,
               Erker1Auf  = CONTROLLINO_D10 , Erker1Ab = CONTROLLINO_D11,
			   Erker2Auf  = CONTROLLINO_D12 , Erker2Ab = CONTROLLINO_D13,
			   Erker3Auf  = CONTROLLINO_D14 , Erker3Ab = CONTROLLINO_D15  };


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  OG  +++++++  Obergeschoss
#endif


const unsigned long courseBadTime = 19 * 1000;
const unsigned long courseSchlafTime = 30 * 1000;
const unsigned long courseArbSuedTime = 30 * 1000;
const unsigned long courseArbWestTime = 18 * 1000;

// hier kann die Hardwarezuordnung der Ausg�nge / Relais umdefiniert werden
#ifdef CONTROLLINO_MAXI
enum OGRolladen {  BadAuf = CONTROLLINO_R6 , BadAb = CONTROLLINO_R7,
				SchlafAuf = CONTROLLINO_R0 , SchlafAb = CONTROLLINO_R1 ,
			 ArbSuedAuf = CONTROLLINO_R2 , ArbSuedAb = CONTROLLINO_R3 ,
			 ArbWestAuf = CONTROLLINO_R4 , ArbWestAb = CONTROLLINO_R5 };
#endif

#ifdef CONTROLLINO_MEGA
enum OGRolladen {  BadAuf = CONTROLLINO_R6 , BadAb = CONTROLLINO_R7,
				SchlafAuf = CONTROLLINO_R8 , SchlafAb = CONTROLLINO_R9 ,
			 ArbSuedAuf = CONTROLLINO_R10 , ArbSuedAb = CONTROLLINO_R11 ,
			 ArbWestAuf = CONTROLLINO_R12 , ArbWestAb = CONTROLLINO_R13 };

#endif
// in den Bits [0..7] wird f�r die Rollade mit der Nummer [0..7] der Wert 1 gespeichert, wenn die Rollade aktiv ist
// bitSet(rolladeEGantiv, rolladenNummer) in flagsrollade[nummerRolladeEG]
// bitClear(rolladeEGantiv, rolladenNummer) in shutters...SetState(state)
byte rolladeEGaktiv;
byte rolladeOGaktiv;

uint8_t rolladeEGlevel[8];    // Level der Rollade mit der Nummer [0..7] wird kontinuierlich gespeichert, Nummer 8 zbV
uint8_t rolladeOGlevel[8];    // und steht f�r Funktionen gem�� Verifizierung vom 05/06.03.22 bereit

/*
// void setupRolladen();    in L45AShutters.h mit den folgenden *.beginn() ergibt Kompilerfehler
*/
byte rolladenEGkalibriert (byte bitnummer, uint8_t level)  {
  static byte flagsEGcalibrate = 0;
  //  im EG gibt es nur 7 Rolladen, entsprechend binummer = [ 1 .. 7 ]
  //  bitnummer = 0 wird verwendet, um nur den Wert von flagsEGcalibrate zur�ck zu geben
  if (bitnummer != 0 ) {
    if (bitRead(flagsEGcalibrate, bitnummer) == false) {
      if (level == 0) {
        bitSet(flagsEGcalibrate, bitnummer);
      }  // Ende if (level == 0)
    }  // Ende if (bitRead(flags ...
  }  //  Ende if (bitnummer != 0 )
  return flagsEGcalibrate;
}  // Ende byte rolladenEGkalibriert


byte rolladenOGkalibriert(byte bitnummer, uint8_t level)  {
  static byte flagsOGcalibrate = 0;
  //  im EG gibt es nur 4 Rolladen, entsprechend binummer = [ 1 .. 4 ]
  //  bitnummer = 0 wird verwendet, um nur den Wert von flagsOGcalibrate zur�ck zu geben
  if (bitnummer != 0 ) {
  	if (bitRead(flagsOGcalibrate, bitnummer) == false) {
      if (level == 0) {
      	bitSet(flagsOGcalibrate, bitnummer);
      }  // Ende if (level == 0)
    }  // Ende if (bitRead(flags ...
  }  //  Ende if (bitnummer != 0 )
  return flagsOGcalibrate;
}  // Ende byte rolladenOGkalibriert


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Zentral
// Funktionsaufrufe f�r die zentralen Relais f�r alle Rolladen �ber die Ausg�nge A0-A1

// Funktionsaufrufe f�r die Rolladenkommandos �ber Relais Zentral Auf/Ab
void shuttersZentralUp() {
  DEBUG_PRINTLN("Shutters Zentral aufw�rts.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite(ZentralAuf, HIGH );
}

void shuttersZentralDown() {
  DEBUG_PRINTLN("Shutters Zentral abw�rts.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( ZentralAb, HIGH );
}

void shuttersZentralHalt() {
  Serial.println("Shutters Zentral Stop");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( ZentralAuf, LOW );
  digitalWrite( ZentralAb, LOW);
}

uint8_t shuttersZentralGetState() {
  return 255;
}

void shuttersZentralSetState(uint8_t state) {
  Serial.print("Saving state Zentral: ");
  Serial.print(state);
  Serial.println(".");
}

void onShuttersZentralLevelReached(uint8_t level) {
  // stateZentral = level;			// Zustand sichern f�r kurze Rolladenlaufzeit
  Serial.print("Shutters Zentral at ");
  Serial.print(level);
  Serial.println("%");
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  EG  +++++++  Erdgeschoss
// Funktionsaufrufe f�r die Rollade im G�steWC
void shuttersGWcUp() {
  DEBUG_PRINTLN("Shutters GWc going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( GWcAuf, HIGH );
}

void shuttersGWcDown() {
  DEBUG_PRINTLN("Shutters GWc going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( GWcAb, HIGH );
}

void shuttersGWcHalt() {
  DEBUG_PRINTLN("Shutters GWc halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( GWcAuf, LOW );
  digitalWrite( GWcAb, LOW);
}

uint8_t shuttersGWcGetState() {
  return 255;
}

void shuttersGWcSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameGWC);  }
  else  {
     bitClear(rolladeEGaktiv, nameGWC);
  }
  DEBUG_PRINT_VALUE("Saving state GWc ", state);
  DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersGWcLevelReached(uint8_t level) {

// DEBUG_PRINT_VALUE("Shutters GWc at ", level);
// DEBUG_PRINTLN_VALUE(" % und rolladeEGaktiv =  ", rolladeEGaktiv);

// Funktion getestet am 26.02.22
// am 05.03.22 Abh�ngigkeit als Fehler gesucht,
// die Zuweisung mu� immer erfolgen, das sonst sperre nie zur�ckgesetzt wird
//  im EG gibt es nur 7 Rolladen, entsprechend binummer = [ 0 .. 6 ], somit GWc mit 0
byte temp = rolladenEGkalibriert(nameGWC, level);
DEBUG_PRINTLN_VALUE("> Rollade GWc kalibriert nach Start  ", temp);
// Level der Rollade mit der Nummer [0..7] wird kontinuierlich gespeichert,
rolladeEGlevel[nameGWC] = level;				// erg�nzt am 06.03.22
}

// Funktionsaufrufe f�r die Rollade in der K�che
void shuttersKuecheUp() {
  DEBUG_PRINTLN("Shutters Kueche going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( KuecheAuf, HIGH );
}

void shuttersKuecheDown() {
  DEBUG_PRINTLN("Shutters Kueche going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( KuecheAb, HIGH );
}

void shuttersKuecheHalt() {
  DEBUG_PRINTLN("Shutters Kueche halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( KuecheAuf, LOW );
  digitalWrite( KuecheAb, LOW);
}

uint8_t shuttersKuecheGetState() {
  return 255;
}

void shuttersKuecheSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameKueche);  }
  else  {
     bitClear(rolladeEGaktiv, nameKueche);
  }
 DEBUG_PRINT_VALUE("Saving state Kueche ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersKuecheLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Kueche at ", level);
 DEBUG_PRINTLN(" %");

// Funktion getestet am 26.02.22
byte temp = rolladenEGkalibriert(nameKueche, level);
DEBUG_PRINTLN_VALUE("> Rollade Kueche kalibriert nach Start  ", temp);
// Level der Rollade mit der Nummer [0..7] wird kontinuierlich gespeichert,
rolladeEGlevel[nameKueche] = level;				// erg�nzt am 06.03.22
}



// Funktionsaufrufe f�r die Rollade im Esszimmer
void shuttersEssenUp() {
  DEBUG_PRINTLN("Shutters Essen going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( EssenAuf, HIGH );
}

void shuttersEssenDown() {
  DEBUG_PRINTLN("Shutters Essen going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( EssenAb, HIGH );
}

void shuttersEssenHalt() {
  DEBUG_PRINTLN("Shutters Essen halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( EssenAuf, LOW );
  digitalWrite( EssenAb, LOW);
}

uint8_t shuttersEssenGetState() {
  return 255;
}

void shuttersEssenSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameEssen);  }
  else  {
     bitClear(rolladeEGaktiv, nameEssen);
  }
 DEBUG_PRINT_VALUE("Saving state Essen ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersEssenLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Essen at ", level);
 DEBUG_PRINTLN(" %");
 // Funktion getestet am 26.02.22
 byte temp = rolladenEGkalibriert(nameEssen, level);
 DEBUG_PRINTLN_VALUE("> Rollade Essen kalibriert nach Start  ", temp);
 // Level der Rollade mit der Nummer [0..7] wird kontinuierlich gespeichert,
 rolladeEGlevel[nameEssen] = level;				// erg�nzt am 06.03.22
}

// Funktionsaufrufe f�r die Rollade Terasse
void shuttersTerasseUp() {
  DEBUG_PRINTLN("Shutters Terasse going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( TerasseAuf, HIGH );
}

void shuttersTerasseDown() {
  DEBUG_PRINTLN("Shutters Terasse going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( TerasseAb, HIGH );
}

void shuttersTerasseHalt() {
  DEBUG_PRINTLN("Shutters Terasse halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( TerasseAuf, LOW );
  digitalWrite( TerasseAb, LOW);
}

uint8_t shuttersTerasseGetState() {
  return 255;
}

void shuttersTerasseSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameTerasse);  }
  else  {
     bitClear(rolladeEGaktiv, nameTerasse);
  }
 DEBUG_PRINT_VALUE("Saving state Terasse ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersTerasseLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Terasse at ", level);
 DEBUG_PRINTLN(" %");
 // Funktion getestet am 26.02.22
 byte temp = rolladenEGkalibriert(nameTerasse, level);
 DEBUG_PRINTLN_VALUE("> Rollade Terasse kalibriert nach Start  ", temp);
 rolladeEGlevel[nameTerasse] = level;				// erg�nzt am 06.03.22
}

#ifdef CONTROLLINO_MEGA
// Funktionsaufrufe f�r die Rollade Erker 1
void shuttersErker1Up() {
  DEBUG_PRINTLN("Shutters Erker1 going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( Erker1Auf, HIGH );
}

void shuttersErker1Down() {
  DEBUG_PRINTLN("Shutters Erker1 going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( Erker1Ab, HIGH );
}

void shuttersErker1Halt() {
  DEBUG_PRINTLN("Shutters Erker1 halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( Erker1Auf, LOW );
  digitalWrite( Erker1Ab, LOW);
}

uint8_t shuttersErker1GetState() {
  return 255;
}

void shuttersErker1SetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameErker1);  }
  else  {
     bitClear(rolladeEGaktiv, nameErker1);
  }
 DEBUG_PRINT_VALUE("Saving state Erker1 ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersErker1LevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Erker1 at ", level);
 DEBUG_PRINTLN(" %");
 // Funktion getestet am 26.02.22
 byte temp = rolladenEGkalibriert(nameErker1, level);
 DEBUG_PRINTLN_VALUE("> Rollade Erker 1 kalibriert nach Start  ", temp);
 rolladeEGlevel[nameErker1] = level;				// erg�nzt am 06.03.22
}

// Funktionsaufrufe f�r die Rollade Erker 2
void shuttersErker2Up() {
  DEBUG_PRINTLN("Shutters Erker2 going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( Erker2Auf, HIGH );
}

void shuttersErker2Down() {
  DEBUG_PRINTLN("Shutters Erker2 going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( Erker2Ab, HIGH );
}

void shuttersErker2Halt() {
  DEBUG_PRINTLN("Shutters Erker2 halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( Erker2Auf, LOW );
  digitalWrite( Erker2Ab, LOW);
}

uint8_t shuttersErker2GetState() {
  return 255;
}

void shuttersErker2SetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameErker2);  }
  else  {
     bitClear(rolladeEGaktiv, nameErker2);
  }
 DEBUG_PRINT_VALUE("Saving state Erker2 ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersErker2LevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Erker2 at ", level);
 DEBUG_PRINTLN(" %");
 // Funktion getestet am 26.02.22
 byte temp = rolladenEGkalibriert(nameErker2, level);
 DEBUG_PRINTLN_VALUE("> Rollade Erker 2 kalibriert nach Start  ", temp);
 rolladeEGlevel[nameErker2] = level;				// erg�nzt am 06.03.22
}

// Funktionsaufrufe f�r die Rollade Erker 3

void shuttersErker3Up() {
  DEBUG_PRINTLN("Shutters Erker3 going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( Erker3Auf, HIGH );
}

void shuttersErker3Down() {
  DEBUG_PRINTLN("Shutters Erker3 going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( Erker3Ab, HIGH );
}

void shuttersErker3Halt() {
  DEBUG_PRINTLN("Shutters Erker3 halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( Erker3Auf, LOW );
  digitalWrite( Erker3Ab, LOW);
}

uint8_t shuttersErker3GetState() {
  return 255;
}

void shuttersErker3SetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeEGaktiv, nameErker3);  }
  else  {
     bitClear(rolladeEGaktiv, nameErker3);
  }
 DEBUG_PRINT_VALUE("Saving state Erker3 ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv = ", rolladeEGaktiv);
}

void onShuttersErker3LevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Erker3 at ", level);
 DEBUG_PRINTLN(" %");
 // Funktion getestet am 26.02.22
 byte temp = rolladenEGkalibriert(nameErker3, level);
 DEBUG_PRINTLN_VALUE("> Rollade Erker 3 kalibriert nach Start  ", temp);
 rolladeEGlevel[nameErker3] = level;				// erg�nzt am 06.03.22
}
#endif
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  OG  +++++++  Obergeschoss
// Funktionsaufrufe f�r die Rollade im Bad

void shuttersBadUp() {
  DEBUG_PRINTLN("Shutters Bad going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( BadAuf, HIGH );
}

void shuttersBadDown() {
  DEBUG_PRINTLN("Shutters Bad going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( BadAb, HIGH );
}

void shuttersBadHalt() {
  DEBUG_PRINTLN("Shutters Bad halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( BadAuf, LOW );
  digitalWrite( BadAb, LOW);
}

uint8_t shuttersBadGetState() {
  return 255;
}

void shuttersBadSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeOGaktiv, nameBad);  }
  else  {
     bitClear(rolladeOGaktiv, nameBad);
  }
 DEBUG_PRINT_VALUE("Saving state Bad ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeOGaktiv = ", rolladeOGaktiv);
}

void onShuttersBadLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Bad at ", level);
 DEBUG_PRINTLN(" %");
 // Funktion getestet am 26.02.22
 byte temp = rolladenOGkalibriert(nameBad, level);
 DEBUG_PRINTLN_VALUE("> Rollade Bad kalibriert nach Start  ", temp);
 rolladeOGlevel[nameBad] = level;				// erg�nzt am 13.03.22 Fehlerkorrektur
}

// Funktionsaufrufe f�r die Rollade im Schlafzimmer

void shuttersSchlafUp() {
  DEBUG_PRINTLN("Shutters Schlaf going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( SchlafAuf, HIGH );
}

void shuttersSchlafDown() {
  DEBUG_PRINTLN("Shutters Schlaf going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( SchlafAb, HIGH );
}

void shuttersSchlafHalt() {
  DEBUG_PRINTLN("Shutters Schlaf halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( SchlafAuf, LOW );
  digitalWrite( SchlafAb, LOW);
}

uint8_t shuttersSchlafGetState() {
  return 255;
}

void shuttersSchlafSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeOGaktiv, nameSchlaf);  }
  else  {
     bitClear(rolladeOGaktiv, nameSchlaf);
  }
 DEBUG_PRINT_VALUE("Saving state Schlaf ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeOGaktiv = ", rolladeOGaktiv);
}

void onShuttersSchlafLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters Schlaf at ", level);
 DEBUG_PRINTLN(" %");
 byte temp = rolladenOGkalibriert(nameSchlaf, level);
 DEBUG_PRINTLN_VALUE("> Rollade Schlafzimmer kalibriert nach Start  ", temp);
 rolladeOGlevel[nameSchlaf] = level;				// erg�nzt am 13.03.22 Fehlerkorrektur
}

// Funktionsaufrufe f�r die Rollade im Arbeitszimmer S�d, gro�e Rollade

void shuttersArbSuedUp() {
  DEBUG_PRINTLN("Shutters ArbSued going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( ArbSuedAuf, HIGH );
}

void shuttersArbSuedDown() {
  DEBUG_PRINTLN("Shutters ArbSued going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( ArbSuedAb, HIGH );
}

void shuttersArbSuedHalt() {
  DEBUG_PRINTLN("Shutters ArbSued halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( ArbSuedAuf, LOW );
  digitalWrite( ArbSuedAb, LOW);
}

uint8_t shuttersArbSuedGetState() {
  return 255;
}

void shuttersArbSuedSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeOGaktiv, nameArbSued);  }
  else  {
     bitClear(rolladeOGaktiv, nameArbSued);
  }
 DEBUG_PRINT_VALUE("Saving state ArbSued ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeOGaktiv = ", rolladeOGaktiv);
}


void onShuttersArbSuedLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters ArbSued at ", level);
 DEBUG_PRINTLN(" %");
 byte temp = rolladenOGkalibriert(nameArbSued, level);
 DEBUG_PRINTLN_VALUE("> Rollade Arbeit S�d kalibriert nach Start  ", temp);
 rolladeOGlevel[nameArbSued] = level;				// erg�nzt am 13.03.22 Fehlerkorrektur
}

// Funktionsaufrufe f�r die Rollade im Arbeitszimmer West, kleine Rollade

void shuttersArbWestUp() {
  DEBUG_PRINTLN("Shutters ArbWest going up.");
  // TODO: Implement the code for the shutters to go up
  digitalWrite( ArbWestAuf, HIGH );
}

void shuttersArbWestDown() {
  DEBUG_PRINTLN("Shutters ArbWest going down.");
  // TODO: Implement the code for the shutters to go down
  digitalWrite( ArbWestAb, HIGH );
}

void shuttersArbWestHalt() {
  DEBUG_PRINTLN("Shutters ArbWest halted.");
  // TODO: Implement the code for the shutters to halt
  digitalWrite( ArbWestAuf, LOW );
  digitalWrite( ArbWestAb, LOW);
}

uint8_t shuttersArbWestGetState() {
  return 255;
}

void shuttersArbWestSetState(uint8_t state) {
  if (state == 255) {
     bitSet(rolladeOGaktiv, nameArbWest);  }
  else  {
     bitClear(rolladeOGaktiv, nameArbWest);
  }
 DEBUG_PRINT_VALUE("Saving state ArbWest ", state);
 DEBUG_PRINTLN_VALUE("  und  rolladeOGaktiv = ", rolladeOGaktiv);
}

void onShuttersArbWestLevelReached(uint8_t level) {
 DEBUG_PRINT_VALUE("Shutters ArbWest at ", level);
 DEBUG_PRINTLN(" %");
 byte temp = rolladenOGkalibriert(nameArbWest, level);
 DEBUG_PRINTLN_VALUE("> Rollade Arbeit West kalibriert nach Start  ", temp);
 rolladeOGlevel[nameArbWest] = level;				// erg�nzt am 13.03.22 Fehlerkorrektur
}

// --------------------------------------------------------------------------



/*
#### Shutters (unsigned long `courseTime`, void (\*`upCallback`)(void), void (\*`downCallback`)(void), void (\*`haltCallback`)(void), byte (\*`getStateCallback`)(void), void (\*`setStateCallback`)(byte state), float `calibrationRatio` = 0.1, void (\*`onLevelReachedCallback`)(byte level))

* **`courseTime`**: Time in milliseconds to do a full shutters course
* **`upCallback()`**: Function to execute for the shutters to go up
* **`downCallback()`**: Function to execute for the shutters to go down
* **`haltCallback()`**: Function to execute for the shutters to halt
* **`getStateCallback()`**: Function to get state. This must return the state byte, or 255 if you don't know the state byte (on first boot)
* **`setStateCallback(byte state)`**: Function to set the state byte. Store this in the EEPROM of SPIFFS, etc.
* **`calibrationRatio`**: The calibration ratio. If the full course is 30 sec. and the ratio is 0.1, the calibration time will be 30 * 0.1 = 3 sec. Defaults to 0.1
* **`onLevelReachedCallback(byte level)`**: Function to be called whenever a new level is reached

Shutters Zentral(courseZentralTime, shuttersZentralUp, shuttersZentralDown, shuttersZentralHalt, shuttersZentralGetState, shuttersZentralSetState, calibrationRatio, onShuttersZentralLevelReached);

Shutters GWc(courseGWcTime, shuttersGWcUp, shuttersGWcDown, shuttersGWcHalt, shuttersGWcGetState, shuttersGWcSetState, calibrationRatio, onShuttersGWcLevelReached);
Shutters Kueche(courseKuecheTime, shuttersKuecheUp, shuttersKuecheDown, shuttersKuecheHalt, shuttersKuecheGetState, shuttersKuecheSetState, calibrationRatio, onShuttersKuecheLevelReached);
Shutters Essen(courseEssenTime, shuttersEssenUp, shuttersEssenDown, shuttersEssenHalt, shuttersEssenGetState, shuttersEssenSetState, calibrationRatio, onShuttersEssenLevelReached);
Shutters Terasse(courseTerasseTime, shuttersTerasseUp, shuttersTerasseDown, shuttersTerasseHalt, shuttersTerasseGetState, shuttersTerasseSetState, calibrationRatio, onShuttersTerasseLevelReached);
Shutters Erker1(courseErker1Time, shuttersErker1Up, shuttersErker1Down, shuttersErker1Halt, shuttersErker1GetState, shuttersErker1SetState, calibrationRatio, onShuttersErker1LevelReached);
Shutters Erker2(courseErker2Time, shuttersErker2Up, shuttersErker2Down, shuttersErker2Halt, shuttersErker2GetState, shuttersErker2SetState, calibrationRatio, onShuttersErker2LevelReached);
Shutters Erker3(courseErker3Time, shuttersErker3Up, shuttersErker3Down, shuttersErker3Halt, shuttersErker3GetState, shuttersErker3SetState, calibrationRatio, onShuttersErker3LevelReached);

Shutters Bad(courseBadTime, shuttersBadUp, shuttersBadDown, shuttersBadHalt, shuttersBadGetState, shuttersBadSetState, calibrationRatio, onShuttersBadLevelReached);
Shutters Schlaf(courseSchlafTime, shuttersSchlafUp, shuttersSchlafDown, shuttersSchlafHalt, shuttersSchlafGetState, shuttersSchlafSetState, calibrationRatio, onShuttersSchlafLevelReached);
Shutters ArbSued(courseArbSuedTime, shuttersArbSuedUp, shuttersArbSuedDown, shuttersArbSuedHalt, shuttersArbSuedGetState, shuttersArbSuedSetState, calibrationRatio, onShuttersArbSuedLevelReached);
Shutters ArbWest(courseArbWestTime, shuttersArbWestUp, shuttersArbWestDown, shuttersArbWestHalt, shuttersArbWestGetState, shuttersArbWestSetState, calibrationRatio, onShuttersArbWestLevelReached);

*/


