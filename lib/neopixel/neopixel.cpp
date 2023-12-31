#include "neopixel.h"

NeoPixel::NeoPixel() : led(16, 2, NEO_GRB + NEO_KHZ800) {
      led.begin();
}

void NeoPixel::SetColor(int8_t led_num_, bool red_, bool green_, bool blue_) {
      int8_t led_num_tmp = 20 - led_num_;
      while (led_num_tmp > 15) {
            led_num_tmp -= 16;
      }
      while (led_num_tmp < 0) {
            led_num_tmp += 16;
      }
      led.setPixelColor(led_num_tmp, led.Color(red_ * brightness, green_ * brightness, blue_ * brightness));
}

void NeoPixel::SetBrightness(uint8_t brightness_) {
      brightness = brightness_;
}

void NeoPixel::Clear() {
      led.clear();
}

void NeoPixel::Show() {
      led.show();
}