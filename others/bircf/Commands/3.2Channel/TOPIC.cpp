#include "../../MessageManager.hpp"

void MessageManager::TOPIC(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "TOPIC", paramsVec, trailing);
    return;
  }

  if (paramsVec.size() < 2) {
    outMessages_[cs].append("TOPIC: ").append(channels_[*paramsVec.begin()].topic).append("\n");
    return;
  }

  if (channels_[*paramsVec.begin()].channelOperators.find(cs) == channels_[*paramsVec.begin()].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "TOPIC", paramsVec, trailing);
    return;
  }

  if (users_[cs].engaged.find(*paramsVec.begin()) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "TOPIC", paramsVec, trailing);
    return;
  }

  std::string topic;
  std::vector<std::string>::iterator pit = ++paramsVec.begin();
  while (pit != paramsVec.end())
    topic.append(*pit++).append(" ");
  channels_[*paramsVec.begin()].topic = topic;
  
  topic = users_[cs].nick + " has set topic: " + topic + "\n";
  announceToChannel(*paramsVec.begin(), topic);
}
