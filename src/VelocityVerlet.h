#ifndef VELOCITY_VERLET_H_INCLUDED
#define VELOCITY_VERLET_H_INCLUDED

#include "Integrator.h"

class VelocityVerley final
: public Integrator
{
public:
  VelocityVerley(const class CommandOption &co) : Integrator(co) {}
  void integrate () override;
  void setup () override;
  static constexpr const char *directive = "VELOCITY_VERLET";
};

#endif // VELOCITY_VERLET_H_INCLUDED
