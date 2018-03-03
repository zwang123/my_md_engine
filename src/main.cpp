#include "backtrace.h"

#include "MD.h"

//#include "Atom.h"
//#include "Doo.h"
//#include "ForceFeild.h"
//#include <vector>
//#include <memory>

int main(int argc, char **argv)
{
  MD md(argc, argv);

  //ForceField::AtomVec av(2, std::make_shared<Atom>());
  //ForceField *doo = new Doo();
  //doo->potential(av);
  //delete doo;

  return md.run();
}
