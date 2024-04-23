#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include "setup.h"

uint8_t dis[4];

void Ultrasonic() {
      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  for (uint8_t i = 0; i < 4; i++) {
                        dis[i] = Serial.read();
                  }
                  while (Serial.available() > 0) Serial.read();
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 8), CenterY(32));
            oled.print("Distance");
      } else if (sub_item == 1) {
            oled.setCursor(0, CenterY(5));
            oled.print("Front:");
            oled.print(dis[0]);
            oled.setCursor(0, CenterY(20));
            oled.print("Right:");
            oled.print(dis[1]);
            oled.setCursor(0, CenterY(35));
            oled.print("Back:");
            oled.print(dis[2]);
            oled.setCursor(0, CenterY(50));
            oled.print("Left:");
            oled.print(dis[3]);
      } else {
            sub_item = 0;
      }
      if (dis[0] <= 50) led.SetPixelColor(0, (50 - dis[0]) * 2, 0, 0);
      if (dis[0] <= 25) {
            led.SetPixelColor(15, (25 - dis[0]) * 2, 0, 0);
            led.SetPixelColor(1, (25 - dis[0]) * 2, 0, 0);
      }
      if (dis[1] <= 50) led.SetPixelColor(4, (50 - dis[1]) * 2, 0, 0);
      if (dis[1] <= 25) {
            led.SetPixelColor(3, (25 - dis[1]) * 2, 0, 0);
            led.SetPixelColor(5, (25 - dis[1]) * 2, 0, 0);
      }
      if (dis[2] <= 50) led.SetPixelColor(8, (50 - dis[2]) * 2, 0, 0);
      if (dis[2] <= 25) {
            led.SetPixelColor(7, (25 - dis[2]) * 2, 0, 0);
            led.SetPixelColor(9, (25 - dis[2]) * 2, 0, 0);
      }
      if (dis[3] <= 50) led.SetPixelColor(12, (50 - dis[3]) * 2, 0, 0);
      if (dis[3] <= 25) {
            led.SetPixelColor(11, (25 - dis[3]) * 2, 0, 0);
            led.SetPixelColor(13, (25 - dis[3]) * 2, 0, 0);
      }
}

#endif