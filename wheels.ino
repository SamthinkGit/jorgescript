//Arduino

// Enable/Disable motor control.
//  HIGH: motor control enabled
//  LOW: motor control disabled
#define PIN_Motor_STBY 3

// Group A Motors (Right Side)
// PIN_Motor_AIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_AIN_1 7
// PIN_Motor_PWMA: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMA 5

// Group B Motors (Left Side)
// PIN_Motor_BIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_BIN_1 8
// PIN_Motor_PWMB: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMB 6

// Yup, im gonna try full speed xD
int BASE_SPEED = 255;
int MIN_SPEED = -255;

void setupWheels() {
    pinMode(PIN_Motor_STBY, OUTPUT);
    pinMode(PIN_Motor_AIN_1, OUTPUT);
    pinMode(PIN_Motor_PWMA, OUTPUT);
    pinMode(PIN_Motor_BIN_1, OUTPUT);
    pinMode(PIN_Motor_PWMB, OUTPUT);

    digitalWrite(PIN_Motor_STBY, HIGH);
}

void setBaseSpeed(int vel) {
  BASE_SPEED = vel;
}
void minSpeed(int speed) {
  MIN_SPEED = speed;
}

void setMotorSpeeds(int speedLeft, int speedRight) {
    digitalWrite(PIN_Motor_AIN_1, speedRight >= 0 ? HIGH : LOW);
    analogWrite(PIN_Motor_PWMA, abs(speedRight));
    digitalWrite(PIN_Motor_BIN_1, speedLeft >= 0 ? HIGH : LOW);
    analogWrite(PIN_Motor_PWMB, abs(speedLeft));
}

void setMotorSpeedsFromSlope(float slope) {
    slope = constrain(slope, -1.0, 1.0);
    slope *= 2;
    
    int speed_left = BASE_SPEED - (slope * BASE_SPEED);
    int speed_right = BASE_SPEED + (slope * BASE_SPEED);

    speed_left = constrain(speed_left, MIN_SPEED, 255);
    speed_right = constrain(speed_right, MIN_SPEED, 255);

    setMotorSpeeds(speed_left, speed_right);
}