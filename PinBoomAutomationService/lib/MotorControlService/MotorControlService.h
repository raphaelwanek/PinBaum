#ifndef MOTORCONTROLSERVICE_H
#define MOTORCONTROLSERVICE_H

#include <Arduino.h>

enum MotorDirection {
    FORWARD,
    REVERSE
};

class MotorControlService {
public:
    MotorControlService(int Speedpin, int Brakepin, int Directionpin);
    void Init();
    void Drive(int speed, MotorDirection direction, int accleration);
    void Stop();
    void update();
private:
    int speedPin;
    int brakePin;
    int directionPin;
    unsigned long timer1;
};

#endif