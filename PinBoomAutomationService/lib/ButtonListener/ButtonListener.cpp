#include "ButtonListener.h"

ButtonListener::ButtonListener(uint8_t pin, bool usePullup, unsigned long debounceTime)
: _pin(pin),
  _usePullup(usePullup),
  _debounceTime(debounceTime),
  _stableState(false),
  _lastReading(false),
  _pressedEvent(false),
  _lastDebounceTime(0)
{}

void ButtonListener::begin() {
  if (_usePullup) {
    pinMode(_pin, INPUT_PULLUP);
    _stableState = HIGH;
    _lastReading = HIGH;
  } else {
    pinMode(_pin, INPUT);
    _stableState = LOW;
    _lastReading = LOW;
  }
}

void ButtonListener::update() {
  bool reading = digitalRead(_pin);

  if (reading != _lastReading) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceTime) {
    if (reading != _stableState) {
      _stableState = reading;

      // Press-Event erkennen
      if (_usePullup && _stableState == LOW) {
        _pressedEvent = true;
      }
      else if (!_usePullup && _stableState == HIGH) {
        _pressedEvent = true;
      }
    }
  }

  _lastReading = reading;
}

bool ButtonListener::wasPressed() {
  if (_pressedEvent) {
    _pressedEvent = false;
    return true;
  }
  return false;
}

bool ButtonListener::isPressed() {
  if (_usePullup) {
    return _stableState == LOW;
  } else {
    return _stableState == HIGH;
  }
}
