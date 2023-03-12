/************* Connect to MQTT Broker ***********/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttConfig.username, mqttConfig.password)) {
      Serial.println("mqtt connected");
      client.subscribe("led/control");   // subscribe the topics here
      client.subscribe("system/config");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      digitalWrite(led, HIGH);
      delay(5000);
    }
    digitalWrite(led, LOW);
  }
}

/**** Method for Publishing MQTT Messages **********/
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

/***** Call back Method for Receiving MQTT messages and Switching LED ****/
void callback(char* topic, byte* data, unsigned int length) {
  Serial.println("Message arrived ["+String(topic)+"]");

  if( strcmp(topic,"led/control") == 0){
    led_control(data, length);
  }
  if( strcmp(topic,"system/config") == 0){
    system_config(data, length);
  }
}

void system_config(byte* data, unsigned int length) {
  invalidate();
  dump();
}
/*
  Receives and process a led control command, the command is a stream of bytes following the format:

  Start of the message:
  0000 0000 0000 0000 - Amount of leds in the message, up to 512
  <leds>

  For each LED:
  0000 0000 0000 0000 - Identifier of the led
  0000 0000           - Mode, at first only mode 0 is available 
  <mode specific payload>

  Mode 0 (light always on):
  0000 0000 0000 0000 - R
  0000 0000 0000 0000 - G
  0000 0000 0000 0000 - B

  Each value in the message is separated by a white space.
 * 
 */
void led_control(byte* data, unsigned int length) {
  char payload[length + 1];
  payload[length] = '\0';
  for (int i = 0; i < length; i++) payload[i] = (char)data[i];

  Serial.println(payload);

  unsigned int ledAmount = 0;
  unsigned int ledPosition = 0;
  unsigned int ledMode = 0;
  unsigned int ledR = 0;
  unsigned int ledG = 0;;
  unsigned int ledB = 0;

  char *nextValue;

  ledAmount = (int)strtol(payload, &nextValue, 16);

Serial.println(ledAmount);
  for (int ledEntryIndex = 0; ledEntryIndex < ledAmount; ledEntryIndex++) {
    ledPosition = (int)strtol(nextValue, &nextValue, 16);
    ledMode = (int)strtol(nextValue, &nextValue, 16);

    switch (ledMode) {
    case 0: // light always on
    default:
      ledR = (int)strtol(nextValue, &nextValue, 16);
      ledG = (int)strtol(nextValue, &nextValue, 16);
      ledB = (int)strtol(nextValue, &nextValue, 16);

      leds[ledPosition].setRGB(ledR, ledG, ledB);
      break;
    }
  }  
}