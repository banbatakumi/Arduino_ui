#ifndef _LINE_H_
#define _LINE_H_

#include "setup.h"

void Line() {
      static int16_t line_dir;
      static int16_t line_inside_dir;
      static uint8_t line_interval;
      static bool is_on_line;
      static bool is_left_line;
      static bool is_right_line;

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t bool_data;
                  line_dir = Serial.read() * 2 - 180;
                  line_inside_dir = Serial.read() * 2 - 180;
                  line_interval = Serial.read();
                  bool_data = Serial.read();
                  is_on_line = (bool_data >> 2) & 1;
                  is_left_line = (bool_data >> 1) & 1;
                  is_right_line = bool_data & 1;

                  while (Serial.available() > 0) Serial.read();
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Line");
      } else if (sub_item == 1) {
            oled.setCursor(0, CenterY(20));
            oled.print("Dir: ");
            oled.print(line_dir);
            oled.setCursor(0, CenterY(32));
            oled.print("Inside: ");
            oled.print(line_inside_dir);
            oled.setCursor(0, CenterY(44));
            oled.print("Interval: ");
            oled.print(line_interval);
      } else {
            sub_item = 0;
      }

      if (is_on_line == 1) led.SetColor(round(line_inside_dir / 22.5) % 16, 0, 1, 0);
      if (is_left_line) led.SetColor(12, 0, 0, 1);
      if (is_right_line) led.SetColor(4, 0, 0, 1);
}

#endif