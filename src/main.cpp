#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RtcDS3231.h>

#include "i2cHelpers.h"
#include "soilMoisture.h"
#include "dioUtil.h"
#include "dhtMeasure.h"
#include "lcdUtil.h"
#include "rtClock.h"
#include "showUtil.h"
#include "textUtil.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

#define DHTPIN 8 // define interface
#define DHTTYPE DHT22

#define SOIL_MOISTURE_1 A0
#define SOIL_MOISTURE_2 A1
#define SOIL_MOISTURE_3 A2

uint32_t delayMS;

// temperature
DHT_Unified dht(DHTPIN, DHTTYPE);
DhtMeasure dhtM = DhtMeasure(&dht);
DhtResult temperatureResult;

// lcd
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

// soil moisture
SoilResult soilResult[3] =
    {
        SoilResult(),
        SoilResult(),
        SoilResult()};

SoilMoisture soil[3] =
    {
        SoilMoisture(SOIL_MOISTURE_1),
        SoilMoisture(SOIL_MOISTURE_2),
        SoilMoisture(SOIL_MOISTURE_3)};

// rt clock
RtcDS3231<TwoWire> Rtc(Wire);

unsigned long lastMeasurement;
unsigned long lastView;

unsigned int showState = 0;
unsigned int lastShowState = 0;

int editMode = 0;
int alarmMode = 0;

bool toggleEditMode = false;

/**
 * @brief measurement called every 10 seconds
 *
 */
void measure()
{
  Serial.println("Measurement loop");
  // read temperature sensor
  dhtM.read(&temperatureResult);
  // read soil moisture
  for (size_t i = 0; i < 3; i++)
  {
    soil[i].read(&soilResult[i]);
  }
  // check rtc
  checkRTC();
}

bool isPlantActive(int idx)
{
  return soil[idx].isEnabled();
}

void setPlantActive(int idx, bool active)
{
  soil[idx].setEnabled(active);
}

void checkAlarm()
{
  alarmMode = 0;
  if (!isWaterLevelOk())
  {
    alarmMode = 4;
  }
  else
  {

    for (size_t i = 0; i < 3; i++)
    {
      if (soilResult[i].isValid() && soilResult[i].getSoilCondition() == SoilResult::SoilCondition::dry)
      {
        alarmMode = i + 1;
      }
    }
  }
}

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

  Serial.print("Compiled: ");
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

  for (size_t i = 0; i < 3; i++)
  {
    soil[i].setEnabled(true);
  }

  unsigned long now = millis();
  lastView = now;
  lastMeasurement = now;
  measure();
  // setup done
  Serial.println("Setup done");
}

/**
 * @brief show state called every 3 seconds
 *
 */
void show()
{
  Serial.println("Show loop");
  bool b1 = isButton1(); // next
  bool b2 = isButton2(); // toggle
  bool buzz = false;

  // check edit mode
  if (b1 && b2)
  {
    if (!toggleEditMode)
    {
      if (editMode == 0)
      {
        editMode = 1;
        lastShowState = showState + 1;
        buzz = true;
      }
      else
      {
        editMode = 0;
      }
      toggleEditMode = true;
    }
    b1 = false;
    b2 = false;
  }
  else
  {
    toggleEditMode = false;
  }

  // Beep

  // handle backlight
  if (editMode || isMotion())
  {
    lcd.backlight();
  }
  else
  {
    lcd.noBacklight();
  }

  // handle state
  if (editMode != 0)
  {
    // handle edit mode
    if (b1)
    {
      editMode++;
    }
    if (editMode > 6)
    {
      editMode = 1;
    }

    if (editMode <= 3) // pumps
    {
      // edit mode run pump manual
      bool runMode = isPumpRunning(editMode);
      if (b2)
      {
        runMode = !runMode;
      }
      runMode &= isWaterLevelOk();
      // show to lcd
      lcdShowOriginIdx(ORIGIN_PUMP);
      lcdShowID(editMode);
      if (runMode)
      {
        lcdShowStateIdx(TEXT_RUNNING);
      }
      else
      {
        lcdShowStateIdx(TEXT_STOPPED);
      }
      // start / stop hardware
      setPumpRunning(editMode, runMode);
    }
    else if (editMode <= 6) // switch plants on and off
    {
      // edit mode run pump manual
      bool active = isPlantActive(editMode - 3);
      if (b2)
      {
        active = !active;
      }

      // show to lcd
      lcdShowOriginIdx(ORIGIN_PLANT);
      lcdShowID(editMode - 3);
      if (active)
      {
        lcdShowStateIdx(TEXT_ACTIVE);
      }
      else
      {
        lcdShowStateIdx(TEXT_DISABLED);
      }
      // start / stop hardware
      setPlantActive(editMode - 3, active);
    }
  }
  else
  {
    // show current values
    showState++;
    if (showState >= STATE_SHOW)
    {
      showState = STATE_SHOW_TEMPERATURE;
    }
    // show on lcd
    if (lastShowState != showState)
    {
      showLcd(showState);
      lastShowState = showState;
    }
  }

  lcdShowAlarm(alarmMode);
  lcdShowMode(editMode);

  lastShowState = showState;

  setBuzzer(buzz);
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
    checkAlarm();
    lastMeasurement = now;
  }

  // check for priorized values
  if ((now - lastView) > 2000)
  {
    // show result
    show();
    lastView = now;
  }
}
