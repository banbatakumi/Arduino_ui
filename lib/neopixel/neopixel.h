#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"

class NeoPixel {
     public:
      NeoPixel();
      Adafruit_NeoPixel led;
      void SetColor(int8_t led_num_, uint8_t red_, uint8_t green_, uint8_t blue_);
      void Clear();
      void Show();

     private:
};

#endif