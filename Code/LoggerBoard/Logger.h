#ifndef LOGGER_H
#define LOGGER_H

enum SD_STATE {
  SD_NOCARD,
  SD_BADCARD,
  SD_INSERTED,
};

class Logger {
  public:
  Logger();
  void begin();

  void start();
  void stop();
  void update();

  bool recording;
  unsigned long last;
  unsigned int linesWritten;
  SD_STATE sd;
};

#endif

