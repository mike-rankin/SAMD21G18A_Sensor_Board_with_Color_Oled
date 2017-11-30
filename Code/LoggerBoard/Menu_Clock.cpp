#include "Menu.h"
#include <RTClib.h>
#include "Color.h"

enum selection_t {
  SEL_HOUR,
  SEL_MIN,
  SEL_SEC,
  SEL_YEAR,
  SEL_MONTH,
  SEL_DAY,

  SEL_MAX,
};

extern DateTime now;
extern RTC_DS1307 rtc;

SetClockMenu::SetClockMenu()
: selection(0)
{}

void SetClockMenu::draw(SSD_13XX* display) const {
  char buff[5];
  drawLayout(display, "Set Clock");

  display->setTextScale(2);
  display->setCursor(10, 20);

  snprintf(buff, 5, "%02i", now.hour());
  if (selection == SEL_HOUR) display->setTextColor(COLOR_RED);
  display->print(buff);
  display->setTextColor(COLOR_TEXT);
  display->print(':');

  snprintf(buff, 5, "%02i", now.minute());
  if (selection == SEL_MIN) display->setTextColor(COLOR_RED);
  display->print(buff);
  display->setTextColor(COLOR_TEXT);
  display->print(':');

  snprintf(buff, 5, "%02i", now.second());
  if (selection == SEL_SEC) display->setTextColor(COLOR_RED);
  display->print(buff);
  display->setTextColor(COLOR_TEXT);

  display->setCursor(15, 40);

  display->setTextScale(1);

  if (selection == SEL_YEAR) display->setTextColor(COLOR_RED);
  display->print(now.year());
  display->setTextColor(COLOR_TEXT);
  display->print('-');

  snprintf(buff, 5, "%02i", now.month());
  if (selection == SEL_MONTH) display->setTextColor(COLOR_RED);
  display->print(buff);
  display->setTextColor(COLOR_TEXT);
  display->print('-');

  snprintf(buff, 5, "%02i", now.day());
  if (selection == SEL_DAY) display->setTextColor(COLOR_RED);
  display->print(buff);
  display->setTextColor(COLOR_TEXT);
}

bool SetClockMenu::update(Buttons& buttons) {
  if (buttons.back()) {
    switchMenu(MENU_SETTINGS);
    return true;
  }
  if (buttons.right()) {
    selection = (selection + 1) % SEL_MAX;
    return true;
  }
  if (buttons.left()) {
    selection = (selection + SEL_MAX - 1) % SEL_MAX;
    return true;
  }

  if (buttons.up() || buttons.down()) {
    TimeSpan change;
    if (selection == SEL_YEAR) change = DateTime(now.year()+1, now.month(), now.day()) - DateTime(now.year(), now.month(), now.day());
    if (selection == SEL_MONTH)change = DateTime(now.year(), now.month()+1, now.day()) - DateTime(now.year(), now.month(), now.day());
    if (selection == SEL_DAY)  change = TimeSpan(1, 0, 0, 0);
    if (selection == SEL_HOUR) change = TimeSpan(0, 1, 0, 0);
    if (selection == SEL_MIN)  change = TimeSpan(0, 0, 1, 0);
    if (selection == SEL_SEC)  change = TimeSpan(0, 0, 0, 1);
    if (buttons.up()) now = now + change;
    if (buttons.down()) now = now - change;
    rtc.adjust(now);
    return true;
  }

  return false;
}

