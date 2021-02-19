#include "unified_stepper.h"

#define pinClock         5
#define pinEnable        6
#define pinDirection     7
#define STEPSPERROTATION 200

StepperJoint myStepper(STEPSPERROTATION, pinClock, pinEnable, pinDirection);

void setup() {}
void loop() {
    myStepper.setHome();
    myStepper.goToAngle(0.001 * random(0, 6000), 0.001 * random(30000, 200000));
    delay(1000);
}
