#include <Arduino.h>
#include <Servo.h>
#include <tcs34725.h>

#define ULTRA_CM 		1
#define ULTRA_MM		2
#define ULTRA_M			3

class KulibinAuto_Sensors {

Servo myServo;
Adafruit_TCS34725 color_sensor;

public:
	KulibinAuto_Sensors(uint8_t trig, uint8_t echo, uint8_t servo, uint8_t reflect_1, uint8_t reflect_2, uint8_t reflect_3);
	KulibinAuto_Sensors();

	void setUpColorSensor(uint8_t IntegrationTime = TCS34725_INTEGRATIONTIME_2_4MS, uint8_t Gain_t = TCS34725_GAIN_1X);
	void getColorRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
	void getColorRGB(float *r, float *g, float *b);
	uint16_t calculateColorLux(uint16_t r, uint16_t g, uint16_t b);
	void setColorGain(tcs34725Gain_t gain);
	void setColorIntegrationTime(tcs34725IntegrationTime_t it);

	float getUltraDistance(uint8_t dimension);
	void setServoAngle(int angle);

	uint8_t getReflect(uint8_t num);

private:
	uint32_t getTiming();

private:
	uint8_t TRIG_PIN = 13;
	uint8_t ECHO_PIN = 9;
	uint8_t SERVO_PIN = 5;

	uint8_t REFLECT_1_PIN = A0;
	uint8_t REFLECT_2_PIN = A1;
	uint8_t REFLECT_3_PIN = A2;
};