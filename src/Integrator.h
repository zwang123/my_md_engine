#ifndef INTEGRATOR_H_INCLUDED
#define INTEGRATOR_H_INCLUDED

#include "ModifyCommand.h"
#include "TimeStep.h"

class Integrator
: public ModifyCommand
{
  TimeStep ts;
public:
  Integrator (const class CommandOption &);
  virtual ~Integrator () {}
  virtual void integrate () = 0;
  void run () override {this->integrate();}
  ;
};

#endif // INTEGRATOR_H_INCLUDED
