#include <KulibinAuto_Sensors.h>

KulibinAuto_Sensors::KulibinAuto_Sensors(uint8_t trig, uint8_t echo, uint8_t servo, uint8_t reflect_1, uint8_t reflect_2, uint8_t reflect_3) {
	TRIG_PIN = trig;
	ECHO_PIN = echo;
	SERVO_PIN = servo;
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
}
KulibinAuto_Sensors::KulibinAuto_Sensors() {
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
	bluetooth_serial.begin(9600);
}
float KulibinAuto_Sensors::getUltraDistance(uint8_t dimension) {
	if (dimension == ULTRA_CM) {
		float dist = getTiming() / 58.0;
		if (dist == 0) 
			dist = 160;
		return dist;
	} else if (dimension == ULTRA_MM) {
		float dist = getTiming() / 5.8;
		if (dist == 0) 
			dist = 1600;
		return dist;
	} else if (dimension == ULTRA_M) {
		float dist = getTiming() / 5800;
		if (dist == 0) 
			dist = 0.16;
		return dist;
	} else return 0;
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
	} else return 0;
}
void KulibinAuto_Sensors::setUpColorSensor(uint8_t IntegrationTime = TCS34725_INTEGRATIONTIME_2_4MS, uint8_t Gain_t = TCS34725_GAIN_1X) {
	color_sensor = Adafruit_TCS34725((tcs34725IntegrationTime_t)IntegrationTime, (tcs34725Gain_t)Gain_t);
	// color_sensor = color_sensor_t;
	if (color_sensor.begin()) {
    	Serial.println("Found sensor");
  	}
}
void KulibinAuto_Sensors::getColorRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
	uint16_t r_, g_, b_, c_;//, colorTemp, lux;
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
void KulibinAuto_Sensors::getBluetoothMessage(uint8_t *return_array, uint8_t print_serial = 0) {
	if (bluetooth_serial.available() > 0) {
		uint8_t start_data = bluetooth_serial.read(); 
		if (start_data == 243 || start_data == 241 || start_data == 242) { 
			uint8_t data[3];
			for (int i = 0; i < 3; i++) {
				delay(1);
				data[i] = bluetooth_serial.read();
			}
			if (data[2] != 243 || data[2] != 242 || data[2] != 241) {
        		command.F_ = start_data; //direction
        		command.speed = data[0]; //speed
        		command.turn = data[1];  //turn
        		command.command_machine = data[2]; //lights and buttons
				if (print_serial != 0) {
					Serial.println(String("dir - ") + command.F_ + String("  speed - ") + command.speed + 
								String("  turn - ") + command.turn + String("  commands - ") + command.command_machine);
				}
				return_array[0] = command.F_;
				return_array[1] = command.speed;
				return_array[2] = command.turn;
				return_array[3] = command.command_machine;
      		}
		}		
	}
}


//private 

uint32_t KulibinAuto_Sensors::getTiming() {
	digitalWrite(TRIG_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);
	uint32_t duration = pulseIn(ECHO_PIN, HIGH, 13000);
	return duration;
}