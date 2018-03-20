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

#ifdef DEBUG
#include "print_vector.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#endif // DEBUG

REGISTER_COMMAND(PotentialEnergy)

PotentialEnergy::PotentialEnergy(const class CommandOption &co)
  : SetupCommand(co)
  , fp (engine->selectFromLabel<Function>(parseCompulsoryString("FUNC")))
  , inputFunction (engine->selectFromLabelVector<Function>(args))
{
  auto map_file = parseCompulsoryString("MAP_FILE");
  auto par_file = parseString("PAR_FILE");
  assert(fp);
  assert(engine->getSystem());
  // TODO if nullptr, should throw
  constructMap (map_file);
  constructPar (par_file);
#ifdef DEBUG
  {
    std::ostream_iterator<double> oid(std::cout, "\n");
    std::ostream_iterator<size_type> ois(std::cout, "\n");

    std::cout << "Map file: " << std::endl;
    for (const auto &x : potential_map) {
      std::cout << "\tSecond: " << x.second << std::endl;
      std::copy(x.first.cbegin(), x.first.cend(), ois);
    }

    std::cout << "Par file: " << std::endl;
    for (const auto &x : parameters) {
      std::copy(x.cbegin(), x.cend(), oid);
    }
  }
#endif // DEBUG
  checkInput();
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
    assert(!input.empty());
#ifdef DEBUG
    //printVector(input) << std::endl;
#endif // DEBUG
    for (const auto &f : inputFunction) {
      f->calculate(input);
      funcInput.push_back(f->getValue());
      jacobian.push_back(f->getDerivative());
#ifdef DEBUG
      //printVector(f->getDerivative()) << std::endl;
#endif // DEBUG
    }

    if (!parameters.empty()) {
      const auto &currPars = parameters[p.second];
      funcInput.insert(funcInput.end(), currPars.cbegin(), currPars.cend());
    }

    assert(!funcInput.empty());
#ifdef DEBUG
    //printVector(funcInput) << std::endl;
#endif // DEBUG
    fp->calculate(funcInput);
    energy += fp->getValue();
    auto der = fp->getDerivative();
#ifdef DEBUG
    //printVector(fp->getDerivative()) << std::endl;
#endif // DEBUG
    
    //auto n = av->size();
    // assert(!jacobian.empty())
    const auto &indicies = idx_and_pos.first;
    const auto ni = inputFunction.size();
    const auto nj = indicies.size();

#ifdef DEBUG
    //printVector(indicies) << std::endl;
#endif // DEBUG

    for (size_type i = 0; i != ni; ++i) {
      for (size_type j = 0; j != nj; ++j) {
        force[indicies[j]] -= jacobian[i][j] * der[i];
      }
    }
  }
}

void PotentialEnergy::constructPar(const std::string &filename)
{
  if (filename.empty()) return;
  using Data = double;

  std::ifstream ifs(filename);
  assert(ifs.is_open());
  std::string line;
  parameters.clear();
  //std::vector<std::vector<Data>> result;

  while (getline(ifs, line)) {
    line = Tools::trim_comment_whitespace(line);
    if (line.empty())
      continue;
    std::istringstream iss(line);
    std::vector<Data> parsedLine;
    //while (!line.empty() && iss && !iss.eof()) {
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
    line = Tools::trim_comment_whitespace(line);
    if (line.empty())
      continue;
    std::istringstream iss(line);
    std::vector<Data> parsedLine;
    //while (!line.empty() && iss && !iss.eof()) {
    while (iss && !iss.eof()) {
      double item;
      iss >> item;
      parsedLine.push_back(item);
    }
    if (!parsedLine.empty()) {
#ifdef DEBUG
      //{
      //  std::ostream_iterator<size_type> ois(std::cout, "\t");

      //  std::cout << "ParsedLine: " << parsedLine.size() << std::endl;
      //  std::copy(parsedLine.cbegin(), parsedLine.cend(), ois);
      //  std::cout << std::endl;
      //}
#endif // DEBUG
      auto target = parsedLine.back();
      parsedLine.pop_back();
      potential_map[std::move(parsedLine)] = target;
    }
  }

  ifs.close();
}

bool PotentialEnergy::checkInput() const
{
  size_type n;
  bool is_first = true;
  for (const auto &x : potential_map) {
    if (is_first) {
      n = x.first.size();
      is_first = false;
    }
    assert(x.first.size() == n);
    if (x.first.size() != n) return false;
  }
  is_first = true;
  for (const auto &x : parameters) {
    if (is_first) {
      n = x.size();
      is_first = false;
    }
    assert(x.size() == n);
    if (x.size() != n) return false;
  }
  for (const auto &x : inputFunction) {
    if (!x) {
      assert(x);
      return false;
    }
  }
  return true;
}
