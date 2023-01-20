#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include <DNSServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define AUX D5

/// CONFIG ///
String lang="/en";  // languages are in sub folders   ex "/en" "/pl if language not used remove this line and lang variable from all code.

const char* AUTH_REQ = "Enter User Name and Password.";  //Enter user name and password string
const char* FAN_AP_NAME ="FAN_CONTROLLER";

char ssid[33] = "MY_WIFI_NETWORK";    // default WiFi SSID
char PASS[33] = "pass";               // default WiFi Password
bool DHCP = true;                     // enable DHCP

// default IP address, gateway etc. if no DHCP.
IPAddress IP(10, 0, 0, 95);
IPAddress  GATE(10, 0, 0, 138);
IPAddress  MASK(255, 255, 255, 0);
IPAddress  DNS1(194, 204, 152, 34);
IPAddress  DNS2(194, 204, 159, 1);

byte securityLevel = 0; /// - 0-none 1-only config 2-all
char userName[33] = "admin";  // security username
char userPass[33] = "admin";  // security pasword

char auxName[129]="AUX Relay OUT"; // aux relay default name


/// END CONFIG ///

AsyncWebServer server(80);  // WEB serwer at port 80

//FANS
bool fanEnabled[4] = {0, 0, 0, 0};                  // all fans disabled by default
int fanValues[4] = {511, 511, 511, 511};            // default fan value - here is 50%
byte fanPins[4] = {D1, D2, D3, D6};                 // Pins assigned to fan1,fan2 etc..

int numberOf_Fans = 4;                // How many fans (1,2,3,4) in system
char fan1[129] = "1. Fan 1";   // FAN1 default name
char fan2[129] = "2. Fan 2";   // FAN2 default name
char fan3[129] = "3. Fan 3";   // FAN3 default name
char fan4[129] = "4. Fan 4";   // FAN4 default name

bool isInAP_Mode = false;  // is server in Acces Point mode ?? 
unsigned long apStartTime; // Acces point start time (to reset after 10 min.)

unsigned long tenSec = 0;   // 10 sec loop
int recon = 0;              // if WiFi disconnected - no. of retires
unsigned long halfSec = 0;  // 0.5 sec loop

bool blinkMe = 0;           // blink AUX
byte auxStatus = 1;         // 0 - not used 1- info 2- relay
int auxSpeed = 1500;        // default speed of blinking

byte RELAY=0;                          // relay status (ON OFF)   

int PWMFreq=22000;                       // PWM Frerquency 22kHz. default is 1 kHz. You can hear it in motor ex beeeeppp.... 22kHz is inaudible

bool  inBoost=true;                     // is fan in Boost mode - for small pwm values less 50% fans can starts faster(50%) for two seconds
unsigned long boostTime;                // used for couint 2 sec.
bool isFanBoosted[4] = {1, 1, 1, 1};    // is this fan in bost mode now ???
bool fanBoost[4] = {1, 1, 1, 1};        // config - can bost fans ???
int minFanValues[4] = {102, 102, 102, 102}; // minimum fan value - here is default 10%-102 

void printInfo()
{
  Serial.println(); Serial.println(); Serial.println();
  Serial.println("==========================");
  Serial.print(FAN_AP_NAME);
  Serial.println(" v1.0");
  Serial.println("(c)'2020 ELFRO Tomasz Fronczek");
  Serial.println("==========================");
}

void printJsonErr(const char* err)
{
#ifdef DEBUG
  Serial.print("ERROR json value for\""); Serial.print(err); Serial.print("\" not found");
#endif
}
void printMeLn(const char* txt)
{
#ifdef DEBUG
  Serial.println(txt);
#endif
}
