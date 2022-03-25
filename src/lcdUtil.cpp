#include <Arduino.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "debugLogger.h"

extern DebugLogger logger;
extern LiquidCrystal_I2C lcd;


void setupLCD(){
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Plant-Support");
  lcd.setCursor(2,1);
  lcd.print("Version 1.1");
  lcd.backlight();
}