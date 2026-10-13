#include <Servo.h>
#include "Arduino.h"
// #include <ArduinoSTL.h>
#include "vector_field_histogram.h"
#include "WiFiS3.h"
#include <ArduinoOTA.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

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

int publishedDistances[180];
int publishedBestAngle = -1;
bool publishedScanComplete = false;
unsigned long scanId = 0;
const int MIN_ANGLE=40;
const int MAX_ANGLE=150;


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

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void setup() {
  Serial.begin(115200);

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to WiFi");
  printWifiStatus(); 

  // Start OTA
  ArduinoOTA.begin(
    WiFi.localIP(),
    "arduino",
    "password",
    InternalStorage
  );
  Serial.println("OTA is working!");
  server.begin();                           // start the web server on port 80

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

void handleWebserver(){
  WiFiClient client = server.available();   // listen for incoming clients

  if (!client) return;

  // Read the first HTTP request line.
  String request = "";
  unsigned long start = millis();

  while (client.connected() && millis() - start < 1000) {
    if (client.available()) {
      char c = client.read();

      if (c == '\n') break;
      if (c != '\r') request += c;
    }
  }

  // Consume the remaining HTTP headers until the blank line.
  start = millis();
  while (client.connected() && millis() - start < 1000) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        // A blank header line ends with just '\n'
        break;
      }
    }
  }

  if (request.startsWith("GET /api/scan ")) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println();

    client.print("{\"scan_complete\":");
    client.print(publishedScanComplete ? "true" : "false");
    client.print(",\"scan_id\":");
    client.print(scanId);
    client.print(",\"best_angle\":");

    if (publishedBestAngle < 0) {
      client.print("null");
    } else {
      client.print(publishedBestAngle);
    }

    client.print(",\"measurements\":[");

    bool first = true;
    for (int a = MIN_ANGLE; a <= MAX_ANGLE; a++) {
      if (!first) client.print(",");
      first = false;

      client.print("{\"angle\":");
      client.print(a);
      client.print(",\"distance_cm\":");

      if (publishedDistances[a] < 0) {
        client.print("null");
      } else {
        client.print(publishedDistances[a]);
      }

      client.print("}");
    }

    client.println("]}");
  } 
  else {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("Use /api/scan");
  }

  client.stop();
}

void loop() {
  ArduinoOTA.poll();
  handleWebserver();

  int scanDistances[180];
  bool scan_complete=false;
  int bestAngle=-1;
  // sv.write(90);

  // scan from 0 to 180 degrees
  for(angle = 40; angle <= 150; angle++){                                  
    sv.write(angle);               
    delay(SWEEP_DELAY);
    int distance = calculateDistance();
    scanDistances[angle]=distance;
    printData(angle, distance);
  } 

  // now scan back from 180 to 0 degrees
  for(angle = 150; angle >= 40; angle--){                                
    sv.write(angle);           
    delay(SWEEP_DELAY);   
    int distance = calculateDistance();
    scanDistances[angle]=(scanDistances[angle]+distance)/2; // this is done so that the distance is measured more reliably
    printData(angle, distance);    
  } 

  scan_complete=true;
  
  bestAngle=findBestPath(scanDistances,10,20);

  pivoturn(bestAngle,180);

  bestAngle = findBestPath(scanDistances, 10, 20);

  for (int a = MIN_ANGLE; a <= MAX_ANGLE; a++) {
    publishedDistances[a] = scanDistances[a];
  }

  publishedBestAngle = bestAngle;
  scanId++;
  publishedScanComplete = true;

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