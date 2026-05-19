#include "ZylinderControlService.h"
#include <Arduino.h>
#include <Servo.h>

ZylinderControlService::ZylinderControlService(int ExtendPin, int RetractPin)
    : extendPin(ExtendPin),
      retractPin(RetractPin),
      currentPosition(RETRACTED),
      targetPosition(RETRACTED),
    moveStartTime(0),
    moveDuration(5000),
    shortMoveDuration(5000),
    longMoveDuration(6300),
    useLongMoveDuration(false) {
}

void ZylinderControlService::setRelayState(bool extend, bool retract) {
    digitalWrite(extendPin, extend ? HIGH : LOW);
    digitalWrite(retractPin, retract ? HIGH : LOW);
}

void ZylinderControlService::Init() {
    pinMode(extendPin, OUTPUT);
    pinMode(retractPin, OUTPUT);
    setRelayState(false, false);
}

void ZylinderControlService::Expand() {
    // Schon ausgefahren oder fährt bereits dorthin → nichts tun
    if (targetPosition == EXTENDED) {
        return;
    }

    targetPosition = EXTENDED;
    currentPosition = MOVING;
    moveStartTime = millis();
    moveDuration = useLongMoveDuration ? longMoveDuration : shortMoveDuration;
    setRelayState(true, false); // Q3 ausfahren
    Serial.println("ZylinderControlService: Expand (Q3)");
}

void ZylinderControlService::Retract() {
    // Schon eingefahren oder fährt bereits dorthin → nichts tun
    if (targetPosition == RETRACTED) {
        return;
    }

    targetPosition = RETRACTED;
    currentPosition = MOVING;
    moveStartTime = millis();
    moveDuration = useLongMoveDuration ? longMoveDuration : shortMoveDuration;
    setRelayState(true, true); // Q3 + Q4 einfahren
    Serial.print("ZylinderControlService: Retract (Q3+Q4), duration=");
    Serial.println(moveDuration);
}

void ZylinderControlService::setShortMoveDuration(unsigned long ms) {
    shortMoveDuration = ms;
}

void ZylinderControlService::setLongMoveDuration(unsigned long ms) {
    longMoveDuration = ms;
}

void ZylinderControlService::setUseLongMoveDuration(bool useLong) {
    useLongMoveDuration = useLong;
}

void ZylinderControlService::Update() {
    if (currentPosition == MOVING) {
        if (millis() - moveStartTime >= moveDuration) {
            currentPosition = targetPosition;
            setRelayState(false, false); // Bewegung beendet, Relais aus
            Serial.println("ZylinderControlService: Move complete, relays off");
        }
    }
}

Position ZylinderControlService::getPosition() {
    return currentPosition;
}
