#define EEPROM_SIZE 2048

void setup_eeprom() {
  delay(2000);
  Serial.println("\nStarting to read from EEPROM");
  EEPROM.begin(EEPROM_SIZE);
  unsigned int address = 0;
  delay(2000);
  EEPROM.get(address, eepromValidity);
  //Assuming that we set the first byte of the EEPROM to a known value to mark is as persisted
  if (eepromValidity.isValid == 'y') {
    address += sizeof(eepromValidity);
    EEPROM.get(address, wifiLogin);
    address += sizeof(wifiLogin);
    EEPROM.get(address, mqttConfig);
    address += sizeof(mqttConfig);
    for(int i = 0; i < NUM_LEDS; i++) {
      EEPROM.get(address, leds[i]);
      address += sizeof(leds[i]);
    }
  } else {
    address += sizeof(eepromValidity);
    address += sizeof(wifiLogin);

    strcpy(mqttConfig.server, "192.168.1.59");
    strcpy(mqttConfig.username, "felipe");
    strcpy(mqttConfig.password, "pipoca22");
    mqttConfig.port = 2883;
    address += sizeof(mqttConfig);
    

    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i].setRGB(0, 0, 0);
      address += sizeof(leds[i]);
    }
  }
  Serial.println(EEPROM.end() ? "EEPROM Ended": "EEPROM did not end well.");
}

void save_eeprom() {
  EEPROM.begin(EEPROM_SIZE);
  unsigned int address = 0;
  eepromValidity.isValid = 'y';
  EEPROM.put(address, eepromValidity);
  address += sizeof(eepromValidity);
  EEPROM.put(address, wifiLogin);
  address += sizeof(wifiLogin);
  EEPROM.put(address, mqttConfig);
  address += sizeof(mqttConfig);
  for(int i = 0; i < NUM_LEDS; i++) {
    EEPROM.put(address, leds[i]);
    address += sizeof(leds[i]);
  }

  Serial.println(EEPROM.commit()  ? "Commit Done." : "Not able to commit");
  Serial.println(EEPROM.end() ? "EEPROM Ended": "EEPROM did not end well.");
}

void invalidate() {
  EEPROM.begin(EEPROM_SIZE);
  unsigned int address = 0;
  eepromValidity.isValid = 'n';
  EEPROM.put(address, eepromValidity);
  Serial.println(EEPROM.commit()  ? "Commit Done." : "Not able to commit");
  Serial.println(EEPROM.end() ? "EEPROM Ended": "EEPROM did not end well.");
}

void dump() {
  EEPROM.begin(EEPROM_SIZE);
  char dump[EEPROM_SIZE];
  EEPROM.get(0, dump);
  Serial.print("EEPROM first byte: ");
  for(int i = 0; i< EEPROM_SIZE; i++) {
    Serial.print((int) dump[i]);
    Serial.print(" ");
    if (i % 32 == 0) {
      Serial.println(" ");
    }
  }
  Serial.println(EEPROM.end() ? "EEPROM Ended": "EEPROM did not end well.");
  Serial.println("\n---------------------------");
}
