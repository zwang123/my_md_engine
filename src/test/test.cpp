#include <iostream>
#include <memory>

struct Base {
  virtual ~Base() { std::cout << "Base des\n"; }
};

struct Deriv : public Base {
  ~Deriv() { std::cout << "Deriv des\n"; }
};

void test1()
{
  std::cout << __LINE__ << std::endl;
  std::shared_ptr<Base> b = std::make_shared<Deriv>();
  std::cout << __LINE__ << std::endl;
  {
  std::cout << __LINE__ << std::endl;
  std::shared_ptr<Deriv> d = std::dynamic_pointer_cast<Deriv>(b);
  std::cout << __LINE__ << std::endl;
  }
  std::cout << __LINE__ << std::endl;
}

int main()
{
  test1();
  std::cout << __LINE__ << std::endl;
  return 0;
}

