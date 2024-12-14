#include "SimpleRT.hpp"

void setup() {


  Serial.begin(9600);
  Serial.println("----- STARTING -----");

  SimpleRT::newTask("aLoop", aLoop, 1);
  SimpleRT::start();

}

void aLoop(void *params) {
  SimpleRT rt = SimpleRT(200);

  while(true) {
    rt.awaitNextIteration();
  }
}


void loop() {
}
