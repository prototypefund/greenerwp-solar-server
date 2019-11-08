const int fetPowerPin = 2;
const float refVoltageFactor = 5.56513;

unsigned int shutdownCountdown;
unsigned int wakeupCountdown;
bool doShutdown;
bool doWakeup;

void setup() {
	doShutdown = false;
	doWakeup = true;
	shutdownCountdown = 0;
	wakeupCountdown = 0;
	Serial.begin(9600);
	Serial1.begin(9600);
	pinMode(fetPowerPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
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
	if (doWakeup && wakeupCountdown == 0) {
		digitalWrite(fetPowerPin, HIGH);
		Serial.println("high");
		doWakeup = false;
	}
	if (doShutdown && shutdownCountdown == 0) {
		digitalWrite(fetPowerPin, LOW);
		Serial.println("low");
		doShutdown = false;
	}
	int sensorValue = analogRead(A7);
	float voltage = sensorValue * (5.0 / 1023.0);
	float batteryVoltage =  voltage * refVoltageFactor;
	Serial1.println(batteryVoltage);

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
	delay( 1000 );
}
