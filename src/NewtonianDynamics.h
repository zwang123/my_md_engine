#ifndef NEWTONIAN_DYNAMICS_H_INCLUDED
#define NEWTONIAN_DYNAMICS_H_INCLUDED

#include "Dynamics.h"

class NewtonianDynamics final
: public Dynamics {
public:
  NewtonianDynamics(const class CommandOption &co) : Dynamics(co) {}
  void setAcceleration(std::shared_ptr<class AtomVector>) const override;
  static constexpr const char *directive = "NEWTONIAN";
};

#endif // NEWTONIAN_DYNAMICS_H_INCLUDED
