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



void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

void printDate(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u.%02u.%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year());
    Serial.print(datestring);
}


void setupRTC () 
{
    //--------RTC SETUP ------------
    // if you are using ESP-01 then uncomment the line below to reset the pins to
    // the available pins for SDA, SCL
    // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
    
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
}

void checkRTC() 
{
    if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
    }

    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();

	RtcTemperature temp = Rtc.GetTemperature();
	temp.Print(Serial);
	// you may also get the temperature as a float and print it
  // Serial.print(temp.AsFloatDegC());
  Serial.println("C");

}

/**
 * @brief show temperature on lcd
 * 
 */
void showLcdTemperature(){
    lcd.setCursor(0,0);
    lcd.print("Temperatur:");
    lcd.setCursor(0,6);
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



