/*
 /*
 * maze_v2.cpp
 *
 *  Created on: Feb 24, 2020
 *      Author: muhammadtarequzzaman
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

// the setup routine runs once when you press reset:
/*
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
 Serial.begin(Baudrate);

 delay(setupDelay);
 }*/

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
void stop();
void stop() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);
}
;
void left();
void left() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	analogWrite(motorB2, 150);
}
void right();
void right() {
	digitalWrite(motorA1, LOW);
	analogWrite(motorA2, 150);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);
}
void straight();

void straight() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, HIGH);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, HIGH);
}

void turnRight(int leftSensorValue, int rightSensorValue);

void turnRight(int leftSensorValue, int rightSensorValue) {
	digitalWrite(motorA1, LOW);
	analogWrite(motorA2, 150);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);
	delay(1000);
	while (true) {
		Serial.println("STUCK TURN RIFGHT");
		leftSensorValue = digitalRead(FrontOpticalSensorLeft);
		rightSensorValue = digitalRead(FrontOpticalSensorRight);
		if (leftSensorValue == 1 && rightSensorValue == 1) {
			break;
			return;
		}
	}
}

void turnLeft(int leftSensorValue, int rightSensorValue);

void turnLeft(int leftSensorValue, int rightSensorValue) {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	analogWrite(motorB2, 150);
	delay(1000);
	while (true) {
		Serial.println("STUCK TURN LEFT");
		leftSensorValue = digitalRead(FrontOpticalSensorLeft);
		rightSensorValue = digitalRead(FrontOpticalSensorRight);
		if (leftSensorValue == 1 && rightSensorValue == 1) {
			break;
			return;
		}
	}
}
