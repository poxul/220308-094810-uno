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
#include "buttonInfo.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

#define DHTPIN 8 // define interface
#define DHTTYPE DHT22

#define SOIL_MOISTURE_1 A0
#define SOIL_MOISTURE_2 A1
#define SOIL_MOISTURE_3 A2

#define MOTION_LOOPS 20

#define WAIT_TIME_EDIT 100
#define WAIT_TIME_SHOW 2500

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

ButtonInfo buttons[2] =
    {
        ButtonInfo(),
        ButtonInfo()};

unsigned long lastMeasurement;
unsigned long lastView;

unsigned int showState = 0;
unsigned int lastShowState = 0;

int editMode = 0;
int alarmMode = 0;

bool toggleEditMode = false;

int motion = MOTION_LOOPS;

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

  for (size_t i = 0; i < 3; i++)
  {
    if (soilResult[i].isValid() && soilResult[i].getSoilCondition() == SoilResult::SoilCondition::dry)
    {
      alarmMode = i + 1;
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

  Serial.print(F("Compiled: "));
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  scanI2C();
  Serial.println(F("Test i2c done"));

  setupLCD();
  Serial.println(F("Setup lcd done"));
  setupDioUtil();
  Serial.println(F("Setup digital io done"));
  delayMS = dhtM.setup();
  Serial.println(F("Setup dht22 done"));

  setupRTC();
  Serial.println(F("Setup rtc done"));

  for (size_t i = 0; i < 3; i++)
  {
    soil[i].setEnabled(true);
  }

  unsigned long now = millis();
  lastView = now;
  lastMeasurement = now;
  measure();
  // setup done
  Serial.println(F("Setup done"));
}

bool checkMotion()
{
  if (isMotion())
  {
    Serial.print(F("Motion detected "));
    Serial.println(motion);
    if (motion <= 0)
    {
      showState = STATE_SHOW;
    }
    motion = MOTION_LOOPS;
  }
  else
  {
    if (motion > 0)
    {
      motion--;
    }
    Serial.print(F("Motion not detected "));
    Serial.println(motion);
  }
  return motion > 0;
}

/**
 * @brief show state called every 3 seconds
 *
 */
void show()
{
  bool b1 = isButton1(); // next
  bool b2 = isButton2(); // toggle

  // check edit mode
  if (b1 && b2)
  {
    if (!toggleEditMode)
    {
      if (editMode == 0)
      {
        editMode = 1;
        lastShowState = showState + 1;
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

  // handle backlight
  if (editMode || motion > 0 || b1 || b2)
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
    if (editMode > 3)
    {
      editMode = 1;
    }

    if (editMode <= 3) // switch plants on and off
    {
      // edit mode run pump manual
      bool active = isPlantActive(editMode - 1); // Index 0 - 2
      if (b2)
      {
        active = !active;
      }

      // show to lcd
      lcdShowOriginIdx(ORIGIN_PLANT);
      lcdShowID(editMode, false); // Number 1- 3
      if (active)
      {
        lcdShowStateIdx(TEXT_ACTIVE);
      }
      else
      {
        lcdShowStateIdx(TEXT_DISABLED);
      }
      // start / stop hardware
      setPlantActive(editMode - 1, active);
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

  lcdShowMode(editMode);
  if (showState != STATE_SHOW_SOIL_SENSOR_1 && showState != STATE_SHOW_SOIL_SENSOR_2 && showState != STATE_SHOW_SOIL_SENSOR_3)
  {
    lcdShowAlarm(alarmMode);
  }
  else
  {
    lcdShowAlarm(-1);
  }

  lastShowState = showState;
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
  checkMotion();
  if ((now - lastMeasurement) > 10000)
  {
    // read new values
    measure();
    checkAlarm();
    lastMeasurement = now;
  }

  unsigned long waitTime;
  if (editMode != 0)
  {
    waitTime = WAIT_TIME_EDIT;
  }
  else
  {
    waitTime = WAIT_TIME_SHOW;
  }

  // check for priorized values
  if ((now - lastView) > waitTime)
  {
    // show result
    show();
    lastView = now;
  }
}
