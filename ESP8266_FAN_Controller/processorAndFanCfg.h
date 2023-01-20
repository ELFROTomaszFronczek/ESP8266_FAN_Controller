String boostChecked(bool isChecked)
{ if (isChecked) return String("checked"); else return String(" ");
}



int fValTo100(int val)
{
  int i = round(val / 10.23);
  if (i == 0 && val != 0) i = 1;
  return i;
}

String processor(const String& var) {

  if (var == "EN0") return String(fanEnabled[0]);
  if (var == "EN1") return String(fanEnabled[1]);
  if (var == "EN2") return String(fanEnabled[2]);
  if (var == "EN3") return String(fanEnabled[3]);
  if (var == "VAL0") return String(fValTo100(fanValues[0] ));
  if (var == "VAL1") return String(fValTo100(fanValues[1] ));
  if (var == "VAL2") return String(fValTo100(fanValues[2] ));
  if (var == "VAL3") return String(fValTo100(fanValues[3] ));
  if (var == "ILEW") return String(numberOf_Fans);
  if (var == "FAN1") return String(fan1);
  if (var == "FAN2") return String(fan2);
  if (var == "FAN3") return String(fan3);
  if (var == "FAN4") return String(fan4);
  if (var == "ILEW") return String(numberOf_Fans);
  if (var == "FMIN1") return String(fValTo100(minFanValues[0] ));
  if (var == "FMIN2") return String(fValTo100(minFanValues[1] ));
  if (var == "FMIN3") return String(fValTo100(minFanValues[2] ));
  if (var == "FMIN4") return String(fValTo100(minFanValues[3] ));
  if (var == "FB1") return boostChecked(fanBoost[0] );
  if (var == "FB2") return boostChecked(fanBoost[1]);
  if (var == "FB3") return boostChecked(fanBoost[2]);
  if (var == "FB4") return boostChecked(fanBoost[3]);
  if (var == "AUX") return String(auxStatus);
  if (var == "AUXN") return String(auxName);
  if (var == "REL") return boostChecked(RELAY);
  if (var == "RELON") return String(RELAY);
  if (var == "PWMF") return String(PWMFreq); 

  return var;
}


String processor2(const String& var) {

  if (var == "E0") return String(fanEnabled[0]);
  if (var == "E1") return String(fanEnabled[1]);
  if (var == "E2") return String(fanEnabled[2]);
  if (var == "E3") return String(fanEnabled[3]);
  if (var == "V0") return String(fValTo100(fanValues[0] ));
  if (var == "V1") return String(fValTo100(fanValues[1] ));
  if (var == "V2") return String(fValTo100(fanValues[2] ));
  if (var == "V3") return String(fValTo100(fanValues[3] ));
  if (var == "RELON") return String(RELAY);
  

  return var;
}



bool  readFanConfig()
{
  bool jsonOK = true;
  if (SPIFFS.exists("/fancfg.json")) {
    //file exists, reading and loading
    printMeLn("==========================");
    Serial.println("Reading fan config file");
    File configFile = SPIFFS.open("/fancfg.json", "r");
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
        // strcpy(output, json["output"]);
        char ilew[10];

        if (json.containsKey("ilew"))
        {
          strcpy(ilew, json["ilew"]);
          numberOf_Fans = (ilew[0] - '0');
        } else  {
          printJsonErr("ilew");
          jsonOK = false;
        }

        if (json.containsKey("w1"))strcpy(fan1, json["w1"]); else  {
          printJsonErr("w1");
          jsonOK = false;
        }
        if (json.containsKey("w2"))strcpy(fan2, json["w2"]); else  {
          printJsonErr("w2");
          jsonOK = false;
        }
        if (json.containsKey("w3"))strcpy(fan3, json["w3"]); else  {
          printJsonErr("w3");
          jsonOK = false;
        }
        if (json.containsKey("w4"))strcpy(fan4, json["w4"]); else  {
          printJsonErr("w4");
          jsonOK = false;
        }

        if (json.containsKey("min1"))
        { strcpy(ilew, json["min1"]);
          minFanValues[0] = round(atoi(ilew)*10.23);
        } else  {
          printJsonErr("min1");
          jsonOK = false;
        }
        if (json.containsKey("min2"))
        { strcpy(ilew, json["min2"]);
          minFanValues[1] = round(atoi(ilew)*10.23);
        } else  {
          printJsonErr("min2");
          jsonOK = false;
        }

        if (json.containsKey("min3"))
        { strcpy(ilew, json["min3"]);
          minFanValues[2] = round(atoi(ilew)*10.23);
        } else  {
          printJsonErr("min3");
          jsonOK = false;
        }

        if (json.containsKey("min4"))
        { strcpy(ilew, json["min4"]);
          minFanValues[3] = round(atoi(ilew)*10.23);
        } else  {
          printJsonErr("min4");
          jsonOK = false;
        }

        if (json.containsKey("b1"))
        { strcpy(ilew, json["b1"]);
          if (ilew[0] == '1') fanBoost[0] = 1; else fanBoost[0] = 0;
        } else  {
          printJsonErr("b1");
          jsonOK = false;
        }

        if (json.containsKey("b2"))
        { strcpy(ilew, json["b2"]);
          if (ilew[0] == '1') fanBoost[1] = 1; else fanBoost[1] = 0;
        } else  {
          printJsonErr("b2");
          jsonOK = false;
        }

        if (json.containsKey("b3"))
        { strcpy(ilew, json["b3"]);
          if (ilew[0] == '1') fanBoost[2] = 1; else fanBoost[2] = 0;
        } else  {
          printJsonErr("b3");
          jsonOK = false;
        }

        if (json.containsKey("b4"))
        { strcpy(ilew, json["b4"]);
          if (ilew[0] == '1') fanBoost[3] = 1; else fanBoost[3] = 0;
        } else  {
          printJsonErr("b4");
          jsonOK = false;
        }

          if (json.containsKey("auxS"))
        { strcpy(ilew, json["auxS"]);
          if (ilew[0] == '1') auxStatus = 1; else 
          if (ilew[0] == '2') auxStatus = 2; else 
           auxStatus = 0;
        } else  {
          printJsonErr("auxS");
          jsonOK = false;
        }
         
         if (json.containsKey("auxN"))strcpy(auxName, json["auxN"]); else  {
          printJsonErr("auxN");
          jsonOK = false;
        }

 if (json.containsKey("PWMFreq"))
        { strcpy(ilew, json["PWMFreq"]);
          PWMFreq = atoi(ilew);
        } else  {
          printJsonErr("PWMFreq");
          PWMFreq=22000;
          jsonOK = false;
        }

        if (!jsonOK) Serial.println("Json fan config file have bad data");
        else Serial.println("Json fan config file data readed, parsed, werified - OK");
      } else {
        Serial.println("Failed to load json fan config");
        jsonOK = false;
      }
    }
    else      {
      Serial.println("No fan config file");
      jsonOK = false;
    }

  } else {
    Serial.println("No fan config file");
    jsonOK = false;
  }
  analogWriteFreq(PWMFreq); 
  return jsonOK;
}
