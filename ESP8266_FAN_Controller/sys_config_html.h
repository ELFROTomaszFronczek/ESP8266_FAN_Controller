String sys_processor(const String& var) {

  if (var == "SECLEVEL") return String(securityLevel);
  if (var == "DHCP")
      if (DHCP) return "1"; else return "0";

  if (var == "SSID") return String(ssid);
  if (var == "PASS1")return String(PASS);
  if (var == "PASS2")return String(PASS);

  if (var == "IP") return IP.toString();
  if (var == "GATE") return GATE.toString();
  if (var == "MASK") return MASK.toString();
  if (var == "DNS1") return DNS1.toString();
  if (var == "DNS2") return DNS2.toString();

  if (var == "USER") return String(userName);
  if (var == "UPASS1")return String(userPass);
  if (var == "UPASS2")return String(userPass);
  return var;
}

void config_html()
{
  server.on("/syscfg.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authSystemPages(request)) return;
    request->send(SPIFFS, lang+"/syscfg.html",   String(), false, sys_processor);
  });
  
  server.on("/syscfg.html", HTTP_POST, [](AsyncWebServerRequest * request) {
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
        File configFile = SPIFFS.open("/syscfg.json", "w");
        if (!configFile) {
#ifdef DEBUG
          Serial.println("Failed to open system config file for writing");
#endif
        }
        else {
          request->send(200, "text/plain", "OK");
          configFile.print(p->value());
          configFile.close();
#ifdef DEBUG
          Serial.println("Saved system config file");
#endif
          return;
        }
      }
    }
    request->send(405, "text/plain", "ERR");
  });
}
