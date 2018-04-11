#include "../print_vector.h"

#include <vector>
#include <deque>
#include <array>
#include <iostream>

int main()
{
  int carr[4] = {0, 3, 5, 6};
  const int ccarr[4] = {0, 3, 5, 6};
  std::vector<int> vint (4, 5);
  const std::deque<int> qint (vint.cbegin(), vint.cend());
  std::array<int, 4> aint {{4, 5, 1, 3}};
  int *cptr = static_cast<int*>(malloc(4 * sizeof(int)));
  if (cptr == nullptr) return 1;
  int *cppptr = new int[4];
  if (cppptr == nullptr) return 2;
  int * const icptr = new int[4];
  const int * ciptr = new int[4];
  const int * const cicptr = new int[4];
  
  printVector(carr) << std::endl;
  printVector(ccarr) << std::endl;
  printVector(vint) << std::endl;
  printVector(qint) << std::endl;
  printVector(aint) << std::endl;
  printVector(cptr, 4) << std::endl;
  printVector(cppptr,4) << std::endl;
  printVector(ciptr,4) << std::endl;
  printVector(icptr,4) << std::endl;
  printVector(cicptr,4) << std::endl;

  delete [] cppptr;
  delete [] icptr;
  delete [] ciptr;
  delete [] cicptr;
  free(cptr);
  return 0;
}
