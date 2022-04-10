void showLcd(unsigned int state);

enum ShowState
{
  STATE_SHOW_TEMPERATURE,
  STATE_SHOW_HUMIDITY,
  STATE_SHOW_SOIL_SENSOR_1,
  STATE_SHOW_SOIL_SENSOR_2,
  STATE_SHOW_SOIL_SENSOR_3,
  STATE_SHOW_DATE,
  STATE_SHOW,
  STATE_TEST_PUMP_1,
  STATE_TEST_PUMP_2,
  STATE_TEST_PUMP_3,
  STATE_MAX
};

void showLcdAlarm(int alarm);
void showLcdEdit(int edit, bool run);