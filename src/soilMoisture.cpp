#include <Arduino.h>
#include <Wire.h> 

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "debugLogger.h"
#include "soilMoisture.h"

#define MAX_SOIL_STR_LEN 15

#define SOIL_MOISTURE_WATER "water"
#define SOIL_MOISTURE_WET "pretty wet"
#define SOIL_MOISTURE_HUMID "humid"
#define SOIL_MOISTURE_DRY "dry"
#define SOIL_MOISTURE_EXTREMLY_DRY "extremly dry"

#define SOIL_MOISTURE_VALUE_1 285 // min value -> sensor is in water
#define SOIL_MOISTURE_VALUE_2 387 // 
#define SOIL_MOISTURE_VALUE_3 489 // 
#define SOIL_MOISTURE_VALUE_4 591 // max value -> sensor is in air

extern DebugLogger logger;

SoilResult::SoilResult(){}

int SoilResult::getValue(){
    return value;
}

void SoilResult::setValue(int v){
    value = v;
}

void SoilResult::setCondition(SoilResult::SoilCondition c){
    soil = c;
}

 SoilResult::SoilCondition SoilResult::getSoilCondition(){
     return soil;
 }

/**
 * @brief Construct a new Soil Moisture:: Soil Moisture object
 * 
 * @param ax address of the analog input
 */
SoilMoisture::SoilMoisture(int ax) {
    analogInput = ax;
}

/**
 * @brief read analog result of soil moisture sensor 
 * 
 * @return SoilResult* 
 */
void SoilMoisture::read(SoilResult* result) {
    soilMoistureValue = analogRead(analogInput); // set the humidity value read by soil humidity sensor to soilMoistureValue
    result->setValue(soilMoistureValue);
    if(soilMoistureValue < SOIL_MOISTURE_VALUE_1)  {
        Serial.println(SOIL_MOISTURE_WATER);
        result->setCondition(SoilResult::SoilCondition::water);
    } else if(soilMoistureValue >= SOIL_MOISTURE_VALUE_1 && soilMoistureValue < SOIL_MOISTURE_VALUE_2)  {
        Serial.println(SOIL_MOISTURE_WET);
        result->setCondition(SoilResult::SoilCondition::wet);
    } else if(soilMoistureValue >= SOIL_MOISTURE_VALUE_2 && soilMoistureValue < SOIL_MOISTURE_VALUE_3) {
        Serial.println(SOIL_MOISTURE_HUMID);
        result->setCondition(SoilResult::SoilCondition::humid);
    } else if(soilMoistureValue < SOIL_MOISTURE_VALUE_4 && soilMoistureValue >= SOIL_MOISTURE_VALUE_3)  {
        Serial.println(SOIL_MOISTURE_DRY);
        result->setCondition(SoilResult::SoilCondition::dry);
    } else {
        Serial.println(SOIL_MOISTURE_EXTREMLY_DRY);
        result->setCondition(SoilResult::SoilCondition::extra_dry);
    }
    Serial.print(soilMoistureValue);
    Serial.println(F(" x")); 
}