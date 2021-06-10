#include "HID.h"

#define HID_BUZZ A2
#define HID_BLD 7
#define HID_RLD 6

/*
 * Register Led State
 *
 * BIT0 = RED State
 * BIT1 = GR/BL State
 * BIT2 = TOGGLE RED
 * BIT3 = TOGGLE GR/BL
 * BIT7 = ACQUIRE
 */

#define RED_BIT 0
#define GRN_BLU_BIT 1
#define TOGGLE_RED_BIT 2
#define TOGGLE_GRN_BLU_BIT 3
#define ACQUIRE_BIT 7

#define ReadBit(word, bit) ((word >> bit) & 0x01U)
#define ClearBit(word, bit) word &= (~(0x02 << bit))
#define SetBit(word, bit) word |= (0x02 << bit)

#define LED_STATE_MASK 0x0FU
#define ACQUIRE_MASK 0x80U

HID::HID(uint8_t const buzz_pin, uint8_t const red_led_pin,
         uint8_t const greenblue_led_pin)
    : buzz_pin(buzz_pin), red_led_pin(red_led_pin),
      greenblue_led_pin(greenblue_led_pin), led_register(ACQUIRE_MASK) {
  pinMode(red_led_pin, OUTPUT);
  pinMode(greenblue_led_pin, OUTPUT);
  pinMode(buzz_pin, OUTPUT);
  setLed(LO_ALL);
  digitalWrite(buzz_pin, LOW);
}

HID &HID::get(void) { return HID::get(HID_BUZZ, HID_RLD, HID_BLD); }

HID &HID::get(uint8_t const buzz_pin, uint8_t const red_led_pin,
              uint8_t const green_led_pin) {
  static HID hid(buzz_pin, red_led_pin, green_led_pin);
  return hid;
}

void HID::CheckTime(void) {
  // Blinking Led method
  // if blink methods are HIGH, led s toggles, else the led s don't change.
  HID &hid = HID::get();
  if (!hid.isLocked()) {
    hid.onCheckTime();
  }

  uint16_t tick = 0;
  // Change hour:minutes if the seconds overflows 60 counts.
  if (tick >= 0) ///////////////////CHANGE to 60 further. 20 is for debugging
                 ///purposes!!!
  {
    // Use if methods, may use less memory than math.
    // Every 60 seconds, add a minute and seconds resets.
    // minutes++;
    tick = 0;
    uint8_t file_ptr = 0;
    ////Every minute the UPLOAD method is available.
    uint8_t gsm_task = 0;
  }
}

void HID::setLed(uint8_t const setup) {
  acquire();
  led_register &= ~(LED_STATE_MASK);
  led_register |= setup & LED_STATE_MASK;
  release();
}

bool HID::isLocked(void) { return static_cast<bool>(ReadBit(led_register, 7)); }

void HID::acquire(void) { ClearBit(led_register, 7); }

void HID::release(void) { SetBit(led_register, 7); }

void HID::onCheckTime(void) {
  uint8_t red =
      ReadBit(led_register, RED_BIT) ^ ReadBit(led_register, TOGGLE_RED_BIT);
  uint8_t green_blue = ReadBit(led_register, GRN_BLU_BIT) ^
                       ReadBit(led_register, TOGGLE_GRN_BLU_BIT);

  digitalWrite(red_led_pin, red);
  digitalWrite(greenblue_led_pin, green_blue);

  ClearBit(led_register, RED_BIT);
  ClearBit(led_register, GRN_BLU_BIT);

  led_register |= (red << RED_BIT) | (green_blue << GRN_BLU_BIT);
}

void HID::successfulChime(void) {
  acquire();
  digitalWrite(red_led_pin, LOW);
  digitalWrite(greenblue_led_pin, HIGH);
  //#warning digitalWrite(HID_BUZZ,HIGH) was commented, it-s annoying!!
  digitalWrite(buzz_pin, HIGH); // BEEEEEP, if commented is for debugging
                                // purposes, it's annoying!!!
  // Reboots SPI communication, switches to RFID again.
  delay(800);
  digitalWrite(buzz_pin, LOW);
  digitalWrite(greenblue_led_pin, LOW);
  release();
  setLed(led_register);
}

void HID::failureChime(void) {
  acquire();
  digitalWrite(greenblue_led_pin, LOW);

  digitalWrite(red_led_pin, HIGH);
  digitalWrite(buzz_pin, HIGH);
  delay(200);
  digitalWrite(red_led_pin, LOW);
  digitalWrite(buzz_pin, LOW);
  delay(200);
  digitalWrite(red_led_pin, HIGH);
  digitalWrite(buzz_pin, HIGH);
  delay(200);
  digitalWrite(red_led_pin, LOW);
  digitalWrite(buzz_pin, LOW);
  delay(200);
  digitalWrite(red_led_pin, HIGH);
  digitalWrite(buzz_pin, HIGH);
  delay(200);
  digitalWrite(red_led_pin, LOW);
  digitalWrite(buzz_pin, LOW);
  release();
  setLed(led_register);
}

void HID::beep(void) {
  digitalWrite(buzz_pin, HIGH);
  delay(10);
  digitalWrite(buzz_pin, LOW);
  delay(10);
  digitalWrite(buzz_pin, HIGH);
  delay(10);
  digitalWrite(buzz_pin, LOW);
}
