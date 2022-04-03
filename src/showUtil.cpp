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

// rt clock
extern RtcDS3231<TwoWire> Rtc;


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
    lcd.print(" %");
}

void showSoil(const char *name, const char *text, unsigned int value, bool blink)
{
    lcd.setCursor(0, 0);
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print(text);
    lcd.print(" (");
    lcd.print(value);
    lcd.print(")");
    if(blink)
    {
        lcd.setCursor(0, 1);
        lcd.blink_on();
    }
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


void showSoilResult(const char* txt, SoilResult* soilResult){
    showSoil(txt, 
        getValueString(soilResult->getSoilCondition()), 
        soilResult->getValue(),
        soilResult->getSoilCondition() == SoilResult::SoilCondition::dry);
}
/**
 * @brief show soil moisture sensor 1
 *
 */
void showSoil1()
{    
    showSoilResult("Pflanze 1", &soilResult1);
}

/**
 * @brief show soil moisture sensor 2
 *
 */
void showSoil2()
{
    showSoilResult("Pflanze 2", &soilResult2); 
}

/**
 * @brief show soil moisture sensor 3
 *
 */
void showSoil3()
{
    showSoilResult("Pflanze 3", &soilResult3 ); 
}

void showDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               20,
               PSTR("%02u/%02u/%04u"),
               dt.Month(),
               dt.Day(),
               dt.Year());
    lcd.setCursor(0, 0);
    lcd.print(datestring);

    snprintf_P(datestring,
               20,
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    lcd.setCursor(0, 1);
    lcd.print(datestring);
}

/**
 * @brief show state called every 3 seconds
 *
 */
void showLcd(unsigned int state)
{
    Serial.print("Start show state: ");
    Serial.println(state);
    lcd.clear();
    lcd.blink_off();
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
    case STATE_DATE:
        showDateTime(Rtc.GetDateTime());
        break;

    default:
        break;
    }
}