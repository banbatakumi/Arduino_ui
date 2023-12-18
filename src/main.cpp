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

void ButtonRead();

void Home();
void Speed();
void Dribbler();
void Imu();
void Ball();
void Goal();
void Line();

int8_t item = 0, sub_item = 0;
int8_t set_val = 0;
bool is_button[3], pre_is_button[3];

// receive data
uint8_t motor_rotation_num[4];

// send data
bool is_own_dir_correction;
uint8_t mode = 0;
uint8_t moving_speed = 80;
uint8_t line_moving_speed = 80;
uint8_t dribbler_sig = 0;

float battery_voltage;

void setup() {
      oled.begin();
      oled.setPowerSave(0);
      oled.setFlipMode(1);
      oled.setFont(u8g2_font_courR10_tr);
      oled.firstPage();

      Serial.begin(9600);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

      led.SetBrightness(50);

      for (uint8_t i = 0; i < 16 * 5; i++) {
            led.Clear();
            led.SetColor(i, 1, 1, 1);
            if (i % 16 == 0) tone(buzzer_pin, 2000, 25);
            led.Show();
            delay(20);
      }
      led.Clear();

      delay(100);
      tone(buzzer_pin, 2000, 100);
      delay(100);
      tone(buzzer_pin, 1000, 100);
      delay(100);
}

void loop() {   // 呼び出しのオーバーヘッド節減
      while (1) {
            oled.firstPage();
            do {
                  led.Clear();

                  ButtonRead();
                  if (sub_item == 0) {
                        if (item > -2) oled.drawTriangle(10, 53, 0, 58, 10, 63);
                        if (item < 4) oled.drawTriangle(117, 53, 128, 58, 117, 63);
                  }

                  if (item == 0) {
                        Home();
                  } else if (item == 1) {
                        Imu();
                  } else if (item == -1) {
                        Speed();
                  } else if (item == -2) {
                        Dribbler();
                  } else if (item == 2) {
                        Line();
                  } else if (item == 3) {
                        Ball();
                  } else if (item == 4) {
                        Goal();
                  }

                  led.Show();
            } while (oled.nextPage());

            // UART送信
            uint8_t send_byte_num = 8;
            uint8_t send_byte[send_byte_num];
            send_byte[0] = 0xFF;
            send_byte[1] = item + 127;
            send_byte[2] = mode;
            send_byte[3] = is_own_dir_correction;
            send_byte[4] = moving_speed;
            send_byte[5] = line_moving_speed;
            send_byte[6] = dribbler_sig;
            send_byte[7] = 0xAA;
            for (uint8_t i = 0; i < send_byte_num; i++) {
                  Serial.write(send_byte[i]);
            }
            Serial.flush();
      }
}

void Home() {
      static int16_t debug_val[4];
      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t debug_val_plus[4];
                  uint8_t debug_val_minus[4];

                  battery_voltage = Serial.read() / 20.00;
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

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Home");

            oled.setCursor(0, CenterY(5));
            oled.print("Battery:");
            oled.print(battery_voltage);
            oled.print("v");
            oled.drawLine(0, 20, 128, 20);
            mode = 0;
      } else if (sub_item == 1) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 7), CenterY(32));
                  oled.print("Offence");
            }
            if (set_val != 0) {
                  mode = 1 - mode;
            }
      } else if (sub_item == 2) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 7), CenterY(32));
                  oled.print("Defense");
            }
            if (set_val != 0) {
                  mode = 2 - mode;
            }
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
            if (set_val != 0) {
                  mode = 3 - mode;
            }
      } else if (sub_item == 4) {
            if (mode == 0) {
                  oled.setCursor(CenterX(64, 8), CenterY(26));
                  oled.print("Attitude");
                  oled.setCursor(CenterX(64, 7), CenterY(38));
                  oled.print("Control");
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
            if (set_val != 0) {
                  mode = 4 - mode;
            }
      } else {
            sub_item = 0;
      }
}

void Imu() {
      static int16_t own_dir;

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t yaw_plus = Serial.read();
                  uint8_t yaw_minus = Serial.read();

                  own_dir = yaw_plus == 0 ? yaw_minus * -1 : yaw_plus;
            }
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 3), CenterY(32));
            oled.print("IMU");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, String(own_dir).length()), CenterY(32));
            oled.print(own_dir);

            static uint8_t cnt = 0;

            if (set_val != 0) {
                  is_own_dir_correction = 1;
            }

            if (is_own_dir_correction == 1) cnt++;
            if (cnt > 10) {
                  is_own_dir_correction = 0;
                  cnt = 0;
            }
      } else {
            sub_item = 0;
      }

      led.SetColor(round((360 - own_dir) / 22.5) % 16, 1, 0, 0);
}

