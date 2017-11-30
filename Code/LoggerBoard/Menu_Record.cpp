#include "Menu.h"
#include "Logger.h"

extern Logger logger;


RecordMenu::RecordMenu()
{}

void RecordMenu::draw(SSD_13XX* display) const {
  drawLayout(display, "Record");

  if (logger.sd != SD_INSERTED) {
    display->setCursor(0, 15);
    display->print("Insert SD card");
    return;
  }
  
  display->setCursor(0, 15);

  if (logger.recording) {
    display->println("Recording...");
    display->println("Press enter to stop");
  } else {
    display->println("Press enter to start recording");
  }
  display->print("Lines: ");
  display->println(logger.linesWritten);
}

bool RecordMenu::update(Buttons& buttons) {
  if (buttons.back()) {
    switchMenu(MENU_SETTINGS);
    return true;
  }
  if (buttons.enter() && logger.sd == SD_INSERTED) {
    if (logger.recording) {
      logger.stop();
    } else {
      logger.start();
    }
    return true;
  }
  return false;
}

