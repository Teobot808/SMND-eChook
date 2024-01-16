#include <Arduino.h>

#include <Battery.hpp>
#include <Bluetooth.hpp>
#include <DataPacket.hpp>
#include <Temperature.hpp>
#include <Bounce2.h>

void setup() {
  Bluetooth::init();
  Battery::init();
}

void loop() {
  DataPacket::setKey("totalVoltage", Battery::readTotalVoltage());
  DataPacket::setKey("cell1Voltage", Battery::readCellVoltage(1));
  DataPacket::setKey("cell2Voltage", Battery::readCellVoltage(2));
  DataPacket::setKey("Temperature", Temperature::readTempOne());
  DataPacket::setKey("Temperature", Temperature::readTempTwo());



  String packet = DataPacket::getJSON();

  Bluetooth::sendData(packet);
  delay(5000);
}