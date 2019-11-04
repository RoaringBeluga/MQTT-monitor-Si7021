#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SI7021.h>
#include "FS.h"
#include <SPIFFSIniFile.h>

// Configuration for this build goes here...
#include "CONFIG.h"
#include "helpers.h"

String SSID;
String WiFiPass;

bool ini_errord = true;

char buffer[80];

void printErrorMessage(uint8_t e, bool eol = true);

SI7021 weather_sensor;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  weather_sensor.begin(D2,D1);
  int deviceid = weather_sensor.getDeviceId();
  MSG_SERIAL("Si 7021 device ID: ", deviceid);
  while(!SPIFFS.begin())
      Serial.println("SPIFFS.begin() failed");
  SPIFFSIniFile ini("/config");
  if (!ini.open()){
    Serial.println(F("Config not found!"));
  }else{
    Serial.println(F("File NOT not found!"));
  };
  if (!ini.validate(buffer, 80)) {
    Serial.println(F("Strings must be shorter than 80 characters"));
  };
  bool pogo = ini.getValue(NULL, "hello", buffer, 80);
  if(!pogo){
    ini_errord = true;
  } else {
    MSG_SERIAL("hello = ", buffer);
  };
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Testing...");
  si7021_env data = weather_sensor.getHumidityAndTemperature();
  MSG_SERIAL("Temperature: ", (data.celsiusHundredths / 100));
  MSG_SERIAL("Humidity: ", (data.humidityBasisPoints / 100));
  delay(15000);
}

void printErrorMessage(uint8_t e, bool eol)
{
  switch (e) {
  case SPIFFSIniFile::errorNoError:
    Serial.print("no error");
    break;
  case SPIFFSIniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case SPIFFSIniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case SPIFFSIniFile::errorBufferTooSmall:
    Serial.print("buffer too small");
    break;
  case SPIFFSIniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case SPIFFSIniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case SPIFFSIniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case SPIFFSIniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case SPIFFSIniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}