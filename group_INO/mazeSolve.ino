/*
SYSC4805 B'dazzled Blue Maze Solver Robot Main Code
Authors: Muhammmad Tarequzzaman, Samy Ibrahim, Jacob Martin, Ahmad Chaudhry
*/

// Pins for the Mototr A
const int motorA1      = 5;  
const int motorA2      = 6; 

// Pins for Motor B
const int motorB1      = 11; 
const int motorB2      = 10;

// Pins for Front IR Sensors
const int FrontOpticalSensorRight   = 6;
const int FrontOpticalSensorLeft   = 7;

// Pins for Side IR Sensors
const int SideOpticalSensorRight   = A5;
const int SideOpticalSensorLeft   = A6;

const int trigPin = 3;
const int echoPin = 4;

long duration;
double distance;

bool isDone = false;

// Array of Strings recording the path taken (printed to LCD).
String pathArray [40];
int index = 0; 

/*
This method is used to set up all the pins (for the HW Sensors) when the Robot is starting.
*/
void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  
  pinMode(FrontOpticalSensorRight, INPUT);
  pinMode(FrontOpticalSensorLeft, INPUT);
  pinMode(SideOpticalSensorRight, INPUT);
  pinMode(SideOpticalSensorLeft, INPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delay(3000);
}

/*
This method is used to stop the robot from moving (for when we reach the end). 
*/
void stop() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
};

/*
This method is used to make the robot fix up its path (left) if it has left the path. And record it in the array of paths. 
*/
void left() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,150);
    pathArray[index] = "Left";
    index++; 
}

/*
This method is used to make the robot fix up its path (right) if it has left the path. And record it in the array of paths. 
*/
void right() {
    digitalWrite (motorA1,LOW);
    analogWrite (motorA2,150);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
    pathArray[index] = "Right";
    index++; 
}

/*
This method is used to make the robot go straight. And record it in the array of paths. 
*/
void straight() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,HIGH);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,HIGH);
    pathArray[index] = "Straight";
    index++; 
}

/*
This method is used to make the robot turn right until it is back on the black tape path. 
@param leftSensorValue: data collected from the sensor on the left side of the robot (1-> turn left).
@param rightSensorValue: data collected from the sensor on the right side of the robot
*/
void turnRight(int leftSensorValue, int rightSensorValue) {
    digitalWrite (motorA1,LOW);
    analogWrite (motorA2,150);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
    delay(1000);
    while(true) {
      Serial.println("STUCK TURN RIFGHT");
      int leftSensorValue = digitalRead(FrontOpticalSensorLeft);
      int rightSensorValue = digitalRead(FrontOpticalSensorRight);
      if(leftSensorValue==1 && rightSensorValue==1) {
        pathArray[index] = "Right";
        index++; 
        return;
      }
    }
}

/*
This method is used to make the robot turn left until it is back on the black tape path. 
@param leftSensorValue: data collected from the sensor on the left side of the robot (1-> turn left).
@param rightSensorValue: data collected from the sensor on the right side of the robot
*/
void turnLeft(int leftSensorValue, int rightSensorValue) {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,150);
    delay(1000);
    while(true) {
      Serial.println("STUCK TURN LEFT");
      int leftSensorValue = digitalRead(FrontOpticalSensorLeft);
      int rightSensorValue = digitalRead(FrontOpticalSensorRight);
      if(leftSensorValue==1 && rightSensorValue==1) {
        pathArray[index] = "Left";
        index++;
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
    if(pathArray[i] == "Left"){
      path.concat("<-,");
    }
    if(pathArray[i] == "Right"){
      path.concat("->,");
    }
    if(pathArray[i] == "Straight"){
      path.concat(" |^ ");
    }
    if(pathArray[i] == "TurnAround"){
      path.concat(" W ");
    }
  }
  serial.println(path);
}


/*
This method is an infinite loop (main) that runs through the logic of the Robot Maze Solver, it reads the sensors and 
decides if there are any turns and intersections. It also records the directions taken in the specified array. 
*/
void loop() {
  // read the input on analog pin 0:
  int leftSensorValue = digitalRead(FrontOpticalSensorLeft);
  int rightSensorValue = digitalRead(FrontOpticalSensorRight);
  int SideRightSensorValue = digitalRead(SideOpticalSensorRight);
  int SideLeftSensorValue = digitalRead(SideOpticalSensorLeft);

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
  //go straigh
  Serial.println(distance);
  if(distance <= 15) {
    turnRight(leftSensorValue, rightSensorValue);
    pathArray[index] = "TurnAround";
    index++; 
  }
  if(leftSensorValue==1 && rightSensorValue==1){
    if(SideRightSensorValue){
        turnRight(leftSensorValue, rightSensorValue);
    } else if(SideLeftSensorValue){
        turnLeft(leftSensorValue, rightSensorValue);
    } else {
      straight();
    }
  }
  //go Right
  if(leftSensorValue==0 && rightSensorValue==1){
    right();
  }
  //go left
  if(leftSensorValue==1 && rightSensorValue==0){
    left();
  }
  //stop
  if(leftSensorValue==0 && rightSensorValue==0){
    stop();
  }
  if(isDone){
    printArray();
  }

}
