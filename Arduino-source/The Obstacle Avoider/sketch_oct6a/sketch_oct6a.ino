#include <Servo.h>
#include "Arduino.h"
// #include <ArduinoSTL.h>
#include "vector_field_histogram.h"

// Motor A Connections
#define ENA_PIN 11
#define IN1_PIN 13
#define IN2_PIN 12

// Motor B Connections
#define ENB_PIN 3
#define IN3_PIN 2
#define IN4_PIN 4

// Servo Connections
#define sv_pin 6 

// HCSR04 Connections
#define trigPin 10
#define echoPin 9

Servo sv;

int SWEEP_DELAY = 20; // Delay between servo movements in milliseconds
float distance;
int angle;

int calculateDistance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  // Duration b/w echo pin high and echo pin low is time taken by pulse to come back
  unsigned long duration = pulseIn(echoPin, HIGH, 30000); // returns time in μs

  if (duration == 0) {
        return -1;  // No valid echo
  }

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
  int turnDelay=abs(90-targetangle)*12;
  int slowSpeed = speed / 2;
  if (targetangle < 90) {
        // Turn Right: Hold left motor still, run right motor forward
        analogWrite(ENA_PIN, speed);
        analogWrite(ENB_PIN, slowSpeed);
    } else {
        // Turn Left: Hold right motor still, run left motor forward
        analogWrite(ENA_PIN, slowSpeed);
        analogWrite(ENB_PIN, speed);
    }

    // Both motors forward
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);

    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);

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
  // sv.write(90);

  // scan from 0 to 180 degrees
  for(angle = 40; angle <= 150; angle++)  
  {                                  
    sv.write(angle);               
    delay(SWEEP_DELAY);
    int distance = calculateDistance();
    scanDistances[angle]=distance;
    printData(angle, distance);
  } 

  // now scan back from 180 to 0 degrees
  for(angle = 150; angle >= 40; angle--)    
  {                                
    sv.write(angle);           
    delay(SWEEP_DELAY);   
    int distance = calculateDistance();
    scanDistances[angle]=(scanDistances[angle]+distance)/2; // this is done so that the distance is measured more reliably
    printData(angle, distance);    
  } 

  int bestAngle=findBestPath(scanDistances,10,20);

  pivoturn(bestAngle,180);

  delay(1000);

  // 1. Move both motors FORWARD at medium speed (value between 0 and 255)
  analogWrite(ENA_PIN, 150); 
  analogWrite(ENB_PIN, 150); 
  
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(500); // Run forward for 1.5 seconds

  // Stop motors
  digitalWrite(IN1_PIN, LOW); digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW); digitalWrite(IN4_PIN, LOW);

  delay(1000);
}