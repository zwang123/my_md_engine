#ifndef __TIME_STEP_H_INCLUDED
#define __TIME_STEP_H_INCLUDED

#include <cassert>
#include <cstddef>

class TimeStep final {
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
  //+- <= ++ --
  TimeStep& operator +=(const StepType incr) & noexcept 
  {step += incr; return *this;}
  TimeStep& operator -=(const StepType decr) & noexcept 
  {step -= decr; return *this;}
  TimeStep operator +(const StepType incr) const noexcept 
  {TimeStep result (*this); result += incr; return result;}
  TimeStep operator -(const StepType decr) const noexcept 
  {TimeStep result (*this); result -= decr; return result;}

  // I know it is dangerous to compare two doubles...
  TimeStep& operator +=(const TimeStep &rhs) & noexcept 
  {assert(getInterval() == rhs.getInterval()); return (*this += rhs.getStep());}
  TimeStep& operator -=(const TimeStep &rhs) & noexcept 
  {assert(getInterval() == rhs.getInterval()); return (*this -= rhs.getStep());}
  TimeStep operator +(const TimeStep &rhs) const noexcept 
  {TimeStep result (*this); result += rhs; return result;}
  TimeStep operator -(const TimeStep &rhs) const noexcept 
  {TimeStep result (*this); result -= rhs; return result;}

  TimeStep& operator ++() & noexcept
  {++step; return *this;}
  TimeStep& operator --() & noexcept
  {--step; return *this;}
  TimeStep operator ++(int) & noexcept
  {TimeStep result (*this); ++*this; return result;}
  TimeStep operator --(int) & noexcept
  {TimeStep result (*this); --*this; return result;}

  bool operator <(const TimeStep &rhs) const noexcept
  {assert(getInterval() == rhs.getInterval()); 
    return (getStep() < rhs.getStep());}
  bool operator ==(const TimeStep &rhs) const noexcept
  {assert(getInterval() == rhs.getInterval()); 
    return (getStep() == rhs.getStep());}
  bool operator >(const TimeStep &rhs) const noexcept
  {return rhs < *this;}
  bool operator >=(const TimeStep &rhs) const noexcept
  {return !(*this < rhs);}
  bool operator <=(const TimeStep &rhs) const noexcept
  {return !(rhs < *this);}
  bool operator !=(const TimeStep &rhs) const noexcept
  {return !(rhs == *this);}
};

#endif // __TIME_STEP_H_INCLUDED
