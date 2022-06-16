#include "../../MessageManager.hpp"

void MessageManager::SELFMSG(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  if (paramsVec[0].length() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  std::map<int, User>::iterator uit = users_.find(cs);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  std::string message = std::string("[from myself, " + SS::toString(cs) + "(" + users_[cs].nick + ")]").append(paramsVec[0]).append(NEWLINE);
  // outMessages_[cs].append(message);
  users_[cs].wbuff.append(message);
}
