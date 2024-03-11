#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include "setup.h"

void Ultrasonic() {
      static uint8_t dis[4];

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
      if (dis[0] <= 15) led.SetPixelColorSimply(0, 1, 0, 0);
      if (dis[1] <= 15) led.SetPixelColorSimply(4, 1, 0, 0);
      if (dis[2] <= 15) led.SetPixelColorSimply(8, 1, 0, 0);
      if (dis[3] <= 15) led.SetPixelColorSimply(12, 1, 0, 0);
}

#endif