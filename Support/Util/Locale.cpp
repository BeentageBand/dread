#include "Locale.h"
#include "Parser.h"

Locale::Locale(void) {}

void Locale::CheckSeconds(void) {
  Locale &locale = Locale::get();
  locale.increaseSeconds();
}

void Locale::CheckMinutes(void) {
  Locale &locale = Locale::get();
  locale.increaseMinutes();
}

Locale &Locale::get(void) {
  static Locale locale;
  return locale;
}

void Locale::getDateTime(uint8_t *const datetime) {
  Parser::printNumber(datetime, 2, year);
  Parser::printNumber(datetime + 3, 2, months);
  Parser::printNumber(datetime + 6, 2, days);
  Parser::printNumber(datetime + 9, 2, hrs);
  Parser::printNumber(datetime + 12, 2, mins);
  Parser::printNumber(datetime + 15, 2, secs);
}

void Locale::setDateTime(uint8_t const *const datetime) {
  year = Parser::parseNumber(datetime, 2);
  months = Parser::parseNumber(datetime + 3, 2);
  days = Parser::parseNumber(datetime + 6, 2);
  hrs = Parser::parseNumber(datetime + 9, 2);
  mins = Parser::parseNumber(datetime + 12, 2);
  secs = Parser::parseNumber(datetime + 15, 2);
}

void Locale::increaseSeconds(void) {
  ++secs;
  fixDateTimeFormat();
}

void Locale::increaseMinutes(void) {
  ++mins;
  fixDateTimeFormat();
}

void Locale::fixDateTimeFormat(void) {
  uint8_t carrier = (secs) / 60;
  secs = secs - (carrier * 60);
  mins += carrier;

  carrier = (mins) / 60;
  mins = mins - (carrier * 60);
  hrs += carrier;

  carrier = (hrs) / 24;
  hrs = hrs - (hrs * 24);
}
