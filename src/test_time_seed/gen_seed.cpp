#ifdef DEBUG
#include "Atom.h"
#include "AtomVector.h"
#include <cmath>
#include <memory>
#endif // DEBUG
#include "TimeSeed.h"
#include "Tools.h"
#include "RandomNumberGenerator.h"
#include <iostream>
#include <array>
#include <valarray>

#ifdef DEBUG
#include "AtomVector.h"
#include "Constants.h"
#include "Engine.h"
#include "System.h"
#include "VelocityInitializer.h"
//#include <cmath>
#include <cassert>
#include <sstream>
#include <string>
//#include <vector>
#include <fstream>
void test_velocity(unsigned n)
{
  constexpr double m1 = 2.5;
  constexpr double m2 = 1.5;
  constexpr double T = 3.0;

  std::ofstream ofs("atom.inp");
  ofs << "0 " << m1;
  for (AtomVector::size_type i = 0; i != AtomVector::dim; ++i)
    ofs << " " << "0.0";
  ofs << std::endl;
  for (unsigned i = 0; i != 3; ++i) {
    ofs << "1 " << m2;
    for (AtomVector::size_type i = 0; i != AtomVector::dim; ++i)
      ofs << " " << "0.0";
    ofs << std::endl;
  }
  ofs.close();
  const auto engine = std::make_shared<Engine>();
  std::istringstream iss(
      "&BEGIN SYSTEM\n"
      "FILE atom.inp\n"
      "&END\n"
      "&BEGIN VELOCITY\n"
      "TEMP " + std::to_string(T) + "\n"
      "&END\n");
  engine->read(iss);
  auto vi = engine->select<VelocityInitializer>().at(0);
  auto av = engine->getSystem()->getAtomVector();


  //std::vector<double> sig;
  //sig.reserve(av->size());
  //for (const auto &mass : av->getConstMassVector()) {
  //  sig.push_back(sqrt(Constants::boltzmann * T / mass));
  //}

  double avg_kin = 0.0;
  for (unsigned i = 0; i != n; ++i) {
    vi->setVelocity(av);
    //auto &vel = av->getConstVelocityVector();
    avg_kin += av->kinetic();
  }
  avg_kin /= n;
  double refKE = .5 * av->size() * Constants::boltzmann * T;
  std::cout << "Cal: " << avg_kin << " Ref: " << refKE << std::endl;
}

