#ifndef _IR_H_
#define _IR_H_

#include "setup.h"

int16_t ir_dir;
uint8_t ir_dis;

void Ir() {
      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  ir_dir = Serial.read() * 2 - 180;
                  ir_dis = Serial.read();

                  while (Serial.available() > 0) Serial.read();
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 2), CenterY(32));
            oled.print("Ir");
      } else if (sub_item == 1) {
            oled.setCursor(0, CenterY(5));
            oled.print("Dir:");
            oled.print(ir_dir);
            oled.setCursor(0, CenterY(20));
            oled.print("Dis:");
            oled.print(ir_dis);
            oled.drawFilledEllipse(96 + ((100 - ir_dis) / 3 + 3) * cos((ir_dir - 90) * PI / 180.000), 32 + ((100 - ir_dis) / 3 + 3) * sin((ir_dir - 90) * PI / 180.000), 2, 2);
            oled.drawEllipse(96, 32, 30, 30);
            oled.drawEllipse(96, 32, 15, 15);
            oled.drawFilledEllipse(96, 32, 2, 2);
            oled.drawLine(64, 32, 128, 32);
            oled.drawLine(96, 0, 96, 64);

            if (ir_dis != 0) led.SetDegree(ir_dir, 1, 0, 0);
      } else {
            sub_item = 0;
      }
}

#endif