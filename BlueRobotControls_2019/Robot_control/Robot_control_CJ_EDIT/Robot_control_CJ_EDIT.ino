 //Made by Brendon Brown
//Last updated March 11th, 2019
//This code takes the serial data it has received from the xbee and performs actions accordingly
//Pins 5 and 6 have PWM frequency of ~980 Hz and the rest ~490 Hz
//NOTE: the pins I have used and the actions of the robot will change depending on need
//this code expects the robot to be expecting RC/servo signals to work. [Most motor controllers accept this as an input]

//This code needs to be updated and can be improved drastically 
//This code will have to be changed every year as long as the robot is changed/updated
//Remember to hook up the circuit correctly and to have a common ground on almost all circuits used in the robot

#include <Servo.h>
//note when using the servo library the frequency is not important because the way servo signals work, be sure to understand why this is...
int COUNTER = 3;  //remove
int timer[2] = {0,0};//remove
int currVal[2] = {95,95}; //will hold the live update
int lastVal[2] = {95,95}; //will update to the currVal, will prevent sudden jerk of motors

Servo leftmotor;
Servo rightmotor;
int armupper0 = 13;
int armupper1 = 12;
int armlower0 = 7;
int armlower1 = 6;
Servo armbase;
char controlinput;      //This is where the serial input data gets put
char lastright = '1';   //Records the last left motor input
char lastleft = '0';    //Records the last right motor input
int variableleft;       //This variable is for the value of the left motor 
int variableright;      //This variable is for the value of the right motor 
int armbasepos = 90;    //This variable is for the value of the base of the arm
int countl = 0;         //The counter here needs to be updated for future applications so its more accurate but right now its a simple solution for lost of signal from the source
int countr = 0;
char armstatel = 'X';   //Records the last state of the lower arm (This actually might not be needed)
char armstateu = 'Y';   //Records the last state of the upper arm (This actually might not be needed)


void setup() {
  leftmotor.attach(9);
  rightmotor.attach(10);
  armbase.attach(11);
  pinMode(armupper0, OUTPUT);
  pinMode(armupper1, OUTPUT);
  pinMode(armlower0, OUTPUT);
  pinMode(armlower1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //Testing phase of the code
  Serial.print("Values-C-CL-CR-LL-LR:");
  Serial.print(controlinput);
  Serial.print(" ");
//  Serial.print(currVal[0]);
//  Serial.print(" ");
//  Serial.print(currVal[1]);
//  Serial.print(" ");
  Serial.print(lastVal[0]);
  Serial.print(" ");
  Serial.print(lastVal[1]);
  Serial.print(" ");
  Serial.print("\n");
  
  //only update when there is serialdata incoming
  if(Serial.available()>0){ 
    controlinput = Serial.read();
    //                               61-78-95-112-129
    //This is for the motor control  30-60-95-120-150
    if(controlinput == '0'){ //val = 95 //stationary
      currVal[0] = 95;
    }
    else if(controlinput == '2'){ //val = 112
      currVal[0]=112;
    }
    else if(controlinput == '4'){ //val = 78
      currVal[0]=78;
    }
    else if(controlinput == '6'){ //val= 129
      currVal[0]=129;
    }
    else if(controlinput == '8'){ //val = 61
      currVal[0]=61;
    }
    else if(controlinput == '1'){ //val = 95
      currVal[1]=95;
    }
    else if(controlinput == '3'){ //val = 112
      currVal[1]=112;
    }
    else if(controlinput == '5'){ //val = 78
      currVal[1]=78;
    }
    else if(controlinput == '7'){ //val = 129
      currVal[1]=129;
    }
    else if(controlinput == '9'){ //val = 61
      currVal[1]=61;
    }
    else if(controlinput == 'A'){
      if(armbasepos < 180){
        armbasepos = armbasepos + 10;
      }
    }
    else if(controlinput == 'B'){
      if(armbasepos > 0){
        armbasepos = armbasepos - 10;
      }
    }
    //This segment of code is for the arm which was controlled with relays and actuactors be sure 
    else if(controlinput == 'X'){
      armstatel = 'X';
      digitalWrite(armlower0, LOW);
      digitalWrite(armlower1, LOW);
    }
    else if(controlinput == 'Y'){
      armstateu = 'Y';
      digitalWrite(armupper0, LOW);
      digitalWrite(armupper1, LOW);
    }
    else if(controlinput == 'L'){
      armstatel = 'L';
      digitalWrite(armlower0, HIGH);
      digitalWrite(armlower1, LOW);
    }
    else if(controlinput == 'R'){
      armstatel = 'R';
      digitalWrite(armlower0, LOW);
      digitalWrite(armlower1, HIGH);
    }
    else if(controlinput == 'U'){
      armstateu = 'U';
      digitalWrite(armupper0, HIGH);
      digitalWrite(armupper1, LOW);
    }
    else if(controlinput == 'D'){
      armstateu = 'D';
      digitalWrite(armupper0, LOW);
      digitalWrite(armupper1, HIGH);
    }
  } 
  
  if(armstatel == 'X'){
    digitalWrite(armlower0, LOW);
    digitalWrite(armlower1, LOW);
  }
  if(armstateu == 'Y'){
    digitalWrite(armupper0, LOW);
    digitalWrite(armupper1, LOW);
  }
  
  //update the currVal to lastVal-can implement 
  if(currVal[0] > lastVal[0] ){
    lastVal[0] += 1 + (currVal[0] - lastVal[0])/16;
    //leftmotor.write(lastVal[0] );
    //leftmotor.write(lastVal[0] );
  }
  else if(currVal[0] < lastVal[0] ){
    lastVal[0] -= 1 - (currVal[0] - lastVal[0])/16;
    //leftmotor.write(lastVal[0] );
    //leftmotor.write(lastVal[0] );
  }
  else{
    //leftmotor.write(lastVal[0] );
  }

  if(currVal[1] > lastVal[1] ){
    //lastVal[1]++;
    lastVal[1] += 1 + (currVal[1] - lastVal[1])/16;
    //rightmotor.write(lastVal[1] );
    //rightmotor.write(lastVal[1] );
  }
  else if(currVal[1] < lastVal[1] ){
    //lastVal[1]--;
    lastVal[1] -= 1 - (currVal[1] - lastVal[1])/16;
    //rightmotor.write(lastVal[1] );
    //rightmotor.write(lastVal[1] );
  }
  else{
    //rightmotor.write(lastVal[1] );
  }
    

  //write value to robot
  leftmotor.write(lastVal[0] );
  rightmotor.write(lastVal[1] );
  
}
