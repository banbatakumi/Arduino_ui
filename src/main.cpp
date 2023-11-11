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

const unsigned char minato_akua[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0x01, 0x3c, 0x08, 0x7e, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfb, 0x7f, 0xff, 0x93, 0x1d, 0x00, 0xef, 0x7d, 0xff, 0xff,
    0xff, 0xf7, 0xff, 0xff, 0xef, 0xdc, 0xfb, 0xfe, 0xff, 0x87, 0x06, 0x80, 0x9d, 0xfb, 0xff, 0xff,
    0xff, 0x7f, 0xfe, 0xdf, 0xed, 0xf8, 0xdd, 0xff, 0xff, 0x9f, 0xc8, 0xe0, 0x77, 0x7e, 0xff, 0xff,
    0x7f, 0xff, 0xdb, 0xff, 0x9e, 0xf3, 0x7d, 0xf9, 0xff, 0x4f, 0x08, 0x3c, 0xff, 0xe5, 0xff, 0x9e,
    0x7f, 0xe7, 0x49, 0x7f, 0x7f, 0x96, 0xfb, 0xef, 0x7f, 0x73, 0x33, 0xf1, 0x6d, 0x7f, 0xfe, 0xe7,
    0xbf, 0xff, 0x2e, 0xbb, 0xf7, 0x3f, 0xef, 0xff, 0xff, 0x93, 0x64, 0xcb, 0xff, 0xbe, 0x33, 0x6f,
    0xff, 0xfd, 0xff, 0xef, 0xbf, 0xff, 0xfc, 0xff, 0xff, 0xa9, 0x4f, 0xc8, 0xb6, 0xbf, 0xed, 0xff,
    0xff, 0xff, 0xff, 0xbf, 0xfd, 0xf8, 0xf9, 0xdf, 0xdf, 0xdd, 0x9b, 0xb2, 0xff, 0x7f, 0xff, 0xcf,
    0xff, 0x7f, 0xf3, 0x9f, 0xf7, 0xf8, 0xf3, 0xdf, 0x7f, 0xde, 0x3e, 0x23, 0xdb, 0x7f, 0xdb, 0xdf,
    0xdf, 0xff, 0xcf, 0x59, 0xde, 0xda, 0x13, 0xff, 0x7f, 0xff, 0x77, 0x4f, 0xfe, 0xdf, 0xff, 0x77,
    0x97, 0xff, 0xbf, 0xdf, 0xfd, 0x7d, 0xa7, 0xf5, 0xdf, 0xff, 0xdf, 0x43, 0xb6, 0xff, 0xf6, 0xff,
    0xa7, 0xfd, 0x7f, 0x9f, 0xef, 0xff, 0xa7, 0xed, 0x9f, 0xef, 0xfb, 0x1f, 0xff, 0xbf, 0xff, 0xbf,
    0xf3, 0xfe, 0x7f, 0x3d, 0x7b, 0xef, 0x8c, 0xe7, 0xff, 0x7d, 0xff, 0x06, 0xec, 0xff, 0xfd, 0xbf,
    0x7f, 0xbe, 0xdb, 0xbd, 0x7b, 0xef, 0xe8, 0xff, 0xcf, 0x77, 0xdf, 0xb7, 0x7c, 0x7f, 0xf7, 0xcf,
    0xdf, 0xbd, 0xed, 0x3e, 0xfb, 0xfb, 0x48, 0xfb, 0xdf, 0xfd, 0xf7, 0x8d, 0xde, 0xef, 0xff, 0xf3,
    0x9f, 0xfd, 0xad, 0x7f, 0x36, 0xdf, 0x27, 0xf6, 0xde, 0xed, 0xbe, 0x2f, 0xfe, 0xff, 0xee, 0x3f,
    0x7f, 0x5e, 0xb6, 0x9d, 0xf6, 0x7c, 0xb3, 0xbc, 0xd9, 0x6f, 0xdb, 0xa7, 0xfc, 0xff, 0xfb, 0x87,
    0xf7, 0xdf, 0x7f, 0xfe, 0xe6, 0xb3, 0xc9, 0xf9, 0xc3, 0x7e, 0xdf, 0x9e, 0xfe, 0xbf, 0xdf, 0x78,
    0xf7, 0xff, 0x6f, 0xde, 0xd8, 0xef, 0x4c, 0x3e, 0x67, 0xd9, 0xbd, 0x0b, 0xfe, 0xff, 0x7f, 0x66,
    0xcf, 0xff, 0x9f, 0xd9, 0x9c, 0x3f, 0xb1, 0xf7, 0x7f, 0xb2, 0xed, 0x6d, 0xfe, 0xff, 0x2f, 0x8d,
    0xbb, 0xb7, 0x37, 0x3d, 0x39, 0x44, 0x9b, 0xfc, 0xde, 0xe3, 0x6f, 0x0d, 0xfe, 0xff, 0xce, 0xfc,
    0xff, 0xfb, 0x7f, 0x71, 0xe0, 0x64, 0x66, 0xbf, 0xbf, 0xe7, 0x9f, 0xad, 0xfe, 0xff, 0xd3, 0x7b,
    0xef, 0x6d, 0xcf, 0xf7, 0xa7, 0x9b, 0x94, 0x7f, 0xbf, 0xff, 0x1f, 0x26, 0xff, 0xff, 0x33, 0xff,
    0xf9, 0xdc, 0xf3, 0xe6, 0x81, 0xfb, 0xd9, 0xd7, 0xce, 0xff, 0x1f, 0x44, 0xff, 0xff, 0xe9, 0xff,
    0xcf, 0xf6, 0x7b, 0xe3, 0x07, 0x26, 0xe3, 0xb7, 0x7b, 0xff, 0xff, 0x99, 0xf7, 0xff, 0xe9, 0xff,
    0xdc, 0xfe, 0xbe, 0xc3, 0x6f, 0x08, 0xce, 0xed, 0x4f, 0xff, 0xbf, 0x9b, 0x80, 0xff, 0xf6, 0xff,
    0x7d, 0xdb, 0xf3, 0x81, 0x3f, 0xb3, 0x9c, 0xd9, 0xf7, 0xff, 0xff, 0x63, 0x2e, 0xfc, 0xf4, 0xff,
    0xe6, 0x3d, 0x7b, 0x2c, 0xff, 0x24, 0x69, 0x72, 0x9b, 0x4f, 0xff, 0xed, 0xb3, 0x79, 0xfc, 0xff,
    0x9e, 0xbd, 0x43, 0x3e, 0xff, 0xe9, 0xcb, 0xfe, 0x6b, 0xff, 0xfc, 0xf6, 0x6f, 0xe3, 0xf8, 0xff,
    0x7b, 0xf3, 0x96, 0x37, 0xfc, 0xfb, 0x9b, 0xfd, 0xcf, 0xff, 0xff, 0xf2, 0xdf, 0xde, 0xfe, 0xff,
    0xed, 0xee, 0xf7, 0x7d, 0xfc, 0x7f, 0xbb, 0xfd, 0xdf, 0xfc, 0x3f, 0xff, 0xbf, 0x9d, 0xf8, 0xff,
    0xbf, 0xed, 0x7f, 0xef, 0xf6, 0xff, 0x47, 0xb3, 0xbd, 0xf9, 0xff, 0xf9, 0xff, 0x7b, 0xfc, 0xff,
    0xf3, 0x7d, 0xfc, 0xbb, 0xe6, 0xdf, 0x47, 0xf6, 0xff, 0xfd, 0x93, 0xee, 0x7f, 0x7f, 0xfe, 0xff,
    0xef, 0x66, 0xdf, 0xfe, 0x9f, 0xfd, 0x82, 0x60, 0x7f, 0x6d, 0xf6, 0xfe, 0xff, 0xf6, 0xf8, 0xff,
    0xde, 0x3b, 0xff, 0xef, 0x7f, 0xf3, 0x02, 0xc8, 0xff, 0xd9, 0xcd, 0xf9, 0xff, 0xbe, 0xf9, 0xff,
    0x96, 0x9d, 0xff, 0x7d, 0xfe, 0x8f, 0x06, 0x90, 0xfe, 0x33, 0x3d, 0xef, 0xff, 0xef, 0xf1, 0xff,
    0x6f, 0xe6, 0x7f, 0xdf, 0xff, 0x6f, 0x85, 0x80, 0xff, 0x63, 0xf3, 0xfe, 0xff, 0x7d, 0x0d, 0xdd,
    0xed, 0x5a, 0x7e, 0xff, 0xff, 0x7f, 0x1d, 0x8c, 0xff, 0xc7, 0xce, 0xfa, 0xff, 0xfd, 0xff, 0xff,
    0xb3, 0x49, 0x93, 0x3f, 0xb2, 0x9f, 0x73, 0x8e, 0xfd, 0xb7, 0xbd, 0xf9, 0xff, 0xbf, 0xfe, 0xff,
    0x3b, 0x77, 0x77, 0xfe, 0x8c, 0xb7, 0xe6, 0xa7, 0xff, 0xbf, 0xb5, 0xcd, 0xfe, 0xfb, 0xfe, 0xdf,
    0xcb, 0xd4, 0xce, 0xdb, 0x13, 0x4e, 0x8c, 0xc6, 0xff, 0xcb, 0x76, 0xbe, 0xfb, 0x6d, 0xff, 0xff,
    0x67, 0x7b, 0xba, 0x77, 0x77, 0x08, 0x63, 0xc6, 0xfd, 0x7f, 0xdb, 0xf3, 0x82, 0x7d, 0xff, 0xff,
    0x66, 0x6b, 0x31, 0x01, 0xed, 0x3b, 0xc6, 0x63, 0xff, 0x7f, 0xdb, 0x5f, 0x16, 0xdf, 0xfe, 0xff,
    0x9a, 0xcc, 0xcf, 0xac, 0x9b, 0x61, 0x26, 0x63, 0xff, 0xff, 0xf5, 0xdc, 0x34, 0xcd, 0xec, 0xff,
    0xe2, 0x36, 0x6f, 0x92, 0xdb, 0x06, 0xe3, 0x24, 0x9f, 0xff, 0x7d, 0x33, 0xef, 0xa6, 0x9f, 0xef,
    0xe6, 0xd9, 0xb7, 0x01, 0x67, 0x1f, 0xd8, 0x36, 0x0f, 0xff, 0x3f, 0xcf, 0x7b, 0x33, 0xff, 0xfc,
    0x46, 0x5f, 0xdf, 0x03, 0xb4, 0xd9, 0x5c, 0x93, 0x7b, 0xbf, 0x07, 0xdb, 0x9e, 0xc9, 0xff, 0xff,
    0x74, 0x73, 0x3f, 0x89, 0x00, 0x9e, 0x73, 0x13, 0xf6, 0xcc, 0x43, 0x2f, 0x5b, 0x4c, 0xde, 0xef,
    0xc4, 0x9e, 0x9f, 0xa4, 0xc3, 0x83, 0xd3, 0x56, 0xf3, 0x99, 0xc3, 0xed, 0x60, 0xdd, 0xbe, 0xdf,
    0x8e, 0xed, 0x9f, 0x84, 0xff, 0x19, 0xbf, 0x4f, 0x08, 0xfc, 0xb1, 0xbf, 0x27, 0xbf, 0xbd, 0xbf,
    0x88, 0x9d, 0x1f, 0x91, 0xff, 0x5d, 0x7e, 0x8b, 0x3e, 0x70, 0xaf, 0xf9, 0xfb, 0x3f, 0x79, 0xbe,
    0x58, 0xf3, 0x5b, 0x06, 0xff, 0xcf, 0xf8, 0x9e, 0x18, 0xc0, 0x6c, 0x6f, 0xf9, 0x7f, 0x77, 0x7e,
    0x18, 0x62, 0x9e, 0x7e, 0xfe, 0xe7, 0xfc, 0x16, 0x02, 0x92, 0x75, 0xfb, 0x3c, 0xff, 0x73, 0xfb,
    0x98, 0x8e, 0x9d, 0x7f, 0xff, 0xff, 0xfd, 0x37, 0x00, 0x38, 0xd5, 0x56, 0xfe, 0xbf, 0x5f, 0xfe,
    0xb0, 0x98, 0x1d, 0xef, 0xfc, 0x9f, 0xfd, 0x6d, 0x40, 0x18, 0xde, 0x9c, 0xff, 0xff, 0xdf, 0x7c,
    0x24, 0x63, 0xde, 0xfb, 0xfc, 0xff, 0xfb, 0xd7, 0x08, 0x00, 0x6e, 0xa7, 0xdf, 0xff, 0xf7, 0xfe,
    0x64, 0x24, 0xdb, 0xff, 0xff, 0x7f, 0xf3, 0x9d, 0xb7, 0x01, 0xbb, 0xeb, 0xd7, 0xff, 0xbf, 0xfe,
    0x68, 0xe7, 0xff, 0xdd, 0xfb, 0xff, 0xce, 0x7f, 0xb7, 0xc3, 0xec, 0xde, 0xf7, 0x7f, 0xef, 0x7f,
    0x62, 0x0e, 0xf6, 0x7f, 0xfb, 0xff, 0x3b, 0x7f, 0xcc, 0xe6, 0x5f, 0xfb, 0xdf, 0xff, 0x7f, 0xff,
    0x92, 0x9e, 0x7c, 0xef, 0xc7, 0xff, 0x7f, 0xfc, 0xdb, 0x64, 0xd8, 0xec, 0xf7, 0xbb, 0xff, 0xbf,
    0xd3, 0x3d, 0xdb, 0xfd, 0x8d, 0xfb, 0xdf, 0x7b, 0xb6, 0x1f, 0xf0, 0xdc, 0xf7, 0xff, 0xf3, 0xfd,
    0x6f, 0x71, 0xef, 0xdf, 0x3f, 0xfe, 0x7f, 0xf7, 0xb4, 0x99, 0x29, 0xff, 0xdb, 0xcf, 0xe7, 0xfd,
    0xab, 0xe7, 0xfd, 0xfe, 0x7e, 0xfe, 0x3f, 0xf8, 0x67, 0x37, 0x27, 0xe3, 0xfb, 0x67, 0x79, 0xff};

