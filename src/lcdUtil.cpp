#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "text.h"

#define CHAR_SPACE ' '

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
  lcd.print("Version 1.3");
  lcd.backlight();
}

void lcdShowOrigin(const char *txt)
{
  lcd.setCursor(2, 0);
  lcd.print(txt);
  size_t len = strlen(txt);
  for (; len <= LCD_TEXT_LEN; len++)
  {
    lcd.print(CHAR_SPACE);
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
    lcd.print(CHAR_SPACE);
  }
}

void lcdShowAlarm(int value)
{
  if (alarmValue != value)
  {
    alarmValue = value;
    if (value > 3)
    {
      lcd.setCursor(0, 1);
      lcd.print('W');
      lcd.setCursor(0, 1);
      lcd.blink_on();
    } 
    else if (value > 0)
    {
      lcd.setCursor(0, 1);
      lcd.print(value);
      lcd.setCursor(0, 1);
      lcd.blink_on();
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print(CHAR_SPACE);
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
      lcd.print(CHAR_SPACE); // info mode
    }
    else
    {
      lcd.print('*'); // edit mode
    }
  }
}

void lcdShowStateIdx(int idx)
{
  lcd.setCursor(2, 1);
  lcd.print(text[idx]);
  size_t len = strlen(text[idx]);
  for (; len <= LCD_TEXT_LEN; len++)
  {
    lcd.print(CHAR_SPACE);
  }
}

void lcdShowValue(const char *txt)
{
  lcd.setCursor(2, 1);
  lcd.print(txt);
  size_t len = strlen(txt);
  for (; len <= LCD_TEXT_LEN; len++)
  {
    lcd.print(CHAR_SPACE);
  }
}

void lcdShowSoilResult(int idx, int value)
{
  char temp[LCD_TEXT_LEN + 1];
  snprintf_P(temp,
             sizeof(temp),
             PSTR("%s %d"),
             text[idx + TEXT_DRY],
             value);
  lcdShowValue(temp);
}