#include "CommandGenerator.h"
#include "CompositeFunction.h"
#include "Engine.h"
#include <cassert>
#include <string>

REGISTER_COMMAND(CompositeFunction)

CompositeFunction::CompositeFunction(const class CommandOption &co)
  : Function(co)
  , var(engine->selectFromLabelVector<Function>
      (parseVector<std::string>("VAR")))
  , func(engine->selectFromLabel<Function>
      (parseCompulsoryString("FUNC")))
{
  assert(check());
}

bool CompositeFunction::check() const
{
  if (!func || var.empty()) return false;
  for (const auto & x : var)
    if (!x) return false;
  return true;
}

void CompositeFunction::calculate(const std::vector<double> &values)
{
  using size_type = std::vector<double>::size_type;

  std::vector<double> funcInput;
  std::vector<std::vector<double>> jacobian;

  for (const auto &f : var) {
    f->calculate(values);
    funcInput.push_back(f->getValue());
    jacobian.push_back(f->getDerivative());
  }
  func->calculate(funcInput);
  setValue(func->getValue());
  const auto der = func->getDerivative();

  const auto ni = var.size();
  const auto nj = values.size();

  std::vector<double> final_dev(nj, 0.0);

  for (size_type i = 0; i != ni; ++i) {
    for (size_type j = 0; j != nj; ++j) {
      final_dev[j] += jacobian[i][j] * der[i];
    }
  }
  setDerivative(final_dev);
}
