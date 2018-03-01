#ifndef VELOCITY_VERLET_H_INCLUDED
#define VELOCITY_VERLET_H_INCLUDED

#include "Integrator.h"

class VelocityVerley final
: public Integrator
{
public:
  virtual void integrate () override;
  static constexpr const char *directive = "VELOCITY_VERLET";
};

#endif // VELOCITY_VERLET_H_INCLUDED
