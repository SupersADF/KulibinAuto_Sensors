#include <KulibinAuto_Sensors.h>

KulibinAuto_Sensors::KulibinAuto_Sensors(uint8_t trig, uint8_t echo, uint8_t servo, uint8_t reflect_1, uint8_t reflect_2, uint8_t reflect_3) {
	TRIG_PIN = trig;
	ECHO_PIN = echo;
	SERVO_PIN = servo;
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
	// myServo.attach(SERVO_PIN);
}
KulibinAuto_Sensors::KulibinAuto_Sensors() {
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
}

float KulibinAuto_Sensors::getUltraDistance(uint8_t dimension) {
	if (dimension == ULTRA_CM) {
		return getTiming() / 58.0;
	} else if (dimension == ULTRA_MM) {
		return getTiming() / 5.8;
	} else if (dimension == ULTRA_M) {
		return getTiming() / 5800.0;
	}
}
void KulibinAuto_Sensors::setServoAngle(int angle) {
	myServo.attach(SERVO_PIN);
	myServo.write(angle);
}
uint8_t KulibinAuto_Sensors::getReflect(uint8_t num) {
	if (num == 1) {
		return !digitalRead(REFLECT_1_PIN);
	} else if (num == 2) {
		return !digitalRead(REFLECT_2_PIN);
	} else if (num == 3) {
		return !digitalRead(REFLECT_3_PIN);
	}
}
void KulibinAuto_Sensors::setUpColorSensor(uint8_t IntegrationTime = TCS34725_INTEGRATIONTIME_2_4MS, uint8_t Gain_t = TCS34725_GAIN_1X) {
	color_sensor = Adafruit_TCS34725(IntegrationTime, Gain_t);
	// color_sensor = color_sensor_t;
	if (color_sensor.begin()) {
    	Serial.println("Found sensor");
  	}
}
void KulibinAuto_Sensors::getColorRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
	uint16_t r_, g_, b_, c_, colorTemp, lux;
	color_sensor.getRawData(&r_, &g_, &b_, &c_);
	*r = r_;
	*g = g_;
	*b = b_;
	*c = c_;
}
void KulibinAuto_Sensors::getColorRGB(float *r, float *g, float *b) {
	float r_, g_, b_;
	color_sensor.getRGB(&r_, &g_, &b_);
	*r = r_;
	*g = g_;
	*b = b_;
}
uint16_t KulibinAuto_Sensors::calculateColorLux(uint16_t r, uint16_t g, uint16_t b) {
	return color_sensor.calculateLux(r, g, b);
}
void KulibinAuto_Sensors::setColorGain(tcs34725Gain_t gain) {
	color_sensor.setGain(gain);
}
void KulibinAuto_Sensors::setColorIntegrationTime(tcs34725IntegrationTime_t it) {
	color_sensor.setIntegrationTime(it);
}

//private 

uint32_t KulibinAuto_Sensors::getTiming() {
	digitalWrite(TRIG_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);
	uint32_t duration = pulseIn(ECHO_PIN, HIGH);
	return duration;
}