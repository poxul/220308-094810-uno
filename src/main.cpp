#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RtcDS3231.h>

#include "debugLogger.h"
#include "i2cHelpers.h"
#include "soilMoisture.h"
#include "dioUtil.h"
#include "dhtMeasure.h"
#include "lcdUtil.h"
#include "rtClock.h"
#include "showUtil.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

#define DHTPIN 8 // define interface
#define DHTTYPE DHT22

#define SOIL_MOISTURE_1 A0
#define SOIL_MOISTURE_2 A1
#define SOIL_MOISTURE_3 A2

DebugLogger logger;

uint32_t delayMS;

// temperature
DHT_Unified dht(DHTPIN, DHTTYPE);
DhtMeasure dhtM = DhtMeasure(&dht);
DhtResult temperatureResult;

// lcd
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

// soil moisture
SoilMoisture soil1(SOIL_MOISTURE_1); // soil moisture sensor 1
SoilResult soilResult1;
SoilMoisture soil2(SOIL_MOISTURE_2); // soil moisture sensor 1
SoilResult soilResult2;
SoilMoisture soil3(SOIL_MOISTURE_3); // soil moisture sensor 1
SoilResult soilResult3;

// rt clock
RtcDS3231<TwoWire> Rtc(Wire);

unsigned long lastMeasurement;
unsigned long lastView;

unsigned int showState = 0;

/**
 * @brief arduino setup call
 *
 */
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  } // Wait for Serial to start
  delay(750);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  scanI2C();
  Serial.println("Test i2c done");

  setupLCD();
  Serial.println("Setup lcd done");
  setupDioUtil();
  Serial.println("Setup digital io done");
  delayMS = dhtM.setup();
  Serial.println("Setup dht22 done");

  setupRTC();
  Serial.println("Setup rtc done");

  unsigned long now = millis();
  lastView = now;
  lastMeasurement = now;

  // setup done
  Serial.println("Setup done");
}

/**
 * @brief measurement called every 10 seconds
 *
 */
void measure()
{
  Serial.println("Measurement loop");
  // read temperature sensor
  dhtM.read(&temperatureResult);
  // read soil moisture 1
  soil1.read(&soilResult1);
  // read soil moisture 2
  soil2.read(&soilResult2);
  // read soil moisture 3
  soil3.read(&soilResult3);
  // check rtc
  checkRTC();
}

/**
 * @brief calculate next view state
 *
 */
void checkState()
{ 
  if (showState >= STATE_MAX)
  {
    showState = STATE_TEMPERATURE;
  }
  Serial.print("Current state ");
  Serial.println(showState);
}

/**
 * @brief show state called every 3 seconds
 *
 */
void show()
{
  Serial.println("Show loop");
  // Beep if the button 1 is pressed
  setBuzzer(isButton1());

  // check motion
  if (isMotion())
  {
    lcd.backlight();
  }
  else
  {
    lcd.noBacklight();
  }
  // show on lcd
  showLcd(showState);

  // change view state if button 2 is not pressed
  if (!isButton2()) 
  {
    showState++;
  }

}

/**
 * @brief arduino main loop
 *
 */
void loop()
{
  unsigned long now = millis();

  // Delay between loops.
  delay(delayMS);
  if ((now - lastMeasurement) > 10000)
  {
    // read new values
    measure();
    lastMeasurement = now;
  }

  // check for priorized values
  checkState();
  if ((now - lastView) > 2000)
  {
    // show result
    show();
    lastView = now;
  }

  // debug out to serial
  logger.printAllWithSerial();
}
