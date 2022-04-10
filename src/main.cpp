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
unsigned int lastShowState = 0;
int editMode = 0;
int lastEditMode = 0;
int alarmMode = 0;
int lastAlarm = 0;

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
  // read soil moisture 1
  soil1.read(&soilResult1);
  // read soil moisture 2
  soil2.read(&soilResult2);
  // read soil moisture 3
  soil3.read(&soilResult3);
  // check rtc
  checkRTC();
}

void checkAlarm()
{
  alarmMode = 0;
  if (!isWaterLevelOk())
  {
    alarmMode = 4;
  }
  else if (soilResult1.isValid() && soilResult1.getSoilCondition() == SoilResult::SoilCondition::dry)
  {
    alarmMode = 1;
  }
  else if (soilResult2.isValid() && soilResult2.getSoilCondition() == SoilResult::SoilCondition::dry)
  {
    alarmMode = 2;
  }
  else if (soilResult3.isValid() && soilResult3.getSoilCondition() == SoilResult::SoilCondition::dry)
  {
    alarmMode = 3;
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

  soil1.setEnabled(true);
  soil2.setEnabled(true);
  soil3.setEnabled(true);

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
    if (editMode > 3)
    {
      editMode = 1;
    }
    bool runMode = isPumpRunning(editMode);
    if (b2)
    {
      runMode = !runMode;
    }
    runMode &= isWaterLevelOk();

    showLcdEdit(editMode, runMode);
    setPumpRunning(editMode, runMode);
  }
  else if (alarmMode != 0)
  {
    // handle alarm mode
    if (lastAlarm != alarmMode)
    {
      showLcdAlarm(alarmMode);
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

  lastAlarm = alarmMode;
  lastShowState = showState;
  lastEditMode = editMode;

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

  // debug out to serial
  logger.printAllWithSerial();
}
