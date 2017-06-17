
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
-----------                HUMBOT SARGANTANA BASIC         -----------------
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-----------------             DESCRIPTION            ------------------------
-----------------------------------------------------------------------------
This program allows Humbot Sargantana to make a previously programmed
sequency of movements.

Remember that we give you this programa to make it easy for you to play with 
the robot but we encourage you to modify it and try new things. You can
modify every line of the code but we indicate you some lines that
are easier to change. You will see that these zones are surrounded by 
several hashtags (########################) and EASY MODIFICATION ZONE text.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
---------------------            DETAILS                 --------------------
-----------------------------------------------------------------------------
--  Program: Programable sequence
--  Version: 1.00b (beta version, it may containt some bugs or errors)
    1.00b First working version
--  Date: 23-03-16
--  Authors: Cristobal Selma, Maria Garcia

-----------------------------------------------------------------------------
-----------                PROGRAM                 --------------------------
-----------------------------------------------------------------------------

/*--LIBRARIES----------------------------------------------------------------
We neeed to include libraries to be able to use servos
*/
#include <Servo.h>

//Definition of the variables and values to make the wheels move 
#define stop_wheel 90
#define forward_left_wheel 0
#define backward_left_wheel 180
#define forward_right_wheel 180
#define backward_right_wheel 0

/*--VARIABLES----------------------------------------------------------------
Definition of pin numbers where sensors and actuators will be connected
Initially we use these, but they can be changed at your convinience
*/
  const int L_wheel_servo_pin = 11;   //Left wheel servo
  const int R_wheel_servo_pin = 10;   //Right wheel servo 

//Create servo object for each one
  Servo L_wheel_servo;
  Servo R_wheel_servo;
  
/*
-----------------------------------------------------------------------------
--------------------             SETUP         ------------------------------
-----------------------------------------------------------------------------
The setup function runs once out every time that the board initializes or 
reset button is pushed. It's used to initiate variables or to setup 
different options.
-----------------------------------------------------------------------------
*/
void setup() 
{ 
//Attach the servos          
   L_wheel_servo.attach(L_wheel_servo_pin);
   R_wheel_servo.attach(R_wheel_servo_pin);
  
}
/*
-----------------------------------------------------------------------------
----------------                    LOOP                 --------------------
-----------------------------------------------------------------------------
The main part of the program is called LOOP and it runs indefinitely.
-----------------------------------------------------------------------------
*/
void loop() 
{ 
/*
NOTE: 1000 miliseconds are 1 second
To finish the sequence you have to call the end-program function.
Functions available:
    Stop(1000);->Stops for 1 second
    go_forward(1000);->Moves forward for 1 second
    turn_right(1000);->Turns right for 1 second
    turn_left(1000);->Turns left for 1 second
    go_backward(2000);->Moves backward for 2 seconds
    end_program();->End of the program
    self_steer(1000);->Spin around for 1 second

You can modify the time by changing the number in brackets. 
If you want to reset the sequence you can press the restart button on 
the board. 
*/
//#################### EASY MODIFICATION ZONE ###############################
//###########################################################################
//Sequence example (READ ABOVE)
Stop(1000);
go_forward(2000);
self_steer(2000);
go_forward(1000);
Stop(1000);
end_program();
//###########################################################################
//###########################################################################
}

/*
-----------------------------------------------------------------------------
-----------                     FUNCTIONS                  ------------------
-----------------------------------------------------------------------------
When we work with code, it's important to divide the code in functions.
These functions can be called from different parts of the program.
-----------------------------------------------------------------------------
*/
//Stop function
  void Stop(int time)     {
    control_servo(L_wheel_servo,20,stop_wheel);//Stop left servo
    control_servo(R_wheel_servo,20,stop_wheel);  //Stop right servo
    delay(time);
}

//Turn right
  void turn_right(int time)  {
    control_servo(L_wheel_servo,20,forward_left_wheel);//Run left servo
    control_servo(R_wheel_servo,20,stop_wheel);  //Stop right servo
    delay(time);
  }

//Turn left
  void turn_left(int time){
    control_servo(L_wheel_servo,20,stop_wheel);//Stop left servo
    control_servo(R_wheel_servo,20,forward_right_wheel);  //Run right servo
    delay(time);
  }

//Move forward with both wheels
  void go_forward(int time)   {
    control_servo(L_wheel_servo,20,forward_left_wheel);//Run left servo
    control_servo(R_wheel_servo,20,forward_right_wheel);  //Run right servo
    delay(time);
  }
  
//Move backward with both wheels
  void go_backward(int time)   {
    control_servo(L_wheel_servo,20,backward_left_wheel);//Run left servo
    control_servo(R_wheel_servo,20,backward_right_wheel);  //Run right servo
    delay(time);
  }
 
//Spin around
  void self_steer(int time)   {
    control_servo(L_wheel_servo,20,forward_left_wheel);//Run left servo
    control_servo(R_wheel_servo,20,backward_right_wheel);  //Run right servo
    delay(time);
  }

//End of the sequence, you can restart with the reset button
void end_program(){
  while(true);
  }
 
//This function controls any servo
  void control_servo(Servo servo, int time, int amount){
    servo.write(amount); //the servo turns
    delay(time); //we give time for the servo to act
  }
