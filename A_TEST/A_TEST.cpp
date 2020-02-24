#include <Arduino.h>
#include <AUnit.h>
/*
 *Author: Muhammad TareqUzzaman
 *
 *Added Test case for Pin set up
 *Added "maze_v2.h" for modularity and test
 * */

#include "maze_v2.h"

/*
 *  "maze_v2.h" has all this
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
 */

/*
 volatile unsigned long duration;
 volatile double distance;

 volatile uint8_t leftSensorValue = 0;
 volatile uint8_t rightSensorValue = 0;
 volatile uint8_t SideRightSensorValue = 0;
 volatile uint8_t SideLeftSensorValue = 0;
 */

void pinSetup();
uint8_t readPinMode(uint8_t pin);

// the setup routine runs once when you press reset:
void setup() {

	pinSetup();
	// initialize serial communication at 9600 bits per second:
	Serial.begin(Baudrate);

	uint8_t T = 10;
	while (T > 0) {
		T = T - 1;
		aunit::TestRunner::run();
	}; // UNIT TEST

	delay(setupDelay);
}

// the loop routine runs over and over again forever:
void loop() {

	// read the input on analog pin 0:
	leftSensorValue = digitalRead(FrontOpticalSensorLeft);
	rightSensorValue = digitalRead(FrontOpticalSensorRight);
	SideRightSensorValue = digitalRead(SideOpticalSensorRight);
	SideLeftSensorValue = digitalRead(SideOpticalSensorLeft);
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
	distance = duration * 0.034 / 2;

	// Follow Line Logic
	//go straigh
	Serial.println(distance);

	if (distance <= 15) {
		turnRight(leftSensorValue, rightSensorValue);
	}
	if (leftSensorValue == 1 && rightSensorValue == 1) {
		if (SideRightSensorValue) {
			turnRight(leftSensorValue, rightSensorValue);
		} else if (SideLeftSensorValue) {
			turnLeft(leftSensorValue, rightSensorValue);
		} else {
			straight();
		}
	}
	//go Right
	if (leftSensorValue == 0 && rightSensorValue == 1) {
		right();
	}
	//go left
	if (leftSensorValue == 1 && rightSensorValue == 0) {
		left();
	}
	//stop
	if (leftSensorValue == 0 && rightSensorValue == 0) {
		stop();
	}

}

void pinSetup() {
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

}
test(pinSetup) {

	assertEqual(motorA1, 5);
	assertEqual(readPinMode(motorA1), 0x1);
	assertEqual(motorA2, 6);
	assertEqual(readPinMode(motorA1), 0x1);
	assertEqual(motorB1, 11);
	assertEqual(readPinMode(motorB2), 0x1);
	assertEqual(motorB2, 10);
	assertEqual(readPinMode(motorA1), 0x1);

	assertEqual(FrontOpticalSensorRight, 8);
	assertEqual(readPinMode(FrontOpticalSensorRight), 0x0);

	assertEqual(FrontOpticalSensorLeft, 7);
	assertEqual(readPinMode(FrontOpticalSensorLeft), 0x0);

	assertEqual(SideOpticalSensorRight, A0);
	assertEqual(readPinMode(SideOpticalSensorRight), 0x0);

	assertEqual(SideOpticalSensorLeft, A1);
	assertEqual(readPinMode(SideOpticalSensorLeft), 0x0);

	assertEqual(trigPin, 3);
	assertEqual(readPinMode(trigPin), 0x1);

	assertEqual(echoPin, 4);
	assertEqual(readPinMode(echoPin), 0x0);
}

uint8_t readPinMode(uint8_t pin) {
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg = portModeRegister(port);

	if (*reg & bit) {
		// It's an output
		return 0x1;
	} else {
		// It's an input
		return 0x0;
	}
}
