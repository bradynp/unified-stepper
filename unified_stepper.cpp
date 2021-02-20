#include "unified_stepper.h"

StepperDriver::StepperDriver(bool *stepsOn) {
    pinsconf       = false;
    stepsOnRising  = stepsOn[0];
    stepsOnFalling = stepsOn[1];
    microsteps     = 0b1;
    edge           = 0b0;

    Vin  = 18.0;
    Vref = 0.85;
}
void StepperDriver::setPins(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d) {
    pin_step      = pin_s;
    pin_disable   = pin_e;
    pin_direction = pin_d;
    digitalWrite(pin_s, LOW);
    pinMode(pin_s, OUTPUT);
    pinMode(pin_e, OUTPUT);
    pinMode(pin_d, OUTPUT);
    pinsconf = true;
}
void StepperDriver::updateMicrosteps() {
    microsteps = 1;
}
void StepperDriver::enableDriver() {
    enabled = true;
    digitalWrite(pin_disable, LOW);
}
void StepperDriver::disableDriver() {
    enabled = false;
    digitalWrite(pin_disable, HIGH);
}
void StepperDriver::toggleDriver() {
    enabled = !enabled;
    digitalWrite(pin_disable, !enabled);
}
void StepperDriver::step() {
    if (!pinsconf) {
        return;
    }
    if (edge == 0b1) {
        digitalWrite(pin_step, LOW);
        edge = 0b0;
    }
    else {
        digitalWrite(pin_step, HIGH);
        edge = 0b1;
    }
}
void StepperDriver::setDirection(uint8_t dir) {
    backward = (bool) dir;
    digitalWrite(pin_direction, backward);
}

StepperMotor::StepperMotor() {
    bool rf[2] = {1, 1};
    Driver     = new StepperDriver(rf);
    Driver->enableDriver();
    Driver->setDirection(FORWARD);

    Imax = 1.0;
}
StepperMotor::~StepperMotor() {
    delete[] Driver;
}
void StepperMotor::setDriverPins(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d) {
    Driver->setPins(pin_s, pin_e, pin_d);
}
void StepperMotor::step(long steps, unsigned long microDelay) {
    if (steps > 0) {
        Driver->setDirection(FORWARD);
        while (steps--) {
            Driver->step();
            delayMicroseconds(microDelay);
        }
        return;
    }
    if (steps < 0) {
        Driver->setDirection(BACKWARD);
        while (steps++) {
            Driver->step();
            delayMicroseconds(microDelay);
        }
    }
}

StepperJoint::StepperJoint(double numSteps, uint8_t pin_s, uint8_t pin_e, uint8_t pin_d) {
    setDriverPins(pin_s, pin_e, pin_d);
    stepsPerRotation = numSteps;
    position         = 0.0;
}
void StepperJoint::setHome() {
    position = 0.0;
}
void StepperJoint::halt() {
    Driver->disableDriver();
}
void StepperJoint::goToAngle(double theta, double rpm) {
    double dist = (theta - position) * (stepsPerRotation * Driver->microsteps) / (2 * PI);
    step(dist, rpmToMicros(rpm));
    position = theta;
}
void StepperJoint::moveByAngle(double theta, double rpm) {
    double dist = theta * (stepsPerRotation * Driver->microsteps) / (2.0 * PI);
    step(dist, rpmToMicros(rpm));
    position += theta;
}
void StepperJoint::moveByExact(long ticks, double rpm) {
    step(ticks, rpmToMicros(rpm));
    position += ticks / (stepsPerRotation * Driver->microsteps) * (2.0 * PI);
}
unsigned long StepperJoint::rpmToMicros(double rpm) {
    return (1.0 / rpm) * (1000000.0 * 60.0) / (stepsPerRotation * Driver->microsteps);
}

StepperRobot::StepperRobot() {
}
StepperRobot::~StepperRobot() {
}
