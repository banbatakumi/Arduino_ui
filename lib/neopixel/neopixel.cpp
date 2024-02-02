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

void NeoPixel::Rainbow() {
      static int step_num = 0;
      for (int i = 0; i < 16; i++) {
            // ストリップの長さに沿って色相環（65536の範囲）を1回転させる量だけピクセルの色相をオフセットします。
            int pixelHue = step_num + (i * 65536L / led.numPixels());
            // ColorHSV関数に色相(0 to 65535)を渡し、その結果をgamma32()でガンマ補正します。
            led.setPixelColor(i, led.gamma32(led.ColorHSV(pixelHue)));
      }
      led.show();
      step_num += 256;
      if (step_num == 65536) {
            step_num = 0;
      }
}