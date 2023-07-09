/*
Controllino Maxi und Librarie Shutters
*/

#include <Arduino.h>

#include <Bounce2.h>
#include <CONTROLLINO.h>

#include "global.h" // Nach CONTROLLINO.h, da hier die EIngänge verwendet werden

Bounce debouncer[NUMBUTTONS];

// folgende eigenen Ordner aus C:\Users\tkaif\OneDrive\Dokumente\Arduino direkt mit
// Drag&Drop in nebenstehenden lib - Ordner kopiert
// siehe sketch_jan15b_controllinino_BasicShutters_Index
#define _SMARTDEBUG
#include "smartdebug.h"

#include <Wire.h> // für die Verwendung von I2C Funktionen zwingend
/*
#ifdef CONTROLLINO_MAXI
#endif

#ifdef CONTROLLINO_MEGA
#endif
*/

#include "L45AShutters.h" // alle Funktionen für die Verwendung in Lindenallee 45a

#include "L45AControllino.h" // alle Definitionen für die Verwendung in Lindenallee 45a

#include "Shutters.h"
Shutters GWc(courseGWcTime, shuttersGWcUp, shuttersGWcDown, shuttersGWcHalt, shuttersGWcGetState, shuttersGWcSetState, calibrationRatio, onShuttersGWcLevelReached);
Shutters Kueche(courseKuecheTime, shuttersKuecheUp, shuttersKuecheDown, shuttersKuecheHalt, shuttersKuecheGetState, shuttersKuecheSetState, calibrationRatio, onShuttersKuecheLevelReached);
Shutters Essen(courseEssenTime, shuttersEssenUp, shuttersEssenDown, shuttersEssenHalt, shuttersEssenGetState, shuttersEssenSetState, calibrationRatio, onShuttersEssenLevelReached);
Shutters Terasse(courseTerasseTime, shuttersTerasseUp, shuttersTerasseDown, shuttersTerasseHalt, shuttersTerasseGetState, shuttersTerasseSetState, calibrationRatio, onShuttersTerasseLevelReached);

//
/*
#ifdef CONTROLLINO_MAXI
Shutters Erker1(courseErker1Time, shuttersErker1Up, shuttersErker1Down, shuttersErker1Halt, shuttersErker1GetState, shuttersErker1SetState, calibrationRatio, onShuttersErker1LevelReached);
Shutters Erker2(courseErker2Time, shuttersErker2Up, shuttersErker2Down, shuttersErker2Halt, shuttersErker2GetState, shuttersErker2SetState, calibrationRatio, onShuttersErker2LevelReached);
Shutters Erker3(courseErker3Time, shuttersErker3Up, shuttersErker3Down, shuttersErker3Halt, shuttersErker3GetState, shuttersErker3SetState, calibrationRatio, onShuttersErker3LevelReached);
#endif
*/
//
Shutters Bad(courseBadTime, shuttersBadUp, shuttersBadDown, shuttersBadHalt, shuttersBadGetState, shuttersBadSetState, calibrationRatio, onShuttersBadLevelReached);
Shutters Schlaf(courseSchlafTime, shuttersSchlafUp, shuttersSchlafDown, shuttersSchlafHalt, shuttersSchlafGetState, shuttersSchlafSetState, calibrationRatio, onShuttersSchlafLevelReached);
Shutters ArbSued(courseArbSuedTime, shuttersArbSuedUp, shuttersArbSuedDown, shuttersArbSuedHalt, shuttersArbSuedGetState, shuttersArbSuedSetState, calibrationRatio, onShuttersArbSuedLevelReached);
Shutters ArbWest(courseArbWestTime, shuttersArbWestUp, shuttersArbWestDown, shuttersArbWestHalt, shuttersArbWestGetState, shuttersArbWestSetState, calibrationRatio, onShuttersArbWestLevelReached);

const int NUM_Shutters = 2;
Shutters ShutterArray[NUM_Shutters] = {
    {courseBadTime, shuttersBadUp, shuttersBadDown, shuttersBadHalt, shuttersBadGetState, shuttersBadSetState, calibrationRatio, onShuttersBadLevelReached},
    {courseSchlafTime, shuttersSchlafUp, shuttersSchlafDown, shuttersSchlafHalt, shuttersSchlafGetState, shuttersSchlafSetState, calibrationRatio, onShuttersSchlafLevelReached}};

#include "L45AAuswertungTasten.h"
#include <L45AI2CPort.h>

// folgende Datei in nebenstehendem include-Ordner mit linker Maus-Taste und Dateiname
// angelegt und im Editor mit Copy und Paste auch aus *.ino Dateien kopiert
// #include "IO_Tools.h" am 10.07.22 auskommentiert Redefinition von void lifeLedD11( waitTime)
// in L45AControllino.h

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < NUMBUTTONS; i++)
  {
    pinMode(buttonPins[i], INPUTMODE);
    debouncer[i].attach(buttonPins[i]);
    debouncer[i].interval(entprellZeit);
    // debouncer[i].setPressedState(HIGH);
  }

  defineControllinoRelais(); // in L45AControllino.h
  initI2C();

  /*
      in sketch_mar02a_controllinino_BasicShutters_Bounce2_Index Kommentar gefunden:
      defineShutters();   in L45AShutters.h mit den folgenden *.beginn() ergibt Kompilerfehler,
      daher muß die folgende void im Hauptprogramm stehen
      setupRolladen();
  */

  // pinMode(CONTROLLINO_D0, OUTPUT);  Definition in L45AShutters
  // D23 zeigt das Lebenslicht an

  // pinMode(CONTROLLINO_D11, OUTPUT);    // wird bei dem Programmieren als Lebeslicht verwendet
  DEBUG_PRINTLN("der erste Programmiertest mit PlatformIO auf dem Controllino Maxi und DEBUG_PRINTLN");
#ifdef CONTROLLINO_MAXI
  DEBUG_PRINTLN("Board ist der Controllino Maxi");
#endif
#ifdef CONTROLLINO_MEGA
  DEBUG_PRINTLN("Board ist der Controllino Mega");
#endif

  setupDebouncer();
}

void loop()
{
  // put your main code here, to run repeatedly:
  lifeLedD11(750);
  // Zentral.loop();
   ShutterArray[0].loop();
   ShutterArray[1].loop();
 
  GWc.loop();
  Kueche.loop();
  Essen.loop();
  Terasse.loop();
#ifdef CONTROLLINO_MEGA
  Erker1.loop();
  Erker2.loop();
  Erker3.loop();
#endif
  Bad.loop();
  Schlaf.loop();
  ArbSued.loop();
  ArbWest.loop();

  // folgende 2 void in L45AAuswertungTasten.h
  checkStartsperre();
  if (startsperre == false)
  {
    auswertungA0bisI17();  // in <L45AAuswertungTasten.h>
    auswertungI2CPort63(); // in <L45AI2CPort.h>
  }
}

void setupRolladen()
{
  //  Erdgeschoss
  //  Zentral.begin();
  GWc.begin();
  Kueche.loop();
  Essen.loop();
  Terasse.loop();
  Kueche.begin();
  Essen.begin();
  Terasse.begin();
#ifdef CONTROLLINO_MEGA
  Erker1.begin();
  Erker2.begin();
  Erker3.begin();
#endif
  Bad.begin();
  Schlaf.begin();
  ArbSued.begin();
  ArbWest.begin();
}