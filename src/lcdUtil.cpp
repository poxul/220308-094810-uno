#include <Arduino.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "debugLogger.h"

extern DebugLogger logger;
extern LiquidCrystal_I2C lcd;

void setupLCD(){
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Plant-Support");
  lcd.clear();
  lcd.noBacklight();
}