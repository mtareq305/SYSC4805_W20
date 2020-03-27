/*
SYSC4805 B'dazzled Blue Maze Solver Robot Main Code
Authors: Muhammmad Tarequzzaman, Samy Ibrahim, Jacob Martin, Ahmad Chaudhry
*/

const int motorA1      = 5;  
const int motorA2      = 6; 

const int motorB1      = 11; 
const int motorB2      = 10;

const int opticalSensorFrontRight   = 8;
const int opticalSensorFrontLeft   = 7;

// Pins for Side IR Sensors
const int SideOpticalSensorRight   = A5;
const int SideOpticalSensorLeft   = A4;

const int trigPin = 3;
const int echoPin = 4;

long duration;
double distance;

bool isDone = false;

// Array of Strings recording the path taken (printed to LCD).
String pathArray [40];
String currentDirection;
int index = 0; 

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
}
/*
This method is used toupdate the robots path 
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
    while (1) {
      Serial.println(distance);
    }
};

/*
This method is used to make the robot go straight. And record it in the array of paths. 
*/
void straight() {
    analogWrite (motorA1,0);
    analogWrite (motorA2,180);
    analogWrite (motorB1,0);
    analogWrite (motorB2,180);
    updatePath("Straight");
    index++; 
}

/*
This method is used to make the robot turn left until it is back on the black tape path. 
@param leftSensorValue: data collected from the sensor on the left side of the robot (1-> turn left).
@param rightSensorValue: data collected from the sensor on the right side of the robot
*/
void turnLeft() {
    digitalWrite (motorA1,LOW);
    analogWrite (motorA2,140);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
    delay(1000);
    while(true) {
      Serial.println("STUCK TURN LEFT");
      int leftSensorValue = digitalRead(opticalSensorFrontLeft);
      int rightSensorValue = digitalRead(opticalSensorFrontRight);
      if(rightSensorValue==1) {
        updatePath("Left");
        return;
      }
    }
}

/*
This method is used to make the robot turn right until it is back on the black tape path. 
@param leftSensorValue: data collected from the sensor on the left side of the robot (1-> turn left).
@param rightSensorValue: data collected from the sensor on the right side of the robot
*/
void turnRight() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,140);
    delay(1000);
    while(true) {
      Serial.println("STUCK TURN RIGHT");
      int leftSensorValue = digitalRead(opticalSensorFrontLeft);
      int rightSensorValue = digitalRead(opticalSensorFrontRight);
      if(leftSensorValue==1) {
        updatePath("Right");
        return;
      }
    }
}

void fixL(){
 digitalWrite (motorA1,LOW);
  analogWrite (motorA2,140);
  digitalWrite (motorB1,LOW);
  digitalWrite (motorB2,LOW);
}

void fixR(){
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,140);
}

/*
This method is used to make the robot turn around until it is back on the black tape path. 
@param leftSensorValue: data collected from the sensor on the left side of the robot (1-> turn left).
@param rightSensorValue: data collected from the sensor on the right side of the robot
*/
void turnAround() {
    analogWrite (motorA1,170);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,170);
    delay(1000);
    while(true) {
      Serial.println("STUCK TURN AROUND");
      int leftSensorValue = digitalRead(opticalSensorFrontLeft);
      int rightSensorValue = digitalRead(opticalSensorFrontRight);
        return;
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
    if(pathArray[i] == "Finished"){
      path.concat(" W ");
    }
  }
  Serial.println(path);
}


/*
This method is an infinite loop (main) that runs through the logic of the Robot Maze Solver, it reads the sensors and 
decides if there are any turns and intersections. It also records the directions taken in the specified array. 
*/
void loop() {
  // read the input on analog pin 0:
  // read the input on analog pin 0:
  int leftSensorValueFront = digitalRead(opticalSensorFrontLeft);
  int rightSensorValueFront = digitalRead(opticalSensorFrontRight);
  Serial.print("left Sensor: ");
  Serial.println(leftSensorValueFront);
  Serial.print("right Sensor: ");
  Serial.println(rightSensorValueFront);  
  
  int SideRightSensorValue = digitalRead(SideOpticalSensorRight);
  int SideLeftSensorValue = digitalRead(SideOpticalSensorLeft);

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
  //go straigh
  Serial.println(distance);
//  if(distance <= 20) {
//    stop();
//    pathArray[index] = "Finished";
//    index++; 
//  }
// if(leftSensorValueFront == rightSensorValueFront){
//   
// }
  //go Right
   if(leftSensorValueFront==1 && rightSensorValueFront==1){
    straight();
  }
  if(leftSensorValueFront==0 && rightSensorValueFront==1){
    fixL();
  }
  if(leftSensorValueFront==1 && rightSensorValueFront==0){
    fixR();
  }
  
  if(SideRightSensorValue && !SideLeftSensorValue){
       turnRight();
   } else if(SideLeftSensorValue && !SideRightSensorValue){
       turnLeft();
   }
   
  if(leftSensorValueFront==0 && rightSensorValueFront==0){
    delay(300);
    if(leftSensorValueFront==0 && rightSensorValueFront==0){
    turnAround();
    }
  }
}
