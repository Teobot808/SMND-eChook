class Battery {
 public:
  static void init();
  static float readTotalVoltage();
  static float readCellVoltage(int cell);
};