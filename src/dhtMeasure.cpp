#include <Arduino.h>
#include <Wire.h> 

#include "dhtMeasure.h"

DhtResult::DhtResult(){}

void DhtResult::setHumidity(float h){
  humidity = h;
}

void DhtResult::setTemperature(float t){
  temperature = t;
}

float DhtResult::getHumidity(){
  return humidity;
}

float DhtResult::getTemperature(){
  return temperature;
}

DhtMeasure::DhtMeasure(DHT_Unified* sensor){
  dhtSensor = sensor;
}

unsigned int DhtMeasure::setup(){
  dhtSensor->begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dhtSensor->temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature sensor"));
  Serial.print  (F("Sensor type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dhtSensor->humidity().getSensor(&sensor);
  Serial.println(F("Humidity sensor"));
  Serial.print  (F("Sensor type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  uint32_t delayMS = sensor.min_delay / 1000 + 100;
  Serial.print  (F("Delay:       ")); Serial.print(delayMS); Serial.println(F("ms"));
  return delayMS;
}

void DhtMeasure::read(DhtResult* result){
  // Get temperature event and print its value.
  sensors_event_t event;
  dhtSensor->temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    result->setTemperature(-300.0);
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    result->setTemperature(event.temperature);
  }
  // Get humidity event and print its value.
  dhtSensor->humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    result->setHumidity(0.0);
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    result->setHumidity(event.relative_humidity);
  }
  
}