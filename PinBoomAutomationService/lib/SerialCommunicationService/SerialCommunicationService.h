#ifndef SERIALCOMMUNICATIONSERVICE_H
#define SERIALCOMMUNICATIONSERVICE_H

#include <Arduino.h>

class SerialCommunicationService {
public:
    SerialCommunicationService();
    void begin(long baudRate);
    void sendData(byte data);
    byte receiveData();
private:
    long _baudRate;
};

#endif 