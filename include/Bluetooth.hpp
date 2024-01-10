class Bluetooth {
 public:
  static void init();
  static void flushSerial();
  static void waitForSerial(int timeOut);
  static void sendData(String data);
};