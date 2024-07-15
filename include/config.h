#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "mode/camera.h"
#include "mode/dribbler.h"
#include "mode/home.h"
#include "mode/ir.h"
#include "mode/line.h"
#include "mode/speed.h"
#include "setup.h"

void ModeRun() {
      oled.firstPage();
      do {
            led.Clear();  // NeoPixelを消す

            if (sub_item == 0) {
                  if (item > -2) oled.drawTriangle(10, 53, 0, 58, 10, 63);
                  if (item < 3) oled.drawTriangle(118, 53, 128, 58, 118, 63);
            }

            if (item == 0) {
                  Home();
            } else if (item == -1) {
                  Speed();
            } else if (item == -2) {
                  Dribbler();
            } else if (item == 1) {
                  Line();
            } else if (item == 2) {
                  Camera();
            } else if (item == 3) {
                  Ir();
            }
            ButtonRead();

            led.Show();
            SendData();
      } while (oled.nextPage());
}

void SendData() {  // UART送信
      const uint8_t send_byte_num = 7;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = item + 127;
      send_byte[2] = sub_item << 4 | mode;
      send_byte[3] = do_own_dir_correction << 4 | dribbler_sig;
      send_byte[4] = moving_speed;
      send_byte[5] = line_moving_speed;
      send_byte[6] = 0xAA;
      Serial.write(send_byte, send_byte_num);
      Serial.flush();
}

void ButtonRead() {
      pre_sub_item = sub_item;

      for (uint8_t i = 0; i < 3; i++) {
            pre_is_button[i] = is_button[i];
            is_button[i] = digitalRead(button_pin[i]);
      }

      if (is_button[0] == 1 && pre_is_button[0] == 0) {
            if (mode == 0) {
                  tone(buzzer_pin, 1000, 50);
                  delay(50);
                  sub_item += 1;
            } else {
                  mode = 0;
            }
      }

      set_val = 0;
      if (is_button[1] == 1) {
            if (sub_item == 0 && item > -2) {
                  item -= 1;
            } else {
                  set_val -= 1;
            }
            if (pre_is_button[1] == 0) {
                  tone(buzzer_pin, 1500, 100);
                  delay(250);
            } else {
                  tone(buzzer_pin, 1500, 50);
                  delay(50);
            }
      }
      if (is_button[2] == 1) {
            if (sub_item == 0 && item < 3) {
                  item += 1;
            } else {
                  set_val += 1;
            }
            if (pre_is_button[2] == 0) {
                  tone(buzzer_pin, 1500, 100);
                  delay(250);
            } else {
                  tone(buzzer_pin, 1500, 50);
                  delay(50);
            }
      }
}

#endif