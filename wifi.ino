/************* Connect to WiFi ***********/
void connect_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(wifiLogin.ssid);

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiLogin.ssid, wifiLogin.password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.getPhyMode());
  Serial.println(WiFi.channel());
}

