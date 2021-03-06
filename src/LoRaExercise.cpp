#include <stdint.h>
#include <SodaqRTC.h>
//#include "HeaderFiles/commonMacros.h"
#include "HeaderFiles/LoRaExercise.h"

float getTemperature();
void configRTC();
void RTC_ISR();
void printEpochTime(SodaqRTC rtc);

SodaqRTC rtc;
bool RTCTriggered = false;

uint32_t epoch = 1508381907;


void setup()
{
   SerialUSB.begin(57600);
//   while(!SerialUSB);
   debug("Initialising...\n");

   configLoRa();
   configRTC();

   for( int i = 1; i <= 5; i++){
      digitalWrite(LED_BUILTIN, HIGH);   delay(1000);
      digitalWrite(LED_BUILTIN, LOW);    delay(1000);
      debug(".");
   }
   debug("Epoch is: "); debugl(epoch);

   SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  //Set deep sleep mode
   debugl(" ...Starting \n");
}



void configRTC()
{

   debugl("initialise RTC");

   rtc.begin();                        //start timer
   rtc.setEpoch(epoch);  //set time
   rtc.setAlarmEpoch(epoch + PERIOD);  //set alarm time to go off in 10 seconds

   rtc.enableAlarm(rtc.MATCH_SS);      //set time to interrupt
   rtc.attachInterrupt(RTC_ISR);       //Attach ISR function to timer interrupt
   debugl("RTC Alarm Set");

}



void loop()
{

   if (RTCTriggered){
      float temp = getTemperature();
      printEpochTime(rtc);
      LoRaSend(temp);
      epoch = rtc.getEpoch();
      rtc.setAlarmEpoch(epoch+PERIOD);
      RTCTriggered = 0;

   }

   __WFI();




}


void printEpochTime(SodaqRTC rtc)
{
   int printFinished = 0;

   while(!printFinished){

      debug(rtc.getDay()); flush; debug("/"); flush;
      debug(rtc.getMonth()); flush; debug("/"); flush;
      debug(rtc.getYear()); flush; debug(" - "); flush;

      debug(rtc.getHours()+11); flush; debug(":"); flush;
      debug(rtc.getMinutes()); flush; debug(":"); flush;
      debug(rtc.getSeconds()); flush; debug(" \t"); flush;
      printFinished = 1;
   }
}



float getTemperature()
{
   //10mV per C, 0C is 500mV
   float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
   float temp = ((mVolts - 500.0) / 10.0)-5;

   return temp;
}



//RTC periodic ISR
void RTC_ISR()
{
   RTCTriggered = true;
}
