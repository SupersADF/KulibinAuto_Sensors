#include <KulibinAuto_Platform.h>
#include <KulibinAuto_Sensors.h>

KulibinAuto_Platform robot;
KulibinAuto_Sensors sensors;

uint8_t control_array[4];

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensors.getBluetoothMessage(control_array, 0);
  if (control_array[0] == 243 and control_array[2] == 50) { //stop power and turn is neutral 
    robot.moveTank(MOVE_OFF);
  } else if (control_array[0] == 241) { //forward
    float turn_ = (50 - control_array[2]) * 2;
    if (turn_ > 0) {
      if (turn_ > 96) turn_ = 96;
      int pow_right = (100 - abs(turn_ / (1 + control_array[1] / 100.0))) / 100.0 * control_array[1]; 
      int pow_left = control_array[1];
      robot.moveTank(MOVE_ON, pow_left, pow_right);
      // Serial.println(String("left = ") + pow_left + String("  right = ") + pow_right);
    } else if (turn_ < 0) {
      if (turn_ < -96) turn_ = -96;
      int pow_right = control_array[1]; 
      int pow_left = (100 - abs(turn_ / (1 + control_array[1] / 100.0))) / 100.0 * control_array[1]; 
      robot.moveTank(MOVE_ON, pow_left, pow_right);
      // Serial.println(String("left = ") + pow_left + String("  right = ") + pow_right);
    }  else if (turn_ == 0) {
      robot.moveTank(MOVE_ON, control_array[1], control_array[1]);
    }
  } else if (control_array[0] == 242) { //backward
    float turn_ = (50 - control_array[2]) * 2;
    if (turn_ > 0) {
      if (turn_ > 96) turn_ = 96;
      int pow_right = (100 - abs(turn_ / (1 + control_array[1] / 100.0))) / 100.0 * control_array[1]; 
      int pow_left = control_array[1];
      robot.moveTank(MOVE_ON, -pow_left, -pow_right);
      // Serial.println(String("left = ") + pow_left + String("  right = ") + pow_right);
    } else if (turn_ < 0) {
      if (turn_ < -96) turn_ = -96;
      int pow_right = control_array[1]; 
      int pow_left = (100 - abs(turn_ / (1 + control_array[1] / 100.0))) / 100.0 * control_array[1]; 
      robot.moveTank(MOVE_ON, -pow_left, -pow_right);
      // Serial.println(String("left = ") + pow_left + String("  right = ") + pow_right);
    }  else if (turn_ == 0) {
      robot.moveTank(MOVE_ON, -control_array[1], -control_array[1]);
    }
  } else if (control_array[0] == 243 and control_array[2] != 50) { //only turn!
    float turn_ = 50 - control_array[2];
    robot.moveTank(MOVE_ON, turn_ * -1, turn_);
  } else {
    robot.moveTank(MOVE_OFF);
  }
}
