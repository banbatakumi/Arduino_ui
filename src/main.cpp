#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <U8g2lib.h>

#include "neopixel.h"
#include "simplify_deg.h"

#define CenterX(x_, charnum_) (x_ - ((charnum_ * 9) / 2))
#define CenterY(y_) (y_ + 6)

const uint8_t led_pin = 2;
const uint8_t buzzer_pin = 8;
const uint8_t button_pin[3] = {6, 7, 5};   // center, left, right

U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled(U8G2_R2, /* reset=*/U8X8_PIN_NONE);
NeoPixel led;

void button_read();

void home();
void lidar();
void speed();
void dribbler();
void imu();
void ball();
void goal();

void battery_voltage_read();

int8_t item = 0, sub_item = 0;
int8_t set_val = 0;
bool is_button[3], pre_is_button[3];
uint8_t serial_write_cnt;

// receive data
int16_t yaw;
uint8_t tof_val[16];
uint8_t min_tof_sensor;
int16_t safe_dir;
uint8_t motor_rotation_num[4];
int16_t ball_dir;
uint8_t ball_dis;
int16_t y_goal_dir;
uint8_t y_goal_size;
int16_t b_goal_dir;
uint8_t b_goal_size;
bool is_ball_catch_front;
bool is_ball_catch_back;
int16_t debug_val[4];

// send data
bool is_yaw_correction;
uint8_t mode = 0;
uint8_t moving_speed = 75;
uint8_t line_moving_speed = 75;
uint8_t dribbler_sig = 0;

bool is_voltage_decrease = 0;
float battery_voltage;

void setup() {
      oled.begin();
      oled.setPowerSave(0);
      oled.setFlipMode(1);
      oled.setFont(u8g2_font_courR10_tr);

      Serial.begin(115200);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

      for (uint8_t i = 0; i < 16; i++) {
            led.SetColor(i, 255, 255, 255);
            delay(50);
            led.Show();
      }

      delay(250);
      tone(buzzer_pin, 1500, 75);
      delay(250);
      tone(buzzer_pin, 2000, 75);
      delay(100);
      tone(buzzer_pin, 3000, 75);
      delay(250);
      led.Clear();
}

void loop() {   // 呼び出しのオーバーヘッド節減
      while (1) {
            battery_voltage_read();

            oled.firstPage();
            do {
                  led.Clear();

                  if (is_voltage_decrease == 1) {
                  } else {
                        button_read();
                        if (sub_item == 0) {
                              if (item > -2) oled.drawTriangle(10, 53, 0, 58, 10, 63);
                              if (item < 4) oled.drawTriangle(117, 53, 128, 58, 117, 63);
                        }

                        if (item == 0) {
                              home();
                        } else if (item == 1) {
                              imu();
                        } else if (item == 2) {
                              lidar();
                        } else if (item == -1) {
                              speed();
                        } else if (item == -2) {
                              dribbler();
                        } else if (item == 3) {
                              ball();
                        } else if (item == 4) {
                              goal();
                        }
                  }

                  led.Show();
            } while (oled.nextPage());

            // UART送信
            if (mode == 0 || serial_write_cnt > 0) {   // デバッグモードもしくは調整中のみに送信
                  Serial.write(0xFF);
                  Serial.write(item + 100);
                  Serial.write(mode);
                  Serial.write(is_yaw_correction);
                  Serial.write(moving_speed);
                  Serial.write(line_moving_speed);
                  Serial.write(dribbler_sig);
                  Serial.write(0xAA);
                  Serial.flush();
            }
      }
}

void home() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Home");

            oled.setCursor(0, CenterY(5));
            oled.print("Battery:");
            oled.print(battery_voltage);
            oled.print("v");
            oled.drawLine(0, 20, 128, 20);
            mode = 0;
            for (uint8_t i = 0; i < 16; i++) {
                  led.SetColor(i, 100, 100, 100);
            }
      } else if (sub_item == 1) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 7), CenterY(32));
                  oled.print("Offence");
                  serial_write_cnt = 0;
            } else {
                  if (serial_write_cnt < 100) serial_write_cnt++;
            }
            if (set_val != 0) {
                  mode = 1 - mode;
            }
      } else if (sub_item == 2) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 7), CenterY(32));
                  oled.print("Defense");
                  serial_write_cnt = 0;
            } else {
                  if (serial_write_cnt < 100) serial_write_cnt++;
            }
            if (set_val != 0) {
                  mode = 2 - mode;
            }
      } else if (sub_item == 3) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 5), CenterY(32));
                  oled.print("Debug");
                  serial_write_cnt = 0;
            } else {
                  if (serial_write_cnt < 100) serial_write_cnt++;
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
            if (set_val != 0) {
                  mode = 3 - mode;
            }
      } else if (sub_item == 4) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 8), CenterY(26));
                  oled.print("Attitude");
                  oled.setCursor(CenterX(64, 7), CenterY(38));
                  oled.print("Control");
                  serial_write_cnt = 0;
            } else {
                  if (serial_write_cnt < 100) serial_write_cnt++;
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
            if (set_val != 0) {
                  mode = 4 - mode;
            }
      } else {
            sub_item = 0;
      }

      if (Serial.available() > 0) {
            uint8_t debug_val_plus[4];
            uint8_t debug_val_minus[4];

            battery_voltage = Serial.read() / 10.0;
            for (int i = 0; i < 4; i++) {
                  debug_val_plus[i] = Serial.read();
                  debug_val_minus[i] = Serial.read();
            }

            for (int i = 0; i < 4; i++) {
                  debug_val[i] = SimplifyDeg(debug_val_plus[i] == 0 ? debug_val_minus[i] * -1 : debug_val_plus[i]);
            }
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }
}

