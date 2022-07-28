#include <Arduino.h>
#include <Wire.h>

#include "dioUtil.h"
#include "ioConstants.h"

void setupDioUtil()
{
  pinMode(MOTION, INPUT);      // motion sensor
  pinMode(BUTTON_1, INPUT);    // button one
  pinMode(BUTTON_2, INPUT);    // button two
  pinMode(BUZZER, OUTPUT);     // buzzer
  pinMode(WATER_LEVEL, INPUT); // water level sensot
  pinMode(MOTOR_1, OUTPUT);    // water pump one
  pinMode(MOTOR_2, OUTPUT);    // water pump two
  pinMode(MOTOR_3, OUTPUT);    // water pump three

  digitalWrite(MOTOR_1, 1); // motor 1 off
  digitalWrite(MOTOR_2, 1); // motor 2 off
  digitalWrite(MOTOR_3, 1); // motor 3 off
}

bool isButton(int digIn)
{
  int b = digitalRead(digIn);
  Serial.print(F("Button "));
  Serial.print(digIn);
  Serial.print(F(" state: "));
  Serial.println(b);
  return b == BUTTON_PRESSED;
}

bool isPump(int digIn)
{
  int b = digitalRead(digIn);
  Serial.print(F("Pump "));
  Serial.print(digIn);
  Serial.print(F(" state: "));
  Serial.println(b);
  return b == PUMP_RUNNING;
}

void setPump(int digIn, uint8_t value)
{
  digitalWrite(digIn, value);
  Serial.print(F("Pump "));
  Serial.print(digIn);
  Serial.print(F(" set to: "));
  Serial.println(value);
}

void setPumpRunning(int idx, bool value)
{
  switch (idx)
  {
  case 1:
    setPump(MOTOR_1, value ? PUMP_ON : PUMP_OFF);
    break;
  case 2:
    setPump(MOTOR_2, value ? PUMP_ON : PUMP_OFF);
    break;
  case 3:
    setPump(MOTOR_3, value ? PUMP_ON : PUMP_OFF);
    break;
  default:
    break;
  }
}

bool isPumpRunning(int idx)
{
  switch (idx)
  {
  case 1:
    return isPump(MOTOR_1);
  case 2:
    return isPump(MOTOR_2);
  case 3:
    return isPump(MOTOR_3);
  default:
    return false;
  }
}

bool isButton1()
{
  return isButton(BUTTON_1);
}

bool isButton2()
{
  return isButton(BUTTON_2);
}

bool isMotion()
{
  // Motion detection
  int m = digitalRead(MOTION);
  Serial.print(F("Motion state: "));
  Serial.println(m);
  return m == HIGH;
}

void setBuzzer(bool on)
{
  if (on)
  {
    tone(BUZZER, 532); // Power amplifier module beep
  }
  else
  {
    noTone(BUZZER); // Power amplifier module no beep
  }
}

bool isWaterLevelOk()
{
  // Motion detection
  int m = digitalRead(WATER_LEVEL);
  Serial.print(F("Water level state: "));
  Serial.println(m);
  return m == LOW;
}
