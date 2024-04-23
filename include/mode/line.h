#ifndef _LINE_H_
#define _LINE_H_

#include "setup.h"

int16_t line_dir;
int16_t line_inside_dir;
uint8_t line_interval;
bool is_on_line;
bool is_left_line;
bool is_right_line;

void Line() {
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

      if (is_on_line == 1) led.SetDegree(line_inside_dir, 0, 1, 0);
      if (is_left_line) led.SetPixelColorSimply(12, 0, 0, 1);
      if (is_right_line) led.SetPixelColorSimply(4, 0, 0, 1);
}

#endif