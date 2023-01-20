/*
  FAN CONTROLLER - ESP 8266 Node MCU project
  (c)' 2020 ELFRO Tomasz Fronczek
****************************************************************
  Program jest darmowy do dowolnego kopiowania i modyfikacji
  PROSZĘ POZOSTAWIĆ WIDOCZNĄ INFORMACJĘ O AUTORZE W OPROGRAMOWANIU.
  TUTAJ W SZKICU  JAK I NA WYŚWIETLANYCH STRONACH SERWERA.
  autor nie ponisi odpowiedzialności za wadliwe działanie oprogramowania - używasz na własne ryzyko i te wszystkie innne blablabla.
  licencja - zwróć uwagę na zapisy w użytych modułach
****************************************************************
****************************************************************
  The program is free for any copying and modification
  PLEASE LEAVE THE AUTHOR NOTICE IN THE SOFTWARE.
  HERE IN SKETCH AND ON THE DISPLAYED SERVER PAGES.
  the author is not responsible for any malfunctioning of the software - you use it at your own risk and all those other blah blah blah blah blah.
  license - see used modules and libraries
****************************************************************
  To Work You Need libraries:
  ESPAsyncTCP       - https://github.com/me-no-dev/ESPAsyncTCP
  ESPAsyncWebServer - https://github.com/me-no-dev/ESPAsyncWebServer
  ArduinoJson       - https://github.com/bblanchon/ArduinoJson  ver 5. For newer version You need a migrate code.
*/

// REMOVE DEBUG FOR LESS SERIAL PORT PRINTING INFO
#define DEBUG
//------------------------------
//------------------------------
#include "inits.h"                  // initialization includes defines constans variables  LOOK IN THIS FILE AND CONFIG IT
//------------------------------
//------------------------------
#include "io.h"                     // FAN PWM and AUX-Relay
#include "eeprom_data.h"            // save/load data to/from eeprom
#include "processorAndFanCfg.h"     // processors for serwer html pages - %VALUES% and read JSON Fan Config
#include "sys_setup.h"              // system setup -  read config, wifi init
#include "authentication.h"         // if web page authentication is nedeed
#include "server_handlers.h"        // handlers for requests html, graph, css, javascript
#include "sys_config_html.h"        // handler for syscfg.html file - system webpage configuration wifi, IP's, authentication
#include "fan_cfg_html.h"           // handler for fancfg.html - fans configuration page 



void setup() {
  Serial.begin(115200);
  initIO();             // initialize PWM and AUX
  printInfo();          // leave it please - (c)' 2020 ELFRO inż. Tomasz Fronczek
  readEEPROMValues();   // read last saved fan values from eepeom
  
  setupWiFi();          // config WiFi - read json cfg files, starts fans, connecting to WiFi - or AccesPoint mode if no WiFi(100tries=100sec loop), or no config, or config error.

  // server handlers
  index_html();         // main page
  about_html();         // about page
  config_html();        // system configuration - WiFi, IP, authentication
  fan_cfg_html();       // fans configuration and AUX
  get_fans_html();      // get fans and AUX values
  set_fans_html();      // set fans and AUX values
  get_images();         // get images, styles, javascript
  reset_html();         // reset controller page. After browser call /reset.html - ESP will reboot. It's recommended turn on authentication in config pages !!!

  server.onNotFound(indexHTML);  //not found go to main page

  server.begin();

  tenSec = millis();    //for 10 seconds loop
}


void loop() {
  
  boostCheck();                               // control fan boosting
  blinkAUX();                                 // blink aux if in status mode

  if (millis() - tenSec > 10000)              // check every 10 seconds
  {
    if (WiFi.status() != WL_CONNECTED)        // if no connection or connection lost
    {
      recon++;
      if (recon > 100) REBOOT();             // reboot after 50 tries - 1000 secs. Yes 50 (100 loops = 50 tries)
      if (recon % 2 == 0) WiFi.reconnect();   // try recconect every 20 seconds
    }
    else recon = 0;

    if (isInAP_Mode && millis() - apStartTime >  600000) REBOOT();  // reboot after 10 minutes if Access Point Mode
    tenSec = millis();
  }
}
