/*
 * Reads battery voltage and controls (power on/off) an attached computer.
 * Communicates over UART with the computer (sends battery voltage, receives
 * shutdown commands)
 *
 * Compatible with Teensy 2.0
 */

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "Arduino.h"

// Pin where the MOSFET powering the attached computer is attached.
const int fetPowerPin = 0;

// Pin where to read the battery voltage.
const int voltageReadPin = A0;

// The read battery voltage gets multiplicated by this factor.
const float refVoltageFactor = 6.15897;

// If set to true, power down the attached computer when shutdownCountdown
// reaches zero.
bool doShutdown;
unsigned int shutdownCountdown;

// If set to true, power up the attached computer when wakeupCountdown
// reaches zero.
bool doWakeup;
unsigned int wakeupCountdown;

// Puts the board to sleep saving energy. Will wake up by itself.
void idle() {
	set_sleep_mode(SLEEP_MODE_IDLE);
	noInterrupts();
	sleep_enable();
	interrupts();
	sleep_cpu();
	sleep_disable();
}


// Setups the board on power up or reset.
void setup() {
	doShutdown = false;
	doWakeup = true;
	shutdownCountdown = 0;
	wakeupCountdown = 0;
	// Save some more power by disabling INPUT mode.
	for ( int i = 1; i <= 23; i++ ) {
		pinMode(i, OUTPUT);
	}
	// Serial.begin(9600);
	Serial.end();
	Serial1.begin(9600);
	pinMode(voltageReadPin, INPUT);
	pinMode(fetPowerPin, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
}

// Keep track of elapsed time during idling.
elapsedMillis usec;

// The main loop.
void loop() {
	// Read commands from the computer over UART
	if (Serial1.available()) {
		int wakeupMinutes = Serial1.parseInt();
		if (wakeupMinutes > 0) {
			wakeupCountdown = wakeupMinutes * 60;
			doShutdown = true;
			doWakeup = true;
			shutdownCountdown = 10;
			Serial.print("wakeup received: ");
			Serial.println(wakeupMinutes);
		}
	}

	// Wake up computer
	if (doWakeup && wakeupCountdown == 0) {
		digitalWrite(fetPowerPin, HIGH);
		Serial.println("high");
		doWakeup = false;
	}

	// Shut down computer
	if (doShutdown && shutdownCountdown == 0) {
		digitalWrite(fetPowerPin, LOW);
		Serial.println("low");
		doShutdown = false;
	}

	// Read battery voltage and send to computer over UART
	int sensorValue = analogRead(voltageReadPin);
	float voltage = sensorValue * (5.0 / 1023.0);
	float batteryVoltage =  voltage * refVoltageFactor;
	if (Serial) {
		Serial.println(batteryVoltage);
	}
	Serial1.println(batteryVoltage);

	// Updated shut down and wake up counters.
	if (wakeupCountdown > 0) {
		wakeupCountdown--;
		Serial.print("wakeup in ");
		Serial.println(wakeupCountdown);
	}
	if (shutdownCountdown > 0) {
		shutdownCountdown--;
		Serial.print("shutdown in ");
		Serial.println(shutdownCountdown);
	}

	// Blink the board's LED to show we are still alive
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay( 1 );
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

	// Sleep until a second is elapsed.
	while (true) {
		// digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
		// delay( 1 );
		// digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
		if (usec <= 900) {
			idle();
		}
		if (usec >= 1000) {
			usec = usec - 1000;
			break;
		}
	}
}
