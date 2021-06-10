#ifndef LOCALE_H_
#define LOCALE_H_

#include <stdint.h>

#define DATETIME_SIZE (13U) //"14/02/14,13:45:08-28"*/

class Locale {
  uint8_t secs;
  uint8_t mins;
  uint8_t hrs;
  uint8_t days;
  uint8_t months;
  uint8_t year;

public:
  Locale(void);

  static void CheckMinutes(void);
  static void CheckSeconds(void);
  static Locale &get(void);

  void getDateTime(uint8_t *const datetime);
  void setDateTime(uint8_t const *const datetime);
  void increaseSeconds(void);
  void increaseMinutes(void);

private:
  void fixDateTimeFormat(void);
};

#endif /* LOCALE_H_*/
