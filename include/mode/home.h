#ifndef _HOME_H_
#define _HOME_H_

#include "setup.h"

#define LOW_VOLTAGE 8

void Home() {
      static int16_t debug_val[4];
      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t debug_val_plus;
                  uint8_t debug_val_minus;

                  battery_voltage = Serial.read() / 20.00;
                  debug_val_plus = Serial.read();
                  debug_val_minus = Serial.read();
                  debug_val[1] = Serial.read();
                  debug_val[2] = Serial.read();
                  debug_val[3] = Serial.read();

                  debug_val[0] = SimplifyDeg(debug_val_plus == 0 ? debug_val_minus * -1 : debug_val_plus);
                  while (Serial.available() > 0) Serial.read();
            }
      }

      if (mode == 0) {
            oled.setCursor(0, CenterY(5));
            if (battery_voltage > 6) {
                  oled.print("Battery: ");
            } else {
                  oled.print("USB: ");
            }
            oled.print(battery_voltage);
            oled.print("v");
            oled.drawLine(0, 15, 128, 15);

            if (battery_voltage < LOW_VOLTAGE) {
                  for (uint8_t i = 0; i < 16; i++) {
                        led.SetPixelColorSimply(i, 1, 0, 0);
                  }
                  if (sub_item == 1 || sub_item == 2) {
                        oled.drawFrame(13, 42, 102, 15);
                        oled.setCursor(CenterX(64, 11), CenterY(48));
                        oled.print("Can't start");
                  }
            } else if (sub_item == 0) {
                  for (uint8_t i = 0; i < round((battery_voltage - 8) * 8); i++) {
                        led.SetPixelColorSimply(i, 1, 1, 1);
                  }
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Home");
            mode = 0;
      } else if (sub_item == 1) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 7), CenterY(32));
                  oled.print("Offence");
            }
            if (set_val != 0 && battery_voltage > LOW_VOLTAGE) mode = 1 - mode;
      } else if (sub_item == 2) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 7), CenterY(32));
                  oled.print("Defense");
            }
            if (set_val != 0 && battery_voltage > LOW_VOLTAGE) mode = 2 - mode;
      } else if (sub_item == 3) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 5), CenterY(32));
                  oled.print("Debug");
            } else {
                  oled.setCursor(0, CenterY(14));
                  oled.print("0: ");
                  oled.print(debug_val[0]);
                  oled.setCursor(0, CenterY(26));
                  oled.print("1: ");
                  oled.print(debug_val[1]);
                  oled.setCursor(0, CenterY(38));
                  oled.print("2: ");
                  oled.print(debug_val[2]);
                  oled.setCursor(0, CenterY(50));
                  oled.print("3: ");
                  oled.print(debug_val[3]);
            }
            if (set_val != 0) mode = 3 - mode;
      } else {
            sub_item = 0;
      }
}

#endif