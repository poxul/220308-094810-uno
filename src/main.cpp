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

#define countof(a) (sizeof(a) / sizeof(a[0]))

#define DHTPIN 8     // define interface
#define DHTTYPE DHT22   

#define SOIL_MOISTURE_1 A0

DebugLogger logger;

uint32_t delayMS;

// temperature
DHT_Unified dht(DHTPIN, DHTTYPE);
DhtMeasure dhtM = DhtMeasure(&dht);
DhtResult temperatureResult;

// lcd
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// soil moisture
SoilMoisture soil1(SOIL_MOISTURE_1); // soil moisture sensor 1
SoilResult soilResult;

// rt clock
RtcDS3231<TwoWire> Rtc(Wire);

/**
 * @brief show temperature on lcd
 * 
 */
void showLcdTemperature(){
    lcd.setCursor(0,0);
    lcd.print("Temperatur:");
    lcd.setCursor(6,1);
    lcd.print(temperatureResult.getTemperature());
    lcd.print(" C");
}

/**
 * @brief arduino setup call
 * 
 */
void setup(){
  Serial.begin(9600);
   while(!Serial) {}  // Wait for Serial to start
  delay(750);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  setupLCD();
  Serial.println("setup lcd done");
  setupDioUtil();
  Serial.println("setup digital io done");
  scanI2C();
  Serial.println("test i2c done");
  delayMS=dhtM.setup(); 
  Serial.println("setup dht22 done");

  setupRTC();
  
  // setup done
  Serial.println("setup done");
}


/**
 * @brief arduino main loop
 * 
 */
void loop(){
  // Delay between loops.
  delay(delayMS);
  // read temperature sensor
  dhtM.read(&temperatureResult);
  // read soil moisture 1
  soil1.read(&soilResult);
  
  //Beep if the button 1 is pressed
  setBuzzer(isButton1());

  // later use
  isButton2();

  // check motion
  if( isMotion() ){
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }

  // check rtc
  checkRTC();
    // show temperature on lcd
  showLcdTemperature();

  // debug out to serial
  logger.printAllWithSerial();
}



