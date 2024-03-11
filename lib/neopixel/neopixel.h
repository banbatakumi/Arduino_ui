#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "simplify_deg.h"

#define LED_QTY 16
class NeoPixel {
     public:
      NeoPixel();
      Adafruit_NeoPixel led;
      void SetPixelColorSimply(int8_t led_num_, bool red_, bool green_, bool blue_);
      void SetPixelColor(int8_t led_num_, uint8_t red_, uint8_t green_, uint8_t blue_);
      void SetDegree(int16_t degree_, bool red_, bool green_, bool blue_);
      void Clear();
      void Show();
      void Rainbow();
      void SetBrightness(uint8_t brightness_);

     private:
      uint8_t brightness;
};

#endif