#ifndef dht_hum_h
#define dht_hum_h

#include "DHT.h"

#define DHT_DEBUG 

uint32_t DHT22TimeStamp;
uint32_t DHT22Delay = 5000;



class SensorHT{
  private:
    DHT* dht;
    float temperature;
    float humidity;
    float humidityAcceptableLevel = 50;
    bool LetsWork = false;

  public:
    SensorHT(){
      dht = nullptr;
    }

    SensorHT(DHT* dht){
      if(dht){
        this->dht = dht;
      }
    }

    void setupSensor(){
      DHT22TimeStamp = millis();
      dht.begin();
      humidifierTimeStamp = millis();
      checkTemperature();
    } 
    void checkTemperature() {
      temperature = dht.readTemperature();
      humidity = dht.readHumidity();
    }

    void 

    void setDHT(DHT* dht){
      if(dht){
        this->dht = dht;
      }
    }
}






DHT dht(DHT22PIN, DHTTYPE);





#endif // dht_hum_h