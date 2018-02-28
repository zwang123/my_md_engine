#ifndef DYNAMICS_H_INCLUDED
#define DYNAMICS_H_INCLUDED

#include "SetupCommand.h"

class Dynamics :
public SetupCommand {
public:
  Dynamics(const class CommandOption &co) : SetupCommand(co) {}
  virtual void setAccelaration(class AtomVector &) const = 0;
  virtual ~Dynamics() {}
  void run() override {}
};

#endif // DYNAMICS_H_INCLUDED
