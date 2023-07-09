/*
 L45AControllino.h
	als Include-Datei erstellt am 15.01.21

aktuelles Release vom 29.06.22
void lifeLedD11(unsigned long waitTime)  erg�nzt f�r Controllino Maxi ( ... h�chste Ausgangsnummer)

aktuelles Release vom 25.02.22

In dieser Datei werden alle Definitionen abgelegt, die f�r die Parametrierung der
Rolladensteuerung Version 1 / Version 2 erforderlich sind.

Erste Test in Verbindung mit
	sketch_jan15b_controllinino_BasicShutters_Index
weitere Test siehe Kommentar am Ende

*/

void defineControllinoEingaenge()  {			//  Definition der Eing�nge f�r folgende Rolladenkommandotaster
  pinMode(CONTROLLINO_A0, INPUT);				// 	ZentralAuf
  pinMode(CONTROLLINO_A1, INPUT);				//	ZentralAb
  pinMode(CONTROLLINO_A2, INPUT);				//  G�ste-WC auf
  pinMode(CONTROLLINO_A3, INPUT);				//	G�ste-WC ab
  pinMode(CONTROLLINO_A4, INPUT);				//	K�che Auf
  pinMode(CONTROLLINO_A5, INPUT);				//	K�che Ab
  pinMode(CONTROLLINO_A6, INPUT);				//	Esszimmer Auf
  pinMode(CONTROLLINO_A7, INPUT);				//	Esszimmer Ab
  pinMode(CONTROLLINO_A8, INPUT);				//	Terasse Auf
  pinMode(CONTROLLINO_A9, INPUT);				//	Terasse Ab

 #ifdef CONTROLLINO_MAXI
  pinMode(CONTROLLINO_IN0, INPUT);				//	Terasse Auf
  pinMode(CONTROLLINO_IN1, INPUT);				//	Terasse Ab
 # endif

  
  #ifdef CONTROLLINO_MEGA
  pinMode(CONTROLLINO_A10, INPUT);				//  Erker 1 Auf
  pinMode(CONTROLLINO_A11, INPUT);				//  Erker 1 Ab
  pinMode(CONTROLLINO_A12, INPUT);				//  Erker 2 Auf
  pinMode(CONTROLLINO_A13, INPUT);				//  Erker 2 Ab
  pinMode(CONTROLLINO_A14, INPUT);				//  Erker 3 Auf
  pinMode(CONTROLLINO_A15, INPUT);				//  Erker 3 Ab
  pinMode(CONTROLLINO_I16, INPUT);  // Auswertung des Tasters an der Haust�r, Analogwert wird in virtuellen Digitalwert umgewandelt
  pinMode(CONTROLLINO_I17, INPUT);  // Auswertung des Lichtsensors Garage,    dito
  #endif
}

void defineControllinoAusgaenge()  {			//  Definition der Ausg�nge f�r folgende Rolladenkommandos
  pinMode(CONTROLLINO_D0, OUTPUT);				// 	ZentralAuf
  pinMode(CONTROLLINO_D1, OUTPUT);				//	ZentralAb
  pinMode(CONTROLLINO_D2, OUTPUT);				//  G�ste-WC auf
  pinMode(CONTROLLINO_D3, OUTPUT);				//	G�ste-WC ab
  pinMode(CONTROLLINO_D4, OUTPUT);				//	K�che Auf
  pinMode(CONTROLLINO_D5, OUTPUT);				//	K�che Ab
  pinMode(CONTROLLINO_D6, OUTPUT);				//	Esszimmer Auf
  pinMode(CONTROLLINO_D7, OUTPUT);				//	Esszimmer Ab
  pinMode(CONTROLLINO_D8, OUTPUT);				//	Terasse Auf
  pinMode(CONTROLLINO_D9, OUTPUT);				//	Terasse Ab
  pinMode(CONTROLLINO_D10, OUTPUT);				//  Erker 1 Auf
  pinMode(CONTROLLINO_D11, OUTPUT);				//  Erker 1 Ab
  #ifdef CONTROLLINO_MEGA
  pinMode(CONTROLLINO_D12, OUTPUT);				//  Erker 2 Auf
  pinMode(CONTROLLINO_D13, OUTPUT);				//  Erker 2 Ab
  pinMode(CONTROLLINO_D14, OUTPUT);				//  Erker 3 Auf
  pinMode(CONTROLLINO_D15, OUTPUT);				//  Erker 3 Ab
  // D22 zeigt die aktivierte Sonnenautomatik an
  pinMode(CONTROLLINO_D22, OUTPUT);       // hier ist auf dem Testboard das Relay angeschlossen

  // D23 zeigt das Lebenslicht an, siehe void lifeLedD23 () in dieser Bibliothek
  pinMode(CONTROLLINO_D23, OUTPUT);       // wird bei dem Programmieren als Lebeslicht verwendet
  #endif
}

