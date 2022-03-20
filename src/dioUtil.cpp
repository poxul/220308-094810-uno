#include <Arduino.h>
#include <Wire.h> 

#include "dioUtil.h"

void setupDioUtil(){
  pinMode(MOTION, INPUT);   // initialize digital pin button as an input.
  pinMode(BUTTON_1, INPUT); // initialize digital pin button as an input.
  pinMode(BUTTON_2, INPUT); // initialize digital pin button as an input.
  pinMode(BUZZER, OUTPUT);  //The digital port 11 is set to the output port
}

bool isButton(int digIn){
  int b = digitalRead(digIn);
  Serial.print(F("Button "));
  Serial.print(digIn);    
  Serial.print(F(" state: "));
  Serial.println(b);    
  return b == BUTTON_PRESSED;
}

bool isButton1(){
  return isButton(BUTTON_1);
}

bool isButton2(){
  return isButton(BUTTON_2);
}

bool isMotion(){
  // Motion detection
  int m = digitalRead(MOTION);
  Serial.print(F("Motion state: "));
  Serial.println(m);
  return m == HIGH;  
}

void setBuzzer(bool on){
  if (on) {
    tone(BUZZER, 532); //Power amplifier module beep
  } else {
    noTone(BUZZER);//Power amplifier module no beep
  }
}
