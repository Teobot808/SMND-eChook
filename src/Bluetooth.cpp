#include <Arduino.h>

#include <Bluetooth.hpp>

void Bluetooth::init() {
  Serial.begin(38400);
}

void Bluetooth::flushSerial() {
  while (Serial.available()) {
    Serial.read();
  }
}

void Bluetooth::waitForSerial(int timeOut) {
  long tempTime = millis() + timeOut;

  while (!Serial.available() && millis() < tempTime) {
  }
}

void Bluetooth::sendData(String data) {
  Serial.print(data);
  Serial.print('\n');
}