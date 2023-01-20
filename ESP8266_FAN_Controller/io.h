
void procedIO()
{
#ifdef DEBUG
  Serial.println("\n==== FANS CFG ====");
  Serial.print("COUNT:"); Serial.println(numberOf_Fans);
  Serial.print("ENABLED:");
  for (byte i = 0; i < 4; i++)
  {
    Serial.print(fanEnabled[i]);
    if (i < 3)Serial.print(',');
  }
  Serial.println();
  Serial.print("VALUES:");
  for (byte i = 0; i < 4; i++)
  {
    Serial.print(fanValues[i]);
    if (isFanBoosted[i] && fanValues[i] < 511) Serial.print("->BoostUpTo511");
    else if (fanValues[i] != 0 && fanValues[i] < minFanValues[i] ) {
      Serial.print("->upTo");
      Serial.print(minFanValues[i]);
    }
    if (i < 3)Serial.print(',');

  }
  Serial.println();
#endif

  for (byte i = 0; i < 4; i++)
  {
    if (fanValues[i] < 0) fanValues[i] = 0;
    if (fanValues[i] > 1023) fanValues[i] = 1023;
    int val = fanValues[i];
    if (isFanBoosted[i] && val < 511) val = 511;
    if (val < minFanValues[i] && val != 0) val = minFanValues[i];
    if (i < numberOf_Fans && fanEnabled[i] == 1)
      analogWrite(fanPins[i], val);
    else
      digitalWrite(fanPins[i], 0);
  }

}

void boostCheck()
{ if (inBoost && millis() - boostTime > 2000)
  {
    inBoost = false;
    for (int i = 0; i < 4; i++) isFanBoosted[i] = 0;
    procedIO();
  }
}

void relayAUX(byte val)
{
  if (auxStatus == 2)
  {
    if (val > 1) val = 1;
    if (val < 0) val = 0;
    RELAY = val;
    digitalWrite(AUX, val);
  }
}


void BLINKME()
{
  if (auxStatus == 1) {
    blinkMe = !blinkMe;
    digitalWrite(AUX, blinkMe);

  }
}
void blinkAUX()
{
  if (auxStatus == 1) {
    if (millis() - halfSec > auxSpeed)               // check every 1.5 seconds
    {
      halfSec = millis();
      BLINKME();             // blink led on AUX- D0
      if (blinkMe) auxSpeed = auxSpeed / 5; else auxSpeed = auxSpeed * 5;
    }
  }
}

void initIO()
{//OUTS INIT
  pinMode(AUX, OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(fanPins[i], OUTPUT);
  halfSec = millis();
  #ifdef DEBUG
  BLINKME(); delay(300); BLINKME();     // for arduino monitor initialize 
  #endif
  
}
