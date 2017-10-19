#include <stdint.h>
//#include "HeaderFiles/commonMacros.h"
#include "HeaderFiles/LoRaExercise.h"


void deserialiseReceivedPacket(char* receivedPacket, int receivedSize)
{
   //Check for a code at the start of the packet.
   // 0. Reserved
   // 1. RTC Init
   // 2. Config Change

   int indexCode = atoi(receivedPacket);

   char* tmp;
   tmp = (char*)malloc(receivedSize);
   memset(tmp,'\0',receivedSize);
   strcpy(tmp, receivedPacket);

   tmp = strtok(tmp,":");
   tmp = strtok(NULL,":");

   strcpy(receivedPacket,tmp);

   switch (indexCode){
      case 0:
         //Reserved
         break;

      case 1:
         changeEpochTime(receivedPacket, receivedSize-2);
         break;

      case 2:
         changeConfig(receivedPacket,receivedSize-2);
         break;



   }

}

void changeEpochTime(char* epochString, int len)
{
   uint32_t epochTime;

   epochTime = (uint32_t) epochString;
   RTC->MODE2.CLOCK.reg = epochTime;

}


void changeConfig(char* configChangeString, int len)
{
   int i = 0;
   int numOfParts = 2;
   char* section;
   char* part[numOfParts];

   section = (char*)malloc(len);
   memset(section,'\0',len);
   strcpy(section,configChangeString);
   section = strtok(section, "-");

   while (section != NULL) {
      while(i < numOfParts){
         part[i] = section;
         section = strtok(NULL,"-");
         i++;
      }
   }

   const char* key = part[0];
   char* val = part[1];

   if (!strcmp(key, "led")){
      changeLED(val);
   }
   else if (!strcmp(key, "add")){
      addLED(val);
   }
   else if (!strcmp(key, "sub")){
      subLED(val);
   }
   else
      debugl("ERROR - Unknown Parameter");

}
