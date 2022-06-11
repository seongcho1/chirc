#include "../../MessageManager.hpp"

void MessageManager::JOIN(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "JOIN", paramsVec, trailing); //461
    return;
  }

  // if (!users_[cs].isAuthenticated()) {
  //   reply(cs, ERR_NOTREGISTERED, "PART", paramsVec, trailing);
  //   return;
  // }

  std::vector<std::string>::iterator it = paramsVec.begin();
  while (it != paramsVec.end()) {
    if (channels_.find(*it) == channels_.end()) {
      channels_[*it] = Channel();
    }

    std::set<int>::iterator mit = channels_[*it].member.begin();
    while (mit != channels_[*it].member.end()) {
      outMessages_[*mit++].append("join [" + users_[cs].nick + "]\n");
    }

    channels_[*it].member.insert(cs);
    users_[cs].engaged.insert(*it);
    ++it;
  }
}
