#define DEBUGGERY

#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SI7021.h>
#include "FS.h"
#include <SPIFFSIniFile.h>
#include <PubSubClient.h>

// Configuration for this build goes here...
#include "CONFIG.h"
#include "helpers.h"

char SSID[3][80] = {
  "None\0",
  "None\0",
  "None\0",
};
char PASS[3][80] = {
  "None\0",
  "None\0",
  "None\0",
};

bool ini_errord = true;

char buffer[80]; // Buffer to read the settings into.

void printErrorMessage(uint8_t e, bool eol = true);

SI7021 weather_sensor;

void setup() {
  Serial.begin(115200);
  char ssid_i[6] = {'s', 's', 'i', 'd', 'X', '\0'};
  char pass_i[11] = {'s', 's', 'i', 'd', '1', ' ', 'p', 'a', 's', 's', '\0'};
// Read configuration from the /config file  
  MSG_SERIAL("\n\n\n", "\n\n");
  if(!SPIFFS.begin()) {
    Serial.println("SPIFFS.begin() failed");
  } else {
    MSG_SERIAL("Config file found!", "\n");
    ini_errord = false;
  };
  SPIFFSIniFile ini("/config");
  // Open config file
  if (!ini.open()){
    ini_errord = true;
    MSG_SERIAL("Config file not found: ", ini.getFilename());
  } else {
    MSG_SERIAL("Found config file: ", ini.getFilename());
  };
  // Check whether we can fit all the strings in the file into our buffer
  if (!ini.validate(buffer, 80)) {
    ini_errord = true;
    MSG_SERIAL("String longer than 80 bytes in: ", ini.getFilename());
  };
  // Load SSIDs (all 3 of them!)
  for (char i = 0; i < 3; i++)
    {
      MSG_SERIAL("Symbolo: ", char('1'+i));
      ssid_i[4] = char('1'+i);
      pass_i[4] = char('1'+i);
      MSG_SERIAL("SSID key: ", ssid_i);
      MSG_SERIAL("PASS key: ", pass_i);
      if(ini.getValue("wifi", ssid_i, SSID[i], 80))
        {
          MSG_SERIAL("Read string: ", SSID[i]);
        };
      if(ini.getValue("wifi", pass_i, PASS[i], 80))
        {
          MSG_SERIAL("Read string: ", PASS[i]);
        };
      MSG_SERIAL("SSID: ", SSID[i]);
      MSG_SERIAL("PASS: ", PASS[i]);
    };
  if(WiFi.status() != WL_CONNECTED)
    {
      for(char i = 0; (i < 3) && ( WiFi.status() != WL_CONNECTED ); i++)
      {
        char count = 0;
        MSG_SERIAL("Iteration: ", i);
        MSG_SERIAL("SSID: ", SSID[i]);
        MSG_SERIAL("PASS: ", PASS[i]);
        WiFi.begin(SSID[i], PASS[i]);
        while((WiFi.status() != WL_CONNECTED)&&(count < 60))
        {
          delay(500);
          Serial.print('.');
          count++;
        };
        Serial.println("!");
      };
      if(WiFi.status() != WL_CONNECTED) {
        Serial.println("Connection FAILED!");
      } else {
        Serial.println("Connected!");
      };
      WiFi.printDiag(Serial);
    };
  // WiFi.begin("network-name", "pass-to-network");
  //read_ssid1;
  //read_passwd1;
  //connect;
  // put your setup code here, to run once:
  weather_sensor.begin(D2,D1);
  int deviceid = weather_sensor.getDeviceId();
  MSG_SERIAL("Si 7021 device ID: ", deviceid);
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