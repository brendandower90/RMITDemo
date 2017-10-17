#include "Arduino.h"

//LED Macros
#define ledR LED_RED
#define ledG LED_GREEN
#define ledB LED_BLUE

//debugging Macros
#define debugSetup SerialUSB.begin(57600);
#define debug(x) SerialUSB.print(x);
#define debugl(x) SerialUSB.println(x);
#define nbsp SerialUSB.print(" ");
#define endl SerialUSB.print("\n");
#define tab SerialUSB.print("\t");
#define flush SerialUSB.flush();

//timing Macros
#define ms(x) delay(x)
#define hms(x) delay(x*100)
#define sec(x) delay(x*1000)


//Colour Definitions
#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255
#define MAGENTA 255,0,255
#define YELLOW 255,255,0
#define CYAN 0,255,255
#define WHITE 255,255,255
#define OFF 0,0,0

//Serial Macros
#define loraSerial Serial2
#define debugSerial SerialUSB

//LoRaWAN Calibration
#define freqPlan TTN_FP_US915

//RTC Macros
#define PERIOD 30    //Interval of Sleeping

//LoRaWAN Functions
void configLoRa();
void LoRaSend(int temp);

//Deserializing functions
void deserialiseReceivedPacket(char* receivedPacket, int receivedSize);
void changeEpochTime(char* EpochTime, int len);
void changeConfig(char* configChangeString, int len);

//LED Control
void changeLED(char* colour);
void addLED(char* colour);
void subLED(char* colour);
void RGB(byte r, byte g, byte b);
void RGBOFF();
