const int motorA1      = 5;  
const int motorA2      = 6; 

const int motorB1      = 11; 
const int motorB2      = 10;

const int opticalSensorRight   = 8;
const int opticalSensorLeft   = 7;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  
  pinMode(opticalSensorRight, INPUT);
  pinMode(opticalSensorLeft, INPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delay(3000);
}


// the loop routine runs over and over again forever:
void loop() {

  
  // read the input on analog pin 0:
  int leftSensorValue = digitalRead(opticalSensorLeft);
  int rightSensorValue = digitalRead(opticalSensorRight);
  Serial.print("left Sensor: ");
  Serial.println(leftSensorValue);
  Serial.print("right Sensor: ");
  Serial.println(rightSensorValue);
  
  // Follow Line Logic 
  if(leftSensorValue==1 && rightSensorValue==1){
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,HIGH);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,HIGH);
  }
  if(leftSensorValue==0 && rightSensorValue==1){
    digitalWrite (motorA1,LOW);
    analogWrite (motorA2,150);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
  }
  if(leftSensorValue==1 && rightSensorValue==0){
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    analogWrite (motorB2,150);
  }
  if(leftSensorValue==0 && rightSensorValue==0){
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,LOW);
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,LOW);
  }
}
