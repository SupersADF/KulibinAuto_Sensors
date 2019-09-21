#include <KulibinAuto_Platform.h>

KulibinAuto_Platform *pointerToClass_encLeft;

static void outsideInterruptHandler_encLeft(void) { 
 	pointerToClass_encLeft->__callBackInterrupt0(); 
}

KulibinAuto_Platform *pointerToClass_encRight;

static void outsideInterruptHandler_encRight(void) { 
 	pointerToClass_encRight->__callBackInterrupt1(); 
}

KulibinAuto_Platform::KulibinAuto_Platform(uint8_t enA, uint8_t enB, uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t enc1, uint8_t enc2) {
	enA_PIN = enA;
	enB_PIN = enB;
	in1_PIN = in1;
	in2_PIN = in2;
	in3_PIN = in3;
	in4_PIN = in4;
	enc1_PIN = enc1;
	enc2_PIN = enc2;

	pinMode(enA_PIN, OUTPUT);
	pinMode(enB_PIN, OUTPUT);
	pinMode(in1_PIN, OUTPUT);
	pinMode(in2_PIN, OUTPUT);
	pinMode(in3_PIN, OUTPUT);
	pinMode(in4_PIN, OUTPUT);
}

KulibinAuto_Platform::KulibinAuto_Platform() {
	pinMode(enA_PIN, OUTPUT);
	pinMode(enB_PIN, OUTPUT);
	pinMode(in1_PIN, OUTPUT);
	pinMode(in2_PIN, OUTPUT);
	pinMode(in3_PIN, OUTPUT);
	pinMode(in4_PIN, OUTPUT);

	pinMode(enc1_PIN, INPUT);
	pinMode(enc2_PIN, INPUT);

	pointerToClass_encLeft = this;
	pointerToClass_encRight = this;
  	attachInterrupt(digitalPinToInterrupt(enc1_PIN), outsideInterruptHandler_encLeft, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(enc2_PIN), outsideInterruptHandler_encRight, CHANGE);
}

int32_t KulibinAuto_Platform::getDegree(uint8_t motor/*, uint8_t compare = 0, int32_t compare_value = 0*/) {
	// if (compare != 0) {
	// 	if (motor == MOTOR_LEFT) {

	// 	} else if (motor == MOTOR_RIGHT) {

	// 	}
	// } else {
		if (motor == MOTOR_LEFT) {
			return encLeft;
		} else if (motor == MOTOR_RIGHT) {
			return encRight;
		}	
	// }
}
void KulibinAuto_Platform::resetDegree(uint8_t motor = 3) {
	if (motor == 3) {
		encLeft = 0;
		encRight = 0;
	} else if (motor == MOTOR_LEFT) {
		encLeft = 0;
	} else if (motor == MOTOR_RIGHT) {
		encRight = 0;
	}
}

void KulibinAuto_Platform::moveTank(uint8_t mode, int8_t powerLeft = 0, int8_t powerRight = 0, int16_t mode_value = 0, uint8_t stop = false) {
	switch (mode) {
		case MOVE_ON:
			__moveLeftMotor(powerLeft);
			__moveLeftMotor(powerRight);
			break;
		case MOVE_OFF:
			__moveLeftMotor(powerLeft);
			__moveLeftMotor(powerRight);
			break;
		case MOVE_SECONDS:
			uint32_t time_start = millis();
			__moveLeftMotor(powerLeft);
			__moveLeftMotor(powerRight);
			while(millis() - time_start < mode_value) {}
			if (stop == true) {
				__moveLeftMotor(0);
				__moveLeftMotor(0);
			}
			break;
		case MOVE_DEGREES:
			uint32_t degree_average = (getDegree(MOTOR_LEFT) + getDegree(MOTOR_RIGHT)) / 2;
			__moveLeftMotor(powerLeft);
			__moveLeftMotor(powerRight);
			while((getDegree(MOTOR_LEFT) + getDegree(MOTOR_RIGHT)) / 2 - degree_average < mode_value) {}
			if (stop == true) {
				__moveLeftMotor(0);
				__moveLeftMotor(0);
			}
			break;
		case MOVE_ROTATION:
			
			break;
	}
}

//private 

void KulibinAuto_Platform::__callBackInterrupt0(void) {
	encLeft++;
}

void KulibinAuto_Platform::__callBackInterrupt1(void) {
	encRight++;
}

void KulibinAuto_Platform::__moveLeftMotor(int8_t power) {
	if (power < 0) {
		analogWrite(enA_PIN)
		digitalWrite(in1_PIN, HIGH);
		digitalWrite(in2_PIN, HIGH);
	}
}