void test_velocity2(unsigned n)
{
  RandomNumberGenerator<>::seed(TimeSeed::gen_seed()); 
  static RandomNumberGenerator<> rng;
  double T = rng();

  std::ofstream ofs("atom.inp");
  for (unsigned i = 0; i != n; ++i) {
    ofs << i << " " << rng();
    for (AtomVector::size_type i = 0; i != AtomVector::dim; ++i)
      ofs << " " << rng() * 3.0;
    ofs << "\n";
  }
  ofs.close();
  {
    const auto engine = std::make_shared<Engine>();
    std::istringstream iss(
        "&BEGIN SYSTEM\n"
        "FILE atom.inp\n"
        "&END\n"
        "&BEGIN VELOCITY\n"
        "TEMP " + std::to_string(T) + "\n"
        "SCALE  trUe\n"
        "ZEro_liNear  trUe\n"
        "ZEro_angulAR  TrUe \n"
        "&END\n");
    engine->read(iss);
    auto vi = engine->select<VelocityInitializer>().at(0);
    auto av = engine->getSystem()->getAtomVector();

    vi->setVelocity(av);
    double refKE = .5 * av->size() * Constants::boltzmann * T;
    std::cout << "Cal: " << av->kinetic() << " Ref: " << refKE << std::endl;

    struct Scope {
      static
      AtomVector::ValueValarray crossProduct(const AtomVector::Value *a,
                                          const AtomVector::ValueValarray &b)
      { assert(AtomVector::dim == 3); AtomVector::Value r1, r2, r3;
        r1 = a[1]*b[2]-a[2]*b[1];
        r2 = a[2]*b[0]-a[0]*b[2];
        r3 = a[0]*b[1]-a[1]*b[0];
        return {r1, r2, r3};
      }
    };

    AtomVector::ValueValarray mom;
    AtomVector::ValueValarray ang;
    auto prstart = av->getConstPositionVector().data();
    auto pvstart = av->getConstVelocityVector().data();
    {
      AtomVector::ValueValarray mom_one(pvstart, AtomVector::dim);
      mom_one *= av->getConstMassVector()[0];
      mom = mom_one;
      if (AtomVector::dim == 3) {
        auto ang_one = Scope::crossProduct(prstart, mom_one);
        ang = ang_one;
      }
    }
    for (AtomVector::size_type i = AtomVector::dim; 
         i != av->size(); i += AtomVector::dim) {
      AtomVector::ValueValarray mom_one(pvstart + i, AtomVector::dim);
      mom_one *= av->getConstMassVector()[i];
      mom += mom_one;
      if (AtomVector::dim == 3) {
        auto ang_one = Scope::crossProduct(prstart + i, mom_one);
        ang += ang_one;
      }
    }
    std::cout << "Cal Momentum:" ;
    for (const auto &x : mom)
      std::cout << " " << x;
    std::cout << std::endl;
    std::cout << "Prog Momentum:" ;
    for (const auto &x : av->momentum())
      std::cout << " " << x;
    std::cout << std::endl;
    if (AtomVector::dim == 3) {
      std::cout << "Cal Ang Momentum:" ;
      for (const auto &x : ang)
        std::cout << " " << x;
      std::cout << std::endl;
    }
    std::cout << "Prog Ang Momentum:" ;
    for (const auto &x : av->angular_momentum())
      std::cout << " " << x;
    std::cout << std::endl;
  }
  {
    const auto engine = std::make_shared<Engine>();
    std::istringstream iss(
        "&BEGIN SYSTEM\n"
        "FILE atom.inp\n"
        "&END\n"
        "&BEGIN VELOCITY\n"
        "TEMP " + std::to_string(T) + "\n"
        "&END\n");
    engine->read(iss);
    auto vi = engine->select<VelocityInitializer>().at(0);
    auto av = engine->getSystem()->getAtomVector();

    vi->setVelocity(av);
    double refKE = .5 * av->size() * Constants::boltzmann * T;
    std::cout << "Cal: " << av->kinetic() << " Ref: " << refKE << std::endl;
  }
}
#endif // DEBUG

int main()
{
  std::cout << TimeSeed::gen_seed() << std::endl;
  std::cout << TimeSeed::gen_seed<int>() << std::endl;
  std::cout << TimeSeed::gen_seed<DEFAULT_GENERATOR::result_type>() 
            << std::endl;

  //RandomNumberGenerator<> rng(TimeSeed::gen_seed());
  RandomNumberGenerator<>::seed(TimeSeed::gen_seed());
  RandomNumberGenerator<> rng;
  for (unsigned i = 0; i != 10; ++i)
    std::cout << rng() << std::endl;

  Tools::cross(std::array<double, 1>{{0.0}}, std::array<double, 1>{{0.0}});
  Tools::cross(std::array<double, 2>{{0.0}}, std::array<double, 2>{{0.0}});
  Tools::cross(std::array<double, 3>{{0.0}}, std::array<double, 3>{{0.0}});

  std::valarray<double> v1{};
  std::valarray<double> v2{1.3};
  std::valarray<double> v3{{0.0, 0.0, 0.0}};
  std::cout << v1.size() << v2.size() << v2[0] << v3.size() << std::endl;

#ifdef DEBUG
  {
    const auto av = std::make_shared<AtomVector>();
    for (unsigned i = 0; i != 10000000; ++i) {
      const auto pos = av->addAtom()->getPosition();
      //std::cout << "Pos:";
      //for (const auto &x : pos)
      //  std::cout << "  " << x;
      //std::cout << std::endl;
    }
    std::array<unsigned, Atom::box> hist_bins {{0}};
    for (const auto &x : av->getConstPositionVector())
      ++hist_bins[static_cast<unsigned>(floor(x))];
    double total = 0.0;
    for (const auto &x : hist_bins)
      total += x;
    std::cout << "Probability:";
    for (const auto &x : hist_bins)
      std::cout << "  " << x/total;
    std::cout << std::endl;
  }
  
  test_velocity(10000000);
  test_velocity2(100000);
  //test_velocity2(3);
#endif // DEBUG

  return 0;
}

