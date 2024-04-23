#ifndef _SETUP_H_
#define _SETUP_H_

#include "Adafruit_NeoPixel.h"
#include "U8g2lib.h"
#include "neopixel.h"
#include "simplify_deg.h"

#define CenterX(x_, charnum_) (x_ - ((charnum_ * 9) / 2))
#define CenterY(y_) (y_ + 6)

const uint8_t led_pin = 2;
const uint8_t buzzer_pin = 8;
const uint8_t button_pin[3] = {6, 7, 5};  // center, left, right

U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled(U8G2_R2, /* reset=*/U8X8_PIN_NONE);
NeoPixel led;

void ButtonRead();
void SendData();

void Home();
void Speed();
void Dribbler();
void Imu();
void Camera();
void Line();
void Ir();
void Ultrasonic();

int8_t item = 0, sub_item = 0;
int8_t set_val = 0;
bool is_button[3], pre_is_button[3];

// send data
bool is_own_dir_correction;
uint8_t mode = 0;
uint8_t moving_speed = 75;
uint8_t line_moving_speed = 75;
uint8_t dribbler_sig = 0;

#endif