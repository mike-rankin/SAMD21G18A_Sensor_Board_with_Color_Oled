#ifndef BUTTONS_H
#define BUTTONS_H

enum KEYS {
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_BACK,

  KEY_MAX,
};

class Buttons {
public:
  Buttons();

  void begin();
  bool update();

  inline bool up() const {return states & 1<<KEY_UP;}
  inline bool right() const {return states & 1<<KEY_RIGHT;}
  inline bool down() const {return states & 1<<KEY_DOWN;}
  inline bool left() const {return states & 1<<KEY_LEFT;}
  inline bool enter() const {return states & 1<<KEY_ENTER;}
  inline bool back() const {return states & 1<<KEY_BACK;}

protected:
  int states;
};

#endif
