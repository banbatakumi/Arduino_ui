#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "setup.h"

int16_t ball_dir;
uint8_t ball_dis;
int8_t ball_velocity_x;
int8_t ball_velocity_y;

int16_t y_goal_dir;
uint8_t y_goal_size;
int16_t b_goal_dir;
uint8_t b_goal_size;

int16_t center_dir;
uint8_t center_dis;
int8_t own_x;
int8_t own_y;
uint8_t front_proximity;
uint8_t back_proximity;

void Camera() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 6), CenterY(32));
            oled.print("Camera");
      } else if (sub_item == 1) {
            if (Serial.available() > 0) {
                  if (Serial.read() == 0xFF) {
                        ball_dir = Serial.read() * 2 - 180;
                        ball_dis = Serial.read();
                        ball_velocity_x = Serial.read() - 127;
                        ball_velocity_y = Serial.read() - 127;
                        while (Serial.available() > 0) Serial.read();
                  }
            }

            oled.setCursor(0, CenterY(5));
            oled.print("Dir:");
            oled.print(ball_dir);
            oled.setCursor(0, CenterY(20));
            oled.print("Dis:");
            oled.print(ball_dis);
            oled.setCursor(0, CenterY(35));
            oled.print("VelX:");
            oled.print(ball_velocity_x);
            oled.setCursor(0, CenterY(50));
            oled.print("VelY:");
            oled.print(ball_velocity_y);
            oled.drawFilledEllipse(96 + (ball_dis / 6 + 3) * cos((ball_dir - 90) * PI / 180.000), 32 + (ball_dis / 6 + 3) * sin((ball_dir - 90) * PI / 180.000), 2, 2);
            oled.drawEllipse(96, 32, 30, 30);
            oled.drawEllipse(96, 32, 15, 15);
            oled.drawLine(64, 32, 128, 32);
            oled.drawLine(96, 0, 96, 64);

            if (ball_dis != 0) led.SetDegree(ball_dir, 1, 0, 0);
      } else if (sub_item == 2) {
            if (Serial.available() > 0) {
                  if (Serial.read() == 0xFF) {
                        y_goal_dir = Serial.read() * 2 - 180;
                        y_goal_size = Serial.read();
                        b_goal_dir = Serial.read() * 2 - 180;
                        b_goal_size = Serial.read();
                        while (Serial.available() > 0) Serial.read();
                  }
            }

            oled.setCursor(0, CenterY(16));
            oled.print("Yellow");
            oled.setCursor(0, CenterY(32));
            oled.print("D:");
            oled.print(y_goal_dir);
            oled.setCursor(0, CenterY(48));
            oled.print("S:");
            oled.print(y_goal_size);

            oled.setCursor(64, CenterY(16));
            oled.print("Blue");
            oled.setCursor(64, CenterY(32));
            oled.print("D:");
            oled.print(b_goal_dir);
            oled.setCursor(64, CenterY(48));
            oled.print("S:");
            oled.print(b_goal_size);

            if (y_goal_size != 0) led.SetPixelColorSimply(round(y_goal_dir / 22.5) % 16, 1, 1, 0);
            if (b_goal_size != 0) led.SetPixelColorSimply(round(b_goal_dir / 22.5) % 16, 0, 0, 1);
      } else if (sub_item == 3) {
            if (Serial.available() > 0) {
                  if (Serial.read() == 0xFF) {
                        center_dir = Serial.read() * 2 - 180;
                        center_dis = Serial.read();
                        own_x = Serial.read() - 100;
                        own_y = Serial.read() - 100;
                        front_proximity = Serial.read();
                        back_proximity = Serial.read();
                        while (Serial.available() > 0) Serial.read();
                  }
            }

            oled.setCursor(0, CenterY(8));
            oled.print("x:");
            oled.print(own_x);
            oled.setCursor(0, CenterY(24));
            oled.print("y:");
            oled.print(own_y);
            oled.setCursor(0, CenterY(40));
            oled.print("dir:");
            oled.print(center_dir);
            oled.setCursor(0, CenterY(54));
            oled.print("dis:");
            oled.print(center_dis);

            oled.drawFrame(74, 0, 54, 64);
            oled.drawFrame(88, 0, 26, 5);
            oled.drawFrame(88, 59, 26, 5);
            oled.drawFilledEllipse(101 + own_x / 2, 32 - own_y / 1.5, 3, 3);
            oled.drawEllipse(101, 32, 9, 9);
            oled.drawEllipse(101, 32, 1, 1);

            if ((front_proximity >> 6) & 1) led.SetPixelColorSimply(13, 1, 0, 0);
            if ((front_proximity >> 5) & 1) led.SetPixelColorSimply(14, 1, 0, 0);
            if ((front_proximity >> 4) & 1) led.SetPixelColorSimply(15, 1, 0, 0);
            if ((front_proximity >> 3) & 1) led.SetPixelColorSimply(0, 1, 0, 0);
            if ((front_proximity >> 2) & 1) led.SetPixelColorSimply(1, 1, 0, 0);
            if ((front_proximity >> 1) & 1) led.SetPixelColorSimply(2, 1, 0, 0);
            if (front_proximity & 1) led.SetPixelColorSimply(3, 1, 0, 0);
            if ((back_proximity >> 6) & 1) led.SetPixelColorSimply(5, 1, 0, 0);
            if ((back_proximity >> 5) & 1) led.SetPixelColorSimply(6, 1, 0, 0);
            if ((back_proximity >> 4) & 1) led.SetPixelColorSimply(7, 1, 0, 0);
            if ((back_proximity >> 3) & 1) led.SetPixelColorSimply(8, 1, 0, 0);
            if ((back_proximity >> 2) & 1) led.SetPixelColorSimply(9, 1, 0, 0);
            if ((back_proximity >> 1) & 1) led.SetPixelColorSimply(10, 1, 0, 0);
            if (back_proximity & 1) led.SetPixelColorSimply(11, 1, 0, 0);
      } else {
            sub_item = 0;
      }
}

#endif