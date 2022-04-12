#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "text.h"

extern LiquidCrystal_I2C lcd;

int alarmValue = -1;
int modeValue = -1;

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


void lcdShowOrigin(const char *txt)
{
  lcd.setCursor(2, 0);
  lcd.print(txt);
  size_t len = strlen(txt);
  for (; len < 12; len++)
  {
    lcd.print('-');
  }
}

void lcdShowOriginIdx(int idx)
{
  lcdShowOrigin(origin[idx]);
}

void lcdShowID(int value)
{
  lcd.setCursor(15, 0);
  if (value > 0)
  {
    lcd.print(value);
  }
  else
  {
    lcd.print('*');
  }
}

void lcdShowAlarm(int value)
{
  if (alarmValue != value)
  {
    alarmValue = value;
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
}

void lcdShowMode(int mode)
{
  if (modeValue != mode)
  {
    modeValue = mode;
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
}

void lcdShowStateIdx(int idx)
{
  lcd.setCursor(2, 1);
  lcd.print(text[idx]);
}

void lcdShowValue(const char *txt)
{
  lcd.setCursor(2, 1);
  lcd.print(txt);
  size_t len = strlen(txt);
  for (; len < 12; len++)
  {
    lcd.print(' ');
  }
}

void lcdShowSoilResult(int idx, int value)
{
  char temp[12];
  snprintf_P(temp,
             12,
             PSTR("%s (%d)"),
             text[idx + TEXT_DRY],
             value);
  lcdShowValue(temp);
}