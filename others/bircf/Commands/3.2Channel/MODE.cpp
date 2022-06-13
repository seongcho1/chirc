#include "../../MessageManager.hpp"

void MessageManager::MODE(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "MODE", paramsVec, trailing);
    return;
  }

  std::string whatis = *paramsVec.begin();
  if (whatis[0] == '#')
    modeChannel(cs, paramsVec);
  else
    modeUser(cs, paramsVec);
}

void MessageManager::modeChannel(int cs, std::vector<std::string> paramsVec) {
  std::string channel = *paramsVec.begin();
  std::string mode = *(++paramsVec.begin());

  if (channels_.find(channel) == channels_.end()) {
    reply(cs, ERR_NOSUCHCHANNEL, "MODE", paramsVec, "");
    return;
  }

  if (paramsVec.size() < 2) {
    outMessages_[cs].append("Mode: ").append(channels_[channel].currentMode()).append("\n");
    return;
  }

  if (channels_[channel].channelOperators.find(cs) == channels_[channel].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "MODE", paramsVec, "");
    return;
  }

  channels_[channel].setMode(mode);
}

void MessageManager::modeUser(int cs, std::vector<std::string> paramsVec) {
  std::string nick = *paramsVec.begin();
  std::string mode = *(++paramsVec.begin());
  int fd;

  if (nickFdPair_.find(nick) == nickFdPair_.end()) {
    reply(cs, ERR_NOSUCHNICK, "MODE", paramsVec, "");
    return;
  }
  else {
    fd = nickFdPair_.find(nick)->second;
  }

  if (paramsVec.size() < 2) {
    outMessages_[cs].append("Mode: ").append(users_[fd].currentMode()).append("\n");
    return;
  }

  if (fd == cs) {
    users_[cs].setMode(mode);
    outMessages_[cs].append("Mode: ").append(users_[fd].currentMode()).append("\n");
  }
  else {
    outMessages_[cs].append("You are not OP\n");
  }
}
