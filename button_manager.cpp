#include "button_manager.h"

ButtonManager::ButtonManager()
  : _lastState(HIGH), _currentState(HIGH),
    _pressedFlag(false), _pressTime(0), _lastDebounce(0) {}

void ButtonManager::begin() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Active LOW
  Serial.println("[BTN] Ready");
}

void ButtonManager::update() {
  bool raw = digitalRead(BUTTON_PIN);
  unsigned long now = millis();

  if (raw != _lastState) {
    _lastDebounce = now;
  }

  if ((now - _lastDebounce) > DEBOUNCE_MS) {
    if (_currentState == HIGH && raw == LOW) {
      // Falling edge — button just pressed
      _pressedFlag = true;
      _pressTime   = now;
    }
    _currentState = raw;
  }

  _lastState = raw;
}

bool ButtonManager::wasPressed() {
  if (_pressedFlag) {
    _pressedFlag = false;
    return true;
  }
  return false;
}

bool ButtonManager::isHeld(int ms) {
  return (_currentState == LOW && (millis() - _pressTime) >= (unsigned long)ms);
}
