#include <GRGB.h>
#include "ThingSpeak.h"

#include "pins.h"
#include "dht_hum.h"
#include "humidifier.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/timers.h"
// #include "freertos/event_groups.h"


GRGB led(COMMON_CATHODE, R_PIN, G_PIN, B_PIN);


uint32_t ledTimeStamp;
const uint16_t ledDelay = 25;
uint16_t led_counter = 0;

bool StatusH = false;
Humidiffer* hum;

void setup() {
  Serial.begin(115200);
  hum = new Humidiffer(HUMIDIFIER_PIN, 1000);
  hum->setupHumidiffer();
  delay(100);
  Serial.println("Starting");
  setupPins();
  setupDHT();
  Serial.println("Setup done");
  hum->OnMode();
}




void loop() {

  hum->autoUpdate(checkTemperature);
  
  if () {
    if(!hum->getFlagStatus()){
      StatusH = true;
    }
  }else {
    StatusH = false;
  }

  hum->humidifferLoop();
}


