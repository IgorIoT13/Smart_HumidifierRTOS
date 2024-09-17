#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#define DHT22PIN 3
#define DHTTYPE DHT22
#define R_PIN 7
#define G_PIN 5
#define B_PIN 6
#define HUMIDIFIER_PIN 1


void setupPins()
{
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(HUMIDIFIER_PIN, OUTPUT);
  digitalWrite(HUMIDIFIER_PIN, LOW);
}

#endif