void ButtonRead();

void Home();
void Speed();
void Dribbler();
void Imu();
void Ball();
void Goal();
void Line();

void BatteryVoltageRead();

int8_t item = 0, sub_item = 0;
int8_t set_val = 0;
bool is_button[3], pre_is_button[3];

// receive data
uint8_t motor_rotation_num[4];

// send data
bool is_own_dir_correction;
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

      Serial.begin(57600);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

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
            BatteryVoltageRead();

            oled.firstPage();
            do {
                  led.Clear();

                  if (is_voltage_decrease == 1) {
                  } else {
                        ButtonRead();
                        if (sub_item == 0) {
                              if (item > -2) oled.drawTriangle(10, 53, 0, 58, 10, 63);
                              if (item < 4) oled.drawTriangle(117, 53, 128, 58, 117, 63);
                        }

                        if (item == 0) {
                              // Home();
                              oled.drawXBMP(0, 0, 128, 64, minato_akua);
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

      static uint8_t data_length;   // データの長さ
      const uint8_t recv_data_num = 9;
      static uint8_t recv_data[recv_data_num];
      uint8_t debug_val_plus[4];
      uint8_t debug_val_minus[4];

      if (data_length == 0) {   // ヘッダの受信
            if (Serial.read() == 0xFF) {
                  data_length++;
            } else {
                  data_length = 0;
            }
      } else if (data_length == recv_data_num + 1) {
            if (Serial.read() == 0xAA) {
                  for (int i = 0; i < 4; i++) {
                        debug_val_plus[i] = recv_data[i * 2];
                        debug_val_minus[i] = recv_data[i * 2 + 1];
                        debug_val[i] = debug_val_plus[i] == 0 ? debug_val_minus[i] * -1 : debug_val_plus[i];
                  }
                  battery_voltage = recv_data[8] / 10.0;
            }

            data_length = 0;
      } else {
            recv_data[data_length - 1] = Serial.read();
            data_length++;
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
            for (uint8_t i = 0; i < 16; i++) {
                  led.SetColor(i, 100, 100, 100);
            }
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

      static uint8_t data_length;   // データの長さ
      const uint8_t recv_data_num = 2;
      static uint8_t recv_data[recv_data_num];
      uint8_t own_dir_plus;
      uint8_t own_dir_minus;

      if (data_length == 0) {   // ヘッダの受信
            if (Serial.read() == 0xFF) {
                  data_length++;
            } else {
                  data_length = 0;
            }
      } else if (data_length == recv_data_num + 1) {
            if (Serial.read() == 0xAA) {
                  own_dir_plus = recv_data[0];
                  own_dir_minus = recv_data[1];

                  own_dir = own_dir_plus == 0 ? own_dir_minus * -1 : own_dir_plus;
            }

            data_length = 0;
      } else {
            recv_data[data_length - 1] = Serial.read();
            data_length++;
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

      led.SetColor(round((360 - own_dir) / 22.5) % 16, 100, 0, 0);
}

void Speed() {
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

      static uint8_t data_length;   // データの長さ
      const uint8_t recv_data_num = 5;
      static uint8_t recv_data[recv_data_num];
      uint8_t ball_dir_plus;
      uint8_t ball_dir_minus;

      if (data_length == 0) {   // ヘッダの受信
            if (Serial.read() == 0xFF) {
                  data_length++;
            } else {
                  data_length = 0;
            }
      } else if (data_length == recv_data_num + 1) {
            if (Serial.read() == 0xAA) {
                  ball_dir_plus = recv_data[0];
                  ball_dir_minus = recv_data[1];
                  ball_dis = recv_data[2];
                  is_ball_catch_front = recv_data[3];
                  is_ball_catch_back = recv_data[4];

                  ball_dir = ball_dir_plus == 0 ? ball_dir_minus * -1 : ball_dir_plus;
            }

            data_length = 0;
      } else {
            recv_data[data_length - 1] = Serial.read();
            data_length++;
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
      } else {
            sub_item = 0;
      }

      led.SetColor(round(ball_dir / 22.5) % 16, 100, 0, 0);
      if (is_ball_catch_front == 1) {
            led.SetColor(0, 0, 0, 100);
      }
      if (is_ball_catch_back == 1) {
            led.SetColor(8, 0, 0, 100);
      }
}

void Goal() {
      static int16_t y_goal_dir;
      static uint8_t y_goal_size;
      static int16_t b_goal_dir;
      static uint8_t b_goal_size;

      static uint8_t data_length;   // データの長さ
      const uint8_t recv_data_num = 6;
      static uint8_t recv_data[recv_data_num];
      uint8_t y_goal_dir_plus;
      uint8_t y_goal_dir_minus;
      uint8_t b_goal_dir_plus;
      uint8_t b_goal_dir_minus;

      if (data_length == 0) {   // ヘッダの受信
            if (Serial.read() == 0xFF) {
                  data_length++;
            } else {
                  data_length = 0;
            }
      } else if (data_length == recv_data_num + 1) {
            if (Serial.read() == 0xAA) {
                  y_goal_dir_plus = recv_data[0];
                  y_goal_dir_minus = recv_data[1];
                  y_goal_size = recv_data[2];
                  b_goal_dir_plus = recv_data[3];
                  b_goal_dir_minus = recv_data[4];
                  b_goal_size = recv_data[5];

                  y_goal_dir = y_goal_dir_plus == 0 ? y_goal_dir_minus * -1 : y_goal_dir_plus;
                  b_goal_dir = b_goal_dir_plus == 0 ? b_goal_dir_minus * -1 : b_goal_dir_plus;
            }

            data_length = 0;
      } else {
            recv_data[data_length - 1] = Serial.read();
            data_length++;
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
      } else {
            sub_item = 0;
      }

      led.SetColor(round(y_goal_dir / 22.5) % 16, 100, 100, 0);
      led.SetColor(round(b_goal_dir / 22.5) % 16, 0, 0, 100);
}

void Line() {
      static int16_t line_dir;
      static int16_t line_inside_dir;
      static uint8_t line_interval;
      static uint8_t line_white_qty;

      static uint8_t data_length;   // データの長さ
      const uint8_t recv_data_num = 6;
      static uint8_t recv_data[recv_data_num];
      uint8_t line_dir_plus;
      uint8_t line_dir_minus;
      uint8_t line_inside_dir_plus;
      uint8_t line_inside_dir_minus;

      if (data_length == 0) {   // ヘッダの受信
            if (Serial.read() == 0xFF) {
                  data_length++;
            } else {
                  data_length = 0;
            }
      } else if (data_length == recv_data_num + 1) {
            if (Serial.read() == 0xAA) {
                  line_dir_plus = recv_data[0];
                  line_dir_minus = recv_data[1];
                  line_inside_dir_plus = recv_data[2];
                  line_inside_dir_minus = recv_data[3];
                  line_interval = recv_data[4];
                  line_white_qty = recv_data[5];

                  line_dir = line_dir_plus == 0 ? line_dir_minus * -1 : line_dir_plus;
                  line_inside_dir = line_inside_dir_plus == 0 ? line_inside_dir_minus * -1 : line_inside_dir_plus;
            }

            data_length = 0;
      } else {
            recv_data[data_length - 1] = Serial.read();
            data_length++;
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

      led.SetColor(round(line_inside_dir / 22.5) % 16, 0, 100, 0);
}

void BatteryVoltageRead() {
      if (Serial.available() > 0) {
            if (Serial.peek() == 'E') {
                  if (Serial.read() == 'E' && Serial.read() == 'R') {
                        is_voltage_decrease = 1;
                  }
            }
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