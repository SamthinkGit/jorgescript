#include "WeighedDetection.hpp"
#include "timer.hpp"
#include "PD.hpp"
#include "SimpleRT.hpp"


WeighedDetector detector;
float input;

float Kp = 2;
float Kd = 8;

float LINEAR_KP = Kp;  
float AGGRESIVE_KP = 2.5;
float LOST_KP = 5;
float CHANGE_KP_AT=0.3;
float obstacle_distance = 0.0;
float STOP_DISTANCE = 10;

PD pd = PD(Kp, Kd);

void setup() {

  Serial.begin(9600);
  setupWheels();
  setupLed();
  setupSound(&obstacle_distance);

  detector = WeighedDetector();
  
  SimpleRT::newTask("aNavigation", aNavigation, 1);
  SimpleRT::newTask("ObstacleDetector", aObstacleDetector, 2);
  SimpleRT::newTask("LED", debugLED, 5);
  SimpleRT::start();
}

// Maximum Delay: 3ms
void aNavigation(void *args) {
  NonBlockingTimer timer;
  SimpleRT rt = SimpleRT(10);
  bool stopped = false;
  rt.await(1000);
  timer.start();

  while (true) {
    rt.awaitNextIteration();

    if (stopped || obstacle_distance < STOP_DISTANCE || timer.getElapsedTime() > 13) {
      setMotorSpeeds(0, 0);
      stopped = true;
      continue;
    }

    detector.read();
    detector.computeProbs();
    detector.applySigmoid();
    detector.computeSlope();

    if (abs(detector.slope) > CHANGE_KP_AT) {
      pd.setKp(AGGRESIVE_KP);
    } else {
      pd.setKp(LINEAR_KP);
    }
    if (detector.lost()) {
      pd.setKp(LOST_KP);
    }

    // detector.log();
    input = pd.next(detector.slope);
    setMotorSpeedsFromSlope(input);
  }
}

// Maximum Delay: 1ms
void debugLED(void *args) {
  SimpleRT rt = SimpleRT(20);
  bool latest = false;
  bool new_state;
  showColor(0, 255, 0);

  while (true) {
    rt.awaitNextIteration();

    new_state = detector.lost();
    if (new_state == latest) {
      continue;
    }
    latest = new_state;

    if (detector.lost()) {
      showColor(255, 0, 0);
    } else {
      showColor(0, 255, 0);
    }
  }
}

// Maximum Delay: 4ms
void aObstacleDetector(void *args) {
    SimpleRT rt = SimpleRT(20);
    while (true) {
      updateDistance();
      rt.awaitNextIteration();
    }
}

void loop() {

}