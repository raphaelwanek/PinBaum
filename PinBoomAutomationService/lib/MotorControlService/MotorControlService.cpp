#include "MotorControlService.h"
#include <Arduino.h>

MotorControlService::MotorControlService(int Speed, int Brake, int Direction)
    : speedPin(Speed), brakePin(Brake), directionPin(Direction) 
    {

    }

void MotorControlService::Init() 
{
 
}

void MotorControlService::Drive(int speed, MotorDirection direction, int accleration) 
{
  
}
