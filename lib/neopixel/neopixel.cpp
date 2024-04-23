#include "neopixel.h"

NeoPixel::NeoPixel() : led(LED_QTY, 2, NEO_GRB + NEO_KHZ800) {
      led.begin();
}

void NeoPixel::SetPixelColorSimply(int8_t led_num_, bool red_, bool green_, bool blue_) {
      int8_t led_num_tmp = 20 - led_num_;
      while (led_num_tmp > LED_QTY - 1) led_num_tmp -= LED_QTY;
      while (led_num_tmp < 0) led_num_tmp += LED_QTY;
      led.setPixelColor(led_num_tmp, led.Color(red_ * brightness, green_ * brightness, blue_ * brightness));
}

void NeoPixel::SetPixelColor(int8_t led_num_, uint8_t red_, uint8_t green_, uint8_t blue_) {
      int8_t led_num_tmp = 20 - led_num_;
      while (led_num_tmp > LED_QTY - 1) led_num_tmp -= LED_QTY;
      while (led_num_tmp < 0) led_num_tmp += LED_QTY;
      led.setPixelColor(led_num_tmp, led.Color(red_, green_, blue_));
}

void NeoPixel::SetDegree(int16_t degree_, bool red_, bool green_, bool blue_) {
      int16_t degree;
      if (degree_ >= -45 && degree_ <= 45) {
            degree = SimplifyDeg(degree_);
            SetPixelColor(0, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -22.5 && degree_ <= 67.5) {
            degree = SimplifyDeg(degree_ - 22.5);
            SetPixelColor(1, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 0 && degree_ <= 90) {
            degree = SimplifyDeg(degree_ - 45);
            SetPixelColor(2, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 22.5 && degree_ <= 112.5) {
            degree = SimplifyDeg(degree_ - 67.5);
            SetPixelColor(3, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 45 && degree_ <= 135) {
            degree = SimplifyDeg(degree_ - 90);
            SetPixelColor(4, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 67.5 && degree_ <= 157.5) {
            degree = SimplifyDeg(degree_ - 112.5);
            SetPixelColor(5, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 90 && degree_ <= 180) {
            degree = SimplifyDeg(degree_ - 135);
            SetPixelColor(6, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 112.5 || degree_ <= -157.5) {
            degree = SimplifyDeg(degree_ - 157.5);
            SetPixelColor(7, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 135 || degree_ <= -135) {
            degree = SimplifyDeg(degree_ - 180);
            SetPixelColor(8, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= 157.5 || degree_ <= -112.5) {
            degree = SimplifyDeg(degree_ + 157.5);
            SetPixelColor(9, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -180 && degree_ <= -90) {
            degree = SimplifyDeg(degree_ + 135);
            SetPixelColor(10, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -157.5 && degree_ <= -67.5) {
            degree = SimplifyDeg(degree_ + 112.5);
            SetPixelColor(11, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -135 && degree_ <= -45) {
            degree = SimplifyDeg(degree_ + 90);
            SetPixelColor(12, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -112.5 && degree_ <= -22.5) {
            degree = SimplifyDeg(degree_ + 67.5);
            SetPixelColor(13, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -90 && degree_ <= 0) {
            degree = SimplifyDeg(degree_ + 45);
            SetPixelColor(14, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
      if (degree_ >= -67.5 && degree_ <= 22.5) {
            degree = SimplifyDeg(degree_ + 22.5);
            SetPixelColor(15, (45 - abs(degree)) / 45.0f * brightness * red_, (45 - abs(degree)) / 45.0f * brightness * green_, (45 - abs(degree)) / 45.0f * brightness * blue_);
      }
}

void NeoPixel::SetBrightness(uint8_t brightness_) {
      this->brightness = brightness_;
}

void NeoPixel::Clear() {
      led.clear();
}

void NeoPixel::Show() {
      led.show();
}

void NeoPixel::Rainbow() {
      static int step_num = 0;
      for (int i = 0; i < LED_QTY; i++) {
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