#include "unified_stepper.h"

StepperDriver::StepperDriver(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d, bool *stepsOn) {
    pin_step      = pin_s;
    pin_disable   = pin_e;
    pin_direction = pin_d;
    pinMode(pin_s, OUTPUT);
    pinMode(pin_e, OUTPUT);
    pinMode(pin_d, OUTPUT);
    stepsOnRising  = stepsOn[0];
    stepsOnFalling = stepsOn[1];
    microsteps     = 0b00000000;
    edge           = 0b0;

    Vin  = 18.0;
    Vref = 0.85;
}
void StepperDriver::setMicrosteps(uint32_t m) {}
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

StepperMotor::StepperMotor(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d) {
    bool rf[2] = {1, 1};
    Driver     = new StepperDriver(pin_s, pin_e, pin_d, rf);
    Driver->enableDriver();
    Driver->setDirection(FORWARD);

    Imax = 1.0;
}
StepperMotor::~StepperMotor() {
    delete[] Driver;
}
void StepperMotor::step(long steps, unsigned long microDelay) {
    if (steps > 0) {
        Driver->setDirection(FORWARD);
        while (steps--) {
            Driver->step();
            delayMicroseconds(microDelay);
        }
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
    Motor                   = new StepperMotor(pin_s, pin_e, pin_d);
    Motor->stepsPerRotation = numSteps;
    stepsPerRotation        = numSteps;
    position                = 0.0;
}
StepperJoint::~StepperJoint() {
    delete[] Motor;
}
unsigned long StepperJoint::rpmToMicros(double rpm) {
    return (1.0 / rpm) * (1000000.0 * 60.0) * (1.0 / stepsPerRotation);
}
void StepperJoint::setHome() {
    position = 0.0;
}
void StepperJoint::goToAngle(double theta, double rpm) {
    double dist = (theta - position) * stepsPerRotation / (2 * PI);
    Motor->step(dist, rpmToMicros(rpm));
    position = theta;
}
void StepperJoint::moveByAngle(double theta, double rpm) {
    double dist = theta * stepsPerRotation / (2.0 * PI);
    Motor->step(dist, rpmToMicros(rpm));
    position += theta;
}
void StepperJoint::moveByExact(long ticks, double rpm) {
    Motor->step(ticks, rpmToMicros(rpm));
    position += ticks / stepsPerRotation * (2.0 * PI);
}
