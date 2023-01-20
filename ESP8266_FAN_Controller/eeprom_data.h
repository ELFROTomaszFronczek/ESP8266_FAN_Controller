
void readEEPROMValues()
{
  EEPROM.begin(512);
  byte b = EEPROM.read(0);
  if (b == 217) //was saved
  {
    for (byte i = 0; i < 4; i++)  fanEnabled[i] = EEPROM.read(i + 1);
    for (byte i = 0; i < 4; i++)  EEPROM.get((i * 4) + 5, fanValues[i]);
  }
  RELAY = EEPROM.read(101);
}


void writeFanValues()
{
  EEPROM.write(0, 217);
  for (byte i = 0; i < 4; i++)  EEPROM.write(i + 1, fanEnabled[i]);
  for (byte i = 0; i < 4; i++)  EEPROM.put((i * 4) + 5, fanValues[i]);
  EEPROM.commit();
}


void writeRelayState()
{
  EEPROM.write(101, RELAY);
  EEPROM.commit();
}
