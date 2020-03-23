/*
 /*
 * maze_v2.cpp
 *
 *  Created on: Feb 24, 2020
 *     Authors: Sammy Ibrahim, Jacob Martin, Ahmad Chaudhry, muhammadtarequzzaman
 */

#include <Arduino.h>

//Constants
#define Baudrate 9600
#define setupDelay 3000

#define  motorA1 5
#define  motorA2 6

#define  motorB1 11
#define  motorB2 10

#define FrontOpticalSensorRight  8
#define  FrontOpticalSensorLeft 7

#define  SideOpticalSensorRight A0
#define  SideOpticalSensorLeft A1

#define  trigPin  3
#define  echoPin  4

volatile unsigned long duration;
volatile double distance;

volatile uint8_t leftSensorValue;
volatile uint8_t rightSensorValue;
volatile uint8_t SideRightSensorValue;
volatile uint8_t SideLeftSensorValue;

bool isDone = false;

String pathArray [40];
volatile uint8_t index = 0;


void pinSetup();

double sonSen() {
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	// Sets the trigPin on HIGH state for 10 micro seconds
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	// Reads the echoPin, returns the sound wave travel time in microseconds
	duration = pulseIn(echoPin, HIGH);
	// Calculating the distance
	distance = duration * 0.034 / 2;
	return distance;
}

void stop() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);
}

//logic to turn left
void left() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	analogWrite(motorB2, 150);
	  pathArray[index] = "Left";
	    index++;
}
//logic to turn right
void right() {
	digitalWrite(motorA1, LOW);
	analogWrite(motorA2, 150);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);
	pathArray[index] = "Right";
	    index++;
}

//logic to turn go straight
void straight() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, HIGH);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, HIGH);
}


//logic to turn right till it reenters maze
void turnRightCont(int leftSensorValue, int rightSensorValue) {
	right();
	delay(1000);
	do{ /*EXIT only when both 1  */ Serial.println(" TURN RIGHT");
		}while(!(digitalRead(FrontOpticalSensorLeft) & digitalRead(FrontOpticalSensorRight) ));

	pathArray[index] = "Right";
	        index++;

}


//logic to left right till it reenters maze
void turnLeftCont(int leftSensorValue, int rightSensorValue) {

	left();
	delay(1000);
	do{ /*EXIT only when both 1  */ Serial.println(" TURN LEFT");
	}while(!(digitalRead(FrontOpticalSensorLeft) & digitalRead(FrontOpticalSensorRight) ));

	 pathArray[index] = "Left";
	        index++;

}

//prints stored map
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
  Serial.println(path);
}

uint8_t readPinMode(uint8_t pin) {
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg = portModeRegister(port);
	return (*reg & bit)? 0x1 : 0x0;
}
