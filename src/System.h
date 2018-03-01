#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "SetupCommand.h"
#include <memory>
#include <vector>

class System
  : public SetupCommand
{
  //std::vector<std::shared_ptr<Atom>> atoms;
  std::shared_ptr<class AtomVector> av;
  std::shared_ptr<class ForceField> ff;
public:
  static constexpr const char *directive = "SYSTEM";
  std::shared_ptr<class AtomVector> getAtomVector() {return av;}
  
};

#endif // SYSTEM_H_INCLUDED
