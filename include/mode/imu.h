#ifndef _IMU_H_
#define _IMU_H_

#include "setup.h"

void Imu() {
      static int16_t own_dir;

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t yaw_plus = Serial.read();
                  uint8_t yaw_minus = Serial.read();

                  own_dir = yaw_plus == 0 ? yaw_minus * -1 : yaw_plus;
            }
            while (Serial.available() > 0) Serial.read();
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 3), CenterY(32));
            oled.print("IMU");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, String(own_dir).length()), CenterY(32));
            oled.print(own_dir);

            static uint8_t cnt = 0;

            if (set_val != 0) is_own_dir_correction = 1;

            if (is_own_dir_correction == 1) cnt++;
            if (cnt > 10) {
                  is_own_dir_correction = 0;
                  cnt = 0;
            }
      } else {
            sub_item = 0;
      }

      led.SetDegree(SimplifyDeg(360 - own_dir), 1, 0, 0);
}

#endif