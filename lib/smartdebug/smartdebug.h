/*********************************************
 * smartdebug
 * Debug - Makros für die Arduino - IDE
 *
 * 12.08.2018 Vers. 2.0
 * Der Hobbyelektroniker
 * der.hobbyelektroniker@gmail.com
 * https://www.youtube.com/c/HobbyelektronikerCh (Youtube - Kanal)
 * https://community.hobbyelektroniker.ch
 * https://www.hobbyelektroniker.ch
 *
 * Der Code ist lizenzfrei und darf von jedermann frei verwendet werden
 * Trotzdem freut es mich natürlich, wenn dieser Kommentar stehen bleibt ;-)
 * Feedback, Verbesserungen und Erweiterungen sind ebenfalls sehr erwünscht.
 *********************************************/
/*
 30.01.22, Kaifer, DEBUG_PRINT, DEBUG_PRINTLN auf F-Makro umgestellt
*/


#ifdef _SMARTDEBUG
  // Hilfsfunktion für WAIT - Makro
  void DebugWait(String txt) {
    // buffer leeren
    char ch;
    while (Serial.available()) ch = Serial.read();
    ch = 0;

    Serial.print(txt);
    Serial.println(" >press 'c' to continue...");

    // auf 'c' warten
    do {
      if (Serial.available() > 0) ch = Serial.read();
    } while (ch != 'c');
    // buffer leeren
    while (Serial.available()) ch = Serial.read();
  }

  // Initialisierung
  #define DEBUG_INIT(speed) Serial.begin(speed)

  // Textausgabe
  #define DEBUG_PRINT(txt) Serial.print(F(txt))
  #define DEBUG_PRINTLN(txt) Serial.println(F(txt))
  #define DEBUG_PRINT_VALUE(txt, val) Serial.print(F(txt)); Serial.print(": "); Serial.print(val)
  #define DEBUG_PRINTLN_VALUE(txt, val) Serial.print(F(txt)); Serial.print(": "); Serial.println(val)
  #define DEBUG_PRINTLN_EMPTY() Serial.println()
  // neu am 19.03.22 zur Ausgabe eines Wertes im Binär-Format
  #define DEBUG_PRINTLN_VALUEBIN(txt, val) Serial.print(F(txt)); Serial.print(": "); Serial.println(val, BIN)

  // andere Befehle nicht in Verbindung mit Shutters.h verwenden
  #define DEBUG_DELAY(ms) delay(ms)
  #define DEBUG_WAIT(txt, condition) if (condition) DebugWait(txt)
#else
  // Leere Makro - Rümpfe, für den Fall, dass kein Debugging erwünscht ist
  #define DEBUG_INIT(speed)
  #define DEBUG_PRINT(txt)
  #define DEBUG_PRINTLN(txt)
  #define DEBUG_PRINT_VALUE(txt, val)
  #define DEBUG_PRINTLN_VALUE(txt, val)
  #define DEBUG_PRINTLN_EMPTY()
  #define DEBUG_DELAY(ms)
  #define DEBUG_WAIT(txt, condition)
  // neu am 19.03.22 zur Ausgabe eines Wertes im Binär-Format
  #define DEBUG_PRINTLN_VALUEBIN(txt, val)
#endif
