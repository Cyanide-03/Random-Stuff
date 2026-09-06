#include <Servo.h>
Servo sv;

int sv_pin=9;
int angle;
 
void setup() {
  // put your setup code here, to run once:
  sv.attach(sv_pin);
  // sv.write()
}

void loop() {
  // put your main code here, to run repeatedly:
  // scan from 0 to 180 degrees
  for(angle = 10; angle < 180; angle++)  
  {                                  
    sv.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 10; angle--)    
  {                                
    sv.write(angle);           
    delay(15);       
  } 
}
