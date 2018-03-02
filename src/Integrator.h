#ifndef INTEGRATOR_H_INCLUDED
#define INTEGRATOR_H_INCLUDED

#include "ModifyCommand.h"
#include "TimeStep.h"

class Integrator
: public ModifyCommand
{
protected:
  TimeStep ts;
  std::shared_ptr<class AtomVector> av;
public:
  Integrator (const class CommandOption &);
  virtual ~Integrator () {}
  virtual void integrate () = 0;
  void run () override {this->integrate();}
  void setup () override = 0;
  ;
};

#endif // INTEGRATOR_H_INCLUDED
