#include <Arduino.h>

#include <Battery.hpp>
#include <Bluetooth.hpp>
#include <DataPacket.hpp>
#include <Temperature.hpp>
#include <Bounce2.h>
#include <time.hpp>
#include <Speed.hpp>

unsigned long long time::t = 0;

void setup() {
  Bluetooth::init();
  Battery::init();
}

void loop() {
  time::t = millis();
  if ((time::t % 5000) == 0) {
    DataPacket::setKey("totalVoltage", Battery::readTotalVoltage());
    DataPacket::setKey("cell1Voltage", Battery::readCellVoltage(1));
    DataPacket::setKey("cell2Voltage", Battery::readCellVoltage(2));
    DataPacket::setKey("Temperature", Temperature::readTempOne());
    DataPacket::setKey("Temperature", Temperature::readTempTwo());
    DataPacket::setKey("RPM", Speed::readRPM());

    String packet = DataPacket::getJSON();
    Bluetooth::sendData(packet);
  }

  Speed::MeasureRPM();



 // delay(5000);

// And thanks for help with debugging to KZBMMM. 
}