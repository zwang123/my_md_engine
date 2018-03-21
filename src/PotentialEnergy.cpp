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

#include <algorithm> // sort

#ifdef DEBUG
#include "print_vector.h"
#include <iostream>
#include <iterator>
#endif // DEBUG

REGISTER_COMMAND(PotentialEnergy)

PotentialEnergy::PotentialEnergy(const class CommandOption &co)
  : SetupCommand(co)
  , fp (engine->selectFromLabel<Function>(parseCompulsoryString("FUNC")))
  , inputFunction (engine->selectFromLabelVector<Function>(args))
{
  auto map_file = parseCompulsoryString("MAP_FILE");
  auto map_from_type_file = parseString("MAP_FROM_TYPE_FILE");
  auto par_file = parseString("PAR_FILE");
  assert(fp);
  assert(engine->getSystem());
  // TODO if nullptr, should throw
  constructMapFromType (map_from_type_file); // must be before constructMap
  constructMap (map_file);
  constructPar (par_file);
#ifdef DEBUG
  {
    std::ostream_iterator<double> oid(std::cout, "\n");
    std::ostream_iterator<size_type> ois(std::cout, "\n");

    std::cout << "Map file: " << std::endl;
    for (const auto &x : potential_map) {
      std::cout << "\tFirst: ";
      printVector(x.first) << std::endl;
      std::cout << "\tSecond: " << x.second << std::endl;
    }

    std::cout << "Par file: " << std::endl;
    for (const auto &x : parameters) {
      printVector(x) << std::endl;
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
      Data item;
      iss >> item;
      parsedLine.push_back(item);
    }
    if (!parsedLine.empty())
      parameters.push_back(std::move(parsedLine));
  }

  ifs.close();
}

void PotentialEnergy::constructMapFromType(const std::string &filename)
{
  if (filename.empty()) return;
  using Data = size_type;
  using IndexVector = std::vector<size_type>;
  using TensorIndex = std::vector<IndexVector::size_type>;
  // Assume AtomVector::Type <= size_type

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
      Data item;
      iss >> item;
      parsedLine.push_back(item);
    }
    if (!parsedLine.empty()) {
      auto target = std::move(parsedLine.back());
      parsedLine.pop_back();

      assert(!parsedLine.empty());
      // Make sure the same typeid are together
      sort(parsedLine.begin(), parsedLine.end());
      // atoms with type parsedLine -> parameter target
      // Generate atom index vector based on parsedLine
      
      const auto av = engine->getSystem()->getAtomVector();
      const auto tensorDim = parsedLine.size();

      TensorIndex tensorIndex; 
      tensorIndex.reserve(tensorDim);

      std::vector<IndexVector> indexVectorList;
      indexVectorList.reserve(tensorDim);

      // tensorIndex = 0;
      Data prev_type = static_cast<Data>(-1);
      IndexVector::size_type prev_index = 
        static_cast<IndexVector::size_type>(-1);
      for (decltype(parsedLine.size()) i = 0; i != tensorDim; ++i) {
        const auto type_idx = parsedLine[i];
        const auto &indices = av->convertType2Index(type_idx);
        assert(!indices.empty());
        indexVectorList.push_back(indices);

        if (type_idx != prev_type) {
          tensorIndex.push_back(0);
          prev_type = type_idx;
          prev_index = 0;
        } else {
          // too little atoms for this atom type
          ++prev_index; // Must not put this in assert
          assert(prev_index != indices.size());
          tensorIndex.push_back(prev_index);
        }
      }
      // guaranteed to run the following loop at least once

      bool not_finished = true;
      while (not_finished) {
        // atom_index_vector = indexVectorList[tensorIndex];
        std::vector<size_type> atom_index_vector;
        for (decltype(parsedLine.size()) i = 0; i != tensorDim; ++i) {
          const auto &indices = indexVectorList[i];
          atom_index_vector.push_back(indices[tensorIndex[i]]);
        }
#ifdef DEBUG
        printVector(atom_index_vector) << std::endl;
#endif // DEBUG
        assert(potential_map.find(atom_index_vector) == potential_map.end());
        // overwrite 
        potential_map[std::move(atom_index_vector)] = target;
        //++tensorIndex
        for (decltype(parsedLine.size()) i = tensorDim; i != 0; ) {
          --i;
          ++tensorIndex[i];

          // if reach limit, add the preceeding 'digit'
          if (tensorIndex[i] == indexVectorList[i].size()) {
            not_finished = i != 0;
            continue;
          }

          // Update the following indices
          bool is_break = true;
          for (decltype(parsedLine.size()) j = i + 1; j < tensorDim; ++j) {
            if (parsedLine[j] != parsedLine[j - 1]) {
              tensorIndex[j] = 0;
            } else {
              tensorIndex[j] = tensorIndex[j - 1] + 1;
            }
            if (tensorIndex[j] == indexVectorList[j].size()) {
              not_finished = i != 0;
              is_break = false;
              break;
            }
          }
          if (is_break) {
            break;
          }
        }
      }
    }
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
      Data item;
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
      auto target = std::move(parsedLine.back());
      parsedLine.pop_back();
      assert(!parsedLine.empty());
      // overwrite 
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
