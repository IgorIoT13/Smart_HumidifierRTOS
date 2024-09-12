#ifndef dht_hum_h
#define dht_hum_h

#include "DHT.h"

#define DHT_DEBUG 

uint32_t DHT22TimeStamp;
uint32_t DHT22Delay = 5000;


uint32_t humidifierTimeStamp;
uint32_t humidifierWorkingTime = 20000;
uint32_t humidifierSleepingTime = 10000;
uint32_t humidifierSleepTimeStamp;
float humidityAcceptableLevel = 50;

bool humidifierWorking = false;
bool humidifierStarting;
bool allowToSendData = true;
bool firstSuccessRequest = true; //todo remove
bool oneTimeTriggerFlag = true;
bool timeForHumidifierLeft = false;
bool RGBWorking = true;

DHT dht(DHT22PIN, DHTTYPE);

float temperature;
float humidity;

void checkTemperature() {
  temperature = dht.readTemperature();
#ifdef DHT_DEBUG
  Serial.print("Temperature (ºC): ");
  Serial.println(temperature);
#endif
  humidity = dht.readHumidity();
#ifdef DHT_DEBUG
  Serial.print("Humidity (%): ");
  Serial.println(humidity);
#endif
}

void setupDHT()
{
    DHT22TimeStamp = millis();
  dht.begin();
  humidifierTimeStamp = millis();
  checkTemperature();
}

#endif // dht_hum_h