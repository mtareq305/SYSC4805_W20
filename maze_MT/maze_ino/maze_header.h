/*
 /*
 * For maze_ino
 *
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

#define  SideOpticalSensorRight A5
#define  SideOpticalSensorLeft A6

#define  trigPin  3
#define  echoPin  4



volatile uint8_t leftSensorValue;
volatile uint8_t rightSensorValue;
volatile uint8_t SideRightSensorValue;
volatile uint8_t SideLeftSensorValue;



String pathArray[40];
volatile uint8_t index = 0;
/***************************************************************************/
double sonSen();
void pinSetup();
uint8_t readPinMode(uint8_t pin);
void WDT_Initializing_ResetTime(uint8_t value);
/****************************************************************************/



void stop() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	analogWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);
	analogWrite(motorB2, LOW);

}
;

void left() {
	stop();
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	analogWrite(motorB2, 150);

}

void left360() {
	/* Author: Muhammad Tarequzzaman|100954008
	 *
	 * Functionality: left turn 360 angle uninterrupted
	 * Environment: Arduino Library
	 * */

	digitalWrite(motorA1, 150);
	digitalWrite(motorA2, LOW);
	digitalWrite(motorB1, LOW);
	analogWrite(motorB2, 150);

}

void right() {
	stop();
	digitalWrite(motorA1, LOW);
	analogWrite(motorA2, 150);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, LOW);

}

void straight() {
	digitalWrite(motorA1, LOW);
	digitalWrite(motorA2, HIGH);
	digitalWrite(motorB1, LOW);
	digitalWrite(motorB2, HIGH);
}

void turnRightCont(uint8_t delayTime) {

	right();
	delay(delayTime);
	do { /*EXIT only when both 1  */
		Serial.println(" TURN RIGHT");
	} while (!(digitalRead(FrontOpticalSensorLeft)
			&& digitalRead(FrontOpticalSensorRight)));

}

void turnLeftCont(uint8_t delayTime) {

	left();
	delay(delayTime);
	do { /*EXIT only when both 1  */
		Serial.println(" TURN LEFT");
	} while (!(digitalRead(FrontOpticalSensorLeft)
			&& digitalRead(FrontOpticalSensorRight)));

}

void printArray() {
	String path = "";
	for (int i = 0; i <= index; i++) {
		if (pathArray[i] == "Left") {
			path.concat("<-,");
		}
		if (pathArray[i] == "Right") {
			path.concat("->,");
		}
		if (pathArray[i] == "Straight") {
			path.concat(" |^ ");
		}
		if (pathArray[i] == "TurnAround") {
			path.concat(" W ");
		}
	}
	Serial.println(path);
}

uint8_t readPinMode(uint8_t pin) {
	/* Author: Muhammad Tarequzzaman|100954008
	 *
	 * Functionality: check the pin setup status for any given pin
	 * Environment: Arduino Library
	 * */
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg = portModeRegister(port);
	return (*reg & bit) ? 0x1 : 0x0;
}
