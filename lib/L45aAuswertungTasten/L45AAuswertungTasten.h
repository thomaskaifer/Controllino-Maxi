/*
 L45AAuswertungTasten.h
	als Include-Datei erstellt am 03.03.22

In dieser Datei werden alle Definitionen und Funktionen abgelegt, die f�r die Auswertung der
Controllino_A0 - A15 , _I16 und _I17 erforderlich sind
Die Auswertung erfolgt �ber die Entprellroutinen in <Bounce2.h>

der Aufruf dieser Librarie kann erst nach
- Indizierung Bounce debouncer[NUMBUTTONS];
- Indizierung Shutters Zimmer(Parameter, ... );
erfolgen. Im anderen Fall ergeben sich Compilerfehler und in void auswertungA0bisI17()
d�rfen keine Rolladenkommandos aus <L45AShutters.h> verwendet werden

Erste Test in Verbindung mit
sketch_mar02a_controllinino_BasicShutters_Bounce2_Index

setup()
void setupDebouncer() ;

loop()
checkStartsperre();
void auswertungA0bisI17();

interne Librarie-Funktionen:
void rolladeEGStopGesamt();
void rolladeOGStopGesamt();
void rolladeEGESObenGesamt();
void rolladeOGESObenGesamt();	neu am 14.03.22
void rolladeEGESUntenGesamt();
void rolladeOGESUntenGesamt();	neu am 14.03.22
void rolladeEGStop(int nummer);

void rolladeEGdistanz(int nummer, int distanz)
void rolladeEGESoben(int nummer);
void rolladeEGESunten(int nummer);

void rolladeEGKommando(int nummerRolladeEG, int befehl, int modus, unsigned long startzeit)

*/

// boolean startsperre = true; in global.h ausgelagert

// folgende Konstanten werden in void rolladeEGKommando(int nummerRolladeEG, int befehl, int modus, unsigned long startzeit)
// mit modus=2 verwendet, um von der Schaltdauer abh�ngige Aktionen durchzuf�hren
const unsigned long zeitpunktDauerTaste1 = 550;	 // Taste < 550ms bet�tigt 	-->	Rollade f�hrt nur mit einer Differenz im Level
const unsigned long zeitpunktDauerTaste2 = 1100; // 550 <= t < 1100			--> ...  in ENdstellung
												 // 1100 <= t				--> Totmannsteuerung  ( Modus 3 nur in <L45AI2CPort.h> )

const char *TasterPush = " -> Taster bet�tigt f�r Rollade:  ";
const char *komandoStringA0ff[]  =  { "Zentral auf ","Zentral ab  ",
						  		"G�steWC auf ","G�steWC ab  ",
						  		"Kueche auf  ","Kueche  ab  ",
						  		"Essen auf   ","Essen ab    ",
						  		"Terasse auf ","Terasse ab  ",
						  		"Erker 1 auf ","Erker 1 ab  ",
						  		"Erker 2 auf ","Erker 2 ab  ",
						  		"Erker 3auf  ","Erker 3 ab  "   };
/*
Auskommentierung am 20.03.22
Festlegung, ob diese Definition hier erfolgen mu�, oder in der <L45AI2CPort.h
char *komandoStringI2C[]  =   { "Bad auf      ","Bad ab      ",
						  		"Schlafen auf ","Schlafenab  ",
						  		"Arb-Sued auf ","Arb-Sued ab " ,
						  		"Arb-West auf ","Arb-West ab "  };

*/
void setupDebouncer()  {
  // erg�nzt am 02.03.22 f�r die Instanzen der Testerentprellroutinen
  for (int i = 0; i < NUMBUTTONS; i++) {
    // pinMode(buttonPins[i], INPUTMODE);  Definition erfolgt in L45AControllino.h
    debouncer[i].attach(buttonPins[i]);
    switch (i)  {
      
	  #ifdef CONTROLLINO_MAXI
  	  case 10:
        debouncer[i].interval(entprellZeitI0);
        break;
      case 11:
        debouncer[i].interval(entprellZeitI1);
        break;
	  #endif
	   
	  
	  #ifdef CONTROLLINO_MEGA
  	  case 16:
        debouncer[i].interval(entprellZeitI16);
        break;
      case 17:
        debouncer[i].interval(entprellZeitI17);
        break;
		#endif
      default:
        //  alle Eing�nge mit Tastern Rolladenkomandos auf/ab
        debouncer[i].interval(entprellZeit);
        break;
    }  //  Ende switch (i)
  }  // Ende for (int i = 0; ...
}  //  Ende void setupDebouncer()


