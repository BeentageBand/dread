#ifndef HID_H
#define HID_H

#include "DreadSystem.h"

enum LedState
{
  LO_ALL  = 0x00,
  HI_RED  = 0x01,
  HI_BLU  = 0x02,
  HI_GRN  = 0x02,
  HI_ORG  = 0x03,
  HI_VLT  = 0x03,
  BLK_RED = 0x05,
  VLT_BLU = 0x07,
  ORG_GRN = 0x07,
  BLK_BLU = 0x0A,
  BLK_GRN = 0x0A,
  VLT_RED = 0x0B,
  ORG_RED = 0x0B,
  RED_BLU = 0x0D,
  RED_GRN = 0x0D,
  BLK_VLT = 0x0F,
  BLK_ORG = 0x0F
};

class HID
{
  uint8_t const buzz_pin;
  uint8_t const red_led_pin;
  uint8_t const greenblue_led_pin;
  uint8_t led_register;

  public:
  HID(uint8_t const buzz_pin, uint8_t const red_led_pin, uint8_t const greenblue_led_pin);

  static HID & get(uint8_t const buzz_pin, uint8_t const red_led_pin, uint8_t const greenblue_led_pin);
  static HID & get(void);
  static void CheckTime(void);

  void setLed(uint8_t const setup);
  void beep(void);
  void acquire(void);
  void release(void);
  void onCheckTime(void);
  bool isLocked(void);
  void successfulChime(void);
  void failureChime(void);
};

#endif /*HID_H*/
