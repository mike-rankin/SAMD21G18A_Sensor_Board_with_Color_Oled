#include "Buttons.h"
#include <Bounce2.h>
#include <Arduino.h>


const int PINS[KEY_MAX] = {5, 2, 3, 11, 1, 0};
Bounce debouncer[KEY_MAX];

Buttons::Buttons()
:states(0)
{}

void Buttons::begin() {
  for (int i = 0; i < KEY_MAX; ++i) {
    pinMode(PINS[i], INPUT);
    debouncer[i].attach(PINS[i]);
    debouncer[i].interval(5);
  }
}

bool Buttons::update() {
  states = 0;
  bool got = false;
  for (int i = 0; i < KEY_MAX; ++i) {
    if (debouncer[i].update()) {
      if (debouncer[i].rose()) {
        got = true;
        states |= 1 << i;
      }
    }
  }
  return got;
}

