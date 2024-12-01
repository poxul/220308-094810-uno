#include <Arduino.h>
#include <Wire.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "soilMoisture.h"

#define SOIL_MOISTURE_MIN 200                                               // 285 = 0% min value -> sensor is in water
#define SOIL_MOISTURE_MAX 400                                               // 591 = 100% max value -> sensor is in air

#define SOIL_PERCENT ((float) 200.0 / (SOIL_MOISTURE_MAX - SOIL_MOISTURE_MIN)) // 1 percent

#define NUM_STEPS 3

SoilResult::SoilResult() {}

void SoilResult::setValid(bool ok)
{
    valid = ok;
}

bool SoilResult::isValid()
{
    return valid;
}

int SoilResult::getValue()
{
    return value;
}

void SoilResult::setValue(int v)
{
    value = v;
}

void SoilResult::setCondition(SoilResult::SoilCondition c)
{
    soil = c;
}

void SoilResult::setPercent(float p)
{
    if (p < -50.0)
    {
        p = -50.0;
    }
    if (p > 150.0)
    {
        p = 150.0;
    }
    percent = p;
}

float SoilResult::getPercent()
{
    return percent;
}

SoilResult::SoilCondition SoilResult::getSoilCondition()
{
    return soil;
}

/**
 * @brief Construct a new Soil Moisture:: Soil Moisture object
 *
 * @param ax address of the analog input
 */
SoilMoisture::SoilMoisture(int ax)
{
    analogInput = ax;
    step = (SOIL_MOISTURE_MAX - SOIL_MOISTURE_MIN) / NUM_STEPS;
}

/**
 * @brief read analog result of soil moisture sensor
 *
 * @return SoilResult*
 */
void SoilMoisture::read(SoilResult *result)
{
    soilMoistureValue = analogRead(analogInput); // set the humidity value read by soil humidity sensor to soilMoistureValue
    result->setValue(soilMoistureValue);
    int idx = ((soilMoistureValue - SOIL_MOISTURE_MIN) + (step / 2)) / step;
    Serial.print("Soil ");
    Serial.print(analogInput);
    Serial.print("= ");
    result->setValid(enabled);
    switch (idx)
    {
    case 0:
        result->setCondition(SoilResult::SoilCondition::water);
        break;
    case 1:
        result->setCondition(SoilResult::SoilCondition::wet);
        break;
    case 2:
        result->setCondition(SoilResult::SoilCondition::humid);
        break;
    default:
        result->setCondition(SoilResult::SoilCondition::dry);
        break;
    }
    Serial.print(" value: ");
    Serial.print(soilMoistureValue);
    Serial.println(F(" x"));
    result->setPercent(((float)(SOIL_MOISTURE_MAX - soilMoistureValue) * SOIL_PERCENT));
    Serial.print(F(" percent: "));
    Serial.print(result->getPercent(),0);
    Serial.println(F(" %"));
}

void SoilMoisture::setEnabled(bool on)
{
    enabled = on;
}

bool SoilMoisture::isEnabled()
{
    return enabled;
}