void checkStartsperre()  {
     
   if (startsperre == true)  {
   		if ((rolladenEGkalibriert (0, 0) == EGkallibriert) && (rolladenOGkalibriert (0, 0) == 14)) {   // 30  = 16 + 8 + 4 + 2
    	DEBUG_PRINTLN("Sperre aufgehoben");    // getestet am 28.02.22 �
    	// globale Variable definieren und UP aufrufen oder als lokale Variable in einer Funktion optional
    	startsperre = false;
  		}
		//else	{
		//DEBUG_PRINTLN("Sperre nicht aufgehoben");	
		//}
	}  //  Ende if (startsperre == true)
}  // Ende void checkStartsperre()



/*

	Parameter
	int nummerRolladeEG  	entspricht  i/2  [0..7] aus void auswertungA0bisI17() und damit der Rolladennummer
	int befehl  			entspricht  i%2  [0..1] aus void auswertungA0bisI17()
	int modus	Modus = 1 	bei (debouncer[i].rose() &&  (debouncer[i].read() == HIGH))
							wenn Rollade aktiv, dann Kommando STOP

							f�r Rollade aktiv wird in den Funktionen flagsrollade[nummerRolladeEG] auch das flagsrollade[nummerRolladeEG]
							ver�ndert, indem das Bit0 = Rollade aktiv gesetzt wird
							in shutters...SetState(state) wird es ge�scht

							wenn nicht aktiv, dann Rollade entsprechend Parameter befehl fahren, Zielposition level +/- 33%
							Parameter dauer nach dauerTastendruck[nummerRolladeEG] sichern
				Modus = 2	bei (debouncer[i].read() == HIGH)
							Vergleich, ob Parameter dauer - dauerTastendruck[nummerRolladeEG] >= 1 sec, dann level auf 100%/0%
				Modus = 3	bei (debouncer[i].fell() &&  (debouncer[i].read() == LOW))
							Auswertung dauerTastendruck[nummerRolladeEG] f�r Sonderbefehle, z.B. EEPROM-Parameter ver�ndern
*/

void rolladeEGStopGesamt()  {
	shuttersGWcHalt();
	GWc.setLevel(rolladeEGlevel[nameGWC]);   // 13.03.22 pr�fe. ob Verwendung aus der enum EGRolladenNamen nach Compileraufruf
	shuttersKuecheHalt();
	Kueche.setLevel(rolladeEGlevel[nameKueche]);
	shuttersEssenHalt();
	Essen.setLevel(rolladeEGlevel[nameEssen]);
	shuttersTerasseHalt();
	Terasse.setLevel(rolladeEGlevel[nameTerasse]);

	#ifdef CONTROLLINO_MEGA
	  shuttersErker1Halt();
	  Erker1.setLevel(rolladeEGlevel[nameErker1]);
	  shuttersErker2Halt();
	  Erker1.setLevel(rolladeEGlevel[nameErker2]);
	  shuttersErker3Halt();
	  Erker1.setLevel(rolladeEGlevel[nameErker3]);
	#endif
}  //  Ende void rolladeEGStopGesamt ()

/* am 20.03.22 nach <L45AI2CPort.h> �bertragen
void rolladeOGStopGesamt ()  {
	shuttersBadHalt();
	Bad.setLevel(rolladeOGlevel[nameBad]);
	shuttersSchlafHalt();
	Schlaf.setLevel(rolladeOGlevel[nameSchlaf]);
	shuttersArbSuedHalt();
	ArbSued.setLevel(rolladeOGlevel[nameArbSued]);
	shuttersArbWestHalt();
	ArbWest.setLevel(rolladeOGlevel[nameArbWest]);
}  //  Ende void rolladeOGStopGesamt ()
*/

//  in Abh�ngigkeit von nummerRolladeEG [0] wird die Rollade �ber den Relaiseingang Zentral ....
//  .... Auf in die obere Endposition gefahren  ( ESOben = Endschalter oben )   13.03.22

void rolladeEGESObenGesamt() {
	GWc.setLevel(0);
	Kueche.setLevel(0);
	Essen.setLevel(0);
	Terasse.setLevel(0);
	#ifdef CONTROLLINO_MEGA
	  Erker1.setLevel(0);
	  Erker2.setLevel(0);
	  Erker3.setLevel(0);
	#endif
}  //  Ende void rolladeEGESObenGesamt()

