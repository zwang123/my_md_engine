#ifndef TIME_STEP_H_INCLUDED
#define TIME_STEP_H_INCLUDED

#include <cstddef>

class TimeStep {
public:
  using StepType = ptrdiff_t;

private:
  StepType step;
  const double dt;

public:
  TimeStep(double dt = 0.0, StepType step = 0) noexcept : step(step), dt(dt) {}
  void setStep(StepType _step) noexcept {step = _step;}
  StepType getStep() const noexcept {return step;}
  double getTime() const noexcept {return step*dt;}
  double getInterval() const noexcept {return dt;}
};

#endif // TIME_STEP_H_INCLUDED