void imu() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 3), CenterY(32));
            oled.print("IMU");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, String(yaw).length()), CenterY(32));
            oled.print(yaw);
            oled.drawFilledEllipse(64 + 25 * cos((yaw - 90) * PI / 180.000), 32 + 25 * sin((yaw - 90) * PI / 180.000), 2, 2);
            oled.drawEllipse(64, 32, 25, 25);

            is_yaw_correction = 0;
            if (set_val != 0) {
                  is_yaw_correction = 1;
            }
      } else {
            sub_item = 0;
      }

      for (int i = 0; i < 16; i++) {
            led.SetColor((360 - (yaw - 11.25)) / 22.5, 0, 0, 200);
      }

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t yaw_plus = Serial.read();
                  uint8_t yaw_minus = Serial.read();

                  yaw = SimplifyDeg(yaw_plus == 0 ? yaw_minus * -1 : yaw_plus);
            }
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }
}

void lidar() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 5), CenterY(32));
            oled.print("LiDAR");

            for (int i = 0; i < 16; i++) {
                  led.SetColor(i, 100 - tof_val[i] * 0.5, tof_val[i] * 0.5, 0);
            }
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, 8), CenterY(12));
            oled.print("Safe dir");
            oled.setCursor(CenterX(64, String(safe_dir).length()), CenterY(32));
            oled.print(safe_dir);

            led.SetColor((safe_dir - 11.25) / 22.5, 100, 0, 0);
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(64, 10), CenterY(12));
            oled.print("Min sensor");
            oled.setCursor(CenterX(64, String(min_tof_sensor).length()), CenterY(32));
            oled.print(min_tof_sensor);
            oled.setCursor(CenterX(64, String(tof_val[min_tof_sensor]).length()), CenterY(52));
            oled.print(tof_val[min_tof_sensor]);

            led.SetColor(min_tof_sensor, 100, 0, 0);
      } else {
            sub_item = 0;
      }

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t safe_dir_plus = Serial.read();
                  uint8_t safe_dir_minus = Serial.read();
                  min_tof_sensor = Serial.read();
                  for (int i = 0; i < 16; i++) {
                        tof_val[i] = Serial.read();
                  }

                  safe_dir = safe_dir_plus == 0 ? safe_dir_minus * -1 : safe_dir_plus;
                  safe_dir -= safe_dir > 180 ? 360 : (safe_dir < -180 ? -360 : 0);
            }
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }
}

void speed() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 5), CenterY(32));
            oled.print("Speed");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, 5), CenterY(16));
            oled.print("Normal");
            oled.setCursor(CenterX(64, String(moving_speed).length()), CenterY(32));
            oled.print(moving_speed);
            moving_speed += set_val * 5;
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(64, 5), CenterY(16));
            oled.print("Line");
            oled.setCursor(CenterX(64, String(line_moving_speed).length()), CenterY(32));
            oled.print(line_moving_speed);
            line_moving_speed += set_val * 5;
      } else {
            sub_item = 0;
      }
}

void dribbler() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 8), CenterY(32));
            oled.print("Dribbler");
            dribbler_sig = 0;
      } else if (sub_item == 1) {
            static uint8_t cnt = 0;
            oled.setCursor(CenterX(64, 5), CenterY(32));
            oled.print("Front");
            if (dribbler_sig == 2) cnt++;
            if (cnt > 5) {
                  dribbler_sig = 0;
                  cnt = 0;
            }
            if (set_val == -1) {
                  dribbler_sig = 1;
            } else if (set_val == 1) {
                  dribbler_sig = 2;
            }
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Back");
            if (set_val == -1) {
                  dribbler_sig = 3;
            } else if (set_val == 1) {
                  dribbler_sig = 4;
            }
      } else {
            sub_item = 0;
      }
}

