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

extern SoilResult soilResult1;
extern SoilResult soilResult2;
extern SoilResult soilResult3;

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

void showSoil(const char *name, const char *text, unsigned int value)
{
    lcd.setCursor(0, 0);
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print(text);
    lcd.print(" ");
    lcd.print(value);
}

const char *getValueString(SoilResult::SoilCondition condition)
{
    switch (condition)
    {
    case SoilResult::SoilCondition::dry:
        return "trocken";
    case SoilResult::SoilCondition::wet:
        return "feucht";
    case SoilResult::SoilCondition::humid:
        return "ok";
    case SoilResult::SoilCondition::water:
        return "nass";
    default:
        return "???";
    }
}

/**
 * @brief show soil moisture sensor 1
 *
 */
void showSoil1()
{
    showSoil("Pflanze 1", getValueString(soilResult1.getSoilCondition()), soilResult1.getValue());
}

/**
 * @brief show soil moisture sensor 2
 *
 */
void showSoil2()
{
    showSoil("Pflanze 2", getValueString(soilResult2.getSoilCondition()), soilResult2.getValue());
}

/**
 * @brief show soil moisture sensor 3
 *
 */
void showSoil3()
{
    showSoil("Pflanze 3", getValueString(soilResult3.getSoilCondition()), soilResult3.getValue());
}

/**
 * @brief show state called every 3 seconds
 *
 */
void showLcd(unsigned int state)
{
    Serial.print("start show: ");
    Serial.println(state);

    switch (state)
    {
    case STATE_TEMPERATURE:
        showLcdTemperature();
        break;
    case STATE_HUMIDITY:
        showLcdHumidity();
        break;
    case STATE_SOIL_SENSOR_1:
        showSoil1();
        break;
    case STATE_SOIL_SENSOR_2:
        showSoil2();
        break;
    case STATE_SOIL_SENSOR_3:
        showSoil3();
        break;

    default:
        break;
    }
}