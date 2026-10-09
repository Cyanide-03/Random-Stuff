#include <Servo.h>
#include <Wire.h>
#include <MPU9250_WE.h>
#include <Wire.h>
#include <MPU9250_WE.h>
#define MPU9250_ADDR 0x68

Servo sv;
int sv_pin = 9;
float roll=0.0;
float pitch=0.0;
float yaw=0.0;
float roll_gyro;
float setpoint = 0.0;
float Kp = 1.0;
float Ki = 0.05;
float Kd = 0.0;
unsigned long previous_time = 0;
float previous_error = 0;
float integral = 0;
// int servo_pos = 90;

// Create the MPU9250 object using the Wire instance and address
MPU9250_WE myMPU9250 = MPU9250_WE(&Wire, MPU9250_ADDR);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);  // Set I2C to fast mode 400kHz

  sv.attach(sv_pin);

  // Wake up the MPU6500 manually via Wire to ensure it's active
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0x00);  // Clear sleep mode
  Wire.endTransmission();
  delay(50);

  Serial.println("Bypassing library init check — MPU6500 active!");

  // Calibrate gyro and accel offsets (keep the sensor flat and still)
  Serial.println("Calibrating offsets... Keep the sensor still.");
  myMPU9250.autoOffsets();
  Serial.println("Calibration done!");

  // Set ranges
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_8G);
  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_500);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);

  previous_time = millis();
}

void loop() {
  unsigned long current_time = millis();
  float dt = (current_time - previous_time) / 1000.0;  // convert milliseconds to seconds
  previous_time = current_time;

  if (dt <= 0.0) dt = 0.001;  // Prevent division by zero

  // Fetch readings from the sensor
  xyzFloat gValue = myMPU9250.getGValues();
  xyzFloat gyr = myMPU9250.getGyrValues();

  // Print out gyroscope rotation in degrees/second
  float Ax = gValue.x;
  float Ay = gValue.y;
  float Az = gValue.z;

  // Print out gyroscope rotation in degrees/second
  float gyr_x = gyr.x;
  float gyr_y = gyr.y;
  float gyr_z = gyr.z;
  
  // From accelerometer
  float roll_acc  = atan2(Ay, Az)* 180.0 / PI;
  float pitch_acc = atan2(-Ax, sqrt(Ay*Ay + Az*Az))* 180.0 / PI;

  // From gyroscope
  roll_gyro  = roll_gyro+gyr_x*dt* 180.0 / PI;
  float delta_roll=gyr_x*dt* 180.0 / PI;
  float pitch_gyro = pitch+gyr_y*dt* 180.0 / PI;
  float yaw_gyro  = yaw+gyr_z*dt* 180.0 / PI;

  // Filter comes here
  
  // Complementary Filter
  float roll_filtered=0.25*roll_acc+0.75*(roll+delta_roll); 

  roll=roll_filtered;

  float error = setpoint - roll_filtered;

  float P = Kp * error;
  integral += (error * dt);
  float I = Ki * integral;
  float D = Kd * ((error - previous_error) / dt);

  float output = P + I + D;

  float servo_pos=90+output;

  if (servo_pos > 180) servo_pos = 180;
  if (servo_pos < 0) servo_pos = 0;

  sv.write(servo_pos);

  Serial.print("Target:0,");
  Serial.print("Measured:");
  Serial.println(roll);
  // Serial.print(",Servo:");
  // Serial.println(servo_pos);

  previous_error = error;

  delay(10);
}
