#pragma once

#include <Arduino.h>
#include "config.h"

class ButtonManager {
public:
  ButtonManager();
  void begin();
  void update();

  bool wasPressed();       // Single press (fires once per press)
  bool isHeld(int ms);     // Returns true if held for ms milliseconds

private:
  bool _lastState;
  bool _currentState;
  bool _pressedFlag;
  unsigned long _pressTime;
  unsigned long _lastDebounce;
};
