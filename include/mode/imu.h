#ifndef _IMU_H_
#define _IMU_H_

#include "setup.h"

int16_t yaw, pitch, roll;

void Imu() {
      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t yaw_H = Serial.read();
                  uint8_t yaw_L = Serial.read();

                  yaw = ((((uint16_t)yaw_H << 8) & 0xFF00) | ((int16_t)yaw_L & 0x00FF)) - 32768;
                  while (Serial.available() > 0) Serial.read();
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 3), CenterY(32));
            oled.print("IMU");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, String(yaw).length()), CenterY(32));
            oled.print(yaw);

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

      led.SetDegree(SimplifyDeg(360 - yaw), 1, 0, 0);
}

#endif