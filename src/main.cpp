#include <Arduino.h>

#include "config.h"

void setup() {
      oled.begin();
      oled.setPowerSave(0);
      oled.setFlipMode(1);
      oled.setFont(u8g2_font_courR10_tr);
      oled.firstPage();
      do {
            oled.setCursor(CenterX(64, 12), CenterY(32));
            oled.print("Crescent(Re)");
      } while (oled.nextPage());

      Serial.begin(9600);  // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

      led.SetBrightness(50);
      for (uint8_t i = 0; i < 16 * 6; i++) {
            led.Clear();
            led.SetPixelColor(i, 200, 200, 200);
            led.SetPixelColor(i - 1, 100, 100, 100);
            led.SetPixelColor(i - 2, 50, 50, 50);
            led.SetPixelColor(i - 3, 25, 25, 25);
            if (i % 16 == 0) tone(buzzer_pin, 2000, 20);
            led.Show();
            delay(20);
      }
      led.Clear();

      delay(100);
      tone(buzzer_pin, 2000, 100);
      delay(100);
      tone(buzzer_pin, 1000, 100);
      delay(100);
}

void loop() {  // 呼び出しのオーバーヘッド節減
      while (1) {
            ModeRun();
      }
}