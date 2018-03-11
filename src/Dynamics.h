#ifndef __DYNAMICS_H_INCLUDED
#define __DYNAMICS_H_INCLUDED

#include "SetupCommand.h"
#include <memory>

class Dynamics
: public SetupCommand {
public:
  Dynamics(const class CommandOption &);
  virtual void setAcceleration(std::shared_ptr<class AtomVector>) const = 0;
};

#endif // __DYNAMICS_H_INCLUDED
