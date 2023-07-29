# Arduino Shutters

[![Build Status](https://travis-ci.org/marvinroger/arduino-shutters.svg?branch=master)](https://travis-ci.org/marvinroger/arduino-shutters) [![Latest version](https://img.shields.io/github/release/marvinroger/arduino-shutters.svg)](https://github.com/marvinroger/arduino-shutters/releases/latest)

This Arduino library allows non-smart roller-shutters to be percentage-controlled using time.
Using relays, it is easy to make shutters go up and down. But I wanted to be able
to make the shutters go halfway (50%) for example. So I built this library.
Diese Arduino-Bibliothek ermöglicht es, nicht-intelligente Rollläden über die Zeit prozentual zu steuern.
Mit Relais ist es einfach, Rollläden hoch und runter zu fahren. Aber ich wollte es können
um die Rollläden zum Beispiel halb hochzufahren (50 %). Also baute ich diese Bibliothek.

## Features

* Ability to set aperture percentage
Möglichkeit, den Blendenprozentsatz einzustellen

* Power outage safe
Stromausfallsicher
  * Shutters state saved using 1 int
  Jalousiezustand gespeichert mit 1 int
  
  * Store in EEPROM, SPIFFS, etc. using callbacks
  Speichern in EEPROM, SPIFFS usw. mit Callbacks
  
* Automatic calibration on extremes (0% and 100%)
Automatische Kalibrierung bei Extremen (0 % und 100 %)

* Flexible control method (might use relays, RF, etc.) using callbacks
Flexible Steuerungsmethode (kann Relais, RF usw. verwenden) unter Verwendung von Rückrufen

## Requirement
Erfordernis

* Measure as precisely as possible the time of a full shutters course
Messen Sie möglichst genau die Zeit eines vollen Rollladenverlaufs

* The initial state callback must return 255 on first boot
Der Initial State Callback muss beim ersten Booten 255 zurückgeben

## Installation

1. Download the [latest version](https://github.com/marvinroger/arduino-shutters/archive/master.zip)
2. Load the `.zip` with **Sketch → Include Library → Add .ZIP Library**

## API

See examples folder for examples.
Beispiele finden Sie im Beispielordner.

#### Shutters (unsigned long `courseTime`, void (\*`upCallback`)(void), void (\*`downCallback`)(void), void (\*`haltCallback`)(void), int (\*`getStateCallback`)(void), void (\*`setStateCallback`)(int state), float `calibrationRatio` = 0.1, void (\*`onLevelReachedCallback`)(int level))

* **`courseTime`**: Time in milliseconds to do a full shutters course
Zeit in Millisekunden für einen vollständigen Rollladenkurs
z.B. :  const unsigned long courseBadTime = 19 * 1000;

* **`upCallback()`**: Function to execute for the shutters to go up
Funktion zum Hochfahren der Rollläden

* **`downCallback()`**: Function to execute for the shutters to go down
Funktion zum Runterfahren der Rollläden

* **`haltCallback()`**: Function to execute for the shutters to halt
Funktion zum Anhalten der Rollläden

* **`getStateCallback()`**: Function to get state. This must return the state int, or 255 if you don't know the state int (on first boot)
Funktion zum Abrufen des Zustands. Dies muss das Zustandsint zurückgeben oder 255, wenn Sie das Zustandsint nicht kennen (beim ersten Start)

* **`setStateCallback(int state)`**: Function to set the state int. Store this in the EEPROM of SPIFFS, etc.
Funktion zum Setzen des Statusints. Speichern Sie dies im EEPROM von SPIFFS usw.


* **`calibrationRatio`**: The calibration ratio. If the full course is 30 sec. and the ratio is 0.1, the calibration time will be 30 * 0.1 = 3 sec. Defaults to 0.1
Das Kalibrierverhältnis. Wenn der gesamte Kurs 30 sek. und das Verhältnis 0,1 beträgt, beträgt die Kalibrierungszeit 30 * 0,1 = 3 Sekunden. Standardmäßig 0,1

* **`onLevelReachedCallback(int level)`**: Function to be called whenever a new level is reached
Funktion, die immer dann aufgerufen wird, wenn eine neue Ebene erreicht wird

#### void .begin ()

Setup the shutters. Must be called once in `setup()`.
Richten Sie die Rollläden ein. Muss einmalig in `setup()` aufgerufen werden.

#### void .setLevel (int `percentage`)

Put the shutters to the given position.
Note that if `percentage` == 0 || `percentage` == 100, the shutters will recalibrate (relays will stay active a bit longer than it should to ensure the shutters are really at their minimum or maximum position).
Bringen Sie die Rollläden in die angegebene Position.
Beachten Sie, dass wenn `Prozentsatz` == 0 || `Prozentsatz` == 100, die Rollläden werden neu kalibriert (Relais bleiben etwas länger aktiv als sie sollten, um sicherzustellen, 
dass die Rollläden wirklich in ihrer minimalen oder maximalen Position sind).


* **`percentage`**: Percentage the shutters must go to. If not 0 <= `percentage` <= 100, nothing will be done
Prozentsatz`**: Prozentsatz, bis zu dem die Rollläden fahren müssen. Wenn nicht 0 <= `Prozentsatz` <= 100, wird nichts getan


#### void .stop ()

Stop the shutters.   
Stoppen Sie die Fensterläden.

#### bool .isIdle ()

Return whether the shutters are currently idle or not.
Zurückgeben, ob die Rollläden derzeit im Leerlauf sind oder nicht.

#### void .loop ()

Handle the shutters. Must be called in `loop()`. **Don't call `delay()` in loop() as it will block the loop, so Shutters will malfunction.**
Behandeln Sie die Fensterläden. Muss in `loop()` aufgerufen werden. **Rufen Sie `delay()` nicht in loop() auf, da dies die Schleife blockiert, sodass Shutters nicht funktionieren.**


#### int .getCurrentLevel ()

Return the current level of the shutters. Might be +/- 1% if the shutters are moving.
Gibt die aktuelle Stufe der Rollläden zurück. Kann +/- 1 % betragen, wenn sich die Rollläden bewegen.

#### void .reset ()

Erase saved state, for example for a reset routine. This disables the library, so don't forget to restart/reset the Arduino.
Gespeicherten Zustand löschen, z. B. für eine Reset-Routine. Dies deaktiviert die Bibliothek, also vergessen Sie nicht, den Arduino neu zu starten/zurückzusetzen.