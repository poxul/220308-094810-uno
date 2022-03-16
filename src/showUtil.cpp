#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RtcDS3231.h>

#include "debugLogger.h"
#include "soilMoisture.h"
#include "dhtMeasure.h"
#include "lcdUtil.h"
#include "rtClock.h"
#include "showUtil.h"

extern DebugLogger logger;
extern LiquidCrystal_I2C lcd;
extern DhtResult temperatureResult;


/**
 * @brief show state called every 3 seconds
 *
 */
void showLcd(unsigned int state)
{
  Serial.println("start show");

  switch (state)
  {
  case STATE_TEMPERATURE:
    showLcdTemperature();
    break;
  case STATE_HUMIDITY:
    showLcdHumidity();
    break;

  default:
    break;
  }
}

/**
 * @brief show temperature on lcd
 *
 */
void showLcdTemperature()
{
  lcd.setCursor(0, 0);
  lcd.print("Temperatur:");
  lcd.setCursor(6, 1);
  lcd.print(temperatureResult.getTemperature());
  lcd.print(" C");
}

/**
 * @brief show air humidity on lcd
 *
 */
void showLcdHumidity()
{
  lcd.setCursor(0, 0);
  lcd.print("Luftfeuchtigkeit");
  lcd.setCursor(4, 1);
  lcd.print(temperatureResult.getHumidity());
  lcd.print(" %%");
}