class DataPacket {
 public:
  static void setKey(String key, int value);
  static void setKey(String key, float value);
  static void setKey(String key, String value);

  static String getKey(String key);
  static void removeKey(String key);
  static String getJSON();
  static void clear();
};