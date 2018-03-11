#ifndef __COMPOSITE_FUNCTION_H_INCLUDED
#define __COMPOSITE_FUNCTION_H_INCLUDED

#include "Function.h"
#include <vector>
#include <memory>

class CompositeFunction final
: public Function {
  std::vector<std::shared_ptr<Function>> var;
  std::shared_ptr<Function> func;
  bool check() const;
public:
  CompositeFunction(const class CommandOption &);
  void calculate(const std::vector<double> &) override;
  static constexpr const char *directive = "COMPOSITE";
};

#endif // __COMPOSITE_FUNCTION_H_INCLUDED
