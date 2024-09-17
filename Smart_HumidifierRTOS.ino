#include <GRGB.h>
#include "ThingSpeak.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

bool needSomeDo = false;

#include "pins.h"
#include "dht_hum.h"
#include "humidifier.h"

#define TASKDELAY(TIME) vTaskDelay(TIME/portTICK_PERIOD_MS)


GRGB led(COMMON_CATHODE, R_PIN, G_PIN, B_PIN);

uint32_t currAuto = 0;
uint32_t period = 1000;


uint32_t ledTimeStamp;
const uint16_t ledDelay = 25;
uint16_t led_counter = 0;

bool StatusH = false;
Humidiffer* hum;
SensorHT* sensor;


void humLoop(void* pvParameters){
  for(;;){
    hum->humidifferLoop();
    TASKDELAY(10);
  }
}

void sensorCheckTemp(void* pvParameters){
  for(;;){
    sensor->checkTemperature();
    TASKDELAY(250);
  }
}

void sensorCheckStatus(void* pvParameters){
  for(;;){
    sensor->Check();
    TASKDELAY(10);
  }
}

void controleHum(void* pvParameters){
  for(;;){
    if (needSomeDo) {
      if(!hum->getFlagStatus()){
        hum->OnMode();
      }
    }else if(hum->getFlagStatus()){
      hum->OffMode();
    }
    TASKDELAY(50);
  }
}


void setup() {
  Serial.begin(115200);

  hum = new Humidiffer(HUMIDIFIER_PIN, 1000);
  sensor = new SensorHT(&dht);

  setupPins();
  
  hum->setupHumidiffer();  
  sensor->setupSensor();

  xTaskCreate(humLoop, "humLoop", 100, NULL, 2, NULL);
  xTaskCreate(sensorCheckTemp, "sensorCheckTemp", 100, NULL, 2, NULL);
  xTaskCreate(sensorCheckStatus, "sensorCheckStatus", 100, NULL, 2, NULL);
  xTaskCreate(controleHum, "controleHum", 100, NULL, 2, NULL);
  
}


void loop() {

}


