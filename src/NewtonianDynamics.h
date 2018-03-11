#ifndef __NEWTONIAN_DYNAMICS_H_INCLUDED
#define __NEWTONIAN_DYNAMICS_H_INCLUDED

#include "Dynamics.h"

class NewtonianDynamics final
: public Dynamics {
public:
  NewtonianDynamics(const class CommandOption &co) : Dynamics(co) {}
  void setAcceleration(std::shared_ptr<class AtomVector>) const override;
  static constexpr const char *directive = "NEWTONIAN";
};

#endif // __NEWTONIAN_DYNAMICS_H_INCLUDED
