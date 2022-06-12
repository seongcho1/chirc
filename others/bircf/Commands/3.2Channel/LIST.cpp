#include "../../MessageManager.hpp"

void MessageManager::LIST(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  
std::cout << "call list\n";

  (void)trailing;
  // somethings;

  // reply(cs, RPL_LISTSTART, "LIST", paramsVec, trailing);
  outMessages_[cs].append("[LIST BEGIN]\n");
  if (paramsVec.size()) {
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {
      if (channels_.find(*it) != channels_.end())
        // reply(cs, RPL_LIST, "LIST", paramsVec, trailing);
        outMessages_[cs].append("[").append(channels_[*it].title).append("]:[").append(channels_[*it].topic).append("]\n");
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      // reply(cs, RPL_LIST, "LIST", paramsVec, trailing);
      outMessages_[cs].append("[").append(it->second.title).append("]:[").append(it->second.topic).append("]\n");
      ++it;
    }
  }

  // reply(cs, RPL_LISTEND, "LIST", paramsVec, trailing);
  outMessages_[cs].append("[LIST END]\n");
}
