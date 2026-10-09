#include "Arduino.h"

const int trigPin = 10;
const int echoPin = 11;
float duration, distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger pin sents the sonic bursts
  analogWrite(trigPin,LOW);
  delayMicroseconds(2);
  analogWrite(trigPin,HIGH);
  delayMicroseconds(10);

  // Duration b/w echo pin high and echo pin low is time taken by pulse to come back
  duration = pulseIn(echoPin, HIGH); // returns time in μs
  distance = (duration*.0343)/2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(2000);
}
