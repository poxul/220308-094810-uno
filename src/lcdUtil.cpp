#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "debugLogger.h"
#include "text.h"

extern DebugLogger logger;
extern LiquidCrystal_I2C lcd;

void setupLCD()
{
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Plant-Support");
  lcd.setCursor(2, 1);
  lcd.print("Version 1.2");
  lcd.backlight();
}

void lcdShowOrigin(int idx)
{
  lcd.setCursor(1, 0);
  lcd.print(origin[idx]);
}

void lcdShowID(int value)
{
  lcd.setCursor(15, 0);
  lcd.print(value);
}

void lcdShowAlarm(int value)
{
  if (value > 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(value);
    lcd.setCursor(0, 1);
    lcd.blink_on();
  }
  else
  {
    lcd.blink_off();
  }
}

void lcdShowMode(int mode)
{
  lcd.setCursor(0, 0);
  if (mode == 0)
  {
    lcd.print('i'); // info mode
  }
  else
  {
    lcd.print('e'); // edit mode
  }
}
