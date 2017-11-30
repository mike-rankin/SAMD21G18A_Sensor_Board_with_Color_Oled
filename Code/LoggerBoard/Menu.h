#ifndef MENU_H
#define MENU_H

#include <SSD_13XX.h>
#include "Buttons.h"

enum Menu_selection {
  MENU_SETTINGS,
  MENU_READINGS,
  MENU_SET_CLOCK,
  MENU_RECORD,
  MENU_BROWSE,

  MENU_MAX,
};

class Menu {
public:
  Menu();
  virtual bool update(Buttons& buttons) = 0;
  virtual void draw(SSD_13XX* display) const = 0;
  virtual void onEnter() {}
protected:
  void drawLayout(SSD_13XX* display, const char* title) const;
};

class SettingsMenu : public Menu {
public:
  SettingsMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;
  int selection = MENU_READINGS;
};

class ReadingsMenu : public Menu {
public:
  ReadingsMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;
};

class SetClockMenu : public Menu {
public:
  SetClockMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;

private:
  int selection;
};

class RecordMenu : public Menu {
public:
  RecordMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;
};

class BrowseMenu : public Menu {
public:
  BrowseMenu();
  bool update(Buttons& buttons);
  void draw(SSD_13XX* display) const;
  void updateListing();
  void onEnter();
private:
  int selection;
  String currentPath;
  bool isDirectory;
};

void registerMenu(Menu_selection type, Menu* instance, const char* title, bool show_in_menu);
void switchMenu(Menu_selection);
Menu* currentMenu();

#endif

