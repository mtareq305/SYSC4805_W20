/*// Remove this to enable it
#include <Arduino.h>
#include <ArduinoUnit.h>
#include <AUnit.h>
#include "maze_v2.h"
void setup(){
	Serial.begin(Baudrate);
}

void loop(){
	Test::run();
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



/**/
