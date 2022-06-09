#include "../MessageManager.hpp"
#include <sstream>

void MessageManager::USER(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 4 || !trailing.empty()) {
    outMessages_[cs].append("** Usage: [USER <user_name> <host> <not use> <real_name>] **\n");
    return;
  }

  if (users_[cs].authenticated & AUTH_LEVEL3) {
  // if (users_.find(cs) == users_.end()) {
    outMessages_[cs].append("** You may not reregister **\n");
    return;
  }

  User &user = users_[cs];
  std::vector<std::string>::iterator it = paramsVec.begin();
  if (AUTH_LEVEL1 <= user.authenticated) {
    user.authenticated |= AUTH_LEVEL3;
    user.user = *it++;
    it++;
    it++;
    user.real = *it;
    outMessages_[cs].append("-- welcome Sir-[").append(user.real).append("]. --\n");
  }
}
