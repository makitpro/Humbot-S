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

-----------------------------------------------------------------------------
-----------------         HUMBOT SARGANTANA LINE FOLLOWER   -----------------
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-------------------             DESCRIPTION            ----------------------
-----------------------------------------------------------------------------
This program allows Humbot Sargantana to follow a dark line over a clear 
background. Use black tape to draw a circuit over a clear surface. you can
also use black paint on a white paper. Make the line thick (2cm) to make
it possible for the robot to follow.

Remember that we give you this programa to make it easy for you to play with 
the robot but we encourage you to modify it and try new things. You can
modify every line of the code but we indicate you some lines that
are easier to change. You will see that these zones are surrounded by 
several hashtags (########################) and EASY MODIFICATION ZONE text.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------------                DETAILS                 --------------------
-----------------------------------------------------------------------------
--  Program: Line follower
--  Version: 1.10b (beta version, it may containt some bugsor errors)
    1.00b Version with 3 IR sensors
    1.10b Added backward when line is lost
          Tested working fine
    1.30b Reduced speed and increased time  
--  Date: 07-03-16
--  Author: Cristobal Selma, Maria Garcia

-----------------------------------------------------------------------------
-----------------               PROGRAM                 ---------------------
-----------------------------------------------------------------------------
^/
/*--LIBRARIES----------------------------------------------------------------
We neeed to include libraries to be able to use servos
*/
#include <Servo.h>

//Definition of the values and speed to make the wheels move 
#define stop_wheel 90
#define forward_left_wheel 70 //max speed is 0
#define backward_left_wheel 110 //max speed is 180
#define forward_right_wheel 110 //max speed is 180
#define backward_right_wheel 70 //max speed is 0

/*--VARIABLES----------------------------------------------------------------
Definition of pin numbers where sensors and actuators will be connected
Initially we use these, but they can be changed at your convinience.*/
  const int L_wheel_servo_pin = 11;   //Left wheel servo
  const int R_wheel_servo_pin = 10;   //Right wheel servo  
  const int L_IR_sensor_pin = 9;   //Left IR sensor 
  const int R_IR_sensor_pin = 8;   //Right IR sensor
  const int C_IR_sensor_pin = 7;   //Center IR sensor 

//Create servo objects for each one
  Servo L_wheel_servo;
  Servo R_wheel_servo;
  
//Other variables  
  int L_irSensor;//State of the left sensor
  int R_irSensor;//State of the right sensor
  int C_irSensor;//State of the center sensor
  byte state;//State of the robot (binario)
  int servo_delay; //Time for the aervo to make his function
  boolean outofline; //keeps track of which way robot is out of line (0 left) (1 right)


//###################### EASY MODIFICATION ZONE #############################
//###########################################################################
//Initially the Humbot will follow a Black line over a clear background
  const int BLACK = 1;//when black is detected, sensor returns 1
  const int WHITE = 0;//when white is detected, sensor returns 0
//###########################################################################
//###########################################################################


/*
-----------------------------------------------------------------------------
-------------------             SETUP         -------------------------------
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
   pinMode(L_IR_sensor_pin,INPUT); 
   pinMode(R_IR_sensor_pin,INPUT);
   pinMode(C_IR_sensor_pin,INPUT);
//Setup initial state  
    state = B000;     //Initial state B000 (STOP)
    servo_delay = 20; //Time for the servo to make his function
    outofline=0;
   
  }
/*
-----------------------------------------------------------------------------
--------------------             LOOP            ----------------------------
-----------------------------------------------------------------------------
The main part of the program is called LOOP and it runs indefinitely.
-----------------------------------------------------------------------------
*/
void loop() 
{ 
//Read sensors
   bitWrite(state,0,digitalRead(L_IR_sensor_pin));
   bitWrite(state,1,digitalRead(C_IR_sensor_pin));
   bitWrite(state,2,digitalRead(R_IR_sensor_pin));
   
//Dependung on the sensors we assign an state
   switch(state) {
     case B111: back(70);break;
     case B011: turn_right();
     outofline=0;//left out of line 
     break;
     case B110: turn_left();
     outofline=1;//right out of line
     break;
     case B010: go_forward(); break;
     case B001: turn_right();
     outofline=0;//left out of line 
     break;
     case B100: turn_left();
     outofline=1;//right out of line
     break;
     case B000: go_forward(); break;
     case B101: go_forward(); break;
     default: Stop(); 
     break;
   }
    delay(20);//increased from 10 to 20 on version 1.30b
         
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
  void Stop(){
    L_wheel_servo.write(stop_wheel);//Stop left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(stop_wheel);//Stop right servo
    delay(servo_delay);//we give time for the servo to act
  }
//Go backwards searching for the line  
  void back(int time)     {
    if(!outofline){//is out of line by the left
    R_wheel_servo.write(backward_right_wheel);  //Back right servo
    delay(servo_delay);//we give time for the servo to act
    }
    else{//is out of line by the right
    L_wheel_servo.write(backward_left_wheel);//Back left servo
    delay(servo_delay);//we give time for the servo to act
    }
    delay(time);
    Stop();
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
//Go forward function 
  void go_forward()   {          
    L_wheel_servo.write(forward_left_wheel);//Run left servo
    delay(servo_delay);//we give time for the servo to act
    R_wheel_servo.write(forward_right_wheel);  //Run right servo
    delay(servo_delay);//we give time for the servo to act
  }

