#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#define DHT22PIN D3
#define DHTTYPE DHT22
#define R_PIN D7
#define G_PIN D5
#define B_PIN D6
#define HUMIDIFIER_PIN D1


void setupPins()
{
    pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(HUMIDIFIER_PIN, OUTPUT);
  digitalWrite(HUMIDIFIER_PIN, LOW);
}

#endif