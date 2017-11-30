#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include "Menu.h"
#include "Color.h"
#include "Logger.h"


const PROGMEM uint8_t SD_ICON[] = {
  0b00000000,
  0b00001110,
  0b00011110,
  0b00111110,
  0b00111110,
  0b00111110,
  0b00111110,
  0b00000000,
};
const PROGMEM uint8_t SD_CROSS[] = {
  0b00000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001,
};
const PROGMEM uint8_t RECORDING_ICON[] = {
  0b00111100,
  0b01111110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
};

extern DateTime now;
extern Logger logger;
Menu* menu[MENU_MAX] = {0};
const char* menu_title[MENU_MAX] = {0};
bool menu_show[MENU_MAX] = {0};
Menu_selection menu_current = MENU_SETTINGS;


void switchMenu(Menu_selection s) {
  menu_current = s;
  menu[menu_current]->onEnter();
}

void registerMenu(Menu_selection type, Menu* instance, const char* title, bool show_in_menu) {
  menu[type] = instance;
  menu_title[type] = title;
  menu_show[type] = show_in_menu;
}

Menu* currentMenu() {
  return menu[menu_current];
}

Menu::Menu() {}

void Menu::drawLayout(SSD_13XX* display, const char* title) const {
  display->fillScreen(COLOR_BG_1);
  display->fillRect(0, 0, display->width(), 8, COLOR_CHROME);

  // Title
  display->setTextColor(COLOR_TEXT);
  display->setTextScale(1);
  display->setCursor(2, 1);
  display->print(title);

  // Clock
  char buff[8];
  display->setCursor(display->width() - 41, 1);
  snprintf_P(buff, 7, PSTR("%02d:%02d"), now.hour(), now.minute());
  display->print(buff);

  display->drawBitmap(display->width() - 8, 0, SD_ICON, 8, 8, logger.sd == SD_BADCARD ? COLOR_RED : 0);
  if (logger.sd == SD_NOCARD) {
    display->drawBitmap(display->width() - 8, 0, SD_CROSS, 8, 8, COLOR_RED);
  }

  if (logger.recording) {
    display->drawBitmap(display->width() - 16, 0, RECORDING_ICON, 8, 8, COLOR_RED);
  }
}

SettingsMenu::SettingsMenu() {}

void SettingsMenu::draw(SSD_13XX* display) const {
  drawLayout(display, "Main");
  int offset = 0;
  for (int i = 0; i < MENU_MAX; ++i) {
    if (menu_show[i] == false) continue;
    int y = offset * 8 + 10;
    display->setCursor(8, y);
    if (selection == i) {
      display->fillRect(0, y, display->width(), 8, COLOR_HEADER);
    }
    display->print(menu_title[i]);
    ++offset;
  }
}

bool SettingsMenu::update(Buttons& buttons) {
  if (buttons.up()) {
    do {
      selection = (selection + MENU_MAX - 1) % MENU_MAX;
    } while (!menu_show[selection]);
    return true;
  } else if (buttons.down()) {
    do {
      selection = (selection + 1) % MENU_MAX;
    } while (!menu_show[selection]);
    return true;
  }
  if (buttons.enter()) {
    switchMenu((Menu_selection)selection);
    return true;
  }
  return false;
}

