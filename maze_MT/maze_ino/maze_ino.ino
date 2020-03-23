
#include <Arduino.h>
#include <ArduinoUnit.h>
#include <avr/wdt.h>
#include "maze_header.h"
//#include <Battery.h>
/*
 *  "maze_header.h" has all the function
 //Constants
 SEE "maze_header.h
 */



/*
*Authors: Muhammad Tarequzzaman, Sammy Ibrahim, Jacob Martin, Ahmad Chaudhry

 * */
/*Who did what ?*/
/*
 *Author: Sammy Ibrahim,
 *Responsible for the codes:
 *
 *Added
*/

/*
 *Author: Jacob Martin
 *Responsible for the codes:
 *
 *Added
*/

/*
 *Author: Ahmad Chaudhry
 *Responsible for the codes:
 *
 *Added
*/

/*
 *Author: Muhammad Tarequzzaman 100954008
 *Responsible for the codes:  ,
 *
 *Added Test case for Pin set up
 *Added Test script on "test(pinSetup)
 *Added void WDT_Initializing_ResetTime(uint8_t value)
 *Added watchdog timer and interrupt ISR ISR(WDT_vect)
 *Added void left360()
 *Added switch case implementation in loop() from original group code.
 *Added "maze_header.h" for modularity and test enabled functionality
 * */


volatile uint8_t state;
bool isDone = false;

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

ISR(WDT_vect){
	/*Author: Muhammad Tarequzzaman|100954008
	 * WARNING: DO NOT MODIFY
	 * Functionality: watch dog timer ISR
	 * Environment: #include <avr/wdt.h> for macro variable
	 */
/*add blink here*/}

void WDT_Initializing_ResetTime(uint8_t value){
	//Initializing

	/*value input must be one of this,  WDTO_15MS;WDTO_30MS;WDTO_60MS;WDTO_120MS;WDTO_250MS;
	 * WDTO_500MS;WDTO_1S;WDTO_2S;WDTO_4S;WDTO_8S;*/

	/*Author: Muhammad Tarequzzaman|100954008
	 * WARNING: DO NOT MODIFY
	 * Functionality: watch dog timer setup function using WDTCSR and MCUSR
	 * Environment: #include <avr/wdt.h> for macro variable
	 *
	 */

	cli(); // disable interrupt

	/** clear WDRF  and reset WDT**/
	__asm__ __volatile__ ("wdr"); //reset WDT register
	MCUSR &= ~(1<<WDRF); // clear MCUSR for existing WDT
	//MCUSR = (1<<WDRF); // This bit is set if a watchdog system reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.

	/* Start timed equence */
	WDTCSR |= (1<<WDCE) | (1<<WDE); //Interrupt and system reset mode. Interrupt, then go to system reset mode

	/* Set new Pre-scaler(time-out) value  */

	switch (value) {
	case WDTO_15MS:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(0<<WDP2)|(0<<WDP1)|(0<<WDP0); //16ms
		break;
	case WDTO_30MS:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(0<<WDP2)|(0<<WDP1)|(1<<WDP0);	//32ms
		break;
	case WDTO_60MS:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(0<<WDP2)|(1<<WDP1)|(0<<WDP0);	//64ms
		break;
	case WDTO_120MS:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(0<<WDP2)|(1<<WDP1)|(1<<WDP0);	//0.125s
		break;
	case WDTO_250MS:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(1<<WDP2)|(0<<WDP1)|(0<<WDP0);	//0.25s
		break;
	case WDTO_500MS:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(1<<WDP2)|(0<<WDP1)|(1<<WDP0);	//0.5s
		break;
	case WDTO_1S:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(1<<WDP2)|(1<<WDP1)|(0<<WDP0);	//1.0s
		break;
	case WDTO_2S:
		WDTCSR = (1<<WDE)|(0<<WDP3)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);	//2.0s
		break;
	case WDTO_4S:
		WDTCSR = (1<<WDE)|(1<<WDP3)|(0<<WDP2)|(0<<WDP1)|(0<<WDP0);	//4.0s
		break;
	case WDTO_8S:
		WDTCSR = (1<<WDE)|(1<<WDP3)|(0<<WDP2)|(0<<WDP1)|(1<<WDP0);	//8.0s
		break;
	default:
		break;
	}
	sei(); // enable interrupt

}

// the setup routine runs once when you press reset:
void setup() {

	pinSetup();
	// initialize serial communication at 9600 bits per second:
	Serial.begin(Baudrate);

	uint8_t T = 10;
	while (T > 0) {
		T--;
		Test::run();
		//	Test::out= &Serial; //ArduinoUnit.h only

	}; // UNIT TEST
	delay(setupDelay);
	//wdt_enable(WDTO_1S); // dont use this one for final version
	WDT_Initializing_ResetTime(WDTO_2S); // recreated by MUHAMMAD TAREQUZZAMAN

}

// the loop routine runs over and over again forever:
void loop() {

	// read the input on analog pin 0:
	leftSensorValue = digitalRead(FrontOpticalSensorLeft);
	rightSensorValue = digitalRead(FrontOpticalSensorRight);
	SideRightSensorValue = digitalRead(SideOpticalSensorRight);
	SideLeftSensorValue = digitalRead(SideOpticalSensorLeft);

	// Follow Line Logic

	if (sonSen() < 16) {
		turnRightCont(700); //700ms delay time for turning loop
	}
	state = (leftSensorValue) ? ((rightSensorValue) ? 0 : 1) : // leftSensorValue = 1
			((rightSensorValue) ? 2 : 3); // leftSensorValue = 0

	Serial.println(distance);
	Serial.println(state);
	switch (state) {

	case 0: // 1 : 1
		if (SideRightSensorValue) {
			turnRightCont(700); // 700ms delay time for turning loop
		} else if (SideLeftSensorValue) {
			turnLeftCont(700); //700ms delay time time for turning loop
		} else {
			straight();
		} // straight

		break;
	case 1: // 1 : 0
		left(); // Left

		break;
	case 2: // 0 : 1
		right(); // Right

		break;

	case 3: // 0 : 0

		stop(); //Stop
		left360(); // left turn
		break;

	default:
		break;
	}

	if (isDone) {
		printArray();
	}

	//wdt_reset(); // reboot
	__asm__ __volatile__ ("wdr");//
}

test(pinSetup) {
	/* Author: Muhammad Tarequzzaman|100954008
	 *
	 * Functionality: unit Test setup for the pin initialization
	 * Environment: #include <Arduino.h> <AUnit.h> <ArduinoUnit.h>
	 * */

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

	assertEqual(SideOpticalSensorRight, A5);
	assertEqual(readPinMode(SideOpticalSensorRight), 0x0);

	assertEqual(SideOpticalSensorLeft, A6);
	assertEqual(readPinMode(SideOpticalSensorLeft), 0x0);

	assertEqual(trigPin, 3);
	assertEqual(readPinMode(trigPin), 0x1);

	assertEqual(echoPin, 4);
	assertEqual(readPinMode(echoPin), 0x0);
}

