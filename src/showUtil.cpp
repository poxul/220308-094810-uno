#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RtcDS3231.h>

#include "soilMoisture.h"
#include "dhtMeasure.h"
#include "lcdUtil.h"
#include "rtClock.h"
#include "showUtil.h"
#include "text.h"


extern LiquidCrystal_I2C lcd;
extern DhtResult temperatureResult;
extern SoilResult soilResult[];
extern RtcDS3231<TwoWire> Rtc;
extern SoilMoisture soil[];

/**
 * @brief show temperature on lcd
 *
 */
void showLcdTemperature(float value)
{
    lcdShowOriginIdx(ORIGIN_TEMPERATURE);
    char temp[LCD_TEXT_LEN + 1];
    snprintf_P(temp,
               sizeof(temp),
               PSTR("%d.%01d \05C"),
               (int)value, (int)(value * 10) % 10);

    lcdShowValue(temp);
}

/**
 * @brief show air humidity on lcd
 *
 */
void showLcdHumidity(float value)
{
    lcdShowOriginIdx(ORIGIN_HUMIDITY);
    char temp[LCD_TEXT_LEN + 1];
    snprintf_P(temp,
               sizeof(temp),
               PSTR("%d %%"),
               (int)(value + 0.5));

    lcdShowValue(temp);
}

/**
 * @brief
 *
 * @param soilResult
 */
void showSoilResult(SoilResult *soilResult)
{
    lcdShowSoilResult(soilResult->getSoilCondition(), soilResult->getPercent());
}

/**
 * @brief
 *
 * @param dt
 */
void showDateTime(const RtcDateTime &dt)
{
    char datestring[LCD_TEXT_LEN + 1];

    snprintf_P(datestring,
               sizeof(datestring),
               PSTR("%02u/%02u/%04u"),
               dt.Month(),
               dt.Day(),
               dt.Year());
    lcdShowOrigin(datestring);

    snprintf_P(datestring,
               sizeof(datestring),
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    lcdShowValue(datestring);
}

/**
 * @brief show state called every 3 seconds
 *
 */
void showLcd(unsigned int state)
{
    Serial.print("Start show state: ");
    Serial.println(state);
    int idx = 0;
    bool active = false;
    switch (state)
    {
    case STATE_SHOW_TEMPERATURE:        
        showLcdTemperature(temperatureResult.getTemperature());
        break;
    case STATE_SHOW_HUMIDITY:
        showLcdHumidity(temperatureResult.getHumidity());
        break;
    case STATE_SHOW_SOIL_SENSOR_1:
        idx = 1;
        lcdShowOriginIdx(ORIGIN_PLANT);
        showSoilResult(&soilResult[0]);
        active = soil[0].isEnabled();
        break;
    case STATE_SHOW_SOIL_SENSOR_2:
        idx = 2;
        lcdShowOriginIdx(ORIGIN_PLANT);
        showSoilResult(&soilResult[1]);
        active = soil[1].isEnabled();
        break;
    case STATE_SHOW_SOIL_SENSOR_3:
        idx = 3;
        lcdShowOriginIdx(ORIGIN_PLANT);
        showSoilResult(&soilResult[2]);
        active = soil[2].isEnabled();
        break;
    case STATE_SHOW_DATE:
        showDateTime(Rtc.GetDateTime());
        break;
    default:
        break;
    }
    lcdShowID(idx, active);
}
