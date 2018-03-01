#ifndef NEWTONIAN_DYNAMICS_H_INCLUDED
#define NEWTONIAN_DYNAMICS_H_INCLUDED

#include "Dynamics.h"

class NewtonianDynamics final
: public Dynamics {
  NewtonianDynamics(const class CommandOption &co) : Dynamics(co) {}
  void setAccelaration(class AtomVector &) const override;
  ;
};

#endif // NEWTONIAN_DYNAMICS_H_INCLUDED
