#pragma once
#include <Arduino.h>
#include <Servo.h>

enum Position {
    RETRACTED,
    EXTENDED,
    MOVING
};

class ZylinderControlService {
private:
    int extendPin;
    int retractPin;

    Position currentPosition;
    Position targetPosition;

    unsigned long moveStartTime;
    unsigned long moveDuration; // in ms (aktuelle Bewegung)
    unsigned long shortMoveDuration; // kurzer Bewegungsmodus
    unsigned long longMoveDuration;  // langer Bewegungsmodus
    bool useLongMoveDuration;        // welcher Modus wird verwendet
   

    void setRelayState(bool extend, bool retract);

public:
    ZylinderControlService(int ExtendPin, int RetractPin);
    void Init();

    void Expand();    // darf ständig aufgerufen werden
    void Retract();   // darf ständig aufgerufen werden

    // Presets und Auswahl
    void setShortMoveDuration(unsigned long ms);
    void setLongMoveDuration(unsigned long ms);
    void setUseLongMoveDuration(bool useLong);

    void Update();
    Position getPosition();
};
