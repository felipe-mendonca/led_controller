#include <ESP8266WebServer.h>
#include <DNSServer.h> 

ESP8266WebServer server(80);
DNSServer dnsServer;

#define DNS_PORT 53

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

/* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "ESP_ap";
const char *softAP_password = "12345678";

void start_soft_ap() {
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  dnsServer.start(DNS_PORT, "*", apIP); // DNS spoofing (Only for HTTP)

  server.on("/", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.onNotFound([]() { server.send(511, "text/html", "<html><body>Does this works?</body></html>"); });
  server.begin(); // Web server start
}

void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( wifiLogin.ssid, wifiLogin.password );
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
}

void captive_portal_loop() {
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
}

/** Wifi config page handler */
void handleWifi() {
  server.send(200, "text/html", "<html><head></head><body><h1>Wifi config</h1>"
  "<form method='POST' action='wifisave'>"
  "<h4>Connect to network:</h4>"
  "<input type='text' placeholder='network' name='ssid'/><br />"
  "<input type='password' placeholder='password' name='password'/><br />"
  "<h4>Configure MQTT server:</h4>"
  "<input type='text' placeholder='broker' name='mqtt_broker'/><br />"
  "<input type='text' placeholder='username' name='mqtt_username'/><br />"
  "<input type='text' placeholder='password' name='mqtt_passwrod'/><br />"
  "<input type='text' placeholder='port' name='mqtt_port'/><br />"
  "<input type='submit' value='Connect'/></form>"
  "</body></html>"
  );
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  // Configure wifi settings
  server.arg("ssid").toCharArray(wifiLogin.ssid, sizeof(wifiLogin.ssid) - 1);
  server.arg("password").toCharArray(wifiLogin.password, sizeof(wifiLogin.password) - 1);

  //Configure MQTT settings
  server.arg("mqtt_broker").toCharArray(mqttConfig.server, sizeof(mqttConfig.server) - 1);
  server.arg("mqtt_username").toCharArray(mqttConfig.username, sizeof(mqttConfig.username) - 1);
  server.arg("mqtt_passwrod").toCharArray(mqttConfig.password, sizeof(mqttConfig.password) - 1);
  mqttConfig.port = server.arg("mqtt_port").toInt();

  save_eeprom();
  dump();

  server.send ( 200, "text/html", "<html><body>Done!</body></html>");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
}