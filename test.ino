#include "unified_stepper.h"

#define pinClock 5
#define pinEnable 6
#define pinDirection 7
#define STEPSPERROTATION 200

StepperJoint myStepper(STEPSPERROTATION, pinClock, pinEnable, pinDirection);

void setup() {}
void loop() {
  myStepper.halt(false);
  myStepper.setHome();
  myStepper.goToAngle(0.001 * random(-12000, 12000),
                      0.001 * random(120000, 300000));
  myStepper.halt();
  delay(10);
}