void defineControllinoRelais()  {
  #ifdef CONTROLLINO_MAXI
  pinMode(CONTROLLINO_R0, OUTPUT);        // Taster Bad Auf, unten 4te von rechts - parallel
  pinMode(CONTROLLINO_R1, OUTPUT);        // Taster Bad Ab, unten 3te von rechts - parallel
  pinMode(CONTROLLINO_R2, OUTPUT);        // unten 2te von rechts - parallel Taster Sch Auf
  pinMode(CONTROLLINO_R3, OUTPUT); 
  pinMode(CONTROLLINO_R4, OUTPUT);        // unten 2te von rechts - parallel Taster Sch Auf
  pinMode(CONTROLLINO_R5, OUTPUT); 
  #endif

  pinMode(CONTROLLINO_R6, OUTPUT);        // Taster Bad Auf, unten 4te von rechts - parallel
  pinMode(CONTROLLINO_R7, OUTPUT);        // Taster Bad Ab, unten 3te von rechts - parallel
  pinMode(CONTROLLINO_R8, OUTPUT);        // unten 2te von rechts - parallel Taster Sch Auf
  pinMode(CONTROLLINO_R9, OUTPUT);        // unten 1te von rechts - parallel Taster Sch Ab

  #ifdef CONTROLLINO_MEGA
  pinMode(CONTROLLINO_R10, OUTPUT);       // oben 1te von links - parallel Taster Ar1 Auf
  pinMode(CONTROLLINO_R11, OUTPUT);       // oben 2te von links - parallel Taster Ar1 Ab
  pinMode(CONTROLLINO_R12, OUTPUT);       // oben 3te von links - parallel Taster Ar2 Auf
  pinMode(CONTROLLINO_R13, OUTPUT);       // oben 3te von links - parallel Taster Ar2 Ab
  pinMode(CONTROLLINO_R14, OUTPUT);       // oben 5te von links - parallel zu D0 f�r Rollade Zentral auf
  pinMode(CONTROLLINO_R15, OUTPUT);       // oben 6te von links - parallel zu D1 f�r Rollade Zentral ab
  #endif
}

// 29.06.22 f�r Test mit dem Controllino Maxi eine Lebenslichtfunktion f�r Ausgang D11 erg�nzt
void lifeLedD11(unsigned long waitTime) {
  // const unsigned long waitTime = 500;
  #ifdef CONTROLLINO_MAXI
  static unsigned long oldTime = 0;
  static int state = LOW;

  if (oldTime + waitTime < millis()) {
    if (state == LOW) state = HIGH; else state = LOW;
    oldTime = millis();
    digitalWrite(CONTROLLINO_D11, state);
    #endif
  }
} //  Ende void lifeLedD11


// 09.02.2022 Test mit sketch_jan15b_controllinino_BasicShutters_Index
void lifeLedD23(unsigned long waitTime) {
  // const unsigned long waitTime = 500;
  #ifdef CONTROLLINO_MEGA
  static unsigned long oldTime = 0;
  static int state = LOW;

  if (oldTime + waitTime < millis()) {
    if (state == LOW) state = HIGH; else state = LOW;
    oldTime = millis();
    digitalWrite(CONTROLLINO_D23, state);
  }
  #endif
} //  Ende void lifeLedD23

/*
 * sketch_feb25a_rolladensteuerung am 25.02.22
 * Basis:  aktuell auf dem Controllino laufende Programm sketch_aug19e_rolladensteuerung
 * Test der Librarie L45AControllino.h
 * in void setup() werden folgende Routinen aufgerufen
 *          void defineControllinoEingaenge()
 *          void defineControllinoAusgaenge()
 *          void defineControllinoRelais()
 * in void loop() stehen folgende Routinen zu Verf�gung
 * f�r das Lebenslicht am Ausgang D23 kann in loop() folgende Routine aufgerufen werden
 *          void lifeLedD23(unsigned long waitTime)    // waitTime[ms]
 * 			ersetzt die bisherige Kombination aus timer.every(1000, toggle_led); und bool toggle_led(void *)
 * 			Test o.k. , mit 500 und 1000ms
 */