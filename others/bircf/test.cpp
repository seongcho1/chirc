#include <iostream>
int main(void) {
  int a = 1;
  int b = 0;

  if (a == b) std::cout << "same" << std::endl;
  else std::cout << (a = b) << std::endl;
}