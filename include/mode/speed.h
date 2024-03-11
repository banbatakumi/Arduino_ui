#ifndef _SPEED_H_
#define _SPEED_H_

#include "setup.h"

void Speed() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 5), CenterY(32));
            oled.print("Speed");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, 6), CenterY(16));
            oled.print("Normal");
            oled.setCursor(CenterX(64, String(moving_speed).length()), CenterY(32));
            oled.print(moving_speed);
            moving_speed += set_val * -5;
            if (moving_speed > 100) moving_speed = 100;

            for (uint8_t i = 0; i < round(moving_speed / 6.25); i++) {
                  led.SetPixelColorSimply(i, 0, 0, 1);
            }
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(64, 4), CenterY(16));
            oled.print("Line");
            oled.setCursor(CenterX(64, String(line_moving_speed).length()), CenterY(32));
            oled.print(line_moving_speed);
            line_moving_speed += set_val * -5;
            if (line_moving_speed > 100) line_moving_speed = 100;

            for (uint8_t i = 0; i < round(line_moving_speed / 6.25); i++) {
                  led.SetPixelColorSimply(i, 0, 0, 1);
            }
      } else {
            sub_item = 0;
      }
}

#endif