void Speed() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 5), CenterY(32));
            oled.print("Speed");
      } else if (sub_item == 1) {
            oled.setCursor(CenterX(64, 6), CenterY(16));
            oled.print("Normal");
            oled.setCursor(CenterX(64, String(moving_speed).length()), CenterY(32));
            oled.print(moving_speed);
            moving_speed += set_val * 5;
            if (moving_speed > 100) moving_speed = 100;

            for (uint8_t i = 0; i < round(moving_speed / 6.25); i++) {
                  led.SetColor(i, 0, 0, 1);
            }
      } else if (sub_item == 2) {
            oled.setCursor(CenterX(64, 4), CenterY(16));
            oled.print("Line");
            oled.setCursor(CenterX(64, String(line_moving_speed).length()), CenterY(32));
            oled.print(line_moving_speed);
            line_moving_speed += set_val * 5;
            if (line_moving_speed > 100) line_moving_speed = 100;

            for (uint8_t i = 0; i < round(line_moving_speed / 6.25); i++) {
                  led.SetColor(i, 0, 0, 1);
            }
      } else {
            sub_item = 0;
      }
}

void Dribbler() {
      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 8), CenterY(32));
            oled.print("Dribbler");
            dribbler_sig = 0;
      } else if (sub_item == 1) {
            static uint8_t cnt = 0;
            oled.setCursor(CenterX(64, 5), CenterY(32));
            oled.print("Front");
            if (set_val == -1) {
                  dribbler_sig = 1;
            } else if (set_val == 1) {
                  dribbler_sig = 2;
            }
            if (dribbler_sig == 2) cnt++;
            if (cnt > 10) {
                  dribbler_sig = 0;
                  cnt = 0;
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

void Ball() {
      static int16_t ball_dir;
      static uint8_t ball_dis;
      static bool is_ball_catch_front;
      static bool is_ball_catch_back;

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t ball_dir_plus = Serial.read();
                  uint8_t ball_dir_minus = Serial.read();
                  ball_dis = Serial.read();
                  is_ball_catch_front = Serial.read();
                  is_ball_catch_back = Serial.read();

                  ball_dir = ball_dir_plus == 0 ? ball_dir_minus * -1 : ball_dir_plus;
                  while (Serial.available() > 0) {
                        Serial.read();
                  }
            }
      }

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

            led.SetColor(round(ball_dir / 22.5) % 16, 1, 0, 0);
            if (is_ball_catch_front == 1) led.SetColor(0, 0, 0, 1);
            if (is_ball_catch_back == 1) led.SetColor(8, 0, 0, 1);
      } else {
            sub_item = 0;
      }
}

void Goal() {
      static int16_t y_goal_dir;
      static uint8_t y_goal_size;
      static int16_t b_goal_dir;
      static uint8_t b_goal_size;

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t y_goal_dir_plus = Serial.read();
                  uint8_t y_goal_dir_minus = Serial.read();
                  y_goal_size = Serial.read();
                  uint8_t b_goal_dir_plus = Serial.read();
                  uint8_t b_goal_dir_minus = Serial.read();
                  b_goal_size = Serial.read();

                  y_goal_dir = y_goal_dir_plus == 0 ? y_goal_dir_minus * -1 : y_goal_dir_plus;
                  b_goal_dir = b_goal_dir_plus == 0 ? b_goal_dir_minus * -1 : b_goal_dir_plus;
                  while (Serial.available() > 0) {
                        Serial.read();
                  }
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Goal");
      } else if (sub_item == 1) {
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

            led.SetColor(round(y_goal_dir / 22.5) % 16, 100, 100, 0);
            led.SetColor(round(b_goal_dir / 22.5) % 16, 0, 0, 100);
      } else {
            sub_item = 0;
      }
}

void Line() {
      static int16_t line_dir;
      static int16_t line_inside_dir;
      static uint8_t line_interval;
      static uint8_t line_white_qty;

      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t line_dir_plus = Serial.read();
                  uint8_t line_dir_minus = Serial.read();
                  uint8_t line_inside_dir_plus = Serial.read();
                  uint8_t line_inside_dir_minus = Serial.read();
                  line_interval = Serial.read();
                  line_white_qty = Serial.read();

                  line_dir = line_dir_plus == 0 ? line_dir_minus * -1 : line_dir_plus;
                  line_inside_dir = line_inside_dir_plus == 0 ? line_inside_dir_minus * -1 : line_inside_dir_plus;
                  while (Serial.available() > 0) {
                        Serial.read();
                  }
            }
      }

      if (sub_item == 0) {
            oled.setCursor(CenterX(64, 4), CenterY(32));
            oled.print("Line");
      } else if (sub_item == 1) {
            oled.setCursor(0, CenterY(14));
            oled.print("Dir: ");
            oled.print(line_dir);
            oled.setCursor(0, CenterY(26));
            oled.print("Inside dir: ");
            oled.print(line_inside_dir);
            oled.setCursor(0, CenterY(38));
            oled.print("Interval: ");
            oled.print(line_interval);
            oled.setCursor(0, CenterY(50));
            oled.print("White QTY: ");
            oled.print(line_white_qty);
      } else {
            sub_item = 0;
      }

      if (line_white_qty > 0) {
            led.SetColor(round(line_inside_dir / 22.5) % 16, 0, 1, 0);
      }
}

void ButtonRead() {
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
            if (sub_item == 0 && item > -2) {
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
            if (sub_item == 0 && item < 4) {
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