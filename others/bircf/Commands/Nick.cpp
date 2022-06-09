#include "../MessageManager.hpp"

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 1 || !trailing.empty()) {
    // do something with errcode errcode:errstr map
    return;
  }

  if (AUTH_LEVEL1 <= anyUser(cs).authenticated) {
    if (isUniqueNick(cs, *paramsVec.begin()))
      anyUser(cs).authenticated = AUTH_LEVEL2;
    else
      outMessages_[cs].append(std::string("** [" + *paramsVec.begin() + "] already occupied **\n"));
  }
}

bool MessageManager::isUniqueNick(int cs, std::string &nick) {
  if (nickFdPair_.find(nick) == nickFdPair_.end()) {
    User &user = anyUser(cs);
    std::string current = user.nick;
    nickFdPair_[nick] = cs;
    user.nick = nick;
    nickFdPair_.erase(current);
    return true;
  }
  return false;
}