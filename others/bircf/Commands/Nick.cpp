#include "../MessageManager.hpp"
#include <sstream>

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 1 || !trailing.empty()) {
    // do something with errcode errcode:errstr map
    outMessages_[cs].append("** Usage: [NICK <nickname>] **\n");
    return;
  }

  if (AUTH_LEVEL1 <= anyUser(cs).authenticated) {
    if (NICK_MAX_LENGTH < (*paramsVec.begin()).length())
      outMessages_[cs].append("** nickname having a maximum length of nine(9) characters **\n");
    else if (isUniqueNick(cs, *paramsVec.begin())) {
      anyUser(cs).authenticated = AUTH_LEVEL2;
      outMessages_[cs].append("-- welcome [").append(*paramsVec.begin()).append("], enjoy it please :) --\n");
    }
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