void rolladeOGESObenGesamt() {
	Bad.setLevel(0);
	Schlaf.setLevel(0);
	ArbSued.setLevel(0);
	ArbWest.setLevel(0);
}  //  Ende void rolladeOGESObenGesamt()


//  .... Ab in die untere Endposition gefahren  ( ESUnten = Endschalter unten )  13.03.22
void rolladeEGESUntenGesamt() {
	GWc.setLevel(100);
	Kueche.setLevel(100);
	Essen.setLevel(100);
	Terasse.setLevel(100);
	#ifdef CONTROLLINO_MEGA
	  Erker1.setLevel(100);
	  Erker2.setLevel(100);
	  Erker3.setLevel(100);
	#endif
}  //  Ende void rolladeEGESUntenGesamt()

void rolladeOGESUntenGesamt() {
	Bad.setLevel(100);
	Schlaf.setLevel(100);
	ArbSued.setLevel(100);
	ArbWest.setLevel(100);
}  //  Ende void rolladeOGESUntenGesamt()



//  in Abh�ngigkeit von nummerRolladeEG [0..7] wird die Rolladenbewegung gestoppt
void rolladeEGStop(int nummer)  {
	switch (nummer)  {
		case 0:
		// shuttersZentralHalt();  zentrale Relais f�r Rolladen werden nicht mehr ben�tigt, per Software m�ssen alle Rolladen per level�nderung
		// verfahren werden
		rolladeEGStopGesamt();
		break;
		case 1:
		shuttersGWcHalt();
		DEBUG_PRINTLN_VALUE("--Rollade G�steWC �ber Befehl gestoppt bei Level ",rolladeEGlevel[nummer]);
		GWc.setLevel(rolladeEGlevel[nummer]);
		break;
		case 2:
		shuttersKuecheHalt();
		DEBUG_PRINTLN_VALUE("--Rollade K�che �ber Befehl gestoppt bei Level ",rolladeEGlevel[nummer]);
		Kueche.setLevel(rolladeEGlevel[nummer]);
		break;
		case 3:
		shuttersEssenHalt();
		DEBUG_PRINTLN_VALUE("--Rollade Essen �ber Befehl gestoppt bei Level ",rolladeEGlevel[nummer]);
		Essen.setLevel(rolladeEGlevel[nummer]);
		break;
		case 4:
		shuttersTerasseHalt();
		DEBUG_PRINTLN_VALUE("--Rollade Terasse gestoppt bei Level ",rolladeEGlevel[nummer]);
		Terasse.setLevel(rolladeEGlevel[nummer]);
		break;
		#ifdef CONTROLLINO_MEGA
		case 5:
		shuttersErker1Halt();
        DEBUG_PRINTLN_VALUE("--Rollade Erker 1 �ber Befehl gestoppt bei Level ",rolladeEGlevel[nummer]);
		Erker1.setLevel(rolladeEGlevel[nummer]);
		break;
		case 6:
		shuttersErker2Halt();
		DEBUG_PRINTLN_VALUE("--Rollade Erker 2 �ber Befehl gestoppt bei Level ",rolladeEGlevel[nummer]);
		Erker2.setLevel(rolladeEGlevel[nummer]);
		break;
		case 7:
		shuttersErker3Halt();
		DEBUG_PRINTLN_VALUE("--Rollade Erker 3 �ber Befehl gestoppt bei Level ",rolladeEGlevel[nummer]);
		Erker3.setLevel(rolladeEGlevel[nummer]);
		break;
		#endif
	}  // Ende switch (nummer)
}  // Ende void rolladeEGStop(nummer)


//  in Abh�ngigkeit von nummerRolladeEG [0..7] wird die Rolladen auf das Level 60% gefahren
//  das erfolgt, wenn die Rollade nicht auf 60% steht
//  als Abw�rtsfahhrt virtuell entsprechend dem Aufruf    									---->   sp�ter �ndern
//  Option ist hier ein 2ter Parameter (int level ), auf das die Rollade gefahren wird

