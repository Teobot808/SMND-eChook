#include <Arduino.h>
#include <ArduinoJson.h>

#include <DataPacket.hpp>

StaticJsonDocument<512> packet;

void DataPacket::setKey(String key, String value) {
  packet[key] = value;
}

void DataPacket::setKey(String key, int value) {
  packet[key] = value;
}

void DataPacket::setKey(String key, float value) {
  packet[key] = value;
}

String DataPacket::getKey(String key) {
  return packet[key];
}

void DataPacket::removeKey(String key) {
  packet.remove(key);
}

String DataPacket::getJSON() {
  String output;
  serializeJson(packet, output);
  return output;
}

void DataPacket::clear() {
  packet.clear();
}