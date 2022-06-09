#include "../MessageManager.hpp"

void MessageManager::SELFMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  if (trailing.length() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  std::map<int, User>::iterator uit = users_.find(cs);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  trailing = std::string("[from myself, " + SS::toString(cs) + "(" + users_[cs].nick + ")]").append(trailing).append(NEWLINE);
  outMessages_[cs].append(trailing);
}
