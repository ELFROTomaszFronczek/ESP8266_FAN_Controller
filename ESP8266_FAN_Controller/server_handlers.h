void indexHTML(AsyncWebServerRequest * request)
{
  if (!authUserPages(request)) return;
  request->send(SPIFFS, lang+"/index.html",  String(), false, processor);
}

void index_html()
{ server.on("/", HTTP_GET, &indexHTML);
  server.on("/index.html", HTTP_GET, &indexHTML);
  server.on("/index.htm", HTTP_GET, &indexHTML);
  server.on("/index", HTTP_GET, &indexHTML);
}


void set_fans_html()
{
  server.on("/set_fans.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authUserPages(request)) return;
    bool isDirty = false;
    String inputMessage = "";
    for (byte i = 0; i < 4; i++)
    {
      String e1 = "e";
      String e = e1 + String(i);
      if (request->hasParam(e)) {
        inputMessage = request->getParam(e)->value();
        byte old = fanEnabled[i];
        if (inputMessage == "1") fanEnabled[i] = 1; else fanEnabled[i] = 0;
        if (old != fanEnabled[i]) isDirty = true;
        if (old == 0 && fanEnabled[i] != 0)
        {
          if (fanBoost[i])
          {
            inBoost = true;
            boostTime = millis();
            isFanBoosted[i] = 1;
            Serial.print("BOOST ");
            Serial.println(i);
          }
        }
      }
    }
    for (byte i = 0; i < 4; i++)
    {
      String v1 = "v";
      String v = v1 + String(i);
      if (request->hasParam(v)) {
        inputMessage = request->getParam(v)->value();
        int val = inputMessage.toInt();
        if (val < 0) val = 0;
        if (val > 100) val = 100;
        byte old = fanValues[i];
        val = (int)round(val * 10.23);
        if (val > 1020) val = 1023;
        fanValues[i] = val;

        if (old != fanValues[i]) isDirty = true;
        if (old == 0 && fanValues[i] != 0)
        {
          if (fanBoost[i])
          {
            inBoost = true;
            boostTime = millis();
            isFanBoosted[i] = 1;
            Serial.print("BOOST ");
            Serial.println(i);

          }
        }
      }
    }
    if (request->hasParam("rel")) {
      inputMessage = request->getParam("rel")->value();
      int val = inputMessage.toInt();
      if (val > 1) val = 1;
      if (val < 0) val = 0;
      byte  old = RELAY;
      RELAY = (byte)val;

      if (old != RELAY)
      {
        writeRelayState();
        relayAUX(RELAY);
      }

    }


    if (isDirty)
    {
      writeFanValues();
      procedIO();
    }
    request->send(200, "text/html", "OK");
  });
}

void get_fans_html()
{
  server.on("/get_fans.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authUserPages(request)) return;
    request->send(SPIFFS, lang+"/get_fans.html", String(), false, processor2);
  });
}

void about_html()
{
  server.on("/about.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authUserPages(request)) return;
    request->send(SPIFFS, lang+"/about.html", "text/html");
  });
}

void get_images()
{
  server.on("/fan_off.gif", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/fan_off.gif", "image/gif");
  });
  server.on("/fan_working.gif", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/fan_working.gif", "image/gif");
  });
  server.on("/menu.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/menu.png", "image/png");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/functions.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authUserPages(request)) return;
    request->send(SPIFFS, lang+"/functions.js", "text/javascript");
  });
}



void reset_html()
{
  server.on("/reset.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!authSystemPages(request)) return;
    request->send(200, "text/plain", "OK");
#ifdef DEBUG
    Serial.print("REBOOT THE MAIN SYSTEM");
#endif
    delay(2000);
    ESP.reset();
    delay(2000);
  });
}
