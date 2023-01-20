
void fan_cfg_html()
{
  server.on("/fancfg.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authSystemPages(request)) return;
    request->send(SPIFFS, lang+"/fancfg.html",   String(), false, processor);
  });
  server.on("/fancfg.html", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (!authSystemPages(request)) return;
    int params = request->params();
    // for (int i = 0; i < params; i++) {
    if (params > 0) {
      AsyncWebParameter* p = request->getParam(0);
      if (p->isPost()) {
#ifdef DEBUG
        Serial.println( p->value());
#endif
        // zapis
        File configFile = SPIFFS.open("/fancfg.json", "w");
        if (!configFile) {
#ifdef DEBUG
          Serial.println("Failed to open fan config file for writing");
#endif

        }
        else {
          request->send(200, "text/plain", "OK");
          configFile.print(p->value());
          configFile.close();
#ifdef DEBUG
          Serial.println("Saved fan config file");
#endif
          readFanConfig();
          procedIO();
          return;
        }


      }

    }
    request->send(405, "text/plain", "ERR");
  }
           );
}
