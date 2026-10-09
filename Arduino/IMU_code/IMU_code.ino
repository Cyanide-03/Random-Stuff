#include <Wire.h>
#include <MPU9250_WE.h>

#define MPU9250_ADDR 0x68

// Create the MPU9250 object using the Wire instance and address
MPU9250_WE myMPU9250 = MPU9250_WE(&Wire, MPU9250_ADDR);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000); // Set I2C to fast mode 400kHz

  // Wake up the MPU6500 manually via Wire to ensure it's active
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0x00); // Clear sleep mode
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
  
  delay(100);
}

void loop() {
  // Fetch readings from the sensor
  xyzFloat gValue = myMPU9250.getGValues();
  xyzFloat gyr = myMPU9250.getGyrValues();
  float temp = myMPU9250.getTemperature();

  // Print out acceleration in g
  Serial.print("Acceleration X: ");
  Serial.print(gValue.x);
  Serial.print(", Y: ");
  Serial.print(gValue.y);
  Serial.print(", Z: ");
  Serial.print(gValue.z);
  Serial.println(" g");

  // Print out gyroscope rotation in degrees/second
  Serial.print("Gyroscope X: ");
  Serial.print(gyr.x);
  Serial.print(", Y: ");
  Serial.print(gyr.y);
  Serial.print(", Z: ");
  Serial.print(gyr.z);
  Serial.println(" deg/s");

  // Print temperature
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" degC");
  Serial.println("");

  delay(1000);
}