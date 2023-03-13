void color_ticker() {
  /*
  typedef struct {
    unsigned int ticks;
    unsigned int counter;
    byte from;
    byte to;
    unsigned int step;
    CHSV actual;
  } LedConfig;

  LedConfig ledsConfig[NUM_LEDS] = {0};
  */

  for(int i = 0; i < NUM_LEDS; i++) {
    switch(ledsConfig[i].mode) {
      case 3:
        ledsConfig[i].counter++;
        if (ledsConfig[i].counter == ledsConfig[i].ticks) {
          ledsConfig[i].counter = 0;
          if (ledsConfig[i].actual.value + ledsConfig[i].step >= ledsConfig[i].to || ledsConfig[i].actual.value + ledsConfig[i].step <= ledsConfig[i].from) {
            ledsConfig[i].step = -ledsConfig[i].step;
          }

          ledsConfig[i].actual.value += ledsConfig[i].step;
          hsv2rgb_rainbow(ledsConfig[i].actual, leds[i]);
        }
        break;
        break;
      case 2:
        ledsConfig[i].counter++;
        if (ledsConfig[i].counter == ledsConfig[i].ticks) {
          ledsConfig[i].counter = 0;
          if (ledsConfig[i].actual.hue + ledsConfig[i].step >= ledsConfig[i].to || ledsConfig[i].actual.hue + ledsConfig[i].step <= ledsConfig[i].from) {
            ledsConfig[i].step = -ledsConfig[i].step;
          }

          ledsConfig[i].actual.hue += ledsConfig[i].step;
          hsv2rgb_rainbow(ledsConfig[i].actual, leds[i]);
        }
        break;
      case 0:
      case 1:
      default:
        break;
    }
  }
}