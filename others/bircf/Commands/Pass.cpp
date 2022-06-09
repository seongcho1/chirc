#include "../MessageManager.hpp"

void MessageManager::PASS(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 1 || !trailing.empty()) {
    // do something with errcode errcode:errstr map
    return;
  }

  if (*paramsVec.begin() == pass) {
    authenticates()[cs].authenticated = AUTH_LEVEL1;
std::cout << "pass: AUTH_LEVEL is 1\n";
  }
}