void ball() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Ball");
      } else if (sub_item == 1) {
            oled.setCursor(0, CenterY(5));
            oled.print("Dir:");
            oled.print(ball_dir);
            oled.setCursor(0, CenterY(20));
            oled.print("Dis:");
            oled.print(ball_dis);

            oled.drawFilledEllipse(96 + ((200 - ball_dis) / 8 + 3) * cos((ball_dir - 90) * PI / 180.000), 32 + ((200 - ball_dis) / 8 + 3) * sin((ball_dir - 90) * PI / 180.000), 2, 2);
            oled.drawEllipse(96, 32, 30, 30);
            oled.drawEllipse(96, 32, 15, 15);
            oled.drawFilledEllipse(96, 32, 2, 2);
            oled.drawLine(64, 32, 128, 32);
            oled.drawLine(96, 0, 96, 64);
      } else {
            sub_item = 0;
      }

      led.SetColor((ball_dir - 11.25) / 22.5, 100, 0, 0);
      if (is_ball_catch_front == 1) {
            led.SetColor(0, 0, 0, 100);
      }
      if (is_ball_catch_back == 1) {
            led.SetColor(8, 0, 0, 100);
      }

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t ball_dir_plus = Serial.read();
                  uint8_t ball_dir_minus = Serial.read();
                  ball_dis = Serial.read();
                  is_ball_catch_front = Serial.read();
                  is_ball_catch_back = Serial.read();

                  ball_dir = SimplifyDeg(ball_dir_plus == 0 ? ball_dir_minus * -1 : ball_dir_plus);
                  while (Serial.available() > 0) {
                        Serial.read();
                  }
            }
      }
}

void goal() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Goal");

            led.SetColor((y_goal_dir - 11.25) / 22.5, 100, 100, 0);
            led.SetColor((b_goal_dir - 11.25) / 22.5, 0, 0, 100);
      } else if (sub_item == 1) {
            oled.setCursor(0, CenterY(16));
            oled.print("Yellow");
            oled.setCursor(0, CenterY(32));
            oled.print("D:");
            oled.print(y_goal_dir);
            oled.setCursor(0, CenterY(48));
            oled.print("S:");
            oled.print(y_goal_size);
            led.SetColor((y_goal_dir - 11.25) / 22.5, 100, 100, 0);

            oled.setCursor(64, CenterY(16));
            oled.print("Blue");
            oled.setCursor(64, CenterY(32));
            oled.print("D:");
            oled.print(b_goal_dir);
            oled.setCursor(64, CenterY(48));
            oled.print("S:");
            oled.print(b_goal_size);
            led.SetColor((b_goal_dir - 11.25) / 22.5, 0, 0, 100);
      } else {
            sub_item = 0;
      }

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t y_goal_dir_plus = Serial.read();
                  uint8_t y_goal_dir_minus = Serial.read();
                  y_goal_size = Serial.read();
                  uint8_t b_goal_dir_plus = Serial.read();
                  uint8_t b_goal_dir_minus = Serial.read();
                  b_goal_size = Serial.read();

                  y_goal_dir = SimplifyDeg(y_goal_dir_plus == 0 ? y_goal_dir_minus * -1 : y_goal_dir_plus);
                  b_goal_dir = SimplifyDeg(b_goal_dir_plus == 0 ? b_goal_dir_minus * -1 : b_goal_dir_plus);
                  while (Serial.available() > 0) {
                        Serial.read();
                  }
            }
      }
}

void battery_voltage_read() {
      if (Serial.available() > 0) {
            if (Serial.peek() == 'E') {
                  if (Serial.read() == 'E' && Serial.read() == 'R') {
                        is_voltage_decrease = 1;
                  }
            }
      }
}

void button_read() {
      for (uint8_t i = 0; i < 3; i++) {
            pre_is_button[i] = is_button[i];
      }
      for (uint8_t i = 0; i < 3; i++) {
            is_button[i] = digitalRead(button_pin[i]);
      }

      if (is_button[0] == 1 && pre_is_button[0] == 0) {
            if (mode == 0) {
                  if (sub_item == 0) {
                        tone(buzzer_pin, 1000, 50);
                        delay(50);
                        tone(buzzer_pin, 1250, 50);
                        delay(50);
                  } else {
                        tone(buzzer_pin, 1250, 50);
                        delay(50);
                  }
                  sub_item += 1;
            } else {
                  mode = 0;
            }
      }

      set_val = 0;
      if (is_button[1] == 1) {
            if (sub_item == 0) {
                  item -= 1;
            } else {
                  set_val -= 1;
            }
            if (pre_is_button[1] == 0) {
                  tone(buzzer_pin, 1500, 50);
                  delay(250);
            } else {
                  tone(buzzer_pin, 1500, 50);
                  delay(50);
            }
      }
      if (is_button[2] == 1) {
            if (sub_item == 0) {
                  item += 1;
            } else {
                  set_val += 1;
            }
            if (pre_is_button[2] == 0) {
                  tone(buzzer_pin, 1500, 50);
                  delay(250);
            } else {
                  tone(buzzer_pin, 1500, 50);
                  delay(50);
            }
      }
}