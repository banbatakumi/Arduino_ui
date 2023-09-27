#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

const uint8_t led_pin = 2;
const uint8_t buzzer_pin = 8;
const uint8_t button_pin[3] = {6, 7, 5};   // center, left, right

Adafruit_NeoPixel pixels(16, led_pin, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void led_effect(uint8_t led_mode_);
void led(int8_t led_num_, uint8_t red_, uint8_t green_, uint8_t blue_);
void print(String str_, uint8_t size_, uint8_t x_ = 0, uint8_t y_ = 0);
void print_continue(String str_);
void print_center(String str_, uint8_t size_, uint8_t y_ = 255);
void button_read();

void home();
void lidar();
void moving_speed();
void dribbler();
void imu();
void cam();

void battery_voltage_read();
void serial_send();

int8_t item = 0, sub_item = 0;
int8_t set_val = 0;
bool is_button[3], pre_is_button[3];

// receive data
int16_t yaw;
uint8_t tof_val[16];
uint8_t min_tof_sensor;
uint8_t wall_dir;
int16_t safe_dir;
uint8_t motor_rotation_num[4];
uint8_t ball;

// send data
bool yaw_correction = 0;
uint8_t mode = 0;
uint8_t moving_speed_1 = 50;
uint8_t dribbler_sig = 0;

bool is_voltage_decrease = 0;
float battery_voltage;

void setup() {
      oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      oled.setTextColor(SSD1306_WHITE);
      oled.clearDisplay();
      print_center("setting", 2);
      oled.display();

      Serial.begin(19200);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

      pixels.begin();
      for (uint8_t i = 0; i < 16; i++) {
            pixels.rainbow();
            for (uint8_t j = 0; j < 15 - i; j++) {
                  led(15 - j, 0, 0, 0);
            }
            pixels.show();
            delay(50);
      }
      delay(250);
      tone(buzzer_pin, 1500, 75);
      delay(250);
      tone(buzzer_pin, 2000, 75);
      delay(100);
      tone(buzzer_pin, 3000, 75);
      delay(250);
      for (uint8_t i = 0; i < 16; i++) {
            pixels.rainbow();
            for (uint8_t j = 0; j < i + 1; j++) {
                  led(j, 0, 0, 0);
            }
            pixels.show();
            delay(50);
      }

      pixels.clear();
      pixels.show();
}

void loop() {
      battery_voltage_read();

      pixels.clear();
      oled.clearDisplay();
      if (is_voltage_decrease == 1) {
            for (int i = 0; i < 16; i++) {
                  led(i, 100, 0, 0);
            }
            oled.clearDisplay();
            print_center("Low", 2, 0);
            print_center("Voltage", 2, 20);
            print("Change to new battery", 0, 0, 50);
      } else {
            button_read();
            if (sub_item == 0) {
                  if (item > -2) oled.fillTriangle(10, 53, 0, 58, 10, 63, WHITE);
                  if (item < 2) oled.fillTriangle(117, 53, 128, 58, 117, 63, WHITE);
            }

            if (item == 0) {
                  if (sub_item == 0) {
                        print_center("Home", 2);
                        print("Battery: ", 1);
                        if (battery_voltage > 8) {
                              print_continue("High");
                        } else if (battery_voltage > 7) {
                              print_continue("Medium");
                        } else if (battery_voltage > 6) {
                              print_continue("Low");
                        } else {
                              print_continue("None");
                        }
                        mode = 0;
                  } else if (sub_item == 1) {
                        if (mode == 0) {
                              print_center("Offence", 2);
                        } else {
                              print_center("Running", 2);
                        }
                        if (set_val != 0) {
                              mode = 1 - mode;
                        }
                  } else if (sub_item == 2) {
                        if (mode == 0) {
                              print_center("Defense", 2);
                        } else {
                              print_center("Running", 2);
                        }
                        if (set_val != 0) {
                              mode = 2 - mode;
                        }
                  } else {
                        sub_item = 0;
                  }
                  home();
            } else if (item == 1) {
                  if (sub_item == 0) {
                        print_center("IMU", 2);
                  } else if (sub_item == 1) {
                        print_center(String(yaw), 5);

                        for (int i = 0; i < 16; i++) {
                              led((360 - (yaw - 11.25)) / 22.5, 0, 0, 200);
                        }

                        yaw_correction = 0;
                        if (set_val != 0) {
                              yaw_correction = 1;
                        }
                  } else {
                        sub_item = 0;
                  }
                  imu();
            } else if (item == 2) {
                  if (sub_item == 0) {
                        print_center("LiDAR", 2);
                  } else if (sub_item == 1) {
                        print_center("Running", 2);

                        for (int i = 0; i < 16; i++) {
                              led(i, 100 - tof_val[i] * 0.5, tof_val[i] * 0.5, 0);
                        }
                  } else if (sub_item == 2) {
                        print_center("Safe", 2, 0);
                        print_center(String(safe_dir), 2);

                        led((safe_dir - 11.25) / 22.5, 100, 0, 0);
                  } else if (sub_item == 3) {
                        print_center("Min sensor", 2, 0);
                        print_center(String(min_tof_sensor), 2, 20);
                        print_center(String(tof_val[min_tof_sensor]), 2, 40);

                        led(min_tof_sensor, 100, 0, 0);
                  } else if (sub_item == 4) {
                        print_center("Wall", 2, 0);
                        print_center(String(wall_dir), 2);

                        led(wall_dir, 100, 0, 0);
                  } else {
                        sub_item = 0;
                  }
                  lidar();
            } else if (item == -1) {
                  if (sub_item == 0) {
                        print_center("Speed", 2);
                  } else if (sub_item == 1) {
                        print_center("1", 2, 0);
                        print_center(String(moving_speed_1), 2);
                        moving_speed_1 += set_val * 5;
                  } else {
                        sub_item = 0;
                  }
                  moving_speed();
            } else if (item == -2) {
                  if (sub_item == 0) {
                        print_center("Dribbler", 2);
                        dribbler_sig = 0;
                  } else if (sub_item == 1) {
                        print_center("Front", 2);
                        if (set_val == -1) {
                              dribbler_sig = 1;
                        } else if (set_val == 1) {
                              dribbler_sig = 2;
                        }
                  } else if (sub_item == 2) {
                        print_center("Back", 2);
                        if (set_val == -1) {
                              dribbler_sig = 3;
                        } else if (set_val == 1) {
                              dribbler_sig = 4;
                        }
                  } else {
                        sub_item = 0;
                  }
                  dribbler();
            } else if (item == 3) {
                  if (sub_item == 0) {
                        print_center("Cam", 2);
                  } else if (sub_item == 1) {
                        print_center(String(ball), 2);
                  } else {
                        sub_item = 0;
                  }
                  cam();
            }
            serial_send();
      }
      oled.display();
      pixels.show();
}

void home() {
      if (Serial.available() > 0) {
            battery_voltage = Serial.read() / 10.0;
            while (Serial.available() > 0) {
                  Serial.read();
            }
      }
}

void imu() {
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
      if (Serial.available() > 0) {
            if (Serial.read() == 0xFF) {
                  uint8_t safe_dir_plus = Serial.read();
                  uint8_t safe_dir_minus = Serial.read();
                  min_tof_sensor = Serial.read();
                  wall_dir = Serial.read();
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
}

void dribbler() {
}

void cam() {
      if (Serial.available() > 0) {
            ball = Serial.read();
            while (Serial.available() > 0) {
                  Serial.read();
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
      Serial.flush();
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

void led_effect(uint8_t led_mode_) {
      if (led_mode_ == 0) {
            pixels.clear();
            pixels.show();
      } else if (led_mode_ == 1) {
            for (uint8_t i = 0; i < 16; i++) {
                  pixels.setPixelColor(i, pixels.Color(50, 0, 0));
                  pixels.show();
                  delay(50);
            }
            for (uint8_t i = 0; i < 16; i++) {
                  pixels.setPixelColor(i, pixels.Color(0, 50, 0));
                  pixels.show();
                  delay(50);
            }
            for (uint8_t i = 0; i < 16; i++) {
                  pixels.setPixelColor(i, pixels.Color(0, 0, 50));
                  pixels.show();
                  delay(50);
            }
      } else if (led_mode_ == 2) {
            for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
                  pixels.rainbow(firstPixelHue);
                  pixels.show();
                  delay(5);
            }
      }
}

void led(int8_t led_num_, uint8_t red_, uint8_t green_, uint8_t blue_) {
      int8_t led_num_tmp = 20 - led_num_;
      if (led_num_tmp > 15) {
            led_num_tmp -= 16;
      }
      if (led_num_tmp < 0) {
            led_num_tmp += 16;
      }
      pixels.setPixelColor(led_num_tmp, pixels.Color(red_, green_, blue_));
}

void print_center(String str_, uint8_t size_, uint8_t y_) {
      uint8_t x_center = 65 - str_.length() * 3 * size_;
      uint8_t y_center = 32 - size_ * 3;

      oled.setTextSize(size_);
      if (y_ != 255) {
            oled.setCursor(x_center, y_);
      } else {
            oled.setCursor(x_center, y_center);
      }
      oled.print(str_.c_str());
}

void print(String str_, uint8_t size_, uint8_t x_, uint8_t y_) {
      oled.setTextSize(size_);
      oled.setCursor(x_, y_);
      oled.print(str_.c_str());
}

void print_continue(String str_) {
      oled.print(str_.c_str());
}
