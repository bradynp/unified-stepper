#pragma once
#include <Arduino.h>

#define FORWARD 0x0
#define BACKWARD 0x1

class StepperDriver {
public:
  StepperDriver(bool *stepsOn);

  void setPins(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d);
  void updateMicrosteps();
  void enableDriver();
  void disableDriver();
  void toggleDriver();
  void step();
  void setDirection(uint8_t dir);
  uint8_t microsteps;

protected:
  uint8_t pin_step;
  uint8_t pin_disable;
  uint8_t pin_direction;
  uint8_t pins_micro[3];
  double Vin;
  double Vref;
  bool pinsconf;
  bool stepsOnRising;
  bool stepsOnFalling;
  bool enabled;
  bool backward;
  uint8_t edge;
};
class StepperMotor {
public:
  StepperMotor();
  ~StepperMotor();

  void setDriverPins(uint8_t pin_s, uint8_t pin_e, uint8_t pin_d);
  void step(long steps, unsigned long microDelay);

protected:
  StepperDriver *Driver;
  double stepsPerRotation;
  double Imax;
};
class StepperJoint : public StepperMotor {
public:
  StepperJoint(double numSteps, uint8_t pin_s, uint8_t pin_e, uint8_t pin_d);

  void setHome();
  void halt(bool stop = true);
  void goToAngle(double theta, double rpm);
  void moveByAngle(double theta, double rpm);
  void moveByExact(long ticks, double rpm);

protected:
  double position;
  unsigned long rpmToMicros(double rpm);
};
class StepperRobot {
public:
  StepperRobot();
  ~StepperRobot();

protected:
  StepperJoint *Joints;
};
