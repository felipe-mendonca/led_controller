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
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  //server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.send(200, "text/html", "<html><head></head><body><h1>Wifi config</h1></body></html>");

  // if (server.client().localIP() == apIP) {
  //   server.sendContent(String("<p>You are connected through the soft AP: ") + softAP_ssid + "</p>");
  // } else {
  //   server.sendContent(String("<p>You are connected through the wifi network: ") + wifiLogin.ssid + "</p>");
  // }
  // server.sendContent(
  //   "\r\n<br />"
  //   "<table><tr><th align='left'>SoftAP config</th></tr>"
  // );
  // server.sendContent(String() + "<tr><td>SSID " + String(softAP_ssid) + "</td></tr>");
  // server.sendContent(String() + "<tr><td>IP " + WiFi.softAPIP().toString() + "</td></tr>");
  // server.sendContent(
  //   "</table>"
  //   "\r\n<br />"
  //   "<table><tr><th align='left'>WLAN config</th></tr>"
  // );
  // server.sendContent(String() + "<tr><td>SSID " + String(wifiLogin.ssid) + "</td></tr>");
  // server.sendContent(String() + "<tr><td>IP " + WiFi.localIP().toString() + "</td></tr>");
  // server.sendContent(
  //   "</table>"
  //   "\r\n<br />"
  //   "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>"
  // );
  // Serial.println("scan start");
  // int n = WiFi.scanNetworks();
  // Serial.println("scan done");
  // if (n > 0) {
  //   for (int i = 0; i < n; i++) {
  //     server.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
  //   }
  // } else {
  //   server.sendContent(String() + "<tr><td>No WLAN found</td></tr>");
  // }
  // server.sendContent(
  //   "</table>"
  //   "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
  //   "<input type='text' placeholder='network' name='n'/>"
  //   "<br /><input type='password' placeholder='password' name='p'/>"
  //   "<br /><input type='submit' value='Connect/Disconnect'/></form>"
  //   "<p>You may want to <a href='/'>return to the home page</a>.</p>"
  //   "</body></html>"
  // );
  // server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(wifiLogin.ssid, sizeof(wifiLogin.ssid) - 1);
  server.arg("p").toCharArray(wifiLogin.password, sizeof(wifiLogin.password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
}