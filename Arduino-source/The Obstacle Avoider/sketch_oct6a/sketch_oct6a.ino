#include <Servo.h>
#include "Arduino.h"
// #include <ArduinoSTL.h>
#include "vector_field_histogram.h"

// Motor A Connections
#define ENA_PIN 3
#define IN1_PIN 4
#define IN2_PIN 2

// Motor B Connections
#define ENB_PIN 9
#define IN3_PIN 7
#define IN4_PIN 8

// Servo Connections
#define sv_pin 5 // # ! CHANGE IT

// HCSR04 Connections
#define trigPin 12
#define echoPin 13

Servo sv;

int SWEEP_DELAY = 20; // Delay between servo movements in milliseconds
float duration, distance;
int angle;

int calculateDistance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);

  // Duration b/w echo pin high and echo pin low is time taken by pulse to come back
  duration = pulseIn(echoPin, HIGH, 30000); // returns time in μs
  distance = (duration*.0343)/2; // return distance in cm

  return distance;
}

void printData(int angle, int distance) {
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}

void pivoturn(int targetangle, int speed){
  // turndelay factor has to be calculated by testing
  int turnDelay=abs(targetangle)*12;
  if (targetAngle > 0) {
        // Turn Right: Hold left motor still, run right motor forward
        analogWrite(ENA_PIN, 0);
        digitalWrite(IN1_PIN, HIGH); 
        digitalWrite(IN2_PIN, LOW); // Left motor STOP

        analogWrite(ENB_PIN, speed);
        digitalWrite(IN3_PIN, HIGH);
        digitalWrite(IN4_PIN, LOW); // Right motor FORWARD
    } else {
        // Turn Left: Hold right motor still, run left motor forward
        analogWrite(ENA_PIN, speed);
        digitalWrite(IN1_PIN, HIGH); 
        digitalWrite(IN2_PIN, LOW); // Left motor FORWARD

        analogWrite(ENB_PIN, 0);
        digitalWrite(IN3_PIN, HIGH);
        digitalWrite(IN4_PIN, LOW); // Right motor STOP
    }

    delay(turnDelay);

    // Stop motors
    digitalWrite(IN1_PIN, LOW); digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN3_PIN, LOW); digitalWrite(IN4_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  sv.attach(sv_pin);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
}

void loop() {
  int scanDistances[180];
  // scan from 0 to 180 degrees
  for(angle = 0; angle < 180; angle++)  
  {                                  
    sv.write(angle);               
    delay(SWEEP_DELAY);
    int distance = calculateDistance();
    scanDistances[angle]=distance;
    printData(angle, distance);
  } 

  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--)    
  {                                
    sv.write(angle);           
    delay(SWEEP_DELAY);   
    int distance = calculateDistance();
    scanDistances[angle]=(scanDistances[angle]+distance)/2; // this is done so that the distance is measured more reliably
    printData(angle, distance);    
  } 

  int bestAngle=findBestPath(scanDistances,10);

  pivoturn(bestAngle,150);

  // 1. Move both motors FORWARD at medium speed (value between 0 and 255)
  analogWrite(ENA_PIN, 200); 
  analogWrite(ENB_PIN, 200); 
  
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(1500); // Run forward for 1.5 seconds

}