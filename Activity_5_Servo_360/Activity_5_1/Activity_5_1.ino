/*  LICENSE:
    MAKIT HUMBOT SARGANTANA
    Copyright (C) 2017  Makit: Cristobal Selma, Maria Garcia
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
-----------------------------------------------------------------------------
-----------       HUMBOT SARGANTANA SERVO TEST  -----------------------------
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------------             DESCRIPTION            ------------------------
-----------------------------------------------------------------------------
This program is used to test the continuous rotation servo with a potentiometer
-----------------------------------------------------------------------------
*/
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

