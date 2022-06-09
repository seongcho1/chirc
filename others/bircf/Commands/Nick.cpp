#include "../MessageManager.hpp"

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 1 || !trailing.empty()) {
    // do something with errcode errcode:errstr map
    return;
  }

  if (AUTH_LEVEL1 <= anyUser(cs).authenticated &&
      isUniqueNick(cs, *paramsVec.begin())) {
    anyUser(cs).authenticated = AUTH_LEVEL2;
  }
}

bool MessageManager::isUniqueNick(int cs, std::string &nick) {
  if (nickFdPair_.find(nick) == nickFdPair_.end()) {
    nickFdPair_[nick] = cs;
    anyUser(cs).nick = nick;
    return true;
  }
  return false;
}