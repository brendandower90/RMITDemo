#include <stdint.h>
//#include "HeaderFiles/commonMacros.h"
#include "HeaderFiles/LoRaExercise.h"

void changeLED(char* colour){
   debug("Making LED "); debugl(colour);
   if (!strcmp(colour,"red"))
      RGB(RED);
   else if (!strcmp(colour,"green"))
      RGB(GREEN);
   else if (!strcmp(colour,"blue"))
      RGB(BLUE);
   else if (!strcmp(colour,"cyan"))
      RGB(CYAN);
   else if (!strcmp(colour,"yell"))
      RGB(YELLOW);
   else if (!strcmp(colour,"pink"))
      RGB(MAGENTA);
   else if (!strcmp(colour,"off")){
      RGBOFF();
   }
   else
      debugl("ERROR: Invalid Colour");
}

void addLED(char* colour){
   debug("Turning "); debug(colour); debugl("LED on");
   if (!strcmp(colour,"red"))
      digitalWrite(LED_RED, LOW);
   else if (!strcmp(colour,"green"))
      digitalWrite(LED_GREEN, LOW);
   else if (!strcmp(colour,"blue"))
      digitalWrite(LED_BLUE, LOW);
   else if (!strcmp(colour,"cyan")){
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_GREEN, LOW);
   }
   else if (!strcmp(colour,"yell")){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN,LOW);
   }
   else if (!strcmp(colour,"pink")){
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_BLUE, LOW);
   }
   else if (!strcmp(colour,"white")){
      RGB(WHITE);
   }
   else if (!strcmp(colour,"off")){
      RGBOFF();
   }
   else
      debugl("ERROR: Invalid Colour");
}

void subLED(char* colour){
   debug("Turning "); debug(colour); debug("LED off");
   if (!strcmp(colour,"red"))
      digitalWrite(LED_RED, HIGH);
   else if (!strcmp(colour,"green"))
      digitalWrite(LED_GREEN, HIGH);
   else if (!strcmp(colour,"blue"))
      digitalWrite(LED_BLUE, HIGH);
   else if (!strcmp(colour,"cyan")){
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_GREEN, HIGH);
   }
   else if (!strcmp(colour,"yellow")){
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN,HIGH);
   }
   else if (!strcmp(colour,"magenta")){
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_BLUE, HIGH);
   }
   else if (!strcmp(colour,"white")){
      strcpy(colour,"off");
   }
   else if (!strcmp(colour,"off")){
      RGBOFF();
   }
   else
      debugl("ERROR: Invalid Colour");
}

void RGBOFF()
{
   digitalWrite(LED_BLUE, HIGH);
   digitalWrite(LED_RED, HIGH);
   digitalWrite(LED_GREEN,HIGH);
}

void RGB(byte r, byte g, byte b)
{
   byte ri = ~r; byte gi = ~g; byte bi = ~b;
   analogWrite(ledR, ri); analogWrite(ledG, gi); analogWrite(ledB, bi);
   //debug(ri); nbsp; debug(gi); nbsp; debug(bi); endl;
   return;
}
