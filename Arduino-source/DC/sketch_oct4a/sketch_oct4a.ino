// Motor A Connections
#define ENA_PIN 3
#define IN1_PIN 4
#define IN2_PIN 2

// Motor B Connections
#define ENB_PIN 9
#define IN3_PIN 7
#define IN4_PIN 8

void setup() {
  // Set all control pins as outputs
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
}

void loop() {
  // 1. Move both motors FORWARD at medium speed (value between 0 and 255)
  analogWrite(ENA_PIN, 255); 
  analogWrite(ENB_PIN, 255); 
  
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  delay(3000); // Run forward for 3 seconds

  // 2. Stop the motors
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(1000); // Stay stopped for 1 second

  // 3. Move both motors in REVERSE at full speed (255)
  analogWrite(ENA_PIN, 100); 
  analogWrite(ENB_PIN, 100); 
  
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  delay(3000); // Run reverse for 3 seconds

  // 4. Stop the motors before repeating loop
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  delay(1000); 
}