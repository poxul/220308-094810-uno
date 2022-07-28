#include <Arduino.h>
#include <Wire.h>

#include "dioUtil.h"
#include "ioConstants.h"

bool b1_last = false;
bool b2_last = false;

void setupDioUtil()
{
  pinMode(MOTION, INPUT);      // motion sensor
  pinMode(BUTTON_1, INPUT);    // button one
  pinMode(BUTTON_2, INPUT);    // button two
}

bool isButton(int digIn)
{
  int b = digitalRead(digIn);
  return b == BUTTON_PRESSED;
}

bool isButton1()
{
  bool b1 = isButton(BUTTON_1);
  if( b1 != b1_last )
  {
    b1_last = b1;
  } 
  else
  {
    b1 = false;
  }
  return b1;
}

bool isButton2()
{
  bool b2 = isButton(BUTTON_2);
  if( b2 != b2_last )
  {
    b2_last = b2;
  } 
  else
  {
    b2 = false;
  }
  return b2;
}

bool isMotion()
{
  // Motion detection
  int m = digitalRead(MOTION);
  return m == HIGH;
}
