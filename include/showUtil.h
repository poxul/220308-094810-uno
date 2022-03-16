void showLcdTemperature();
void showLcdHumidity();
void showLcd(unsigned int state);


enum ShowState
{
  STATE_TEMPERATURE,
  STATE_HUMIDITY,
  STATE_SOIL_SENSOR_1,
  STATE_SOIL_SENSOR_2,
  STATE_SOIL_SENSOR_3,
  STATE_MAX
};
