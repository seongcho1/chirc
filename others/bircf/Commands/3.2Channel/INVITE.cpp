#include "../../MessageManager.hpp"

void MessageManager::INVITE(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  
  if (paramsVec.size() == 0) {
    std::set<std::string>::iterator invited = users_[cs].invited.begin();
    while (invited != users_[cs].invited.end())
      outMessages_[cs].append("invited: ").append(*invited++).append("\n");
    outMessages_[cs].append("End of INVITE list\n");
    return;
  }

  if (paramsVec.size() < 2) {
    reply(cs, ERR_NEEDMOREPARAMS, "INVITE", paramsVec, trailing);
    return;
  }

  std::string channel = *(++paramsVec.begin());
  std::map<std::string, int>::iterator nickfdit = nickFdPair_.find(*paramsVec.begin());

  if (nickfdit == nickFdPair_.end()) {
    reply(cs, ERR_NOSUCHNICK, "INVITE", paramsVec, trailing);
    return;
  }

  if (users_[cs].engaged.find(channel) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "INVITE", paramsVec, trailing);
    return;
  }

  if (users_[nickfdit->second].engaged.find(channel) != users_[nickfdit->second].engaged.end()) {
    reply(cs, ERR_USERONCHANNEL, "INVITE", paramsVec, trailing);
    return;
  }

  if (channels_[channel].channelCreator != cs &&
      channels_[channel].channelOperators.find(cs) == channels_[channel].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "INVITE", paramsVec, trailing);
    return;
  }

  users_[nickfdit->second].invited.insert(channel);
  // reply(cs, RPL_INVITING, "INVITE", paramsVec, trailing);
  outMessages_[cs].append("inviting ").append(nickfdit->first).append(" to #").append(channel).append("\n");
  // reply(cs, RPL_AWAY, "INVITE", paramsVec, trailing);
  outMessages_[nickfdit->second].append(users_[cs].nick).append(" invite you to #").append(channel).append("\n");
}
