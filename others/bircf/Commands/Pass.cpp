#include "../MessageManager.hpp"

void MessageManager::PASS(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (users_[cs].authenticated & AUTH_LEVEL1)
    return;

  if (paramsVec.size() != 1 || !trailing.empty()) {
    // do something with errcode errcode:errstr map
    outMessages_[cs].append("** Usage: [PASS <password>] **\n");
    return;
  }

  if (*paramsVec.begin() == pass) {
    users_[cs].authenticated |= AUTH_LEVEL1;
    outMessages_[cs].append("-- welcome [SOMEBODY], setup nickname please --\n");
  }
  else
    outMessages_[cs].append("** WRONG PASSWORD! **\n");
}
