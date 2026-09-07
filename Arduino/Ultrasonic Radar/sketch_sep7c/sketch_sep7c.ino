#include <Servo.h>
#include "Arduino.h"

Servo sv;

int sv_pin=9;
int trigPin = 12;
int echoPin = 13;
int SWEEP_DELAY = 5; // Delay between servo movements in milliseconds
float duration, distance;
int angle;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  sv.attach(sv_pin);
}

void loop() {
  // scan from 0 to 180 degrees
  for(angle = 0; angle < 180; angle++)  
  {                                  
    sv.write(angle);               
    delay(SWEEP_DELAY);
    int distance = calculateDistance();
    printData(angle, distance);
  } 

  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--)    
  {                                
    sv.write(angle);           
    delay(SWEEP_DELAY);   
    int distance = calculateDistance();
    printData(angle, distance);    
  } 
}

int calculateDistance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);

  // Duration b/w echo pin high and echo pin low is time taken by pulse to come back
  duration = pulseIn(echoPin, HIGH, 30000); // returns time in μs
  distance = (duration*.0343)/2;

  return distance;
}

void printData(int angle, int distance) {
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}
