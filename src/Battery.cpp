#include <Arduino.h>

#include <Battery.hpp>

void Battery::init() {
  pinMode(A0, INPUT);
  pinMode(A7, INPUT);
}

float Battery::readTotalVoltage() {
  float voltage = analogRead(A0);
  voltage = (voltage / 1024) * 5;
  voltage = voltage * 6.15;

  return voltage;
}

float Battery::readCellVoltage(int cell) {
  if (cell < 1 || cell > 2) return 0;

  if (cell == 1) {
    float voltageCell1 = analogRead(A7);
    voltageCell1 = (voltageCell1 / 1024) * 5;
    voltageCell1 = voltageCell1 * 6.15;

    return voltageCell1;
  } else if (cell == 2) {
    float totalVoltage = readTotalVoltage();
    float voltageCell2 = totalVoltage - readCellVoltage(1);

    return voltageCell2;
  }
  return 0;
}