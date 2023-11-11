#include "neopixel.h"

NeoPixel::NeoPixel() : led(16, 2, NEO_GRB + NEO_KHZ800) {
      led.begin();
}

void NeoPixel::SetColor(int8_t led_num_, uint8_t red_, uint8_t green_, uint8_t blue_) {
      int8_t led_num_tmp = 20 - led_num_;
      while (led_num_tmp > 15) {
            led_num_tmp -= 16;
      }
      while (led_num_tmp < 0) {
            led_num_tmp += 16;
      }
      led.setPixelColor(led_num_tmp, led.Color(red_, green_, blue_));
}

void NeoPixel::Clear() {
      led.clear();
}

void NeoPixel::Show() {
      led.show();
}