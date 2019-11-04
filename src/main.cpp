#include <Wire.h>
#include <Arduino.h>
#include <SI7021.h>

// Configuration for this build goes here...
#include "CONFIG.h"
#include "helpers.h"


SI7021 weather_sensor;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  weather_sensor.begin(D2,D1);
  int deviceid = weather_sensor.getDeviceId();
  Serial.print("Device ID: "); Serial.print(deviceid);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Testing...");
  si7021_env data = weather_sensor.getHumidityAndTemperature();
  MSG_SERIAL("Temperature: ", (data.celsiusHundredths / 100))
  MSG_SERIAL("Humidity: ", (data.humidityBasisPoints / 100))
  delay(5000);
}