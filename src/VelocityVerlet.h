#ifndef __VELOCITY_VERLET_H_INCLUDED
#define __VELOCITY_VERLET_H_INCLUDED

#include "Integrator.h"

class VelocityVerley final
: public Integrator
{
public:
  VelocityVerley(const class CommandOption &co) : Integrator(co) {}
  void integrate () override;
  void setup () override;
  static constexpr const char *directive = "VELOCITY_VERLET";
  void end_of_step ();
};

#endif // __VELOCITY_VERLET_H_INCLUDED
