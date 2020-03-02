//Constants
const int motorA1      = 5;  
const int motorA2      = 6; 

const int motorB1      = 11; 
const int motorB2      = 10;

const int FrontOpticalSensorRight   = 8;
const int FrontOpticalSensorLeft   = 7;

const int SideOpticalSensorRight   = A0;
const int SideOpticalSensorLeft   = A1;

const int trigPin = 3;
const int echoPin = 4;

long duration;
double distance;

bool isDone = false;

String pathArray [40];
int index = 0; 

// the setup routine runs once when you press reset:
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

void stop() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
};

void left() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,150);
    pathArray[index] = "Left";
    index++; 
}

void right() {
    digitalWrite (motorA1,LOW);
    analogWrite (motorA2,150);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
    pathArray[index] = "Right";
    index++; 
}

void straight() {
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,HIGH);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,HIGH);
    pathArray[index] = "Straight";
    index++; 
}

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


// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int leftSensorValue = digitalRead(FrontOpticalSensorLeft);
  int rightSensorValue = digitalRead(FrontOpticalSensorRight);
  int SideRightSensorValue = digitalRead(SideOpticalSensorRight);
  int SideLeftSensorValue = digitalRead(SideOpticalSensorLeft);
//  Serial.print("left Sensor: ");
//  Serial.println(leftSensorValue);
//  Serial.print("right Sensor: ");
//  Serial.println(rightSensorValue);

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
