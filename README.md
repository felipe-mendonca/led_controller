Install the board following: https://github.com/esp8266/Arduino?tab=readme-ov-file#installing-with-boards-manager

Install those libraries using the library manager:

PubSubClient by Nick O'Leary
ArduinoJson by Nenoit Blanchon
FastLED by Daniel Garcia
Ticker by Stefan Staub

Setup a MQTT broker and create the topics:

led/control
system/config


Configure the board to use D1 Mini.

Check Protocol.md to see how the messages should be sent to the topic.

Hope it works!