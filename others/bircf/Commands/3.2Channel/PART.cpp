#include "../../MessageManager.hpp"

void MessageManager::PART(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PART", paramsVec, trailing); //461
    return;
  }

  // if (!users_[cs].isAuthenticated()) {
  //   reply(cs, ERR_NOTREGISTERED, "PART", paramsVec, trailing);
  //   return;
  // }

  std::vector<std::string>::iterator it = paramsVec.begin();
  while (it != paramsVec.end()) {
    if (channels_.find(*it) == channels_.end()) {
      reply(cs, ERR_NOSUCHCHANNEL, "PART", paramsVec, trailing);
    }
    else if (!channels_[*it].leave(cs)) {
      reply(cs, ERR_NOTONCHANNEL, "PART", paramsVec, trailing);
    }
    else {
      outMessages_[cs].append("leaves room [" + *it + "]\n");

      if (channels_[*it].member.size() == 0) {
        channels_.erase(*it);
      }
      else {
        std::set<int>::iterator mit = channels_[*it].member.begin();
        while (mit != channels_[*it].member.end()) {
          outMessages_[*mit++].append("leaves [" + users_[cs].nick + "]\n");
        }
      }

    }
    ++it;
  }
}
