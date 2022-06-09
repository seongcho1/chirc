#include "../MessageManager.hpp"

void MessageManager::PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 1) {
    // do something with errcode errcode:errstr map
    return;
  }
  if (trailing.length() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }

  int toFd;
  try {
    toFd = std::stoi(paramsVec[0]);
  }
  catch (const std::exception &e) {
    // do something with errcode errcode:errstr map
    return;
  }

  std::map<int, User>::iterator uit = users_.find(toFd);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  trailing = std::string("[from " + SS::toString(cs) + "(" + users_[cs].nick + ")" + ", to " + paramsVec[0] + "]").append(trailing).append(NEWLINE);
  outMessages_[toFd].append(trailing);
}
