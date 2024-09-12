#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "ThingSpeak.h"

#include "pins.h"
#include "index_html.h"
#include "dht_hum.h"
#include <GRGB.h>

#define ESP_WIFI_MODE 2 // 1 - Station mode, 2 - Access Point mode

WiFiClient client;
GRGB led(COMMON_CATHODE, R_PIN, G_PIN, B_PIN);

AsyncWebServer server(80);

const char *ssid = "espBunny";
const char *password = "12345678";
const char *myWriteAPIKey = "BXKWRSV58VTRXVF3";

uint16_t myChannelNumber = 1;
uint32_t sendDataTimeStamp;
const uint32_t sendDataDelay = 20000;

uint32_t ledTimeStamp;
const uint16_t ledDelay = 25;
uint16_t led_counter = 0;



void connectWiFi() {
  Serial.println("Attempting to connect to WiFi");
  if (RGBWorking) digitalWrite(R_PIN, HIGH);
  WiFi.begin(ssid, password);
  uint16_t numOfconnectionAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && numOfconnectionAttempts != 15) {
    delay(500);
    Serial.print(".");
    numOfconnectionAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) Serial.println("Connected to WiFi");
  else Serial.println("Failed to connect to WiFi");
  digitalWrite(R_PIN, LOW);
}


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Starting");

  setupPins();
  
  setupDHT();

  sendDataTimeStamp =millis();
#if(ESP_WIFI_MODE == 1)
  WiFi.mode(WIFI_STA);
  // Connect to Wi-Fi network with SSID and password
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }
  connectWiFi();
  Serial.println();
  Serial.print("ESP IP Address: http://");
  IPAddress IP = WiFi.localIP();
  Serial.println(IP);

  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  
  ThingSpeak.begin(client); 

  sendData();

#else// (ESP_WIFI_MODE == WIFI_AP)
  WiFi.mode(WIFI_AP);
  Serial.println("Setting AP (Access Point)…");
  WiFi.softAP(ssid, NULL);   //WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

   initWeb();
#endif

  Serial.println("Setup done");
}

void loop() {
  if (digitalRead(HUMIDIFIER_PIN) == HIGH) {
    if (millis() - humidifierTimeStamp > humidifierWorkingTime) {
      digitalWrite(HUMIDIFIER_PIN, LOW);
      timeForHumidifierLeft = true;
      humidifierSleepTimeStamp = millis();
    }
  }

  if (humidity < humidityAcceptableLevel) {
    if (oneTimeTriggerFlag) {
      humidifierStarting = true;
      humidifierTimeStamp = millis();
      digitalWrite(HUMIDIFIER_PIN, HIGH);
      oneTimeTriggerFlag = false;
    }

    if ((millis() - ledTimeStamp > ledDelay) && RGBWorking) {
      led.setWheel(led_counter);
      led_counter++;
      if (led_counter == 1530) {
        led_counter = 0;
      }
    } else if (!RGBWorking) {
      digitalWrite(R_PIN, LOW);
      digitalWrite(G_PIN, LOW);
      digitalWrite(B_PIN, LOW);
    }

    if (timeForHumidifierLeft && (millis() - humidifierSleepTimeStamp > humidifierSleepingTime)) {
      oneTimeTriggerFlag = true;
      timeForHumidifierLeft = false;
    }

  } else {
    oneTimeTriggerFlag = true;
    digitalWrite(R_PIN, LOW);
    digitalWrite(G_PIN, LOW);
    digitalWrite(B_PIN, LOW);
  }

  if (millis() - DHT22TimeStamp > DHT22Delay) {
    checkTemperature();
    DHT22TimeStamp = millis();
  }

#if (ESP_WIFI_MODE == 1)
  if (millis() - sendDataTimeStamp > sendDataDelay) {
    sendData();
    sendDataTimeStamp = millis();
  }
#endif
}



void sendData() 
{
  if (WiFi.status() == WL_CONNECTED) {
    if (allowToSendData) {
      if (!isnan(temperature) && !isnan(humidity)) {
        ThingSpeak.setField(1, temperature);
        ThingSpeak.setField(2, humidity);
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        if (x == 200) {
          Serial.println("Channel update successful.");
         if (firstSuccessRequest) {
           Serial.print("ESP IP Address: http://");
            IPAddress IP = WiFi.localIP();
            Serial.println(IP);
            initWeb();
           firstSuccessRequest = false;
         }
        }
      } else {
        Serial.println("Error in sending data, can`t read values from sensor");
      }
    }
  } else {
    connectWiFi();
  }
}


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void initWeb() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/wifi_true", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send data by wifi - true");
    allowToSendData = true;
    request->send(200, "text/plain", "ok");
  });

  server.on("/wifi_false", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send data by wifi - false");
    allowToSendData = false;
    request->send(200, "text/plain", "ok");
  });

  server.on("/set_temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      DHT22Delay = value.toInt() * 1000;
      Serial.print("Temperature value received: ");
      Serial.println(DHT22Delay);
    }
    request->send(200, "text/plain", "ok");
  });
  server.on("/set_humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      humidityAcceptableLevel = value.toInt();
      Serial.print("Humidity value received: ");
      Serial.println(humidityAcceptableLevel);
    }
    request->send(200, "text/plain", "ok");
  });

  server.on("/set_humidifier_working_time", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      humidifierWorkingTime = value.toInt() * 1000;
      Serial.print("Humidifier working time received: ");
      Serial.println(humidifierWorkingTime);
    }
    request->send(200, "text/plain", "ok");
  });

  server.on("/set_humidifier_sleeping_time", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      humidifierSleepingTime = value.toInt() * 1000;
      Serial.print("Humidifier sleeping time received: ");
      Serial.println(humidifierSleepingTime);
    }
    request->send(200, "text/plain", "ok");
  });

  server.on("/rgb_true", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("RGB lighting - true");
    RGBWorking = true;
    request->send(200, "text/plain", "ok");
  });

  server.on("/rgb_false", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("RGB lighting - false");
    RGBWorking = false;
    request->send(200, "text/plain", "ok");
  });

  server.on("/checkTemperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    String temperatureString = String(temperature, 1);
    request->send(200, "text/plain", temperatureString);
  });

  server.on("/checkHumidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    String humidityString = String(humidity, 1);
    request->send(200, "text/plain", humidityString);
  });

  server.on("/tempDelayCheck", HTTP_GET, [](AsyncWebServerRequest *request) {
    String tempDelayString = String(DHT22Delay);
    request->send(200, "text/plain", tempDelayString);
  });

  server.on("/humidTimeCheck", HTTP_GET, [](AsyncWebServerRequest *request) {
    String humidTimeString = String(humidifierWorkingTime);
    request->send(200, "text/plain", humidTimeString);
  });

  server.on("/humidDelayCheck", HTTP_GET, [](AsyncWebServerRequest *request) {
    String humidDelayString = String(humidifierSleepingTime);
    request->send(200, "text/plain", humidDelayString);
  });

  server.on("/humidityLevel", HTTP_GET, [](AsyncWebServerRequest *request) {
    String humidityLevelString = String(humidityAcceptableLevel);
    request->send(200, "text/plain", humidityLevelString);
  });

  server.onNotFound(notFound);
  server.begin();
}
