#include <map>
#include <stack>
#include <iostream>

class U {
public:
  int id;
  U(int id) : id(id) {}
};

int main (void) {
  std::map<int, U> map0;
  std::map<int, U> map1;

  std::pair<int, U> p = std::pair<int, U>(10, U(10));
  map0.insert(p);
  map1.insert(*map0.begin());

  std::cout << &map0.begin()->second << std::endl;
  std::cout << &map1.begin()->second << std::endl;

  map0.erase(map0.begin()->first);

  std::cout << &map0.begin()->second << std::endl;
  std::cout << &map1.begin()->second << std::endl;

}