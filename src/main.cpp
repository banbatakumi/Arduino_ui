#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "neopixel.h"
#include "simplify_deg.h"

#define CenterX(_size, _x, _charnum) (_x - (((6 * (_charnum - 1) * _size) + (5 * _size)) / 2))
#define CenterY(_size, _y) (_y - (7 / 2 * _size))

const uint8_t led_pin = 2;
const uint8_t buzzer_pin = 8;
const uint8_t button_pin[3] = {6, 7, 5};   // center, left, right

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
NeoPixel led;

void button_read();

void home();
void lidar();
void moving_speed();
void dribbler();
void imu();
void ball();
void goal();

void battery_voltage_read();
void serial_send();

int8_t item = 0, sub_item = 0;
int8_t set_val = 0;
bool is_button[3], pre_is_button[3];

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

// send data
bool yaw_correction = 0;
bool is_y_goal_front = 0;
uint8_t mode = 0;
uint8_t moving_speed_1 = 50;
uint8_t dribbler_sig = 0;

bool is_voltage_decrease = 0;
float battery_voltage;

void setup() {
      delay(250);
      oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      oled.setTextColor(SSD1306_WHITE);
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setCursor(CenterX(2, 64, 7), CenterY(2, 32));
      oled.print("setting");
      oled.display();

      Serial.begin(19200);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

      delay(250);
      tone(buzzer_pin, 1500, 75);
      delay(250);
      tone(buzzer_pin, 2000, 75);
      delay(100);
      tone(buzzer_pin, 3000, 75);
      delay(250);
}

void loop() {
      battery_voltage_read();

      oled.clearDisplay();
      oled.setTextSize(2);
      led.Clear();
      if (is_voltage_decrease == 1) {
      } else {
            button_read();
            if (sub_item == 0) {
                  if (item > -2) oled.fillTriangle(10, 53, 0, 58, 10, 63, WHITE);
                  if (item < 4) oled.fillTriangle(117, 53, 128, 58, 117, 63, WHITE);
            }

            if (item == 0) {
                  home();
            } else if (item == 1) {
                  imu();
            } else if (item == 2) {
                  lidar();
            } else if (item == -1) {
                  moving_speed();
            } else if (item == -2) {
                  dribbler();
            } else if (item == 3) {
                  ball();
            } else if (item == 4) {
                  goal();
            }
            serial_send();
      }
      oled.display();
      led.Show();
}

void home() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(2, 64, 4), CenterY(2, 32));
            oled.print("Home");

            oled.setCursor(0, 0);
            oled.setTextSize(1);
            oled.print("Battery: ");
            oled.print(battery_voltage);
            oled.print("v");
            mode = 0;
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(2, 64, 7), CenterY(2, 32));
            if (mode == 0) {
                  oled.print("Offence");
            } else {
                  oled.print("Running");
            }
            if (set_val != 0) {
                  mode = 1 - mode;
            }
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(2, 64, 7), CenterY(2, 32));
            if (mode == 0) {
                  oled.print("Defense");
            } else {
                  oled.print("Running");
            }
            if (set_val != 0) {
                  mode = 2 - mode;
            }
      } else {
            sub_item = 0;
      }

      if (Serial.available() > 0) {
            battery_voltage = Serial.read() / 10.0;
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }
}

void imu() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(2, 64, 3), CenterY(2, 32));
            oled.print("IMU");
      } else if (sub_item == 1) {
            oled.setTextSize(5);
            oled.setCursor(CenterX(5, 64, String(yaw).length()), CenterY(5, 32));
            oled.print(yaw);

            yaw_correction = 0;
            if (set_val != 0) {
                  yaw_correction = 1;
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

                  yaw = yaw_plus == 0 ? yaw_minus * -1 : yaw_plus;
                  yaw -= yaw > 180 ? 360 : (yaw < -180 ? -360 : 0);
            }
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }
}

void lidar() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(2, 64, 5), CenterY(2, 32));
            oled.print("LiDAR");

            for (int i = 0; i < 16; i++) {
                  led.SetColor(i, 100 - tof_val[i] * 0.5, tof_val[i] * 0.5, 0);
            }
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(2, 64, 8), CenterY(2, 12));
            oled.print("Safe dir");
            oled.setCursor(CenterX(2, 64, String(safe_dir).length()), CenterY(2, 32));
            oled.print(safe_dir);

            led.SetColor((safe_dir - 11.25) / 22.5, 100, 0, 0);
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(2, 64, 10), CenterY(2, 12));
            oled.print("Min sensor");
            oled.setCursor(CenterX(2, 64, String(min_tof_sensor).length()), CenterY(2, 32));
            oled.print(min_tof_sensor);
            oled.setCursor(CenterX(2, 64, String(tof_val[min_tof_sensor]).length()), CenterY(2, 52));
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

