#include <stdint.h>
#include <TheThingsNetwork.h>
#include "HeaderFiles/commonMacros.h"
#include "HeaderFiles/LoRaExercise.h"

TheThingsNetwork ttn(loraSerial,debugSerial,freqPlan);

const char *devAddr = "0C5000C6";
const char *nwkSKey = "72CC77830CC4BDB177F68C55D1EE7C33";
const char *appSKey = "772E22E70C2D495DCFEC39877DE8CB16";

//Callback when Message received
void message(const uint8_t *payload, size_t size, port_t port);

//Global Variables for callback
char* receivedPacket;
volatile int receivedSize;


void configLoRa()
{
   loraSerial.begin(57600);
   ttn.personalize(devAddr, nwkSKey, appSKey);
   ttn.showStatus();
   ttn.onMessage(message);
}

uint8_t testPayload[] =
{
  0x53, 0x4F, 0x44, 0x41, 0x52
};


const char* getPacket(float temp)
{
   temp = roundf(temp * 100) / 100;

   int len = 15;
   char* packet;
   packet = (char*)malloc(len);
   memset(packet,'\0',len);

   const char str_start[] = "{\"temp\":\"";
   char str_temp[6];
   sprintf(str_temp,"%g",temp);
   const char str_end[] = "\"}";

   strcat(packet, str_start);
   strcat(packet, str_temp);
   strcat(packet, str_end);

   return packet;
}


void LoRaSend(int temp)
{
const char* packet = getPacket(temp);
int len = strlen(packet);

int response = ttn.sendBytes((uint8_t*)packet,len);
switch(response){
   case 1:
      debug("Transmission Successful: ");
      debug(packet); endl;
      break;
   case 2:
      debug("Transmission Successful: ")
      debugl(packet);
      debugl("\n\t -- DOWNLINK MESSAGE RECEIVED -- ");

      debug(">> ASCII: \"");
      for (int i = 0; i < receivedSize; i++){
         debug(receivedPacket[i]);
      }
      debugl("\"\n");

      deserialiseReceivedPacket(receivedPacket, receivedSize);

      break;
   }
}



//Callback function when message is received
void message(const uint8_t *payload, size_t size, port_t port)
{
   char* tmp = (char*) payload;
   receivedPacket = (char*)malloc(size);
   strcpy(receivedPacket, tmp);
   receivedSize = size;

}