void rolladeEGdistanz(int nummer, int distanz)  {
	switch (nummer)  {
		case 0:
		//
		break;
		case 1:
		GWc.setLevel(distanz);
		break;
		case 2:
		Kueche.setLevel(distanz);
		break;
		case 3:
		Essen.setLevel(distanz);
		break;
		case 4:
		Terasse.setLevel(distanz);
		break;
		#ifdef CONTROLLINO_MEGA
		case 5:
		Erker1.setLevel(distanz);
		break;
		case 6:
		Erker2.setLevel(distanz);
		break;
		case 7:
		Erker3.setLevel(distanz);
		break;
		#endif
	}  // Ende switch (nummer)
}  // Ende void rolladeEGdistanz(nummer)


//  in Abh�ngigkeit von nummerRolladeEG [0..7] wird die Rolladen auf das Level 0% gefahren
//  das erfolgt, wenn die Rollade auf nicht auf 30% steht
//  als Aufw�rtsfahhrt virtuell entsprechend dem Aufruf   									---->   sp�ter �ndern
//  Option ist hier ein 2ter Parameter (int level ), auf das die Rollade gefahren wird
void rolladeEGESoben(int nummer)  {
	switch (nummer)  {
		case 0:
		//
		break;
		case 1:
		GWc.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade G�steWC �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		case 2:
		Kueche.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade K�che �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		case 3:
		Essen.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade Essen �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		case 4:
		Terasse.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade Terasse �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		#ifdef CONTROLLINO_MEGA
		case 5:
		Erker1.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade Erker1 �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		case 6:
		Erker2.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade Erker2 �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		case 7:
		Erker3.setLevel(0);
		DEBUG_PRINTLN_VALUE("--Rollade Erker1 �ber Befehl nach Endschalter oben ",rolladeEGlevel[nummer]);
		break;
		#endif
	}  // Ende switch (nummer)
}  // Ende void rolladeEGESoben(nummer)


void rolladeEGESunten(int nummer)  {
	switch (nummer)  {
		case 0:
		//
		break;
		case 1:
		GWc.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade G�ste-WC �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		break;
		case 2:
		Kueche.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade K�che �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		break;
		case 3:
		Essen.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade Essen �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		break;
		case 4:
		Terasse.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade Terasse �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		break;
		#ifdef CONTROLLINO_MEGA
		case 5:
		Erker1.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade Erker1 �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		case 6:
		Erker2.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade Erker2 �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		break;
		case 7:
		Erker3.setLevel(100);
		DEBUG_PRINTLN_VALUE("--Rollade Erker3 �ber Befehl nach Endschalter unten ",rolladeEGlevel[nummer]);break;
		break;
		#endif
	}  // Ende switch (nummer)
}  // Ende void rolladeEGESunten(nummer)