void moving_speed() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(2, 64, 5), CenterY(2, 32));
            oled.print("Speed");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(2, 64, String(moving_speed_1).length()), CenterY(2, 32));
            oled.print(moving_speed_1);
            moving_speed_1 += set_val * 5;
      } else {
            sub_item = 0;
      }
}

void dribbler() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(2, 64, 8), CenterY(2, 32));
            oled.print("Dribbler");
            dribbler_sig = 0;
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(2, 64, 5), CenterY(2, 32));
            oled.print("Front");
            if (dribbler_sig == 2) dribbler_sig = 0;
            if (set_val == -1) {
                  dribbler_sig = 1;
            } else if (set_val == 1) {
                  dribbler_sig = 2;
            }
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(2, 64, 4), CenterY(2, 32));
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
            oled.setCursor(CenterX(2, 64, 4), CenterY(2, 32));
            oled.print("Ball");
      } else if (sub_item == 1) {
            oled.setCursor(0, 0);
            oled.setTextSize(1);
            oled.println("Dir");
            oled.setTextSize(2);
            oled.println(ball_dir);
            oled.setTextSize(1);
            oled.println("Dis");
            oled.setTextSize(2);
            oled.println(ball_dis);

            oled.fillCircle(96 + ((200 - ball_dis) / 8 + 3) * cos((ball_dir - 90) * PI / 180.000), 32 + ((200 - ball_dis) / 8 + 3) * sin((ball_dir - 90) * PI / 180.000), 2, WHITE);
            oled.drawCircle(96, 32, 28, WHITE);
            oled.drawCircle(96, 32, 14, WHITE);
            oled.fillCircle(96, 32, 2, WHITE);
            oled.drawFastHLine(64, 32, 64, WHITE);
            oled.drawFastVLine(96, 0, 64, WHITE);
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
            oled.setCursor(CenterX(2, 64, 4), CenterY(2, 32));
            oled.print("Goal");

            led.SetColor((y_goal_dir - 11.25) / 22.5, 100, 100, 0);
            led.SetColor((b_goal_dir - 11.25) / 22.5, 0, 0, 100);
      } else if (sub_item == 1) {
            if (set_val != 0) is_y_goal_front = 1 - is_y_goal_front;
            if (is_y_goal_front == 1) {
                  oled.setTextSize(1);
                  oled.setCursor(CenterX(1, 64, 3), CenterY(1, 10));
                  oled.println("Dir");
                  oled.setTextSize(2);
                  oled.setCursor(CenterX(2, 64, String(y_goal_dir).length()), CenterY(2, 25));
                  oled.println(y_goal_dir);
                  oled.setTextSize(1);
                  oled.setCursor(CenterX(1, 64, 3), CenterY(1, 40));
                  oled.println("Dis");
                  oled.setTextSize(2);
                  oled.setCursor(CenterX(2, 64, String(y_goal_size).length()), CenterY(2, 55));
                  oled.println(y_goal_size);

                  led.SetColor(0, 100, 100, 0);
                  led.SetColor(8, 0, 0, 100);
            } else {
                  oled.setTextSize(1);
                  oled.setCursor(CenterX(1, 64, 3), CenterY(1, 10));
                  oled.println("Dir");
                  oled.setTextSize(2);
                  oled.setCursor(CenterX(2, 64, String(b_goal_dir).length()), CenterY(2, 25));
                  oled.println(b_goal_dir);
                  oled.setTextSize(1);
                  oled.setCursor(CenterX(1, 64, 3), CenterY(1, 40));
                  oled.println("Dis");
                  oled.setTextSize(2);
                  oled.setCursor(CenterX(2, 64, String(b_goal_size).length()), CenterY(2, 55));
                  oled.println(b_goal_size);

                  led.SetColor(0, 0, 0, 100);
                  led.SetColor(8, 100, 100, 0);
            }
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

void serial_send() {
      Serial.write(0xFF);
      Serial.write(item + 100);
      Serial.write(mode);
      Serial.write(yaw_correction);
      Serial.write(moving_speed_1);
      Serial.write(dribbler_sig);
      Serial.write(0xAA);
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
                        tone(buzzer_pin, 1200, 50);
                  } else {
                        tone(buzzer_pin, 1500, 50);
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