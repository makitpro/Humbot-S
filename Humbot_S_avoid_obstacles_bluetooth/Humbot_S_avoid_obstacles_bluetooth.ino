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

Portions of this code come from Arduino code for the Pollywog robot by BQ
licensed under GNU General Public License v3 (github.com/bq/robopad).
*/
/*
-----------------------------------------------------------------------------
-----------       HUMBOT SARGANTANA AVOID OBSTACLES BT  ---------------------
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------------             DESCRIPTION            ------------------------
-----------------------------------------------------------------------------
This program allows Humbot Sargantana to avoid obstacles thanks to its 
ultrasonic sensor.
In this version it is possible to control the Humbot over Bluetooth with Android
APP.

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
--  Version: 1.30b (beta version, it may containt some bugs or errors)
    1.00 First working version
    1.10 Added random turn
    1.20 Added tolerance for measurements
    1.30 Added bluetooth control
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
#define TRIG_PIN 8             //Trigger pin
#define ECHO_PIN 7             //Echo pin

//Variables for manual or follow line modes
#define MANUAL_CONTROL 0
#define AVOID_MODE  1

//BaudRate for bluetooth connection
#define BLUETOOTH_BR 9600

// Size of the received data buffer
#define bufferSize 1

// Default delay 
#define defaultDelay 10

//#################### EASY MODIFICATION ZONE ###############################
//###########################################################################
#define TOO_CLOSE 15  //Distance(cm)at which the robot will turn
#define TURN_TIME 500 //Amount of time (ms) that the robot will be turning
//###########################################################################
//###########################################################################

//Create servo objects for each servo
  Servo L_wheel_servo;
  Servo R_wheel_servo;
  
//Other variables  
  int servo_delay = 20; //Time for the servo to make his function
  int read_Time =150; //Time interval between reading distance
  long distance; //Distance to objects in cm
  long previous_distance;
  int tolerance = 5; //tolerance value to take a reading as valid
  
  char dataBuffer[bufferSize]; //A char buffer to storage the received data from the Bluetooth Serial
  int i = 0; //Buffer iterator
  int numChar = 0; //Number of characters availables in the Serial
  int currentState;   // Variable that controls the current state of the program 
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
  Stop();
  Serial.begin(BLUETOOTH_BR); 
  Serial.flush();
  
//Setup servos
   L_wheel_servo.attach(L_wheel_servo_pin);
   R_wheel_servo.attach(R_wheel_servo_pin);

//Setup as input or output for the pins
   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
   
   currentState = MANUAL_CONTROL;  // Default state is manual control   

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
/* If there is something in the Bluetooth serial port */
  if (Serial.available() > 0) { 
   
    /* Reset the iterator and clear the buffer */
    i = 0;
    memset(dataBuffer, 0, sizeof(dataBuffer));  
    
    /* Wait for let the buffer fills up. Depends on the length of 
       the data, 1 ms for each character more or less */
    delay(bufferSize); 

    /* Number of characters availables in the Bluetooth Serial */
    numChar = Serial.available();   
    
    /* Limit the number of characters that will be read from the
       Serial to avoid reading more than the size of the buffer */
    if (numChar > bufferSize) {
          numChar = bufferSize;
    }

    /* Read the Bluetooth Serial and store it in the buffer*/
    while (numChar--) {
        dataBuffer[i++] = Serial.read();

        /* As data trickles in from your serial port you are 
         grabbing as much as you can, but then when it runs out 
         (as it will after a few bytes because the processor is 
         much faster than a 9600 baud device) you exit loop, which
         then restarts, and resets i to zero, and someChar to an 
         empty array.So please be sure to keep this delay */
        delay(3);
    } 

    /* Manage the data */
    setAction(dataBuffer);
    
  }

  if(currentState == AVOID_MODE) {
    avoid();
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
//Turn right function  
  void turn_right()  {
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(stop_wheel);  //Stop right servo
    delay(servo_delay);//we give time for the servo to act
  }
//Turn left function   
  void turn_left(){      
    L_wheel_servo.write(stop_wheel);//Stop left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(forward_right_wheel);  //Run right servo
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
//Go backward function 
  void go_backward()   {          
    L_wheel_servo.write(backward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(backward_right_wheel);  //Run right servo
    delay(servo_delay);//we give time for the servo to act
  }

//Function that reads the ultrasonic sensor and resturns distance in cm
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
  void avoid() 
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
  
//Function for manual control
  void setAction(char* data) {
  
  switch(data[0]) {

    /* Line follower mode button pressed */
    case 'I':
      currentState = AVOID_MODE;
      break;

    /* Manual control mode button pressed */
    case 'M':
      currentState = MANUAL_CONTROL;
      Stop();
      break;
   
    /* Stop button pressed */
    case 'S':
      Stop();
      break;

    /* Up button pressed  */
    case 'U':
//###################### EASY MODIFICATION ZONE #############################
//###########################################################################
/*If you change this line between turn_right, turn_left, go_backward,
 go_forward, you can play with the robot to make it more difficult to 
 control. Your humbot won't go exactly as you say, but that's the fun!
 Chanllenge your friends to do some laps changing these! */
      go_forward();
//###########################################################################
//###########################################################################    
      break;

    /* Down button pressed  */ 
    case 'D':
      go_backward();
      break;

    /* Left button pressed  */
    case 'L':
      turn_left();
      break;

    /* Right button pressed  */
    case 'R':
      turn_right();
      break;
  }
    /* Empty the Serial */      
  Serial.flush();
  }

