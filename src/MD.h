#ifndef __MD_H_INCLUDED
#define __MD_H_INCLUDED

#include <fstream>
#include <memory>

class Engine;

// The class that de-codes the command-line options
// and run the engine
class MD final {
  std::ifstream ifs;
  std::ofstream ofs;
  std::shared_ptr<Engine> engine;

  void print_help();
public:
  explicit MD(int, char **);
  ~MD();
  int run();
};

#endif // __MD_H_INCLUDED
