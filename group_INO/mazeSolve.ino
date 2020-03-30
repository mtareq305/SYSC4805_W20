/*
SYSC4805 B'dazzled Blue Maze Solver Robot Main Code
Authors: Muhammmad Tarequzzaman, Samy Ibrahim, Jacob Martin, Ahmad Chaudhry
*/

//library packages required for I2C communication for LCD screen
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//constants for motors (left and right)
const int motorA1      = 5;  
const int motorA2      = 6; 

const int motorB1      = 11; 
const int motorB2      = 10;

const int opticalSensorFrontRight   = 8;
const int opticalSensorFrontLeft   = 7;

// Pins for Side IR Sensors
const int SideOpticalSensorRight   = A5;
const int SideOpticalSensorLeft   = A4;

//pins for ultrasonic sensor
const int trigPin = 3;
const int echoPin = 4;

//variables to calculate distance with ultrasonic
long duration;
double distance;

//variable to confirm when maze finished
bool isDone = false;

// Array of Strings recording the path taken (printed to LCD).
String pathArray [40];
String currentDirection;
int index = 0; 

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);  

/*
This method is used to set up all the pins (for the HW Sensors) when the Robot is starting.
*/
void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  
  pinMode(opticalSensorFrontRight, INPUT);
  pinMode(opticalSensorFrontLeft, INPUT);
  pinMode(SideOpticalSensorRight, INPUT);
  pinMode(SideOpticalSensorLeft, INPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delay(3000);

  lcd.init();
}
/*
This method is used to update the robots path 
*/
void updatePath(String s) {
  if (s != currentDirection) {
      pathArray[index] = s;
      index++;
      currentDirection = s;
  }
}

/*
This method is used to stop the robot from moving (for when we reach the end). 
*/
void stop() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
    //while loop used for debugging errors 
//    while (1) {
//      Serial.println(distance);
//    }
};

/*
This method is used to make the robot go straight. And record it in the array of paths. 
*/
void straight() {
    analogWrite (motorA1,0);
    analogWrite (motorA2,140);
    analogWrite (motorB1,0);
    analogWrite (motorB2,140);
    updatePath("Straight");
    index++; 
}

/*
This method is used to make the robot turn left until it is back on the black tape path. 
*/
void turnLeft() {
    digitalWrite (motorA1,LOW);
    analogWrite (motorA2,140);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
    delay(550);
    while(true) {
      Serial.println("STUCK TURN LEFT");
      int leftSensorValue = digitalRead(opticalSensorFrontLeft);
      int rightSensorValue = digitalRead(opticalSensorFrontRight);
      //once robot is repositioned on path it will return 
      if(rightSensorValue==1 && leftSensorValue == 1) {
        updatePath("Left");
        return;
      }
    }
}

/*
This method is used to make the robot turn right until it is back on the black tape path. 
*/
void turnRight() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,140);
    delay(600);
    while(true) {
      Serial.println("STUCK TURN RIGHT");
      int leftSensorValue = digitalRead(opticalSensorFrontLeft);
      int rightSensorValue = digitalRead(opticalSensorFrontRight);
      //once robot is repositioned on path it will return
      if(leftSensorValue==1) {
        updatePath("Right");
        return;
      }
    }
}

/*
This method is used to make the robot curve right to fix left lean. 
*/
void fixL(){
 digitalWrite (motorA1,LOW);
  analogWrite (motorA2,120);
  digitalWrite (motorB1,LOW);
  digitalWrite (motorB2,LOW);
}

/*
This method is used to make the robot curve left to fix right lean. 
*/
void fixR(){
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,120);
}

/*
This method is used to make the robot turn around until it is back on the black tape path. 
*/
void turnAround() {
    analogWrite (motorA1,160);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,130);
    delay(400);
    while(true) {
      Serial.println("STUCK TURN AROUND");
      int leftSensorValue = digitalRead(opticalSensorFrontLeft);
      int rightSensorValue = digitalRead(opticalSensorFrontRight);
      //once robot has found path it will turn back to loop
      if (leftSensorValue == 1 && rightSensorValue == 1) {
        updatePath("Back");
        return;
      }
    }
}
/*
This method is used to print out the successful path from the Maze Solving.
The Robot keeps track of the turns it takes (and any intersections) in an array of Strings (pathArray).
*/
void printArray(){
  String path = ""; 
  for(int i=0; i<= index; i++){
    //if robot has made a left turn add to path
    if(pathArray[i] == "Left"){
      path.concat("L");
    }
    //if robot has made a right turn add to path
    if(pathArray[i] == "Right"){
      path.concat("R");
    }
    //if robot is going straight add to path
    if(pathArray[i] == "Straight"){
      path.concat("S");
    }
    //if robot has to make u-turn add to path
    if(pathArray[i] == "Back"){
      path.concat("B");
    }
    //if robot has solved path complete path
    if(pathArray[i] == "Finished"){
      path.concat("F");
    }
  }
  lcd.setCursor(0,0);
    for (int i=0; i<index; i++) {
    lcd.print(path);
    }
}


/*
This method is an infinite loop (main) that runs through the logic of the Robot Maze Solver, it reads the sensors and 
decides if there are any turns and intersections. It also records the directions taken in the specified array. 
Once the robot reaches the end of the maze (ultrasonic sensor trighgers end of maze), the path taken is printed on the screen.
*/
void loop() {
  // read the input on analog pin 0:
  int leftSensorValueFront = digitalRead(opticalSensorFrontLeft);
  int rightSensorValueFront = digitalRead(opticalSensorFrontRight);
  Serial.print("left Sensor: ");
  Serial.println(leftSensorValueFront);
  Serial.print("right Sensor: ");
  Serial.println(rightSensorValueFront);  
  
  int SideRightSensorValue = digitalRead(SideOpticalSensorRight);
  int SideLeftSensorValue = digitalRead(SideOpticalSensorLeft);

  //print values for debugging and performance testing
  Serial.print("side left Sensor: ");
  Serial.println(SideLeftSensorValue);
  Serial.print("side right Sensor: ");
  Serial.println(SideRightSensorValue);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;

  // Follow Line Logic 
  Serial.println(distance);
  
  //if ultrasonic detects object within 20, it will stop. Maze is solved
  if(distance <= 20) {
    stop();
    updatePath("Finished"); 
    printArray();
    }
  }
  //Both sensors are on black, go straight
   if(leftSensorValueFront==1 && rightSensorValueFront==1){
    straight();
  }
  //robot is curving left, correct with right curve
  if(leftSensorValueFront==0 && rightSensorValueFront==1){
    fixL();
  }
  //robot is curving right, correct with left curve
  if(leftSensorValueFront==1 && rightSensorValueFront==0){
    fixR();
  }
  //Robot has found a path on the right
  if(SideRightSensorValue && !SideLeftSensorValue){
       turnRight();
   }
   //robot has found a path on the left
   if(SideLeftSensorValue && !SideRightSensorValue){
      turnLeft();
   }
  //robot has reached the end of a path, must make a u-turn
  if(leftSensorValueFront==0 && rightSensorValueFront==0){
    delay(100);
    if (leftSensorValueFront==0 && rightSensorValueFront==0){
    turnAround();
    }
    }
}
