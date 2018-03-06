#ifndef DYNAMICS_H_INCLUDED
#define DYNAMICS_H_INCLUDED

#include "SetupCommand.h"
#include <memory>

class Dynamics
: public SetupCommand {
public:
  Dynamics(const class CommandOption &);
  virtual void setAcceleration(std::shared_ptr<class AtomVector>) const = 0;
};

#endif // DYNAMICS_H_INCLUDED
