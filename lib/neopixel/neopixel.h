#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"

class NeoPixel {
     public:
      NeoPixel();
      Adafruit_NeoPixel led;
      void SetColor(int8_t led_num_, bool red_, bool green_, bool blue_);
      void Clear();
      void Show();
      void Rainbow();
      void SetBrightness(uint8_t brightness_);

     private:
      uint8_t brightness;
};

#endif