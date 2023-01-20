

void REBOOT()
{ delay(1000);
  ESP.reset();
  delay(1000);
}

// Going to Acces Point Mode for configuration
void AP_MODE()
{
  WiFi.disconnect();
  WiFi.softAP(FAN_AP_NAME);
#ifdef DEBUG
  Serial.print("Acces point name:");
  Serial.println(FAN_AP_NAME);
  Serial.print("My IP is: 192.168.4.1");
#endif
  isInAP_Mode = true;
  apStartTime = millis();
}



bool  readSystemConfig()
{
  bool jsonOK = true;
  if (SPIFFS.exists("/syscfg.json")) {
    //file exists, reading and loading
    printMeLn("==========================");
    Serial.println("Reading system config file");
    File configFile = SPIFFS.open("/syscfg.json", "r");
    if (configFile) {
      printMeLn("Opened system config file");
      size_t size = configFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);
      configFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
#ifdef DEBUG
      json.printTo(Serial);
#endif
      configFile.close();
      if (json.success()) {
        printMeLn("\nParsed json");
        if (json.containsKey("SSID")) strcpy(ssid, json["SSID"]); else  {
          printJsonErr("SSID");
          jsonOK = false;
        }
        if (json.containsKey("PASS1")) strcpy(PASS, json["PASS1"]); else  {
          printJsonErr("PASS1");
          jsonOK = false;
        }
        char isDHCP[16];
        if (json.containsKey("isDHCP")) strcpy(isDHCP, json["isDHCP"]); else  {
          printJsonErr("isDHCP");
          jsonOK = false;
        }
        if (isDHCP[0] == 'f' || isDHCP[0] == 'F' ) DHCP = false; else DHCP = true;
        char ip[16], gate[16], mask[16], dns1[16], dns2[16];
        if (json.containsKey("IP"))strcpy(ip, json["IP"]); else  {
          printJsonErr("IP");
          jsonOK = false;
        }
        if (json.containsKey("GATE"))strcpy(gate, json["GATE"]); else  {
          printJsonErr("GATE");
          jsonOK = false;
        }
        if (json.containsKey("MASK"))strcpy(mask, json["MASK"]); else  {
          printJsonErr("MASK");
          jsonOK = false;
        }
        if (json.containsKey("DNS1"))strcpy(dns1, json["DNS1"]); else  {
          printJsonErr("DNS1");
          jsonOK = false;
        }
        if (json.containsKey("DNS2"))strcpy(dns2, json["DNS2"]); else  {
          printJsonErr("DNS2");
          jsonOK = false;
        }
        IP.fromString(ip);
        GATE.fromString(gate);
        MASK.fromString(mask);
        DNS1.fromString(dns1);
        DNS2.fromString(dns2);
        char secLev[16];
        if (json.containsKey("securityLevel"))
        { strcpy(secLev, json["securityLevel"]);
          securityLevel = (secLev[0] - '0');
        }
        else  jsonOK = false;
        if (json.containsKey("userName")) strcpy(userName, json["userName"]); else  {
          printJsonErr("userName");
          jsonOK = false;
        }
        if (json.containsKey("userPass1")) strcpy(userPass, json["userPass1"]); else  {
          printJsonErr("userPass1");
          jsonOK = false;
        }
        if (!jsonOK) Serial.println("Json system config file have bad data");
        else Serial.println("Json system config file data readed, parsed, werified - OK");
      } else {
        Serial.println("Failed to load json system config");
        jsonOK = false;
      }
    }
    else      {
      Serial.println("No system config file");
      jsonOK = false;
    }

  } else {
    Serial.println("No system config file");
    jsonOK = false;
  }
  return jsonOK;
}






void setupWiFi()
{
  printMeLn("=== Setup ===");
  bool jsonOK = true;
  printMeLn("Read config files ...");
  if (SPIFFS.begin()) {
    printMeLn("Mounted file system");
    jsonOK = readSystemConfig();
    readFanConfig();
  } else  {
    printMeLn("Failed to mount FS");
    jsonOK = false;
  }

  printMeLn("==========================");
  // Fan boost prepare
  for (int i = 0; i < 4; i++) if (fanBoost[i]) inBoost = true;
  boostTime = millis();

  procedIO();           // set fan values
  relayAUX(RELAY);      // set last relay state if in relay mode

  if ( jsonOK) {



    WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration

    WiFi.hostname(FAN_AP_NAME);
#ifdef DEBUG
    Serial.print("Connect to Network (SSID):"); Serial.println(ssid);
    Serial.print("Password :"); Serial.println(PASS);
#endif
    if (!DHCP)
    {
#ifdef DEBUG
      Serial.println ("Static IP:");
      Serial.println(IP.toString());
      Serial.println(GATE.toString());
      Serial.println(MASK.toString());
      Serial.println(DNS1.toString());
      Serial.println(DNS2.toString());
#endif
      WiFi.config(IP, GATE, MASK, DNS1, DNS2);
    }
    else printMeLn("Dynamic IP: DHCP");
    WiFi.begin(ssid, PASS);

    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 100) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
      BLINKME();
      boostCheck();
      i++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("Connected to: ");
      Serial.println(ssid);
      Serial.print("My IP is: ");
      Serial.println(WiFi.localIP());
      printMeLn("============== CONFIG OK ==============");
    }
    else
    {
      Serial.println("Not connected to WiFi - Acces Point Mode");
      AP_MODE();
      auxSpeed = 300;
      BLINKME();
      boostCheck();
    }
  }
  else
  {
    Serial.println("Configuration file not exists or bad - Acces Point Mode");
    AP_MODE();
    auxSpeed = 300;
    BLINKME();
    boostCheck();
  }


}
