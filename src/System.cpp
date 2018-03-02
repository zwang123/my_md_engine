#include "AtomVector.h"
#include "CommandGenerator.h"
#include "ForceField.h"
#include "System.h"
#include "Tools.h"
#include <cassert>
#include <fstream>
#include <memory>
#include <sstream>

REGISTER_COMMAND(System)

System::System(const class CommandOption &co)
  : SetupCommand(co)
  , av(std::make_shared<AtomVector>())
  , ff(std::make_shared<ForceField>())
{
  auto filename = parseCompulsoryString("FILE");

  std::ifstream ifs(filename);
  assert(ifs.is_open());
  std::string line;

  using Type = AtomVector::Type;
  using Value = AtomVector::Value;
  using ValueArray = AtomVector::ValueArray;
  using size_type = AtomVector::size_type;
  static constexpr const auto dim = AtomVector::dim;

  while (getline(ifs, line)) {
    std::istringstream iss(Tools::trim_comment_whitespace(line));
    Type type; Value mass; ValueArray pos, vel {{0.0}};
    iss >> type >> mass;

    for(size_type i = 0; i != dim; ++i) {
      iss >> pos[i];
    }

    if (iss && !iss.eof()) {
      for(size_type i = 0; i != dim; ++i) {
        iss >> vel[i];
      }
    }
    assert(iss);
    av->addAtom(type, mass, pos, vel);
  }

  ifs.close();
}
