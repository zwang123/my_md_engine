#include "AtomVector.h"
#include "CommandGenerator.h"
#include "ForceField.h"
#include "Function.h"
#include "Engine.h"
#include "System.h"
#include "PotentialEnergy.h"
#include "Tools.h"
#include <cassert>
#include <memory>
#include <utility>

REGISTER_COMMAND(PotentialEnergy)

PotentialEnergy::PotentialEnergy(const class CommandOption &co)
  : SetupCommand(co)
  , fp (engine->selectFromLabel<Function>(parseCompulsoryString("FUNC")))
  , inputFunction (engine->selectFromLabelVector<Function>(args))
{
  auto map_file = parseCompulsoryString("MAP_FILE");
  auto par_file = parseCompulsoryString("PAR_FILE");
  assert(fp);
  assert(engine->getSystem());
  // TODO if nullptr, should throw
  constructMap (map_file);
  constructPar (par_file);
}

void PotentialEnergy::postConstruct()
{
  engine->getSystem()->getForceField()->pes.push_back(shared_from_this());
}

void PotentialEnergy::calculate(std::shared_ptr<const AtomVector> av)
{
  energy = 0.0;
  force.assign(av->size(), 0.0);

  for (const auto &p : potential_map) {
    std::vector<double> funcInput;
    std::vector<std::vector<double>> jacobian;

    auto &&idx_and_pos = av->slicePositionVector(p.first);
    auto input = std::move(idx_and_pos.second);
    for (const auto &f : inputFunction) {
      f->calculate(input);
      funcInput.push_back(f->getValue());
      jacobian.push_back(f->getDerivative());
    }

    const auto &currPars = parameters[p.second];
    funcInput.insert(funcInput.end(), currPars.cbegin(), currPars.cend());

    fp->calculate(funcInput);
    energy += fp->getValue();
    auto der = fp->getDerivative();
    
    //auto n = av->size();
    // assert(!jacobian.empty())
    auto &indicies = idx_and_pos.first;
    const auto ni = inputFunction.size();
    const auto nj = indicies.size();

    for (size_type i = 0; i != ni; ++i) {
      for (size_type j = 0; j != nj; ++j) {
        force[indicies[j]] -= jacobian[i][j] * der[i];
      }
    }
  }
}

void PotentialEnergy::constructPar(const std::string &filename)
{
  using Data = double;

  std::ifstream ifs(filename);
  assert(ifs.is_open());
  std::string line;
  parameters.clear();
  //std::vector<std::vector<Data>> result;

  while (getline(ifs, line)) {
    std::istringstream iss(Tools::trim_comment_whitespace(line));
    std::vector<Data> parsedLine;
    while (iss && !iss.eof()) {
      double item;
      iss >> item;
      parsedLine.push_back(item);
    }
    if (!parsedLine.empty())
      parameters.push_back(std::move(parsedLine));
  }

  ifs.close();
}

void PotentialEnergy::constructMap(const std::string &filename)
{
  using Data = size_type;

  std::ifstream ifs(filename);
  assert(ifs.is_open());
  std::string line;
  //std::vector<std::vector<Data>> result;

  while (getline(ifs, line)) {
    std::istringstream iss(Tools::trim_comment_whitespace(line));
    std::vector<Data> parsedLine;
    while (iss && !iss.eof()) {
      double item;
      iss >> item;
      parsedLine.push_back(item);
    }
    if (!parsedLine.empty()) {
      auto target = parsedLine.back();
      parsedLine.pop_back();
      potential_map[std::move(parsedLine)] = target;
    }
  }

  ifs.close();
}

