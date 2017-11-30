#include <vector>
#include <Arduino.h>
#include <SD.h>
#include "Menu.h"
#include "Logger.h"
#include "Color.h"


extern Logger logger;

struct FileInfo {
  String name;
  bool isDirectory;
  FileInfo(const String& name, bool isDirectory)
  : name(name), isDirectory(isDirectory) {}
};

enum FILE_OPTIONS {
  OPTION_BACK,
  OPTION_DELETE,

  OPTION_MAX,
};

std::vector<FileInfo> currentFileListing;

String baseDir(const String& path) {
  if (path.length() <= 1) {
    return path;
  }
  size_t last = path.lastIndexOf('/', path.length() - 2);
  return path.substring(0, last + 1);
}


const PROGMEM uint8_t FOLDER_ICON[] = {
  0b00000000,
  0b01110000,
  0b01111110,
  0b01000010,
  0b01000010,
  0b01000010,
  0b01111110,
  0b00000000,
};


BrowseMenu::BrowseMenu()
: selection(0)
, currentPath("/")
, isDirectory(true)
{}

void BrowseMenu::draw(SSD_13XX* display) const {
  drawLayout(display, "Browse SD");

  if (logger.sd != SD_INSERTED) {
    display->setCursor(0, 15);
    display->print("Insert SD card");
    return;
  }

  int y = 10;
  if (isDirectory) {
    int start = max(0, selection - 3);
    for (int i = start; i < currentFileListing.size() && y < display->height(); ++i) {
      display->setCursor(8, y);
      if (selection == i) {
        display->fillRect(0, y, display->width(), 8, COLOR_HEADER);
      }
      display->print(currentFileListing[i].name);
      if (currentFileListing[i].isDirectory) {
        display->drawBitmap(0, y, FOLDER_ICON, 8, 8, 0);
      }
      y += 8;
    }
  } else {
    display->setCursor(8, y);
    display->print(currentPath);
    y += 8 * 2;
    for (int i = 0; i < OPTION_MAX; ++i) {
      display->setCursor(8, y);
      if (selection == i) {
        display->fillRect(0, y, display->width(), 8, COLOR_HEADER);
      }
      switch (i) {
        case OPTION_BACK: 
          display->print("Back");
          display->drawBitmap(0, y, FOLDER_ICON, 8, 8, 0);
          break;
        case OPTION_DELETE:
          display->print("Delete");
          break;
      }
      y += 8;
    }
  }
}

bool BrowseMenu::update(Buttons& buttons) {
  if (buttons.back()) {
    switchMenu(MENU_SETTINGS);
    return true;
  }
  if (logger.sd != SD_INSERTED && currentFileListing.size() > 0) {
     currentFileListing.clear();
     return true;
  } else if (logger.sd == SD_INSERTED && currentFileListing.size() == 0) {
    updateListing();
    return true;
  }
  if (buttons.up() || buttons.down()) {
    int max = isDirectory ? currentFileListing.size() : OPTION_MAX;
    if (buttons.up()) {
      selection = selection <= 0 ? max - 1 : selection - 1;
    } else {
      selection = (selection + 1) % max;
    }
    return true;
  }
  if (buttons.enter()) {
    if (isDirectory) {
      if (selection >= currentFileListing.size() || selection < 0) return false;
      if (currentFileListing[selection].name == "..") {
        currentPath = baseDir(currentPath);
      } else {
        currentPath += currentFileListing[selection].name;
        if (currentFileListing[selection].isDirectory) {
          currentPath += "/";
        }
      }
      updateListing();
      return true;
    } else {
      if (selection == OPTION_BACK) {
        currentPath = baseDir(currentPath);
        updateListing();
        return true;
      } else if (selection == OPTION_DELETE) {
        SD.remove(currentPath);
        currentPath = baseDir(currentPath);
        updateListing();
        return true;
      }
    }
  }
  return false;
}

void BrowseMenu::updateListing() {
  selection = 0;
  currentFileListing.clear();
  if (currentPath.length() != 1) {
    currentFileListing.push_back(
      FileInfo("..", true)
    );
  }
  File root = SD.open(currentPath);
  if (!root) return;
  isDirectory = root.isDirectory();
  if (isDirectory) {
    root.rewindDirectory();
    File entry = root.openNextFile();
    while (entry) {
      currentFileListing.push_back(
        FileInfo(
          entry.name(),
          entry.isDirectory()
        )
      );
      entry.close();
      entry = root.openNextFile();
    }
  }
  root.close();
}

void BrowseMenu::onEnter() {
  currentPath = "/";
  selection = 0;
  isDirectory = true;
  updateListing();
}


