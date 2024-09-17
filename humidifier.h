#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#define NOWT millis()
#define READ(PIN_CHECK) digitalRead(PIN_CHECK)
#define WRITE(PIN_CHECK, MODS) digitalWrite(PIN_CHECK, MODS)



class Humidiffer{
  private:
    bool humidifferFlag = false;
    uint8_t pinOut;

    uint32_t currTime = 0;
    uint32_t workTime;
    //Task delay
    uint16_t timeDelay;

  public:
    Humidiffer(uint8_t pin, uint32_t workTime){
      sleepUpdate();
      changeWorkTime(workTime);
      this->pinOut = pin;
    }

    void setupHumidiffer(){
      pinMode(pinOut, OUTPUT);
    }
    void setDelay(uint8_t time){
        this->timeDelay = time;
    }

    void sleepUpdate(){
      this->currTime = millis();
    }
    void changeWorkTime(uint32_t workTime){
      this->workTime = workTime;
    }

    bool getFlagStatus(){
      return this->humidifferFlag;
    }
    void changeFlag(){
      this->humidifferFlag = !this->humidifferFlag;
    }

    void OnMode(){
      this->humidifferFlag = true;
      sleepUpdate();
    }
    void OffMode(){
      this->humidifferFlag = false;
    }

    void humidifferLoop(){
      if(humidifferFlag){
        if(NOWT - currTime > workTime){
          WRITE(pinOut, HIGH);
        }else{
          OffMode();
        }
      }else if(READ(pinOut) != LOW){
          WRITE(pinOut, LOW);
      }
    }
};
