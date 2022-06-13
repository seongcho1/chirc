#include "../../MessageManager.hpp"

void MessageManager::KICK(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  if (paramsVec.size() < 2) {
    reply(cs, ERR_NEEDMOREPARAMS, "KICK", paramsVec, trailing);
    return;
  }

  if (channels_.find(*paramsVec.begin()) == channels_.end()) {
    reply(cs, ERR_NOSUCHCHANNEL, "KICK", paramsVec, trailing);
    return;
  }

  if (users_[cs].engaged.find(*paramsVec.begin()) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "KICK", paramsVec, trailing);
    return;
  }

  std::map<std::string, int>::iterator nfit = nickFdPair_.find(*(++paramsVec.begin()));
  if (nfit == nickFdPair_.end()) {
    reply(cs, ERR_NOTONCHANNEL, "KICK", paramsVec, trailing);
    return;
  }

  if (channels_[*paramsVec.begin()].member.find(nfit->second) == channels_[*paramsVec.begin()].member.end()) {
    reply(cs, ERR_NOTONCHANNEL, "KICK", paramsVec, trailing);
    return;
  }

  if (channels_[*paramsVec.begin()].channelOperators.find(cs) == channels_[*paramsVec.begin()].channelOperators.end()) {
    // reply(cs, ERR_BADCHANMASK, "KICK", paramsVec, trailing);
    reply(cs, ERR_CHANOPRIVSNEEDED, "KICK", paramsVec, trailing);
    return;
  }

  users_[nfit->second].engaged.erase(*paramsVec.begin());
  channels_[*paramsVec.begin()].member.erase(nfit->second);

  reply(cs, RPL_AWAY, "KICK", paramsVec, trailing);
  reply(nfit->second, RPL_AWAY, "KICK", paramsVec, trailing);
}
