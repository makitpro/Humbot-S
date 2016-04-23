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
---------- HUMBOT SARGANTANA AVOID OBSTACLES WITH CREST LED  ----------------
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
--  Program: Avoid obstacles with led
--  Version: 1.20b (beta version, it may containt some bugs or errors)
    1.00 First working version
    1.10 Added random turn
    1.20 Added tolerance for measurements
--  Date: 23-03-16
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
#define TRIG_PIN 7             //Trigger pin
#define ECHO_PIN 8             //Echo pin

//#################### EASY MODIFICATION ZONE ###############################
//###########################################################################
#define TOO_CLOSE 15  //Distance(cm)at which the robot will turn
#define TURN_TIME 500 //Amount of time (ms) that the robot will be turning
//###########################################################################
//###########################################################################

//---Start of LED extra siren------------------  
const int redLED_pin = 2;   //Red LED 
const int greenLED_pin = 3;   //Green LED 
const int blueLED_pin = 4;   // Blue LED
//---End of LED extra siren--------------------

//Create servo objects for each servo
  Servo L_wheel_servo;
  Servo R_wheel_servo;
  
//Other variables  
  int servo_delay = 20; //Time for the servo to make his function
  int read_Time =150; //Time interval between reading distance
  long distance;
  long previous_distance;
  int tolerance = 5; //tolerance value to take a reading as valid
/*
-----------------------------------------------------------------------------
------------------------             SETUP         --------------------------
-----------------------------------------------------------------------------
The setup function runs once every time that the board initializes or 
reset button is pushed. It's used to initciate variables or to setup 
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

//Start of LED extra siren---------------------------------------------------
   pinMode(redLED_pin,OUTPUT); //setup LED as output  
   pinMode(greenLED_pin,OUTPUT); //setup LED as output
   pinMode(blueLED_pin,OUTPUT); //setup LED as output
   writeleds(0,155,0);          //initial color for crest led (green)
//---End of LED extra siren--------------------------------------------------
    
}
/*
-----------------------------------------------------------------------------
-----------------------             LOOP            -------------------------
-----------------------------------------------------------------------------
The main part of the program is called LOOP and it runs indefinitely.
-----------------------------------------------------------------------------
*/
void loop() 
{
distance = readsr(); //Read sensor 
//Define the action depending on the distance
  if (previous_distance-tolerance < distance < previous_distance+tolerance){
  if (distance < TOO_CLOSE){
  turn_around(random(TURN_TIME-200,TURN_TIME+200));
  }
    else{
    go_forward();
    }
  previous_distance = distance;
  }
  delay(read_Time);
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

//----Start of LED extra siren-----------------------------------------------
//########## EASY MODIFICATION ZONE ############
//###########################################################################
    /* Here we can change the color of the led while it's spining. 
    We can assign a value between 0 and 255 for each color (red, green, blue)
    and we can combine them. In the example the led is blue.*/
    writeleds(0,0,155); //Color of the led while it's spining
//###########################################################################
//###########################################################################
//---End of LED extra siren-------------------------------------------------- 

    delay(time);
  }


//Move forward
  void go_forward()   {          
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(forward_right_wheel);  //Run right servo
    delay(servo_delay);//we give time for the servo to act
    
//----------Start of LED extra siren------------------    
//########## EASY MODIFICATION ZONE ############
//###########################################################################
    /*Here we can change the color of the led while it's running to some obstacle.
    We can assign a value between 0 and 255 for each color (red, green, blue) 
    and change the distance for each color. In the example if the distance is less than 25 
    the led is red while if the distance is between 24 and 40 the led is yellow.*/
    
    if (distance < 25){        //Distance where the led colour is changed
    writeleds(155,0,0);}       //red led
    else if (distance < 40){   //Distance where the led colour is changed to the next colour
    writeleds(155,155,0);}     //yellow led
    else writeleds(0,155,0);   //if distance is higher, greed led
//###########################################################################
//###########################################################################
//-------End of LED extra siren----------------------------------------------
  }

  
//-------Start of LED extra siren--------------------------------------------
  void writeleds(int red, int green, int blue){
   analogWrite(redLED_pin, red); //write red led
   analogWrite(greenLED_pin, green); //write green led
   analogWrite(blueLED_pin, blue); //write blue led
   }
//------End of LED extra siren-----------------------------------------------

//Function that reads the ultrasonic sensor and returns distance in cm
  int readsr(){
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  return(distance);
  
}

