#include "SerialCommunicationService.h"

SerialCommunicationService::SerialCommunicationService() {

}

void SerialCommunicationService::begin(long baudRate) {
    _baudRate = baudRate;
    Serial.begin(_baudRate);
}

void SerialCommunicationService::sendData(byte data) {
    Serial.write(data);
}

byte SerialCommunicationService::receiveData() {
    if(Serial.available() == 0) {
        return 0x00; // No data available
    }
    return Serial.read();
}