void rolladeEGKommando(int nummerRolladeEG, int befehl, int modus, unsigned long startzeit)  {
	// static byte flagsrollade[8];	Bit 0 = Rollade Auf aktiv, Bit 1 = Rollade Ab aktiv, Bit 2 = Rollade angehalten aus Befehl
	static unsigned long dauerTastendruck[8];


	switch (modus) {
		case 1:				// steigende Flanke ergibt Rolladenkommando Level�nderung um 30%
		//
		DEBUG_PRINT_VALUE("--> Modus = 1 und befehl =  ", befehl);
		DEBUG_PRINTLN_VALUE("  und  rolladeEGaktiv =  ", rolladeEGaktiv);
		//
		if (bitRead(rolladeEGaktiv,nummerRolladeEG))  {
			DEBUG_PRINTLN("   --->  Rollade Stop �ber Modus=1 und bitRead(rolladeEGaktiv,nummerRolladeEG) ");
			rolladeEGStop(nummerRolladeEG);

		}   else  {
		if (befehl == 0)  {   // Befehl auf, die Schlie�fl�che wird um 30% verkelinert - > level 1 30%
			// 11.03.22 nach Test hier eingef�gt. Die �bergabe der Zeiger geht nicht in Verbindung mit DEBUG_PRINTLN,
			// ??? nicht mit F-Makro , hier noch Kompilerdirektive mit _smartdebug erg�nzen
			Serial.print(TasterPush);
            Serial.println(komandoStringA0ff[nummerRolladeEG]);
            // rolladeEGdistanz(int nummer, int distanz)
			rolladeEGdistanz(nummerRolladeEG, (rolladeEGlevel[nummerRolladeEG] - 30));
	   		}
	   	if (befehl == 1)  {      // Befehl auf, die Schlie�fl�che wird um 30% verg��ert - > level + 30%
			Serial.print(TasterPush);
            Serial.println(komandoStringA0ff[nummerRolladeEG]);
			// rolladeEGdistanz(int nummer, int distanz)
			rolladeEGdistanz(nummerRolladeEG, (rolladeEGlevel[nummerRolladeEG] + 30));
	   		}
		}  // else bitRead(
		break;
		case 2:       // Signal 24V kontant, SIgnaldauer wird ermittelt und abh�ngig Kommandos ausgel�st
			dauerTastendruck[nummerRolladeEG] = millis() - startzeit;
			if (dauerTastendruck[nummerRolladeEG] > zeitpunktDauerTaste1)  {
				DEBUG_PRINTLN_VALUE("---> Modus 2 , Dauer Tastendruck gr��er 1sec mit Wer  ",dauerTastendruck[nummerRolladeEG]);
				if (dauerTastendruck[nummerRolladeEG] > zeitpunktDauerTaste2)  {
					DEBUG_PRINTLN_VALUE("Dauer Tastendruck am Eingang > 2000 ms mit :", dauerTastendruck[nummerRolladeEG]);
				}  else  {		// ab hier ist 1000ms < dauerTastendruck[nummerRolladeEG] < 2000
				   if (befehl == 0)  {   // Befehl auf, Schlie�fl�che 0% , Fahrt bis oberen Endschalter
				   DEBUG_PRINTLN_VALUE("   ---> Modus 2 , Befehl fahre Rollade in Endschalter oben, Rolladennummer ", nummerRolladeEG);
				   rolladeEGESoben(nummerRolladeEG);
				   }  // Ende if (befehl == 0)
				   if (befehl == 1)  {   // Befehl ab, Schlie�fl�che 100% , Fahrt bis unteren Endschalter
				   DEBUG_PRINTLN_VALUE("   ---> Modus 2 , Befehl fahre Rollade in Endschalter unten, Rolladennummer ", nummerRolladeEG);
				   rolladeEGESunten(nummerRolladeEG);
				   }  // Ende if (befehl == 1)

		        }  // if (dauerTastendruck[nummerRolladeEG] > zeitpunktDauerTaste1)
			}  //  Ende if (dauerTastendruck[nummerRolladeEG] > zeitpunktDauerTaste1)
		//
		break;
		case 3:
		//
		break;
    }  // Ende switch (modus)

}  //  Ende void rolladeEGKommando

/*
    erg�nzt am 02.03.22 f�r die Auswertung der Testerentprellroutinen  -   Anfang
*/

