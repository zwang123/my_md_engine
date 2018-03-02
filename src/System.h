#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "SetupCommand.h"
#include <memory>
#include <vector>

class System final
  : public SetupCommand
{
  //std::vector<std::shared_ptr<Atom>> atoms;
  std::shared_ptr<class AtomVector> av;
  std::shared_ptr<class ForceField> ff; // ff should be after av
public:

  System(const class CommandOption &);

  static constexpr const char *directive = "SYSTEM";
  std::shared_ptr<class AtomVector> getAtomVector() const {return av;}
  std::shared_ptr<class ForceField> getForceField() const {return ff;}
};

#endif // SYSTEM_H_INCLUDED
