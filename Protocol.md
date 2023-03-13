# Proposal for the V2 protocol of led controlling

There is no more space between the bytes, also the message is more machine focused than human focused, so the bytes are based on the type that they carry. 

`<amount of leds>[for each led:<led index><mode><mode specific data>]`

Where:
amount of leds - 16 bits unsigned int value of how many leds will be controlled by this message
led index - 16 bits unsigned int value of the led that is being controlled
mode - 8 bit byte indicating which mode will be represented

## Constant timer

There will be a 10ms timer that will be used by any time based mode.
https://github.com/esp8266/Arduino/blob/master/libraries/Ticker/examples/TickerBasic/TickerBasic.ino

### Modes

#### Mode 0

Mode 0 is a simple mode that just light the leds based on a RGB value:

`<r><g><b>`
r - 8 bits byte value of the R channel
g - 8 bits byte value of the G channel
b - 8 bits byte value of the B channel

#### Mode 1

Mode 1 is a simple mode that just light the leds based on a HSV value:
`<h><s><v>`
h - 8 bits byte value of the hue 
s - 8 bits byte value of the saturation
v - 8 bits byte value of the value

#### Mode 2

Mode 2 provide a way to cycle trought the hue value of a led configured with HSV.
`<h><s><v><max h><step><update frequency>`
h - 8 bits byte value of the initial hue 
s - 8 bits byte value of the saturation
v - 8 bits byte value of the value
max h - 8 bits byte value of the maximum hue value acceptable
step - 8 bits byte step at which the hue should change
update frequency - 8 bits byte value of how many ticks based ont the constant timer a value should be kept

For example: a mdoe 2 command with `h = 0, s = 63, v = 63, max h = 32, step = 2, update frequency = 2` means that every 2 constant timer ticks (20ms) the `h` value should be
added by 2 until it reachs 32, after that the step will be recomputed to be -2 so the h value will decrease towards 0 and the cycle will keepe repeating.
