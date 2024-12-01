#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "text.h"

byte ERROR_PLANT_1[] = {
  B11111,
  B11101,
  B11001,
  B10101,
  B11101,
  B11101,
  B11111,
  B00000

 };

byte ERROR_PLANT_2[] = {
  B11111,
  B10001,
  B11101,
  B10011,
  B10111,
  B10001,
  B11111,
  B00000
 };

byte ERROR_PLANT_3[] = {
  B11111,
  B10001,
  B11101,
  B11001,
  B11101,
  B10001,
  B11111,
  B00000
 };

byte SMILY_FACE[] = {
  B00000,
  B11011,
  B11011,
  B00000,
  B00100,
  B10001,
  B10001,
  B11111
 };

byte FLOWER[] = {
  B10101,
  B11111,
  B11111,
  B01110,
  B00100,
  B00100,
  B00100,
  B00000
 };

 byte DEGREE[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
 };

 byte EDIT_MODE[] = {
  B11111,
  B11011,
  B10101,
  B10101,
  B10101,
  B11111,
  B01110,
  B00100
 };


extern LiquidCrystal_I2C lcd;

int alarmValue = -1;
int modeValue = -1;

#define IDX_SMILY 0
#define IDX_ERROR_1 1
#define IDX_ERROR_2 2
#define IDX_ERROR_3 3
#define IDX_FLOWER 4
#define IDX_DEGREE 5
#define IDX_EDIT 6

void setupLCD()
{
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.createChar(IDX_SMILY,SMILY_FACE);
  lcd.createChar(IDX_ERROR_1,ERROR_PLANT_1);
  lcd.createChar(IDX_ERROR_2,ERROR_PLANT_2);
  lcd.createChar(IDX_ERROR_3,ERROR_PLANT_3);
  lcd.createChar(IDX_FLOWER,FLOWER);
  lcd.createChar(IDX_DEGREE,DEGREE);
  lcd.createChar(IDX_EDIT,EDIT_MODE);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(APP_NAME);
  lcd.setCursor(2, 1);
  lcd.print(APP_VERSION);
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

void lcdShowID(int value, bool active)
{
  lcd.setCursor(14, 0);
  if (value > 0 && value <= 3)
  {
    if( active )
    {
      lcd.write(IDX_FLOWER); // flower
      lcd.write(value);
    }
    else{
      lcd.print(CHAR_SPACE);
      lcd.print(value);  
    }
  }
  else
  {
    lcd.print(CHAR_SPACE);
    lcd.print(CHAR_SPACE);
  }
}

void lcdShowAlarm(int value)
{
  if (alarmValue != value)
  {
    alarmValue = value;
    lcd.setCursor(0, 1);
    lcd.write(value); // IDX_SMILY == 0, IDX_ERROR_X = 1,2,3
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
      lcd.write(IDX_EDIT); // edit mode
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
               PSTR("%s %d%%"),
               text[idx + TEXT_DRY],
               (int)(value+0.5));
  lcdShowValue(temp);
}