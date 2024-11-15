#include <Ticker.h>

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#define FASTLED_ESP8266_D1_PIN_ORDER
#include <FastLED.h>
#include <Ticker.h>

/****** Configure the leds *****/
#define NUM_LEDS 512
CRGB leds[NUM_LEDS];

typedef struct {
  byte mode = 0;
  unsigned int ticks;
  unsigned int counter;
  int from;
  int to;
  int step;
  CHSV actual;
} LedConfig;

LedConfig ledsConfig[NUM_LEDS] = {0};

/******* MQTT Broker Connection Details *******/
#define MQTT_SERVER_MAX_LENGTH 63
#define MQTT_USERNAME_MAX_LENGTH 32
#define MQTT_PASSWORD_MAX_LENGTH 63
typedef struct {
  char server[MQTT_SERVER_MAX_LENGTH + 1] = "";
  char username[MQTT_USERNAME_MAX_LENGTH + 1] = "";
  char password[MQTT_PASSWORD_MAX_LENGTH + 1] = "";
  int port = 0;
} MQTTConfig;
MQTTConfig mqttConfig;

/**** LED Settings *******/
const int led = 2; //Set LED pin as GPIO2

#define SSID_MAX_SIZE 32
#define WIFI_PASSWORD_MAX_SIZE 63
typedef struct {
  char ssid[SSID_MAX_SIZE + 1] = "";
  char password[WIFI_PASSWORD_MAX_SIZE + 1] = "";
} WifiLogin;
WifiLogin wifiLogin;

/****** EEPROM Config and Setup *******/
typedef struct{
  char isValid;
} EEPROM_Validity;
EEPROM_Validity eepromValidity;

/**** Secure WiFi Connectivity Initialisation *****/
WiFiClient espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

/**** Color control timer ****/
Ticker colorTicker;
#define TICK_TIMER_MS 10

/**** Application Initialisation Function******/
void setup() {
  pinMode(led, OUTPUT); //set up LED
  Serial.begin(115200);
  while (!Serial) delay(1);
  setup_eeprom();

  if (eepromValidity.isValid != 'y'){
    start_soft_ap();
  } else {
    connect_wifi();
  }

  client.setServer(mqttConfig.server, mqttConfig.port);
  client.setCallback(callback);

  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  FastLED.show();
  
  colorTicker.attach_ms(TICK_TIMER_MS, color_ticker);
}
void loop() {
  if (eepromValidity.isValid != 'y'){
    captive_portal_loop();
  } else {
    if(!WiFi.isConnected()) connect_wifi();
    if (!client.connected()) reconnect(); // check if mqtt client is connected
    client.loop();
    FastLED.show();
  }
}

