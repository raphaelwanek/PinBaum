#ifndef BUTTONLISTENER_H
#define BUTTONLISTENER_H

#include <Arduino.h>

class ButtonListener {
  public:
    ButtonListener(uint8_t pin, bool usePullup = true, unsigned long debounceTime = 50);

    void begin();
    void update();

    bool wasPressed();     // true NUR einmal pro Tastendruck
    bool isPressed();      // aktueller stabiler Zustand

  private:
    uint8_t _pin;
    bool _usePullup;
    unsigned long _debounceTime;

    bool _stableState;
    bool _lastReading;
    bool _pressedEvent;

    unsigned long _lastDebounceTime;
};

#endif