void auswertungA0bisI17()  {
  // int nachkomma;
  // int vorkomma;
  unsigned long currentTastendruckdauer;		// Test 18.07.22
  unsigned long previosTastendruckdauer;

  if ( startsperre == false )  {
    for (int i = 0; i < NUMBUTTONS; i++)
    {
      debouncer[i].update();   // Status pr�fen
      if (debouncer[i].fell()) // Wenn die Taste losgelassen wird, Logik High-Aktives Signal �bergang von Ub nach 0V
      {
        endeZeit[i] = millis();

        Serial.print(" debouncer[i].fell() von index i =  ");
        Serial.print(i);
        Serial.print(" - > ");
        Serial.println(debouncer[i].fell());
        if (endeZeit[i] - startZeit[i])  {
          DEBUG_PRINTLN_VALUE("Schaltzeit [i] berechnet von index i =   ", i);
          DEBUG_PRINTLN_VALUE(" - > ", (endeZeit[i] - startZeit[i]));
          // Serial.println(endeZeit[i] - startZeit[i]);
          /* Programmteile in switch
          vorkomma = i/2;
          nachkomma = i%2;
          DEBUG_PRINT_VALUE(" i DIV 2    = ", vorkomma);
          DEBUG_PRINTLN_VALUE(" - i Modulo 2 = ", nachkomma);
          */
          startZeit[i] = endeZeit[i];         // Startbedingung der Schleife zur�cksetzen
        }

        if (debouncer[i].fell() &&  (debouncer[i].read() == LOW)) {
          DEBUG_PRINTLN_VALUE("Taster nicht mehr gedr�ckt von index i =   ", i);

          switch (i)  {
            case 17:
              DEBUG_PRINTLN("Lichtsensor Garage ist jetzt Aus = D�mmerung Morgens ");
              // Einschaltung am 02.03.22 getestet, hier Befehl oder Funktionsaufruf
              break;
          }  // Ende switch (i)

        }  //  Ende if (debouncer[i].fell() &&  (debouncer[i].read() == LOW))
      }  // Ende if (debouncer[i].fell())

      if (debouncer[i].rose()) // Wenn die Taste gedr�ckt wird, Logik High-Aktives Signal �bergang von 0V nach Ub
      {
        startZeit[i] = millis();
        Serial.print(" debouncer[i].rose() von index i =  ");
        Serial.print(i);
        Serial.print(" - > ");
        Serial.println(debouncer[i].rose());
      }

      /*
      * folgende Bedingung ist mit debouncer[].rose() nur in einem loop() - Zyklus erf�llt, danach wird nur noch  das statische Signalzustand
      * bis zur fallenden Flanke debouncer[].fell() �ber debouncer[i].update();  ausgegeben
      * hier erfolgt die Auswertung der Zustands�nderungen, die Auswertung der Zeitdauer siehe in  ............
      */

      if (debouncer[i].rose() &&  (debouncer[i].read() == HIGH)) {
        DEBUG_PRINTLN_VALUE("Taster gedr�ckt von index i =   ", i);
if (i==1)  {
		currentTastendruckdauer = debouncer[i].currentDuration();
		DEBUG_PRINTLN_VALUE("......... > Taster 1 mit currentTastendruckdauer   ", currentTastendruckdauer);
		previosTastendruckdauer = debouncer[i].previousDuration();
		DEBUG_PRINTLN_VALUE("......... > Taster 1 mit previosTastendruckdauer   ", previosTastendruckdauer);
}
        switch (i)  {
          case 0:
            Serial.print(TasterPush);
            Serial.println(komandoStringA0ff[i]);
            // Zentral.setLevel(0); // Go to 50%
            DEBUG_PRINTLN("       --> L45AAuswertungTasten.h   Zeile 479  case 0:  Code schreiben");
            break;
          case 1:
            Serial.print(TasterPush);
            Serial.println(komandoStringA0ff[i]);
            // Zentral.setLevel(100); // Go to 50%
            DEBUG_PRINTLN("       --> L45AAuswertungTasten.h   Zeile 485  case 1:  Code schreiben");
            break;
          // --------------------------------------------------------------- Sonderfunktionen
          case 16:
		     DEBUG_PRINTLN("Toggle Flag Sonnenautomatik");
		     // Toggeln am 02.03.22 getestet, hier Befehl oder Funktionsaufruf
		     break;
		  case 17:
		      DEBUG_PRINTLN("Lichtsensor Garage ist jetzt Ein = D�mmerung Abends ");
		      // Einschaltung am 02.03.22 getestet, hier Befehl oder Funktionsaufruf
              break;

          // --------------------------------------------------------------- Funktionen identisch

          default:
            // Call void Parameter 1 und 2 ohne Verwendung der Hilfsvariablen vorkomma und Nachkomma
            rolladeEGKommando(i/2 , i%2 , 1 , startZeit[i]);       // Modus 1 mit steigender Flanke des Eingangssignals
            break;

/*
nach Auslagern der Statusmeldungen in ein char *komandoStringA0ff[]  erfolgt der Test mit Default f�r alle gleichartigen Komandos
*/
		 }  //  Ende switch
      }  //  if (debouncer[i].rose() &&  (debouncer[i].read() == HIGH))

      if (!debouncer[i].rose() &&  (debouncer[i].read() == HIGH))   {
		//  hier Aufruf mit modus = 2  ,  Signal liegt an, Zeitdauer Signal ermitteln
		// DEBUG_PRINTLN_VALUE(" Signaldauer messen von Taster gedr�ckt mit index i =   ", i);
		switch (i)  {
          case 0:
          //
          break;
          case 1:
		  //
          break;
          // --------------------------------------------------------------- Sonderfunktionen
          case 16:
		  //
          break;
          case 17:
		  //
          break;
          default:
          // Call void Parameter 1 und 2 ohne Verwendung der Hilfsvariablen vorkomma und Nachkomma
          rolladeEGKommando(i/2 , i%2 , 2 , startZeit[i]);       // Modus 2 mit konstantem Eingangssignal 24V
            break;
	    }   //  Ende switch

	  }  //  if (debouncer[i].rose() &&  (debouncer[i].read() == HIGH))


    }  //  Ende  for (int i = 0; i < NUMBUTTONS; i++)
  }  // Ende  if ( startsperre == false )
}  //  Ende  void auswertungA0bisI17 ()
