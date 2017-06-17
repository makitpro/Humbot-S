/*  LICENSE:
    MAKIT HUMBOT SARGANTANA
    Copyright (C) 2016  Makit: Cristobal Selma, Maria Garcia
    
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
-----------       HUMBOT SARGANTANA AVOID OBSTACLES BASIC  ------------------
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------------             DESCRIPTION            ------------------------
-----------------------------------------------------------------------------
This program allows Humbot Sargantana to avoid obstacles thanks to its 
ultrasonic sensor.

Remember that we give you this programa to make it easy for you to play with 
the robot but we encourage you to modify it and try new things. You can
modify every line of the code but we indicate you some lines that
are easier to change. You will see that these zones are surrounded by 
several hashtags (########################) and EASY MODIFICATION ZONE text.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
---------------                DETAILS                 ----------------------
-----------------------------------------------------------------------------
--  Program: Avoid obstacles basic
--  Version: 2.00b (beta version, it may containt some bugs or errors)
    1.00 First working version
    1.10 Added random turn
    1.20 Added tolerance for measurements
    2.00 Improved obstacles detection
--  Date: 06-04-17
--  Authors: Cristobal Selma, Maria Garcia

-----------------------------------------------------------------------------
-----------                PROGRAM                 -----------------
-----------------------------------------------------------------------------

/*--LIBRARIES----------------------------------------------------------------
We neeed to include libraries to be able to use servos
*/
#include <Servo.h>

//Definition of the values and speed to make the wheels move 
#define stop_wheel 90
#define forward_left_wheel 0
#define backward_left_wheel 180
#define forward_right_wheel 180
#define backward_right_wheel 0


/*--VARIABLES----------------------------------------------------------------
Definition of pin numbers where sensors and actuators will be connected
Initially we use these, but they can be changed at your convinience.*/
#define L_wheel_servo_pin 11   //Left wheel servo
#define R_wheel_servo_pin 10   //Right wheel servo 
#define TRIG_PIN 8             //Trigger pin
#define ECHO_PIN 7             //Echo pin

//#################### EASY MODIFICATION ZONE ###############################
//###########################################################################
float TOO_CLOSE = 1000;  //Distance at which the robot will turn
#define TURN_TIME 500 //Amount of time (ms) that the robot will be turning
//###########################################################################
//###########################################################################

//Create servo objects for each servo
  Servo L_wheel_servo;
  Servo R_wheel_servo;
  
//Other variables  
  int servo_delay = 20; //Time for the servo to make his function
  int read_Time =100; //Time interval between reading distance

/*
-----------------------------------------------------------------------------
-------------             SETUP-CONFIGURACION          --- ------------------
-----------------------------------------------------------------------------
The setup function runs once every time that the board initializes or 
reset button is pushed. It's used to iniciate variables or to setup 
different options.
-----------------------------------------------------------------------------
*/
void setup() 
{ 
//Setup servos
   L_wheel_servo.attach(L_wheel_servo_pin);
   R_wheel_servo.attach(R_wheel_servo_pin);

//Setup as input or output for the pins
   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
}
/*
-----------------------------------------------------------------------------
-----------             LOOP-BUCLE            ------------------
-----------------------------------------------------------------------------
The main part of the program is called LOOP and it runs indefinitely.
-----------------------------------------------------------------------------
*/
void loop() 
{
//Function that reads the ultrasonic sensor and resturns distance in cm
  int dataOk=0;
  float distance;
  delay(read_Time);
  for (int i = 0; i < 3 && dataOk != 1; i++) {
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  distance = pulseIn(ECHO_PIN, HIGH);
    if (distance!=0){
    dataOk=1;
    }
  }
//Check if an obstacle is too close, then turn
if (distance < TOO_CLOSE && distance!=0){
  turn_around(random(TURN_TIME-200,TURN_TIME+200));
  }
    else{
    go_forward();
    }

}

/*
-----------------------------------------------------------------------------
--------------------             FUNCTIONS            -----------------------
-----------------------------------------------------------------------------
When we work with code, it's important to divide the code in functions.
These functions can be called from different parts of the program.
-----------------------------------------------------------------------------
*/
//Stop function
  void Stop()     {
    L_wheel_servo.write(stop_wheel);//Stop left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(stop_wheel);//Stop right servo
    delay(servo_delay);//we give time for the servo to act
  }

//Turn around
  void turn_around(int time)  {
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(backward_right_wheel);  //Stop right servo
    delay(servo_delay);//we give time for the servo to act
    delay(time);
  }

//Move forward
  void go_forward()   {          
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(forward_right_wheel);  //Run right servo
    delay(servo_delay);//we give time for the servo to act
  }


  
 

