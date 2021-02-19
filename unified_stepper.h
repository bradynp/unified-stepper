#pragma once
#include <Arduino.h>

#define FORWARD  0x0
#define BACKWARD 0x1

class StepperDriver {
public:
    StepperDriver(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d, bool *stepsOn);

    void setMicrosteps(uint32_t m);
    void enableDriver();
    void disableDriver();
    void toggleDriver();
    void step();
    void setDirection(uint8_t dir);

protected:
    uint8_t pin_step;
    uint8_t pin_disable;
    uint8_t pin_direction;
    uint8_t pins_micro[3];
    uint8_t microsteps;
    double Vin;
    double Vref;
    bool stepsOnRising;
    bool stepsOnFalling;
    bool enabled;
    bool backward;
    uint8_t edge;
};
class StepperMotor {
public:
    StepperMotor(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d);
    ~StepperMotor();

    void step(long steps, unsigned long microDelay);
    double stepsPerRotation;

protected:
    StepperDriver *Driver;
    double Imax;
};
class StepperJoint {
public:
    StepperJoint(double numSteps, uint8_t pin_s, uint8_t pin_e, uint8_t pin_d);
    ~StepperJoint();

    unsigned long rpmToMicros(double rpm);
    void setHome();
    void goToAngle(double theta, double rpm);
    void moveByAngle(double theta, double rpm);
    void moveByExact(long ticks, double rpm);

protected:
    StepperMotor *Motor;
    double stepsPerRotation;
    double position;
};
