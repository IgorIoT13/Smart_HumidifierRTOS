#ifndef dht_hum_h
#define dht_hum_h

#include "DHT.h"

#define DHT_DEBUG 

uint32_t DHT22TimeStamp;
uint32_t DHT22Delay = 5000;



DHT dht(DHT22PIN, DHTTYPE);

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
      this->dht->begin();
      checkTemperature();
    } 

    void checkTemperature() {
      temperature = dht->readTemperature();
      humidity = dht->readHumidity();
    }

    void Check(){
      if(humidity < humidityAcceptableLevel){
        needSomeDo = true;
      }else{
        needSomeDo = false;
      }
    }

    void setDHT(DHT* dht){
      if(dht){
        this->dht = dht;
      }
    }
};

#endif // dht_hum_h