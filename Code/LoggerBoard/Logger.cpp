#include "Logger.h"
#include <RTClib.h>
#include <SD.h>
#include <Adafruit_MPL115A2.h>
#include <ClosedCube_HDC1080.h>

extern Adafruit_MPL115A2 pressure;
extern ClosedCube_HDC1080 humidity;
extern DateTime now;

File currentLog;

void dateTime(uint16_t* date, uint16_t* time);

Logger::Logger()
: recording(false)
, linesWritten(0)
, last(0)
, sd(SD_NOCARD)
{}

void Logger::update() {
  if (recording && sd != SD_INSERTED) {
    stop();
  }

  if (recording && currentLog && millis() - last > 1000) {
    char buffer[200];
    float temperature, pressureReading;
    pressure.getPT(&pressureReading, &temperature);
    snprintf(
      buffer, 200,
      "%i-%i-%iT%02i:%02i:%02i,%f,%f,%f,%f",
      now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
      temperature,
      humidity.readTemperature(),
      pressureReading,
      humidity.readHumidity()
    );
    currentLog.println(buffer);
    last = millis();
    ++linesWritten;
  }
}

void Logger::start() {
  if (recording) return;
  SdFile::dateTimeCallback(dateTime);
  currentLog = SD.open("LOG.csv", FILE_WRITE);
  if (currentLog) recording = true;
  linesWritten = 0;
}

void Logger::stop() {
  if (!recording) return;
  if (currentLog) currentLog.close();
  recording = false;
}

void dateTime(uint16_t* date, uint16_t* time) {
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year(), now.month(), now.day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

