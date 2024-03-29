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
---------            HUMBOT SARGANTANA LIGHT FOLLOWER       -----------------
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------            DESCRIPTION            ------------------
-----------------------------------------------------------------------------
This program allows Humbot Sargantana to follow a light. Initially the robot 
follows the light but also can run away from light. Look for the easy 
modification zone and modify the variable.

Remember that we give you this programa to make it easy for you to play with 
the robot but we encourage you to modify it and try new things. You can
modify every line of the code but we indicate you some lines that
are easier to change. You will see that these zones are surrounded by 
several hashtags (########################) and EASY MODIFICATION ZONE text.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------                DETAILS                 -----------------
-----------------------------------------------------------------------------
--  Program: Light follower
--  Version: 1.10b (beta version, it may containt some bugs or errors)
    1.00b First working version
    1.10b Added "huir" function
--  Date: 23-03-16
--  Authors: Cristobal Selma, Maria Garcia  

-----------------------------------------------------------------------------
-----------                PROGRAM                 -----------------
-----------------------------------------------------------------------------
/*--LIBRARIES------------------------------------------------------
We neeed to include libraries to be able to use servos
*/
#include <Servo.h>


//Definition of the variables and values to make the wheels move  
#define stop_wheel 90
#define forward_left_wheel 0
#define backward_left_wheel 180
#define forward_right_wheel 180
#define backward_right_wheel 0
#define difference 40

//Variables for manual or follow line modes
#define MANUAL_CONTROL 0
#define LINE_FOLLOWER  1

//BaudRate for bluetooth connection
#define BLUETOOTH_BR 9600

// Size of the received data buffer
#define bufferSize 1

// Default delay 
#define defaultDelay 10

//######################### EASY MODIFICATION ZONE ##########################
//###########################################################################
#define dark 1000 //Define the value of darkness that will make the robot stop
#define light 30 //Define the value of light that will make the robot spin (the smaller number the more light)
#define spin_time 2000 //Time that the robot will be spinning
#define action "seguir" // You can change between "seguir" (follow) or "huir" (escape)
//###########################################################################
//###########################################################################

/*--VARIABLES----------------------------------------------------------------
Definition of pin numbers where sensors and actuators will be connected
Initially we use these, but they can be changed at your convinience.*/
  const int L_wheel_servo_pin = 11;   //Left wheel servo
  const int R_wheel_servo_pin = 10;   //Right wheel servo 
  const int L_LDR_sensor_pin = A3;   //Left LDR analog sensor 
  const int R_LDR_sensor_pin = A4;   //Right LDR analog sensor

  char dataBuffer[bufferSize]; //A char buffer to storage the received data from the Bluetooth Serial
  int i = 0; //Buffer iterator
  int numChar = 0; //Number of characters availables in the Serial
  int currentState;   // Variable that controls the current state of the program 

//Create servo objects for each one
  Servo L_wheel_servo;
  Servo R_wheel_servo;
  
//Other variables  
  int L_ldrSensor;//State of the left sensor
  int R_ldrSensor;//State of the right sensor
  int state;//State of the robot
  int servo_delay; //Time for the servo to make its function


/*
-----------------------------------------------------------------------------
-------------                 SETUP                    --- ------------------
-----------------------------------------------------------------------------
The setup function runs once every time that the board initializes or 
reset button is pushed. It's used to iniciate variables or to setup 
different options.
-----------------------------------------------------------------------------
*/
void setup() 
{ 
  
  Serial.begin(BLUETOOTH_BR); 
  Serial.flush(); 
  
//Attach the servos            
   L_wheel_servo.attach(L_wheel_servo_pin);
   R_wheel_servo.attach(R_wheel_servo_pin);

//Ports can be either inputs or outputs
//In this case sensors are configured as inputs
   pinMode(L_LDR_sensor_pin,INPUT); 
   pinMode(R_LDR_sensor_pin,INPUT);

//We need to assign an initial value to the robot  
    state = 0;  //Initialize state 0 (STOP)
    servo_delay = 20; //Time for the servo to make his function
    currentState = MANUAL_CONTROL;  // Default state is manual control
}
/*
-----------------------------------------------------------------------------
---------------------             LOOP            ---------------------------
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

  if(currentState == LINE_FOLLOWER) {
    light_follow();
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
//Go backward function  
  void go_backward()     {
    L_wheel_servo.write(backward_left_wheel);//Stop left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(backward_right_wheel);//Stop right servo
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
//Move forward  
  void go_forward()   {          
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(forward_right_wheel);  //Run right servo
    delay(servo_delay);//we give time for the servo to act
  }
//Spinning  
  void spin()   {          
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(backward_right_wheel);  //Run right servo
    delay(servo_delay);//we give time for the servo to act
  }
  
void light_follow(){  
  //Read sensors
   L_ldrSensor = analogRead(L_LDR_sensor_pin);
   R_ldrSensor = analogRead(R_LDR_sensor_pin);
   
//Assign state dependig on the sensors readings
  if (L_ldrSensor > dark && R_ldrSensor > dark){           //darkness
  state = 0;}
  else if (L_ldrSensor < light && R_ldrSensor < light){    //lightness
  state = 1;}  
  else if (abs(L_ldrSensor - R_ldrSensor) < difference){   //same on two sensors
  state = 2;}
  else if (L_ldrSensor > R_ldrSensor){                     //more light on the right
  state = 3;}
  else if (L_ldrSensor < R_ldrSensor){                     //more light on the left
  state = 4;}
  else {//
  state = 0;}   

//Change action depneding on the function ("huir" o "seguir")
    if (action == "huir"){
    state=state+10;
    }
   
   switch(state) {
     case 2: go_forward();break;
     case 3: turn_right();break;
     case 4: turn_left();break;
     case 0: Stop(); break;
     case 1: spin(); break;
     case 12: go_backward(); break;
     case 13: turn_left(); break;
     case 14: turn_right(); break;
     case 10: spin(); break;
     case 11: Stop(); break;
     default: Stop();break;
   }
    delay(10);
}
//Function for manual control
  void setAction(char* data) {
  
  switch(data[0]) {

    /* Line follower mode button pressed */
    case 'I':
      currentState = LINE_FOLLOWER;
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

