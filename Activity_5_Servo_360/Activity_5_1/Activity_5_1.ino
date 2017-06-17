#include <Servo.h>
int servoPin=6;
int potPin=A0;
int potValue = 0;
int mappedValue;
Servo myservo;

void setup() {
  myservo.attach(servoPin);
}

void loop() {
// read the value from the sensor:
  potValue = analogRead(potPin); 
//map values  
  mappedValue = map(potValue, 0, 1024, 0, 180);
//write values to the servo  
  myservo.write(mappedValue);
//give time to the servo to act
  delay(10);
}

